#include "InputDuplicationSM.hpp"
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

InputDuplicationSM::InputDuplicationSM(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = WAIT_INPUT;
}

InputDuplicationSM::~InputDuplicationSM()
{
}

void InputDuplicationSM::step()
{
    while (smallstep() == CONTINUE)
    {

    }
}

STEP_RETURN InputDuplicationSM::smallstep()
{
    switch (state)
    {
        case WAIT_INPUT:
        #ifdef INPUTDUPLICATIONSM
			printf("InputDuplication:WAIT_INPUT\n");
			#endif

            if (i_event) {
                x = i;
                i_ack_event = true;
                state = WAIT_ACK1;
                o1 = x;
                o1_event = true;
                return CONTINUE;
            } else {
                state = WAIT_INPUT;
                return WAIT;
            }
        break;
        case WAIT_ACK1:
        #ifdef INPUTDUPLICATIONSM
			printf("InputDuplication:WAIT_ACK1\n");
			#endif

            if (o_ack1_event) {
                state = WAIT_ACK2;
                o2 = x;
                o2_event = true;
                return CONTINUE;
            } else {
                state = WAIT_ACK1;
                o1 = x;
                o1_event = true;
                return WAIT;
            }
        break;

        case WAIT_ACK2:
        #ifdef INPUTDUPLICATIONSM
			printf("InputDuplication:WAIT_ACK2\n");
			#endif

            if (o_ack2_event) {
                state = WAIT_INPUT;
                return CONTINUE;
            } else {
                state = WAIT_ACK2;
                o2 = x;
                o2_event = true;
                return WAIT;
            }
        break;
        default:
            #ifdef INPUTDUPLICATIONSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}
