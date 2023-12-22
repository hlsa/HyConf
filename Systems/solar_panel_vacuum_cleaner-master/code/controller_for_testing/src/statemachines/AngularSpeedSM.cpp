#include "AngularSpeedSM.hpp"
#include "VacuumCleanerPlatform.hpp"
#include "OutputLeftMotorOp.hpp"
#include "OutputRightMotorOp.hpp"
// #include "PIDOp.hpp"
#include "FIFO.hpp"
#include <math.h>
#include <iostream>
#include <cmath>
#include "Operation.hpp"

using namespace std;

AngularSpeedSM::AngularSpeedSM(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = WAIT_INPUT;
    turn_angle = 0;
}

AngularSpeedSM::~AngularSpeedSM()
{
}

void AngularSpeedSM::step()
{
    while (smallstep() == CONTINUE)
    {
        #ifdef ANGULARSPEEDSM
			printf("turn_angle = %.4f\n",turn_angle);
			#endif

    }
}

STEP_RETURN AngularSpeedSM::smallstep()
{
    switch (state)
    {
        case WAIT_INPUT:
            #ifdef ANGULARSPEEDSM
			printf("AngularSpeedSM:WAIT_INPUT\n");
			#endif

            if (acc_r_event) {
                ar.X = acc_r.X;
                ar.Y = acc_r.Y;
                ar.Z = acc_r.Z;
                acc_r_ack_event = true;
                state = WAIT_AL;
                return CONTINUE;
            }
            else if (acc_l_event) {
                al.X = acc_l.X;
                al.Y = acc_l.Y;
                al.Z = acc_l.Z;
                acc_l_ack_event = true;
                state = WAIT_AR;
                return CONTINUE;
            } else {
                state = WAIT_INPUT;
                return WAIT;
            }
        break;
        case WAIT_AL:
        #ifdef ANGULARSPEEDSM
			printf("AngularSpeedSM:WAIT_AL\n");
			#endif

            if (acc_l_event) {
                al.X = acc_l.X;
                al.Y = acc_l.Y;
                al.Z = acc_l.Z;
                acc_l_ack_event = true;
                state = UPDATE_VALUE;
                return CONTINUE;
            } else {
                state = WAIT_AL;
                return WAIT;
            }
        break;
        case WAIT_AR:
        #ifdef ANGULARSPEEDSM
			printf("AngularSpeedSM:WAIT_AR\n");
			#endif

            if (acc_r_event) {
                ar.X = acc_r.X;
                ar.Y = acc_r.Y;
                ar.Z = acc_r.Z;
                acc_r_ack_event = true;
                state = UPDATE_VALUE;
                return CONTINUE;
            } else {
                state = WAIT_AR;
                return WAIT;
            }
        break;
        case UPDATE_VALUE: {
            #ifdef ANGULARSPEEDSM
			printf("AngularSpeedSM:UPDATE_VALUE\n");
			#endif

            if (!in_state) {
                turn_angle = turn_angle + sqrt(abs(al.Y-ar.Y)/acc_distance)*t*p->step_size;
                in_state = true;
                return CONTINUE;
            } else {
                state = WAIT_ANGLE_ACK;
                angle = -get_sign(al.Y)*(turn_angle);
                #ifdef ANGULARSPEEDSM
			        printf("angle = %.4f\n", angle);
			    #endif
                angle_event = true;
                in_state = false;
                return CONTINUE;
            }
        }
        break;
        case WAIT_ANGLE_ACK:
        #ifdef ANGULARSPEEDSM
			printf("AngularSpeedSM:WAIT_ANGLE_ACK\n");
			#endif

            if (angle_ack_event) {
                state = WAITING;
                return CONTINUE;
            } else {
                state = WAIT_ANGLE_ACK;
                angle = -get_sign(al.Y)*(turn_angle);
                angle_event = true;
                return WAIT;
            }
        break;
        // this state was added to deal with wait(t) in the transition action
        case WAITING:
        #ifdef ANGULARSPEEDSM
			printf("AngularSpeedSM:WAITING\n");
			#endif

            if (!in_state)
            {
                // a state has not yet been entered
                // if a state has an entry action, it must terminate before in_state is set to true.
                // see state S_FOUND_SECOND_SHAPE_TS
                // reset implicit clock
                clock = 0;
                in_state = true;
                return CONTINUE;
            }
            else
            {
                if (clock < t)
                {
                    clock++;
                    return WAIT;
                }
                else
                {
                    // otherwise: delay has been violated
                    // go to first state of OPERATION, not sure if time should pass here
                    in_state = false;
                    state = WAIT_INPUT;
                    return CONTINUE;
                }
            }
        break;
        default:
            #ifdef ANGULARSPEEDSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}
