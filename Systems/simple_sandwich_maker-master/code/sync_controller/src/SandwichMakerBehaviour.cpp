#include "SandwichMakerBehaviour.hpp"
#include "SandwichMakerPlatform.hpp"
#include "FIFO.hpp"
#include <math.h>
#include <iostream>

using namespace std;

SandwichMakerBehaviour::SandwichMakerBehaviour(SandwichMakerPlatform* _p) {
    p = _p;
}

SandwichMakerBehaviour::~SandwichMakerBehaviour() {
}

void SandwichMakerBehaviour::step() {
    while(simDectectionBehaviourStm_step() == CONTINUE) {
    }
}

STEP_RETURN SandwichMakerBehaviour::go_home() {
    float newx = HOME_X + 0.47;
    float newy = HOME_Y;
    double aux = sqrt(newx*newx+newy*newy);
    float theta = acos(newx/aux);
    float radius = aux - 0.32;
    // I'm not considering the time it takes to lift due to the
    // assumption that it is extremely fast
    p->lift();
    STEP_RETURN v = pid(theta, radius);
    while (v == CONTINUE) {
        v = pid(theta, radius);
    }
    if (v == TERMINATED) pid_state = 0;
    return v;
}

STEP_RETURN SandwichMakerBehaviour::intercept(float x, float y, float t)
{
    float newx = x + 0.47;
    float newy = y + p->step_size * (p->beltVelocity);
    double aux = sqrt(newx * newx + newy * newy);
    float theta = acos(newx / aux);
    float radius = aux - 0.32;
    STEP_RETURN v = pid(theta, radius);
    while (v == CONTINUE)
    {
        v = pid(theta, radius);
    }
    if (v == TERMINATED)
        pid_state = 0;
    return v;
}

float get_x(float t, float r) {
    return r*cos(t);
}

float get_y(float t, float r) {
    return r*sin(t);
}

STEP_RETURN SandwichMakerBehaviour::pid(float p_theta, float p_radius) {
    t_theta = p_theta;
    t_radius = p_radius;
    float target_x = t_radius*cos(t_theta);
    float target_y = t_radius*sin(t_theta);
    switch (pid_state) {
        case 0: {
            p_e_theta = t_theta - p->c_theta;
            p_e_radius = t_radius - p->c_radius;
            i_e_radius = 0.0;
            i_e_theta = 0.0;
            d_e_radius = 0.0;
            d_e_theta = 0.0;
            pid_state++;
            return WAIT;
            break;
        }
        case 1: {
            float e_theta = t_theta - p->c_theta;
            float e_radius = t_radius - p->c_radius;
            d_e_theta = (e_theta - p_e_theta)/p->step_size;
            d_e_radius = (e_radius - p_e_radius)/p->step_size;
            i_e_theta += (e_theta - p_e_theta)*p->step_size;
            i_e_radius += (e_radius - p_e_radius)*p->step_size;
            //cout << "target: " << target_x << ", " << target_y << endl;
            //cout << "current: " << get_x(p->c_theta,p->c_radius) << ", " << get_y(p->c_theta,p->c_radius) << endl;
            //cout << "error: " << e_theta << ", " << e_radius << endl;
            if (-epsilon_theta < e_theta && e_theta < epsilon_theta && -epsilon_radius < e_radius && e_radius < epsilon_radius) {
                pid_state++;
            } else {
                p->v_theta = (Pt*e_theta + It*i_e_theta + Dt*d_e_theta);
                p->v_radius = (Pr*e_radius + Ir*i_e_radius + Dr*d_e_radius);
                //cout << "Velocities: " << p->v_theta << ", " << p->v_radius << endl;
            }
            p_e_theta = e_theta;
            p_e_radius = e_radius;
            return WAIT;
            break;
        }
        default: {
            p->v_theta = 0.0;
            p->v_radius = 0.0;
            //cout << "pid done" << endl;
            return TERMINATED;
            break;
        }
    }
}

