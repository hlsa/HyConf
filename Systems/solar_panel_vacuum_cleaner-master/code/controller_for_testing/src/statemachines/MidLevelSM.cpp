#include "MidLevelSM.hpp"
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

MidLevelSM::MidLevelSM(VacuumCleanerPlatform *_p)
{
    p = _p;
    state = GO_UP_ENTRY0;
    orientation = Orientation_up; lspeed = 0; rspeed = 0;
}

MidLevelSM::~MidLevelSM()
{
}

void MidLevelSM::step()
{
    while (smallstep() == CONTINUE)
    {}
}

STEP_RETURN MidLevelSM::smallstep()
{
    #ifdef MIDLEVELSM
		printf("move_forward_event == %s\n", move_forward_event?"true":"false");
		printf("turn_event == %s\n",  turn_event?"true":"false");
		printf("angle_adjustment_event == %s\n", angle_adjustment_event?"true":"false");
		printf("speed_adjustment_event == %s\n",  speed_adjustment_event?"true":"false");
		printf("stop_event == %s\n",  stop_event?"true":"false");
	#endif

    
    switch (state)
    {
        // this pre-state resets the clock and imediately moves on to the ENTRY1 state; it is the same everywhere
        case GO_UP_ENTRY0:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_UP_ENTRY0\n");
			#endif

            clock = 0;
            state = GO_UP_ENTRY1;
            return CONTINUE;
        break;
        case GO_UP_ENTRY1:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_UP_ENTRY1\n");
			#endif

            if (!in_state) {
                target_speed_event = true;
                target_speed = climb_speed;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_speed_ack_event) {
                    state = GO_UP_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_UP_ENTRY1;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case GO_UP_ENTRY2:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_UP_ENTRY2\n");
			#endif

                if (speed_adjustment_event) {
                    speed_adj = speed_adjustment;
                    speed_adjustment_ack_event = true;
                    state = GO_UP_ENTRY3;
                    return CONTINUE;
                } else {
                    state = GO_UP_ENTRY2;
                    clock++;
                    return WAIT;
                }
        break;
        case GO_UP_ENTRY3:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_UP_ENTRY3\n");
			#endif

            if (!in_state) {
                target_angle_event = true;
                target_angle = forward_angle;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_angle_ack_event) {
                    state = GO_UP_ENTRY4;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_UP_ENTRY3;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case GO_UP_ENTRY4:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_UP_ENTRY4\n");
			#endif

            if (angle_adjustment_event) {
                angle_adj = angle_adjustment;
                angle_adjustment_ack_event = true;
                state = GO_UP_ENTRY5;
                return CONTINUE;
            } else {
                state = GO_UP_ENTRY4;
                clock++;
                return WAIT;
            }
        break;
        case GO_UP_ENTRY5:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_UP_ENTRY5\n");
			#endif

            // entry action
            lspeed = lspeed - speed_adj - angle_adj;
            rspeed = rspeed - speed_adj + angle_adj;
            #ifdef MIDLEVELSM
			printf("speed_adj: %f, angle_adj: %f\n", speed_adj, angle_adj);
			#endif

            #ifdef MIDLEVELSM
			printf("Setting speeds: %f,%f\n", lspeed, rspeed);
			#endif

            p->output_left_motor(lspeed);
            p->output_right_motor(rspeed);
            // immediate transition
            state = GO_UP_DURING;
            return CONTINUE;
        break;
        case GO_UP_DURING:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_UP_DURING\n");
			#endif

                if (stop_event) {
                    lspeed = stopsp; rspeed = stopsp;
                    stop_ack_event = true;
                    state = STOP_ENTRY0;
                    return CONTINUE;
                } else if (turn_event && turn == Direction_left) {
                    direction = turn;
                    turn_ack_event = true;
                    orientation = turn_left(orientation); lspeed = stopsp; rspeed = stopsp;
                    state = TURN_LEFT_ENTRY0;
                    return CONTINUE;
                } else if (turn_event && turn == Direction_right) {
                    direction = turn;
                    turn_ack_event = true;
                    orientation = turn_right(orientation); lspeed = stopsp; rspeed = stopsp;
                    state = TURN_RIGHT_ENTRY0;
                    return CONTINUE;
                } else if (clock >= c_step) {
                    state = GO_UP_ENTRY0;
                    return CONTINUE;
                } else {
                    state = GO_UP_DURING;
                    clock++;
                    return WAIT;
                }
        break;


        case STOP_ENTRY0:
			#ifdef MIDLEVELSM
			printf("MidLevel:STOP_ENTRY0\n");
			#endif

            clock++;
            state = STOP_ENTRY1;
            return CONTINUE;
        break;
        case STOP_ENTRY1:
			#ifdef MIDLEVELSM
			printf("MidLevel:STOP_ENTRY1\n");
			#endif

            if (!in_state) {
                target_speed_event = true;
                target_speed = turn_speed;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_speed_ack_event) {
                    state = STOP_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = STOP_ENTRY1;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case STOP_ENTRY2:
			#ifdef MIDLEVELSM
			printf("MidLevel:STOP_ENTRY2\n");
			#endif

            if (speed_adjustment_event) {
                speed_adj = speed_adjustment;
                speed_adjustment_ack_event = true;
                state = STOP_ENTRY3;
                return CONTINUE;
            } else {
                state = STOP_ENTRY2;
                clock++;
                return WAIT;
            }
        break;
        case STOP_ENTRY3:
			#ifdef MIDLEVELSM
			printf("MidLevel:STOP_ENTRY3\n");
			#endif

            if (!in_state) {
                target_angle_event = true;
                target_angle = forward_angle;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_angle_ack_event) {
                    state = STOP_ENTRY4;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = STOP_ENTRY3;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case STOP_ENTRY4:
			#ifdef MIDLEVELSM
			printf("MidLevel:STOP_ENTRY4\n");
			#endif

            if (angle_adjustment_event) {
                angle_adj = angle_adjustment;
                angle_adjustment_ack_event = true;
                state = STOP_ENTRY5;
                return CONTINUE;
            } else {
                state = STOP_ENTRY4;
                clock++;
                return WAIT;
            }
        break;
        case STOP_ENTRY5:
			#ifdef MIDLEVELSM
			printf("MidLevel:STOP_ENTRY5\n");
			#endif

            lspeed = lspeed - speed_adj - angle_adj;
            rspeed = rspeed - speed_adj + angle_adj;
            p->output_left_motor(lspeed);
            p->output_right_motor(rspeed);
            state = STOP_DURING;
            return CONTINUE;    
        break;
        case STOP_DURING:
			#ifdef MIDLEVELSM
			printf("MidLevel:STOP_DURING\n");
			#endif

            if (turn_event && turn == Direction_left) {
                direction = turn;
                turn_ack_event = true;
                orientation = turn_left(orientation);
                state = TURN_LEFT_ENTRY0;
                return CONTINUE;
            } else if (turn_event && turn == Direction_right) {
                direction = turn;
                turn_ack_event = true;
                orientation = turn_right(orientation);
                state = TURN_RIGHT_ENTRY0;
                return CONTINUE;
            } else if (clock >= c_step) {
                state = STOP_ENTRY0;
                return CONTINUE;
            } else {
                state = STOP_DURING;
                clock++;
                return WAIT;
            }
        break;


        case TURN_RIGHT_ENTRY0:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_RIGHT_ENTRY0\n");
			#endif

            clock = 0;
            state = TURN_RIGHT_ENTRY1;
            return CONTINUE;
        break;
        case TURN_RIGHT_ENTRY1:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_RIGHT_ENTRY1\n");
			#endif

            if (!in_state) {
                target_speed_event = true;
                target_speed = turn_speed;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_speed_ack_event) {
                    state = TURN_RIGHT_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = TURN_RIGHT_ENTRY1;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case TURN_RIGHT_ENTRY2:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_RIGHT_ENTRY2\n");
			#endif

            if (speed_adjustment_event) {
                speed_adj = speed_adjustment;
                speed_adjustment_ack_event = true;
                state = TURN_RIGHT_ENTRY3;
                return CONTINUE;
            } else {
                state = TURN_RIGHT_ENTRY2;
                clock++;
                return WAIT;
            }
        break;
        case TURN_RIGHT_ENTRY3:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_RIGHT_ENTRY3\n");
			#endif

            if (!in_state) {
                target_angle_event = true;
                target_angle = right_angle;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_angle_ack_event) {
                    state = TURN_RIGHT_ENTRY4;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = TURN_LEFT_ENTRY3;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case TURN_RIGHT_ENTRY4:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_RIGHT_ENTRY4\n");
			#endif

            if (angle_adjustment_event) {
                angle_adj = angle_adjustment;
                angle_adjustment_ack_event = true;
                state = TURN_RIGHT_ENTRY5;
                return CONTINUE;
            } else {
                state = TURN_RIGHT_ENTRY4;
                clock++;
                return WAIT;
            }
        break;
        case TURN_RIGHT_ENTRY5:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_RIGHT_ENTRY5\n");
			#endif

            lspeed = lspeed - speed_adj - angle_adj;
            rspeed = rspeed - speed_adj + angle_adj;
            p->output_left_motor(lspeed);
            p->output_right_motor(rspeed);
            state = TURN_RIGHT_DURING;
            return CONTINUE;
        break;
        case TURN_RIGHT_DURING:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_RIGHT_DURING\n");
			#endif

            if (move_forward_event && (orientation == Orientation_left || orientation == Orientation_right)) {
                move_forward_ack_event = true;
                lspeed = movesp;
                rspeed = movesp;
                state = GO_SIDE_ENTRY0;
                return CONTINUE;
            } else if (move_forward_event && orientation == Orientation_down) {
                move_forward_ack_event = true;
                lspeed = movesp;
                rspeed = movesp;
                state = GO_DOWN_ENTRY0;
                return CONTINUE;
            } else if (move_forward_event && orientation == Orientation_up) {
                move_forward_ack_event = true;
                lspeed = movesp;
                rspeed = movesp;
                state = GO_UP_ENTRY0;
                return CONTINUE;
            } else if (turn_event && turn == Direction_right) {
                direction = turn;
                turn_ack_event = true;
                orientation = turn_right(orientation);
                state = TURN_RIGHT_ENTRY0;
                return CONTINUE;
            } else if (turn_event && turn == Direction_left) {
                direction = turn;
                turn_ack_event = true;
                orientation = turn_left(orientation);
                state = TURN_LEFT_ENTRY0;
                return CONTINUE;
            } else if (clock >= c_step) {
                state = TURN_RIGHT_ENTRY0;
                return CONTINUE;
            } else {
                state = TURN_RIGHT_DURING;
                clock++;
                return WAIT;
            }
        break;


        case TURN_LEFT_ENTRY0:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_LEFT_ENTRY0\n");
			#endif

            clock = 0;
            state = TURN_LEFT_ENTRY1;
            return CONTINUE;
        break;
        case TURN_LEFT_ENTRY1:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_LEFT_ENTRY1\n");
			#endif

            if (!in_state) {
                target_speed_event = true;
                target_speed = turn_speed;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_speed_ack_event) {
                    state = TURN_LEFT_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = TURN_LEFT_ENTRY1;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case TURN_LEFT_ENTRY2:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_LEFT_ENTRY2\n");
			#endif

            if (speed_adjustment_event) {
                speed_adj = speed_adjustment;
                speed_adjustment_ack_event =true;
                state = TURN_LEFT_ENTRY3;
                return CONTINUE;
            } else {
                state = TURN_LEFT_ENTRY2;
                clock++;
                return WAIT;
            }
        break;
        case TURN_LEFT_ENTRY3:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_LEFT_ENTRY3\n");
			#endif

            if (!in_state) {
                target_angle_event = true;
                target_angle = left_angle;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_angle_ack_event) {
                    state = TURN_LEFT_ENTRY4;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = TURN_LEFT_ENTRY3;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case TURN_LEFT_ENTRY4:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_LEFT_ENTRY4\n");
			#endif

            if (angle_adjustment_event) {
                angle_adj = angle_adjustment;
                angle_adjustment_ack_event = true;
                state = TURN_LEFT_ENTRY5;
                return CONTINUE;
            } else {
                state = TURN_LEFT_ENTRY4;
                clock++;
                return WAIT;
            }
        break;
        case TURN_LEFT_ENTRY5:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_LEFT_ENTRY5\n");
			#endif

            lspeed = lspeed - speed_adj - angle_adj;
            rspeed = rspeed - speed_adj + angle_adj;
            p->output_left_motor(lspeed);
            p->output_right_motor(rspeed);
            state = TURN_LEFT_DURING;
            return CONTINUE;
        break;
        case TURN_LEFT_DURING:
			#ifdef MIDLEVELSM
			printf("MidLevel:TURN_LEFT_DURING\n");
			#endif

            if (move_forward_event && (orientation == Orientation_left || orientation == Orientation_right)) {
                move_forward_ack_event = true;
                lspeed = movesp;
                rspeed = movesp;
                state = GO_SIDE_ENTRY0;
                return CONTINUE;
            } else if (move_forward_event && orientation == Orientation_down) {
                move_forward_ack_event = true;
                lspeed = movesp;
                rspeed = movesp;
                state = GO_DOWN_ENTRY0;
                return CONTINUE;
            } else if (move_forward_event && orientation == Orientation_up) {
                move_forward_ack_event = true;
                lspeed = movesp;
                rspeed = movesp;
                state = GO_UP_ENTRY0;
                return CONTINUE;
            } else if (turn_event && turn == Direction_right) {
                direction = turn;
                turn_ack_event = true;
                orientation = turn_right(orientation);
                state = TURN_RIGHT_ENTRY0;
                return CONTINUE;
            } else if (turn_event && turn == Direction_left) {
                direction = turn;
                turn_ack_event = true;
                orientation = turn_left(orientation);
                state = TURN_LEFT_ENTRY0;
                return CONTINUE;
            } else if (clock >= c_step) {
                state = TURN_LEFT_ENTRY0;
                return CONTINUE;
            } else {
                state = TURN_LEFT_DURING;
                clock++;
                return WAIT;
            }
        break;


        case GO_SIDE_ENTRY0:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_SIDE_ENTRY0\n");
			#endif

            clock = 0;
            state = GO_SIDE_ENTRY1;
            return CONTINUE;
        break;
        case GO_SIDE_ENTRY1:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_SIDE_ENTRY1\n");
			#endif

            if (!in_state) {
                target_speed_event = true;
                target_speed = turn_speed;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_speed_ack_event) {
                    state = GO_SIDE_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_SIDE_ENTRY1;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case GO_SIDE_ENTRY2:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_SIDE_ENTRY2\n");
			#endif

            if (speed_adjustment_event) {
                speed_adj = speed_adjustment;
                speed_adjustment_ack_event = true;
                state = GO_SIDE_ENTRY3;
                return CONTINUE;
            } else {
                state = GO_SIDE_ENTRY2;
                clock++;
                return WAIT;
            }
        break;
        case GO_SIDE_ENTRY3:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_SIDE_ENTRY3\n");
			#endif

            if (!in_state) {
                target_angle_event = true;
                target_angle = forward_angle;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_angle_ack_event) {
                    state = GO_SIDE_ENTRY4;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_SIDE_ENTRY3;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case GO_SIDE_ENTRY4:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_SIDE_ENTRY4\n");
			#endif

            if (angle_adjustment_event) {
                angle_adj = angle_adjustment;
                angle_adjustment_ack_event = true;
                state = GO_SIDE_ENTRY5;
                return CONTINUE;
            } else {
                state = GO_SIDE_ENTRY4;
                clock++;
                return WAIT;
            }
        break;
        case GO_SIDE_ENTRY5:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_SIDE_ENTRY5\n");
			#endif

            lspeed = lspeed - speed_adj - angle_adj;
            rspeed = rspeed - speed_adj + angle_adj;
            p->output_left_motor(lspeed);
            p->output_right_motor(rspeed);
            state = GO_SIDE_DURING;
            return CONTINUE;
        break;
        case GO_SIDE_DURING:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_SIDE_DURING\n");
			#endif

            if (turn_event && turn == Direction_right) {
                direction = turn;
                turn_ack_event = true;
                orientation = turn_right(orientation); lspeed = stopsp; rspeed = stopsp;
                state = TURN_RIGHT_ENTRY0;
                return CONTINUE;
            } else if (turn_event && turn == Direction_left) {
                direction = turn;
                turn_ack_event = true;
                orientation = turn_left(orientation); lspeed = stopsp; rspeed = stopsp;
                state = TURN_LEFT_ENTRY0;
                return CONTINUE;
            } else if (stop_event) {
                stop_ack_event = true;
                lspeed = stopsp; rspeed = stopsp;
                state = STOP_ENTRY0;
                return CONTINUE;
            } else if (clock >= c_step) {
                state = GO_SIDE_ENTRY0;
                return CONTINUE;
            } else {
                state = GO_SIDE_DURING;
                clock++;
                return WAIT;
            }
        break;


        case GO_DOWN_ENTRY0:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_DOWN_ENTRY0\n");
			#endif

            clock = 0;
            state = GO_DOWN_ENTRY1;
            return CONTINUE;
        break;
        case GO_DOWN_ENTRY1:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_DOWN_ENTRY1\n");
			#endif

            if (!in_state) {
                target_speed_event = true;
                target_speed = climb_speed;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_speed_ack_event) {
                    state = GO_DOWN_ENTRY2;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_ENTRY1;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case GO_DOWN_ENTRY2:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_DOWN_ENTRY2\n");
			#endif

                if (speed_adjustment_event) {
                    speed_adj = speed_adjustment;
                    speed_adjustment_ack_event = true;
                    state = GO_DOWN_ENTRY3;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_ENTRY2;
                    clock++;
                    return WAIT;
                }
        break;
        case GO_DOWN_ENTRY3:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_DOWN_ENTRY3\n");
			#endif

            if (!in_state) {
                target_angle_event = true;
                target_angle = forward_angle;
                in_state = true;
                return CONTINUE;
            } else {
                if (target_angle_ack_event) {
                    state = GO_DOWN_ENTRY4;
                    in_state = false;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_ENTRY3;
                    in_state = false;
                    clock++;
                    return WAIT;
                }
            }
        break;
        case GO_DOWN_ENTRY4:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_DOWN_ENTRY4\n");
			#endif

            if (angle_adjustment_event) {
                angle_adj = angle_adjustment;
                angle_adjustment_ack_event = true;
                state = GO_DOWN_ENTRY5;
                return CONTINUE;
            } else {
                state = GO_DOWN_ENTRY4;
                clock++;
                return WAIT;
            }
        break;
        case GO_DOWN_ENTRY5:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_DOWN_ENTRY5\n");
			#endif

            // entry action
            lspeed = lspeed - speed_adj - angle_adj;
            rspeed = rspeed - speed_adj + angle_adj;
            p->output_left_motor(lspeed);
            p->output_right_motor(rspeed);
            // immediate transition
            state = GO_DOWN_DURING;
            return CONTINUE;
        break;
        case GO_DOWN_DURING:
			#ifdef MIDLEVELSM
			printf("MidLevel:GO_DOWN_DURING\n");
			#endif

                if (stop_event) {
                    stop_ack_event = true;
                   lspeed = stopsp; rspeed = stopsp;
                    state = STOP_ENTRY0;
                    return CONTINUE;
                } else if (turn_event && turn == Direction_left) {
                    direction = turn;
                    turn_ack_event = true;
                    orientation = turn_left(orientation); lspeed = stopsp; rspeed = stopsp;
                    state = TURN_LEFT_ENTRY0;
                    return CONTINUE;
                } else if (turn_event && turn == Direction_right) {
                    direction = turn;
                    turn_ack_event = true;
                    orientation = turn_right(orientation); lspeed = stopsp; rspeed = stopsp;
                    state = TURN_RIGHT_ENTRY0;
                    return CONTINUE;
                } else if (clock >= c_step) {
                    state = GO_DOWN_ENTRY0;
                    return CONTINUE;
                } else {
                    state = GO_DOWN_DURING;
                    clock++;
                    return WAIT;
                }
        break;
        default:
            #ifdef MIDLEVELSM
			printf("This state should not be reachable");
			#endif

            return WAIT;
        break;
    }
}
