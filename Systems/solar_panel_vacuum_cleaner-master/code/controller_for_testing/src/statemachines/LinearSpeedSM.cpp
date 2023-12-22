#include "LinearSpeedSM.hpp"
#include "VacuumCleanerPlatform.hpp"
#include "OutputLeftMotorOp.hpp"
#include "OutputRightMotorOp.hpp"
// #include "PIDOp.hpp"
#include "FIFO.hpp"
#include <math.h>
#include <iostream>
#include "Operation.hpp"
#include "Definitions.hpp"

using namespace std;

LinearSpeedSM::LinearSpeedSM(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = WAIT_INPUT;
}

LinearSpeedSM::~LinearSpeedSM()
{
}

void LinearSpeedSM::step()
{
    while (smallstep() == CONTINUE)
    {
    }
}

STEP_RETURN LinearSpeedSM::smallstep()
{
    switch (state)
    {
        case WAIT_INPUT:
        #ifdef LINEARSPEEDSM
			printf("LinearSpeed: WAIT_INPUT\n");
			#endif

            if (speed_r_event) {
                spr.X = speed_r.X;
                spr.Y = speed_r.Y;
                spr.Z = speed_r.Z;
                speed_r_ack_event = true;
                state = WAIT_LEFT;
                return CONTINUE;
            }
            else if (speed_l_event) {
                spl.X = speed_l.X;
                spl.Y = speed_l.Y;
                spl.Z = speed_l.Z;
                speed_l_ack_event = true;
                state = WAIT_RIGHT;
                return CONTINUE;
            } else {
                state = WAIT_INPUT;
                return WAIT;
            }
        break;
        case WAIT_LEFT:
        #ifdef LINEARSPEEDSM
			printf("LinearSpeed: WAIT_LEFT\n");
			#endif

            if (speed_l_event) {
                spl.X = speed_l.X;
                spl.Y = speed_l.Y;
                spl.Z = speed_l.Z;
                speed_l_ack_event = true;
                state = SEND_OUTPUT;
                return CONTINUE;
            } else {
                state = WAIT_LEFT;
                return WAIT;
            }
            
        break;
        case WAIT_RIGHT:
        #ifdef LINEARSPEEDSM
			printf("LinearSpeed: WAIT_RIGHT\n");
			#endif

            if (speed_r_event) {
                spr.X = speed_r.X;
                spr.Y = speed_r.Y;
                spr.Z = speed_r.Z;
                speed_r_ack_event = true;
                state = SEND_OUTPUT;
                return CONTINUE;
            } else {
                state = WAIT_RIGHT;
                return WAIT;
            }
        break;
        case SEND_OUTPUT:
        #ifdef LINEARSPEEDSM
			printf("LinearSpeed: SEND_OUTPUT\n");
			#endif

            state = WAIT_OUTPUT_ACK;
            linear_speed_event = true;
            linear_speed = get_linear_speed(spl.Z,spr.Z);
            return CONTINUE;
        break;
        case WAIT_OUTPUT_ACK:
            #ifdef LINEARSPEEDSM
			printf("LinearSpeed: WAIT_OUTPUT_ACK\n");
			#endif

            if (ls_ack_event) {
                state = WAIT_INPUT;
                // CHANGE: trying to force exec here to avoid infinite loop
                return WAIT;
            } else {
                state = WAIT_OUTPUT_ACK;
                linear_speed_event = true;
                linear_speed = get_linear_speed(spl.Z,spr.Z);
                #ifdef LINEARSPEEDSM
                printf("linear_speed = %.4f\n", linear_speed);
                #endif
                return WAIT;
            }
        break;
        default:
            #ifdef LINEARSPEEDSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}