bool SandwichMakerBehaviour::mstep() {
        bool result=true;
        /* switch(state) {

                case 0:  // state.name is FINDING_FIRST_SHAPE
                           if (p->x1_event){
                                slice_x=p->x1;
                                state=1; //state name="FIRST_SHAPE_X"
                                result= true;
		           }
                           else {
                               result= false;
                           }
		           break;

                case 1:  // state.name is FIRST_SHAPE_X
                           if (p->y1_event){
                                t=p->get_time();
                                slice_y=p->y1;
                                state=2; //state name="FIRST_SHAPE_Y"
                           }
                           result = true;
		           break;

		case 2:  // state.name is FIRST_SHAPE_Y
                           slice_ts=t;
                           state=3; //state.name is S_FOUND_FIRST_SHAPE
                           result=true;
		           break;

	        case 3: //state.name is S_FOUND_FIRST_SHAPE
                           if (slice_x<0){
                                   flag = 0; //First shape is on the left side
		                   shape1_x = slice_x;
		                   shape1_y = slice_y;
		                   shape1_ts = slice_ts;
                                  // printf(" shape1_x=%lf\n", shape1_x);

		            }
                            else if (slice_x>0){ //slice_x>0
                                   flag = 1; //First shape is on the right side
		                   shape2_x = slice_x;
		                   shape2_y = slice_y;
		                   shape2_ts = slice_ts;
                                  // printf(" shape2_x=%lf\n", shape2_x);
		            }
                            else{ //(slice_x == 0)
                               state=5;
                            }
                            T=p->get_time();
                            state=4;   //state name is D_FOUND_FIRST_SHAPE
                            result=true;
                            break;

                case 4: // state.name is D_FOUND_FIRST_SHAPE
                            if (p->get_time()-T<=DELAY_DISCARD){
                                if (p->x2_event == true){
                                        slice_x=p->x2;
                                	state=6; // state.name is SECOND_SHAPE_X
                                        result=true;
                                }
                                else { // p->x_event == false
                                     result=false;
                                }
                            }
                            else{ //p->get_time()-T > DELAY_DISCARD
                                state=5;
                                result= true;
                            }

		            break;

                case 5: // state.name is DISCARDING_SHAPE
                            cout << "Discarding previous shapes." << endl;
                            state=0;    //state name="FINDING_FIRST_SHAPE";
                            result = true;
                            break;

                case 6: // state.name is SECOND_SHAPE_X
                            if (p->y2_event){
                                 t=p->get_time();
                                 slice_y=p->y2;
                                 state=7; //state name="FIRST_SHAPE_Y"
                            }
                            result=true;
                            break;

                case 7: // state.name is SECOND_SHAPE_Y
                            slice_ts=t;
                            if (flag==1 && slice_x<0){
                                 state=8;   // state name="FOUND_SECOND_SHAPE";
                            }
                            else if (flag==0 && slice_x>0){
                                state=8; // state name="FOUND_SECOND_SHAPE";
                            }
                            else{ // (flag==0 && slice_x<0) || (flag==1 && slice_x>0)
                                 state=5;  //state name="DISCARDING_SHAPE";
                            }
                            state=8;
                            result=true;
                            break;

                case 8:    //state.name is FOUND_SECOND_SHAPE
                            if (flag==1){   // First shape is on the right side.
                                   shape1_x = slice_x;
                                   shape1_y = slice_y;
                                   shape1_ts = slice_ts;
                                   // printf(" shape1_x=%lf\n", shape1_x);
                            }
                            if (flag==0){ // First shape is on the left side.
                                   shape2_x = slice_x;
                                   shape2_y = slice_y;
                                   shape2_ts = slice_ts;
                                   // printf(" shape2_x=%lf\n", shape2_x);
                            }
                            state=9;  // state name="S_GO_TO_FIRST_SHAPE";
                            result=true;
                            break;

                case 9: //state.name is S_GO_TO_FIRST_SHAPE
                            aux=UpdateY1_1(shape1_y, BELT_SPEED, p->VERTICAL_SPEED, p->HEIGHT_TO_SHAPE1, shape1_ts, shape2_ts);
                            t= (sqrt(shape1_x*shape1_x+aux*aux/ARM_SPEED))+shape2_ts+0.7;
                            T=p->get_time();
                            state=10;    // state name="D_GO_TO_FIRST_SHAPE";
                            result=true;
                            break;

                case 10: //state.name is D_GO_TO_FIRST_SHAPE
                            if(p->get_time() - T <t){
                               result=false;
                            }
                            else{
                               //shape1_y=aux;
                               state=11;  //state name="ON_FIRST_SHAPE";
                               result=true;
                            }
                            break;

                case 11: //state.name is ON_FIRST_SHAPE
                            state=12;   //state.name is S_LOWER_ON_FIRST_SHAPE
                            result=true;
                            break;

                case 12: //state.name is S_LOWER_ON_FIRST_SHAPE
                            T=p->get_time();
                            state=13;    //state.name is D_LOWER_ON_FIRST_SHAPE
                            result=true;
                            break;

                case 13: //state.name is D_LOWER_ON_FIRST_SHAPE
                            if (p->get_time()-T<p->WAIT_LOWER1){
                                result=false;
                            }
                            else{
                                p->lower(p->HEIGHT_TO_SHAPE1);
                                //printf("The z--coordinate of arm after lowering down on first shape is = %lf\n", p->pz);
                                state=16;   //state.name is S_PICK_FIRST_SHAPE
                                result=true;
                            }
                            break;

                case 14: //state.name is S_PICK_FIRST_SHAPE

                            p->activate_gripper();
                            p->lift();
                            state=15;  //state.name is D_PICK_FIRST_SHAPE
                            //T=p->get_time();
                            result=true;
                            break;

                case 15: //state.name is D_PICK_FIRST_SHAPE
                            //if (p->get_time()-T<WAIT_PICK){
                            //    result=false;
                            //}
                            //else{
                                p->activate_gripper();
                                state=16; //state.name is S_LIFT_FIRST_SHAPE
                                result=true;
                            //}
                            break;

                case 16: //state.name is S_LIFT_FIRST_SHAPE
                            T=p->get_time();
                            result=true;
                            state=17;  //state.name is D_LIFT_FIRST_SHAPE
                            break;

                case 17: //state.name is D_LIFT_FIRST_SHAPE
                            if(p->get_time()-T<p->WAIT_LOWER1){
                                result=false;
                            }
                            else{
                                p->lift();
                                printf("The z--coordinate of arm after lifting up on fisrt shape is = %lf \n", p->pz);
                                state=18;  //state.name is S_GO_TO_SECOND_SHAPE
                                result=true;
                            }
                           break;

                case 18:  //state.name is S_GO_TO_SECOND_SHAPE
                            //aux=UpdateY2(shape2_y, p->py, BELT_SPEED, ARM_SPEED, shape2_ts, t);
                            aux=0;
                            T=p->get_time();
                            t=(aux-(shape2_y)) / BELT_SPEED - shape2_ts;
                            shape2_y=aux;
                            state=19;  //state.name is D_GO_TO_SECOND_SHAPE
                            result=true;
                            break;

                case 19: //state.name is D_GO_TO_SECOND_SHAPE
                            if(p->get_time()-T<=t){
                                result=false;
                            }
                            else{
                                p->move(shape2_x, aux);
                                state=20;  //state.name is ON_SECOND_SHAPE
                                result=true;
                            }
                            break;

                case 20:  //state.name is ON_SECOND_SHAPE
                            state=21;   //state.name is S_LOWER_ON_SECOND_SHAPE
                            result=true;
                            break;

                case 21: //state.name is S_LOWER_ON_SECOND_SHAPE
                            T=p->get_time();
                            state=22;    //state.name is D_LOWER_ON_SECOND_SHAPE
                            result=true;
                            break;

                case 22: //state.name is D_LOWER_ON_SECOND_SHAPE
                            if(p->get_time()-T<=p->WAIT_LOWER2){
                                result=false;
                            }
                            else{
                                p->lower(p->HEIGHT_TO_SHAPE2);
                                printf("The z--coordinate of arm after lowering down on second shape is = %lf\n", p->pz);
                                state=23;   //state.name is S_DROP_ON_SECOND_SHAPE
                                result=true;
                            }
                            break;

                case 23:  //state.name is S_DROP_ON_SECOND_SHAPE
                            T=p->get_time();
                            state=24;  //state.name is D_DROP_ON_SECOND_SHAPE
                            result=true;
                            break;

                case 24: //state.name is D_PICK_FIRST_SHAPE
                            if(p->get_time()-T<WAIT_PICK){
                                result=false;
                            }
                            else{
                                p->deactivate_gripper();
                                state=25; //state.name is S_LIFT_ON_SECOND_SHAPE
                                result=true;
                            }
                            break;

                case 25: //state.name is S_LIFT_ON_SECOND_SHAPE
                            T=p->get_time();
                            state=26;  //state.name is D_LIFT_FIRST_SHAPE
                            result=true;
                            break;

                case 26: //state.name is D_LIFT_ON_SECOND_SHAPE
                            if(p->get_time()-T<p->WAIT_LOWER2){
                                result=false;
                            }
                            else{
                                p->lift();
                                printf("The z--coordinate of arm after lifting up on second shape is = %lf \n", p->pz);
                                state=27;  //state.name is S_GO_TO_SECOND_SHAPE
                                result=true;
                            }
                            break;


                case 27: //state.name is S_GO_TO_HOME_POSITION
                            t= sqrt((shape2_y)*(shape2_y)+(shape2_x)*(shape2_x)) / ARM_SPEED;
                            T=p->get_time();
                            state=28;  //state.name is D_GO_TO_SECOND_SHAPE
                            result=true;
                            break;

                case 28: //state.name is D_GO_TO_HOME_POSITION
                            if(p->get_time()-T<t){
                                result=false;
                            }
                            else{
                                p->go_home();
                                printf("The arm is in home position. \n\n");
                                state=0;  //state.name is FINDING_FIRST_SHAPE
                                result=true;
                            }
                            break;


		default:

                            break;

        } */
        return result;
}


