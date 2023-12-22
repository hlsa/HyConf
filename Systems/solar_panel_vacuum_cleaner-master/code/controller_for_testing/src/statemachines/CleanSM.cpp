#include "CleanSM.hpp"
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

CleanSM::CleanSM(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = DONT_CLEAN;
}

CleanSM::~CleanSM()
{
}

void CleanSM::step()
{
    while (smallstep() == CONTINUE)
    {
    }
}

STEP_RETURN CleanSM::smallstep()
{
    switch (state)
    {
        case DONT_CLEAN:
            if (!in_state) {
                p->brush(0);
                p->vacuum(0);
                in_state = true;
                return CONTINUE;
            } else {
                if (clean_event && clean == true) {
                    state = CLEAN;
                    clean_ack_event = true;
                    in_state = false;
                    return CONTINUE;
                } else {
                    in_state = false;
                    state = DONT_CLEAN;
                    return WAIT;
                }
            }
        break;
        case CLEAN: {
            if (!in_state) {
                p->brush(voltage);
                p->vacuum(voltage);
                in_state = true;
                return CONTINUE;
            } else {
                if (clean_event && clean == false) {
                    state = DONT_CLEAN;
                    clean_ack_event = true;
                    in_state = false;
                    return CONTINUE;
                } else {
                    in_state = false;
                    state = CLEAN;
                    return WAIT;
                }
            }
        }
        break;
        default:
            #ifdef CLEANSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}
