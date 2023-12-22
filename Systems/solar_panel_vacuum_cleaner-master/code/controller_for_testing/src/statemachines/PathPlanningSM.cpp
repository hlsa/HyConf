#include "PathPlanningSM.hpp"
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

PathPlanningSM::PathPlanningSM(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = RESUME_ENTRY1;
    cycles = 0;
}

PathPlanningSM::~PathPlanningSM()
{
}

void PathPlanningSM::step()
{
    while (smallstep() == CONTINUE)
    {
    }
}

STEP_RETURN PathPlanningSM::smallstep()
{
    switch (state)
    {
        case RESUME_ENTRY1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RESUME_ENTRY1\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_right;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = RESUME_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RESUME_ENTRY1;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case RESUME_ENTRY2:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RESUME_ENTRY2\n");
			#endif

            if (!in_state) {
                move_forward_event = true;
                in_state = true;
                return CONTINUE;
            } else {
                if (move_forward_ack_event) {
                    state = RESUME_ENTRY3;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RESUME_ENTRY2;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case RESUME_ENTRY3:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RESUME_ENTRY3\n");
			#endif

            if (!in_state) {
                displacement_out_event = true;
                displacement_out = 0;
                in_state = true;
                return CONTINUE;
            } else {
                if (displacement_out_ack_event) {
                    state = RESUME_EXIT1;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RESUME_ENTRY3;
                    in_state = false;
                    return WAIT;
                }
            }
        break;    
        case RESUME_EXIT1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RESUME_EXIT1\n");
			#endif

                if (displacement_in_event && displacement_in >= cycles*2*nozzle) {
                    d = displacement_in;
                    displacement_in_ack_event = true;
                    // exit action
                    turn_event = true;
                    turn = Direction_left;

                    state = RESUME_EXIT2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RESUME_EXIT1;
                    return WAIT;
                }
        break;    
        case RESUME_EXIT2:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RESUME_EXIT2\n");
			#endif

                if (turn_ack_event) {
                    state = RESUME_EXIT3;
                    clean_event = true;
                    clean = true;
                    return CONTINUE;
                } else {
                    state = RESUME_EXIT2;
                    turn_event = true;
                    turn = Direction_left;
                    return WAIT;
                }
        break;    
        case RESUME_EXIT3:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RESUME_EXIT3\n");
			#endif

            if (clean_ack_event) {
                state = GO_UP_ENTRY1;
                return CONTINUE;
            } else {
                state = RESUME_EXIT3;
                return WAIT;
            }
        break;                
        case GO_UP_ENTRY1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_UP_ENTRY1\n");
			#endif

            if (!in_state) {
                move_forward_event = true;
                in_state = true;
                return CONTINUE;
            } else {
                if (move_forward_ack_event) {
                    state = GO_UP_EXIT;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_UP_ENTRY1;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case GO_UP_EXIT:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_UP_EXIT\n");
			#endif

            if (ultrasonic_event && ultrasonic >= cliff) {
                u = ultrasonic;
                ultrasonic_ack_event = true;
                state = GO_RIGHT_ENTRY1;
                return CONTINUE;
            } else {
                state = GO_UP_EXIT;
                return WAIT;
            }
        break;
        case GO_RIGHT_ENTRY1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_RIGHT_ENTRY1\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_right;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = GO_RIGHT_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_RIGHT_ENTRY1;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case GO_RIGHT_ENTRY2:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_RIGHT_ENTRY2\n");
			#endif

            if (!in_state) {
                move_forward_event = true;
                in_state = true;
                return CONTINUE;
            } else {
                if (move_forward_ack_event) {
                    state = GO_RIGHT_ENTRY3;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_RIGHT_ENTRY2;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case GO_RIGHT_ENTRY3:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_RIGHT_ENTRY3\n");
			#endif

            if (!in_state) {
                displacement_out_event = true;
                displacement_out = 0;
                in_state = true;
                return CONTINUE;
            } else {
                if (displacement_out_ack_event) {
                    state = GO_RIGHT_EXIT;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_RIGHT_ENTRY3;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case GO_RIGHT_EXIT:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_RIGHT_EXIT\n");
			#endif

            if (ultrasonic_event && ultrasonic >= cliff) {
                u = ultrasonic;
                ultrasonic_ack_event = true;
                state = GO_DOWN_ENTRY1;
                return CONTINUE;
            } else if (displacement_in_event && displacement_in >= nozzle) {
                d = displacement_in;
                displacement_in_ack_event = true;
                state = GO_DOWN_ENTRY1;
                return CONTINUE;
            } else {
                state = GO_RIGHT_EXIT;
                return WAIT;
            }
        break;                
        case GO_DOWN_ENTRY1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_DOWN_ENTRY1\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_right;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = GO_DOWN_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_ENTRY1;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case GO_DOWN_ENTRY2:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_DOWN_ENTRY2\n");
			#endif

            if (!in_state) {
                move_forward_event = true;
                in_state = true;
                return CONTINUE;
            } else {
                if (move_forward_ack_event) {
                    state = GO_DOWN_EXIT1;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_ENTRY2;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case GO_DOWN_EXIT1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_DOWN_EXIT1\n");
			#endif

            if (ultrasonic_event && ultrasonic >= cliff) {
                u = ultrasonic;
                ultrasonic_ack_event = true;
                state = GO_DOWN_EXIT2;
                return CONTINUE;
            } else {
                state = GO_DOWN_EXIT1;
                return WAIT;
            }

        break;
        case GO_DOWN_EXIT2:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_DOWN_EXIT2\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_left;
                in_state = false;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = GO_DOWN_EXIT3;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_EXIT2;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case GO_DOWN_EXIT3:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_DOWN_EXIT3\n");
			#endif

            if (!in_state) {
                move_forward_event = true;
                in_state = false;
                return CONTINUE;
            } else {
                if (move_forward_ack_event) {
                    state = GO_DOWN_EXIT4;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_EXIT3;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case GO_DOWN_EXIT4:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_DOWN_EXIT4\n");
			#endif

            if (!in_state) {
                displacement_out_event = true;
                displacement_out = 0;
                in_state = false;
                return CONTINUE;
            } else {
                if (displacement_out_ack_event) {
                    state = GO_RIGHT_AGAIN;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_EXIT4;
                    in_state = false;
                    return WAIT;
                }
            }
        break;

        case GO_RIGHT_AGAIN:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:GO_RIGHT_AGAIN\n");
			#endif

            if (ultrasonic_event && ultrasonic < cliff) {
                u = ultrasonic;
                ultrasonic_ack_event = true;
                state = CHECK_END;
                return CONTINUE;
            } else if (ultrasonic_event && ultrasonic >= cliff) {
                u = ultrasonic;
                ultrasonic_ack_event = true;
                cycles = 0;
                state = RETURN_ENTRY1;
                return CONTINUE;
            } else {
                state = GO_RIGHT_AGAIN;
                return WAIT;
            }
        break;
        case CHECK_END:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:CHECK_END\n");
			#endif

            if (displacement_in_event && displacement_in < nozzle) {
                d = displacement_in;
                displacement_in_ack_event = true;
                state = GO_RIGHT_AGAIN;
                return CONTINUE;
            } else if (displacement_in_event && displacement_in >= nozzle) {
                d = displacement_in;
                displacement_in_ack_event = true;
                cycles = cycles + 1;
                state = CHECK_BATTERY;
                return CONTINUE;
            } else {
                state = CHECK_END;
                return WAIT;
            }
        break;
        case CHECK_BATTERY:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:CHECK_BATTERY\n");
			#endif

            if (battery_level_event && battery_level > battery_low) {
                b = battery_level;
                battery_level_ack_event = true;
                state = CHECK_BATTERY_EXIT;
                return CONTINUE;
            } else if (battery_level_event && battery_level <= battery_low) {
                b = battery_level;
                battery_level_ack_event = true;
                state = RETURN_ENTRY1;
                return CONTINUE;
            } else {
                state = CHECK_BATTERY;
                return WAIT;
            }
        break;

        case CHECK_BATTERY_EXIT:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:CHECK_BATTERY_EXIT\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_left;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = GO_UP_ENTRY1;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = CHECK_BATTERY_EXIT;
                    in_state = false;
                    return WAIT;
                }
            }
            
        break;
        case RETURN_ENTRY1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RETURN_ENTRY1\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_left;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = RETURN_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RETURN_ENTRY1;
                    in_state = false;
                    return WAIT;
                }
            }        
        break;
        case RETURN_ENTRY2:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RETURN_ENTRY2\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_left;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = RETURN_ENTRY3;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RETURN_ENTRY2;
                    in_state = false;
                    return WAIT;
                }
            }        
        break;
        case RETURN_ENTRY3:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RETURN_ENTRY3\n");
			#endif

            if (!in_state) {
                move_forward_event = true;
                in_state = true;
                return CONTINUE;
            } else {
                if (move_forward_ack_event) {
                    state = RETURN_ENTRY4;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RETURN_ENTRY3;
                    in_state = false;
                    return WAIT;
                }
            }        
        break;
        case RETURN_ENTRY4:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RETURN_ENTRY4\n");
			#endif

            if (!in_state) {
                clean_event = true;
                clean = false;
                in_state = true;
                return CONTINUE;
            } else {
                if (clean_ack_event) {
                    state = RETURN_ENTRY5;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = RETURN_ENTRY4;
                    in_state = false;
                    return WAIT;
                }
            }        
        break;
        case RETURN_ENTRY5:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:RETURN_ENTRY5\n");
			#endif

            if (ultrasonic_event && ultrasonic >= cliff) {
                u = ultrasonic;
                ultrasonic_ack_event = true;
                state = DOCK_ENTRY1;
                return CONTINUE;
            } else {
                state = RETURN_ENTRY5;
                return WAIT;
            }
        break;
        case DOCK_ENTRY1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:DOCK_ENTRY1\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_left;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = DOCK_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = DOCK_ENTRY1;
                    in_state = false;
                    return WAIT;
                }
            }        
        break;
        case DOCK_ENTRY2:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:DOCK_ENTRY2\n");
			#endif

            if (!in_state) {
                move_forward_event = true;
                in_state = true;
                return CONTINUE;
            } else {
                if (move_forward_ack_event) {
                    state = DOCK_ENTRY3;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = DOCK_ENTRY2;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case DOCK_ENTRY3:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:DOCK_ENTRY3\n");
			#endif

            if (charging_event) {
                charging_ack_event = true;
                state = SLEEP;
                return CONTINUE;
            } else {
                state = DOCK_ENTRY3;
                return WAIT;
            }
        break;
        case SLEEP:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:SLEEP\n");
			#endif

            if (!in_state) {
                stop_event = true;
                in_state = true;
                return CONTINUE;
            } else {
                if (stop_ack_event) {
                    state = WAITING;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = SLEEP;
                    in_state = false;
                    return WAIT;
                }
            }        
            return WAIT;
        break;
        // this state was added to deal with wait(t) in the transition action
        case WAITING:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:WAITING\n");
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
                if (clock < sleep_time)
                {
                    clock++;
                    return WAIT;
                }
                else
                {
                    // otherwise: delay has been violated
                    // go to first state of OPERATION, not sure if time should pass here
                    in_state = false;
                    state = SLEEP_EXIT1;
                    return CONTINUE;
                }
            }
        break;
        case SLEEP_EXIT1:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:SLEEP_EXIT1\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_left;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = SLEEP_EXIT2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = SLEEP_EXIT1;
                    in_state = false;
                    return WAIT;
                }
            }
        break;  
        case SLEEP_EXIT2:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:SLEEP_EXIT2\n");
			#endif

            if (!in_state) {
                turn_event = true;
                turn = Direction_left;
                in_state = true;
                return CONTINUE;
            } else {
                if (turn_ack_event) {
                    state = SLEEP_EXIT3;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = SLEEP_EXIT2;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        case SLEEP_EXIT3:
			#ifdef PATHPLANNINGSM
			printf("PathPlanning:SLEEP_EXIT3\n");
			#endif

            if (!in_state) {
                move_forward_event = true;
                in_state = true;
                return CONTINUE;
            } else {
                if (move_forward_ack_event) {
                    state = RESUME_ENTRY1;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = SLEEP_EXIT3;
                    in_state = false;
                    return WAIT;
                }
            }
        break;
        default:
            #ifdef PATHPLANNINGSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}