//This function corresponds function UpdateY1 in RoboSim. This function updates the y coordinate of the robot while moving toward shape1.
//moves out of control -- not a good function
//arm moves distance height1 vertically
double SandwichMakerBehaviour::UpdateY1_1(double origin_y1, double belt_speed, double vertical_speed, double height, double sh1TS, double sh2TS){
    double updatedY;
    updatedY= belt_speed * (height/vertical_speed + sh1TS+sh2TS) + origin_y1;
    return updatedY;
}

STEP_RETURN SandwichMakerBehaviour::simDectectionBehaviourStm_step() {
    if (simDectectionBehaviourStm_state == TURNING_ON) {
        if (p->on_event) {
            simDectectionBehaviourStm_state = OPERATION;
            return CONTINUE;
        } else {
            return WAIT;
        }
    } else {
        // simDectectionBehaviourStm_state == OPERATION
        if (p->off()) {
            simDectectionBehaviourStm_state = TURNING_ON;
            return CONTINUE;
        } else {
            // execute substates
            return OPERATION_step();
        }
    }
}

STEP_RETURN SandwichMakerBehaviour::OPERATION_step() {
    switch (OPERATION_state)
    {
        case FINDING_FIRST_SHAPE:
            //printf("FINDING_FIRST_SHAPE\n");
            // since there are no entry action, I don't need to check OPERATION_in_state here
            if (p->lShapeX.event) {
                shape1_x = p->lShapeX.value;
                OPERATION_state = FOUND_FIRST_SHAPE_X;
                OPERATION_in_state = false;
                return CONTINUE;
            } else {
                return WAIT;
            }
            break;
        case FOUND_FIRST_SHAPE_X:
            //printf("FOUND_FIRST_SHAPE_X\n");
            if (p->lShapeY.event) {
                shape1_y = p->lShapeY.value;
                OPERATION_state = FOUND_FIRST_SHAPE_Y;
                OPERATION_in_state = false;
                return CONTINUE;
            } else {
                // this models the fact that there are no alternative transitions that force an exec, so
                // if the event lShapeY is not available, the step diverges, reflecting the deadline 0
                // in the RoboChart model
                return CONTINUE;
            }
            break;
        case FOUND_FIRST_SHAPE_Y:
            if (p->lShapeTS.event) {
                shape1_ts = p->lShapeTS.value;
                OPERATION_state = FOUND_FIRST_SHAPE_TS;
                OPERATION_in_state = false;
                return CONTINUE;
            } else {
                return CONTINUE;
            }
            break;
        case FOUND_FIRST_SHAPE_TS:
            //printf("Found first shape\n");
            if (!OPERATION_in_state) {
                // a state has not yet been entered
                // if a state has an entry action, it must terminate before OPERATION_in_state is set to true.
                // see state S_FOUND_SECOND_SHAPE_TS
                // reset implicit clock
                clock_FOUND_FIRST_SHAPE_TS = 0;       
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                if (clock_FOUND_FIRST_SHAPE_TS <= DELAY_DISCARD && p->rShapeX.event) {
                    shape2_x = p->rShapeX.value;
                    OPERATION_in_state = false;
                    OPERATION_state = FOUND_SECOND_SHAPE_X;
                    return CONTINUE;
                } else if (clock_FOUND_FIRST_SHAPE_TS <= DELAY_DISCARD && !p->rShapeX.event) {
                    // if the delay has not yet been reached, and the event has not been observed, increment
                    // clock and wait a cycle
                    clock_FOUND_FIRST_SHAPE_TS++;
                    return WAIT;
                } else {
                    // otherwise: delay has been violated
                    // go to first state of OPERATION, not sure if time should pass here
                    OPERATION_in_state = false;
                    OPERATION_state = FINDING_FIRST_SHAPE;
                    return CONTINUE;
                }
            }
            
            break;
        case FOUND_SECOND_SHAPE_X:
            if (p->rShapeY.event) {
                shape2_y = p->rShapeY.value;
                OPERATION_state = FOUND_SECOND_SHAPE_Y;
                OPERATION_in_state = false;
                return CONTINUE;
            } else {
                return CONTINUE;
            }
            break;
        case FOUND_SECOND_SHAPE_Y:
            if (p->rShapeTS.event) {
                shape2_ts = p->rShapeTS.value;
                OPERATION_state = FOUND_SECOND_SHAPE_TS;
                OPERATION_in_state = false;
                return CONTINUE;
            } else {
                return CONTINUE;
            }
            break;
        case FOUND_SECOND_SHAPE_TS:
            //printf("Found second shape");
            if (!OPERATION_in_state) {
                STEP_RETURN r = intercept(shape1_x, shape1_y, shape1_ts);
                if (r == TERMINATED) {
                    OPERATION_in_state = true;    
                    return CONTINUE;
                } else {
                    return r;
                }   
            } else {
                OPERATION_in_state = false;
                OPERATION_state = S_LOWER_ON_FIRST_SHAPE;
                return CONTINUE;
            }
            break;
        case S_LOWER_ON_FIRST_SHAPE:
            if (!OPERATION_in_state) {
                p->pick();
                p->lower(p->HEIGHT_TO_SHAPE1);
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                OPERATION_in_state = false;
                OPERATION_state = D_LOWER_ON_FIRST_SHAPE;
                return CONTINUE;
            }
            break;
        case D_LOWER_ON_FIRST_SHAPE:
            if (!OPERATION_in_state) {
                clock_D_LOWER_ON_FIRST_SHAPE = 0;
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                if (clock_D_LOWER_ON_FIRST_SHAPE <= WAIT_LOWER) {
                    clock_D_LOWER_ON_FIRST_SHAPE++;
                    return WAIT;
                } else {
                    OPERATION_in_state = false;
                    OPERATION_state = S_PICK_FIRST_SHAPE;
                    return CONTINUE;
                }
            }
            break;
        case S_PICK_FIRST_SHAPE:
            if (!OPERATION_in_state) {
                p->pick();
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                OPERATION_in_state = false;
                OPERATION_state = D_PICK_FIRST_SHAPE;
                return CONTINUE;
            }
            break;
        case D_PICK_FIRST_SHAPE:
            if (!OPERATION_in_state) {
                clock_D_PICK_FIRST_SHAPE = 0;
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                if (clock_D_PICK_FIRST_SHAPE <= WAIT_PICK) {
                    clock_D_PICK_FIRST_SHAPE++;
                    return WAIT;
                } else {
                    OPERATION_in_state = false;
                    OPERATION_state = S_LIFT_FIRST_SHAPE;
                    return CONTINUE;
                }
            }
            break;
        case S_LIFT_FIRST_SHAPE:
            if (!OPERATION_in_state) {
                p->lift();
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                OPERATION_in_state = false;
                OPERATION_state = D_LIFT_FIRST_SHAPE;
                return CONTINUE;
            }
            break;
        case D_LIFT_FIRST_SHAPE:
            if (!OPERATION_in_state) {
                clock_D_LIFT_FIRST_SHAPE = 0;
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                if (clock_D_LIFT_FIRST_SHAPE <= WAIT_LIFT) {
                    clock_D_LIFT_FIRST_SHAPE++;
                    return WAIT;
                } else {
                    OPERATION_in_state = false;
                    OPERATION_state = GO_TO_SECOND_SHAPE;
                    return CONTINUE;
                }
            }
            break;
        case GO_TO_SECOND_SHAPE:
            if (!OPERATION_in_state) {
                STEP_RETURN r = intercept(shape2_x, shape2_y, shape2_ts);
                if (r == TERMINATED) {
                    OPERATION_in_state = true;    
                    return CONTINUE;
                } else {
                    return r;
                }   
            } else {
                OPERATION_in_state = false;
                OPERATION_state = S_LOWER_ON_SECOND_SHAPE;
                return CONTINUE;
            }
            break;
        case S_LOWER_ON_SECOND_SHAPE:
            if (!OPERATION_in_state) {
                p->lower(p->HEIGHT_TO_SHAPE2);
                p->drop();
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                OPERATION_in_state = false;
                OPERATION_state = D_LOWER_ON_SECOND_SHAPE;
                return CONTINUE;
            }
            break;
        case D_LOWER_ON_SECOND_SHAPE:
            if (!OPERATION_in_state) {
                clock_D_LOWER_ON_SECOND_SHAPE = 0;
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                if (clock_D_LOWER_ON_SECOND_SHAPE <= WAIT_LOWER) {
                    clock_D_LOWER_ON_SECOND_SHAPE++;
                    return WAIT;
                } else {
                    OPERATION_in_state = false;
                    OPERATION_state = S_DROP_ON_SECOND_SHAPE;
                    return CONTINUE;
                }
            }
            break;
        case S_DROP_ON_SECOND_SHAPE:
            if (!OPERATION_in_state) {
                p->drop();
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                OPERATION_in_state = false;
                OPERATION_state = D_DROP_ON_SECOND_SHAPE;
                return CONTINUE;
            }
            break;
        case D_DROP_ON_SECOND_SHAPE:
            if (!OPERATION_in_state) {
                clock_D_DROP_ON_SECOND_SHAPE = 0;
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                if (clock_D_DROP_ON_SECOND_SHAPE <= WAIT_DROP) {
                    clock_D_DROP_ON_SECOND_SHAPE++;
                    return WAIT;
                } else {
                    OPERATION_in_state = false;
                    OPERATION_state = S_LIFT_ON_SECOND_SHAPE;
                    return CONTINUE;
                }
            }
            break;
        case S_LIFT_ON_SECOND_SHAPE:
            if (!OPERATION_in_state) {
                p->lift();
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                OPERATION_in_state = false;
                OPERATION_state = D_LIFT_ON_SECOND_SHAPE;
                return CONTINUE;
            }
            break;
        case D_LIFT_ON_SECOND_SHAPE:
            if (!OPERATION_in_state) {
                clock_D_LIFT_ON_SECOND_SHAPE = 0;
                OPERATION_in_state = true;
                return CONTINUE;
            } else {
                if (clock_D_LIFT_ON_SECOND_SHAPE <= WAIT_LIFT) {
                    clock_D_LIFT_ON_SECOND_SHAPE++;
                    return WAIT;
                } else {
                    OPERATION_in_state = false;
                    OPERATION_state = GO_TO_HOME_POSITION;
                    return CONTINUE;
                }
            }
            break;
        case GO_TO_HOME_POSITION:
            if (!OPERATION_in_state) {
                STEP_RETURN r = go_home();
                if (r == TERMINATED) {
                    OPERATION_in_state = true;    
                    return CONTINUE;
                } else {
                    return r;
                }   
            } else {
                OPERATION_in_state = false;
                OPERATION_state = FINDING_FIRST_SHAPE;
                return CONTINUE;
            }
            break;
        default:
            printf("This state should not be reachable");
            return WAIT;
            break;
    }
    

}