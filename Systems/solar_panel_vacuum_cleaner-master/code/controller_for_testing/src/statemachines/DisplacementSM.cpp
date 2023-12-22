#include "DisplacementSM.hpp"
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

DisplacementSM::DisplacementSM(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = WAIT_INPUT;
    d = 0;
}

DisplacementSM::~DisplacementSM()
{
}

void DisplacementSM::step()
{
    while (smallstep() == CONTINUE)
    {
    }
}

STEP_RETURN DisplacementSM::smallstep()
{
    switch (state)
    {
        case WAIT_INPUT:
        #ifdef DISPLACEMENTSM
			printf("Displacement:WAIT_INPUT\n");
			#endif

            if (displacement_in_event && displacement_in == 0) {
                d = displacement_in;
                displacement_in_ack_event = true;
                state = WAIT_INPUT;
                return WAIT;
            }
            else if (sp_event) {
                speed = sp;
                sp_ack_event = true;
                state = UPDATE_DISPLACEMENT;
                return CONTINUE;
            } else {
                state = WAIT_INPUT;

                return WAIT;
            }
        break;
        case UPDATE_DISPLACEMENT: {
            #ifdef DISPLACEMENTSM
			printf("Displacement:UPDATE_DISPLACEMENT\n");
			#endif

            if (!in_state) {
                d = d + speed.X*t*p->step_size;
                in_state = true;
                return CONTINUE;
            } else {
                displacement_out = d;
                displacement_out_event = true;
                in_state = false;
                // CHANGE: There is a deadlock in the code. I'm trying to prevent it
                // by making the displacement_out_event of DisplacementSM's always
                // available. Therefore, the code should not wait for acknowledgement
                // of displacement_out and go straight into WAITING.
                state = WAITING;//WAIT_DISPLACEMENT_ACK;
                return CONTINUE;
            }
        }
        break;
        case WAIT_DISPLACEMENT_ACK:
        #ifdef DISPLACEMENTSM
			printf("Displacement:WAIT_DISPLACEMENT_ACK\n");
			#endif

            if (displacement_out_ack_event) {
                state = WAITING;
                return CONTINUE;
            } else {
                state = WAIT_DISPLACEMENT_ACK;
                displacement_out = d;
                displacement_out_event = true;
                return WAIT;
            }
        break;
        // this state was added to deal with wait(t) in the transition action
        case WAITING:
        #ifdef DISPLACEMENTSM
			printf("Displacement:WAITING\n");
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
            #ifdef DISPLACEMENTSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}
