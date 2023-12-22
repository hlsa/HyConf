#include "PID.hpp"
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

PID::PID(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = WAIT_TARGET;
    prior_error = 0;
    integral = 0;
    // There is a deadlock in the code. I'm trying to prevent it
    // by making the err_output_event of PID's always available.
    err_output_event = true;
    err_output = 0;
}

PID::~PID()
{
}

void PID::step()
{
    while (smallstep() == CONTINUE)
    {
        #ifdef PIDSM
		printf("target: %.2f, err: %.2f\n", target, err_output);
		#endif

    }
}

STEP_RETURN PID::smallstep()
{
    switch (state)
    {
        case WAIT_TARGET:
            #ifdef PIDSM
			printf("PID:wait_target (%s)\n", target_event?"true":"false");
			#endif

            if (target_event && target == old_target) {
                target_value = target;
                target_ack_event = true;
                state = WAIT_ACTUAL;
                return CONTINUE;
            } else if (target_event && target != old_target) {
                target_value = target;
                target_ack_event = true;
                state = RESET;
                return CONTINUE;
            } else {
                state = WAIT_TARGET;
                return WAIT;
            }
        break;
        case WAIT_ACTUAL:
            #ifdef PIDSM
			printf("PID:wait_actual\n");
			#endif

            if (actual_event) {
                actual_value = actual;
                actual_ack_event = true;
                state = UPDATE;
                return CONTINUE;
            } else {
                state = WAIT_ACTUAL;
                return WAIT;
            }
        break;
        case RESET:
            #ifdef PIDSM
			printf("PID:reset\n");
			#endif

            if (!in_state) {
                prior_error = 0;
                integral = 0;
                in_state = true;
                return CONTINUE;
            } else {
                if (actual_event) {
                    actual_value = actual;
                    actual_ack_event = true;
                    state = UPDATE;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RESET;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case UPDATE:
            #ifdef PIDSM
			printf("PID:update\n");
			#endif

            if (!in_state) {
                clock = 0;
                error = actual_value-target_value;
                integral = integral+(error*c_step*p->step_size);
                derivative = floor((error-prior_error)/(c_step*p->step_size));
                out = kp*error + ki*integral + kd*derivative;
                prior_error = error;
                err_output_event = true;
                err_output = out;
                in_state = true;
                return CONTINUE;
            } else {
                in_state = false;
                // CHANGE: There is a deadlock in the code. I'm trying to prevent it
                // by making the err_output_event of PID's always available.
                // Therefore, the code should not wait for acknowledgement
                // of err and go straight into WAITING.
                state = WAITING;//WAIT_ERR_ACK;
                return CONTINUE;
            }
        break;
        case WAIT_ERR_ACK:
            #ifdef PIDSM
			printf("PID:wait_err_ack\n");
			#endif

            if (err_ack_event) {
                old_target = target_value;
                state = WAITING;
                return CONTINUE;
            } else {
                state = WAIT_ERR_ACK;
                return WAIT;
            }
        break;
        // this state was added to deal with wait(t) in the transition action
        case WAITING:
            #ifdef PIDSM
			printf("PID:waiting\n");
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
                if (clock < c_step)
                {
                    clock++;
                    return WAIT;
                }
                else
                {
                    // otherwise: delay has been violated
                    // go to first state of OPERATION, not sure if time should pass here
                    in_state = false;
                    state = WAIT_TARGET;
                    return CONTINUE;
                }
            }
        break;
        default:
            #ifdef PIDSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}
