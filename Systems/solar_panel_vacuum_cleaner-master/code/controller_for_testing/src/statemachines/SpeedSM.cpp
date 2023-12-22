#include "SpeedSM.hpp"
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

SpeedSM::SpeedSM(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = WAIT_INPUT;
    speed.X = 0;
    speed.Y = 0;
    speed.Z = 0;
}

SpeedSM::~SpeedSM()
{
}

void SpeedSM::step()
{
    while (smallstep() == CONTINUE)
    {
    }
}

STEP_RETURN SpeedSM::smallstep()
{
    switch (state)
    {
        case WAIT_INPUT:
			#ifdef SPEEDSM
			printf("SpeedSM:WAIT_INPUT\n");
			#endif

            if (acc_event) {
                acceleration.X = acc.X;
                acceleration.Y = acc.Y;
                acceleration.Z = acc.Z;
                acc_ack_event = true;
                state = UPDATE_SPEED;
                return CONTINUE;
            } else {
                state = WAIT_INPUT;
                return WAIT;
            }
        break;
        case UPDATE_SPEED:
			#ifdef SPEEDSM
			printf("SpeedSM:UPDATE_SPEED\n");
			#endif
 {
            if (!in_state) {
                // a state has not yet been entered
                // if a state has an entry action, it must terminate before in_state is set to true.
                speed.X = speed.X + acceleration.X*t*p->step_size;
                speed.Y = speed.Y + acceleration.Y*t*p->step_size;
                speed.Z = speed.Z + acceleration.Z*t*p->step_size;
                in_state = true;
                return CONTINUE;
            } else {
                sp = speed;
                sp_event = true;
                #ifdef SPEEDSM
                printf("speed = (%.4f,%.4f,%.4f)\n", speed.X, speed.Y, speed.Z);
                #endif
                in_state = false;
                state = WAIT_SP_ACK;
                return CONTINUE;
            }
        }
        break;
        case WAIT_SP_ACK:
			#ifdef SPEEDSM
			printf("SpeedSM:WAIT_SP_ACK\n");
			#endif

            if (sp_ack_event) {
                state = WAITING;
                return CONTINUE;
            } else {
                state = WAIT_SP_ACK;
                // reactivation of action sp!speed
                sp = speed;
                sp_event = true;
                return WAIT;
            }
        break;

        // this state was added to deal with wait(t) in the transition action
        case WAITING:
			#ifdef SPEEDSM
			printf("SpeedSM:WAITING\n");
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
            #ifdef SPEEDSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}
