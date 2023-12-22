#include "VacuumCleanerBehaviour.hpp"
#include "VacuumCleanerPlatform.hpp"
#include "OutputLeftMotorOp.hpp"
#include "OutputRightMotorOp.hpp"
// #include "PIDOp.hpp"
#include "FIFO.hpp"
#include <math.h>
#include <iostream>
#include "Operation.hpp"

#include "statemachines/AngularSpeedSM.hpp"
#include "statemachines/CleanSM.hpp"
#include "statemachines/DisplacementSM.hpp"
#include "statemachines/InputDuplicationSM.hpp"
#include "statemachines/LinearSpeedSM.hpp"
#include "statemachines/MidLevelSM.hpp"
#include "statemachines/PathPlanningSM.hpp"
#include "statemachines/PID.hpp"
#include "statemachines/SpeedSM.hpp"



struct TripleAxis;

using namespace std;

VacuumCleanerBehaviour::VacuumCleanerBehaviour(VacuumCleanerPlatform *_p)
{
    p = _p;
    angularspeedsm = new AngularSpeedSM(p);
    cleansm = new CleanSM(p);
    displacementsm = new DisplacementSM(p);
    inputduplicationsm1 = new InputDuplicationSM(p);
    inputduplicationsm2 = new InputDuplicationSM(p);
    inputduplicationsm3 = new InputDuplicationSM(p);
    linearspeedsm = new LinearSpeedSM(p);
    midlevelsm = new MidLevelSM(p);
    pathplanningsm = new PathPlanningSM(p);
    pid1 = new PID(p);
    pid2 = new PID(p);
    speedsm1 = new SpeedSM(p);
    speedsm2 = new SpeedSM(p);


    // interceptop = new InterceptOp(p, this);
    // pidop = new PIDOp(p, this);
}

VacuumCleanerBehaviour::~VacuumCleanerBehaviour()
{
}

/* 
 * In the current implementation, events may propagate between state machines within a step.
 * We can change that by setting up all state machine inputs before calling all steps.
 */

void VacuumCleanerBehaviour::step()
{
    // read from the platform -- for multicontroller model, we need to figure out exactly where this is done.
    // Ideally, it should be done at the level of a Module, but in the current frameworks this is not available,
    // so it is being done here. In this model, it is not too much of a problem as there is a single controller.

    acc_l_event = p->acc_l_event;
    acc_l = p->acc_l;

    acc_r_event = p->acc_r_event;
    acc_r = p->acc_r;

    ultrasonic_event = p->ultrasonic_event;
    ultrasonic = p->ultrasonic;

    charging_event = p->charging_event;

    battery_level_event = p->battery_level_event;
    battery_level = p->battery_level;


    if (warm_up_counter < WARM_UP) {
        warm_up_counter++;
        // trying to callibraate the accelerometer
        p->base_lX = acc_l.X;
        p->base_lY = acc_l.Y;
        p->base_lZ = acc_l.Z;
        p->base_rX = acc_r.X;
        p->base_rY = acc_r.Y;
        p->base_rZ = acc_r.Z;
        return;
    }

    acc_l.X -= p->base_lX;
    acc_l.Y -= p->base_lY;
    acc_l.Z -= p->base_lZ;
    acc_r.X -= p->base_rX;
    acc_r.Y -= p->base_rY;
    acc_r.Z -= p->base_rZ;


    // p->output_left_motor(1);
    // p->output_right_motor(1);
    printf("(%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f),%.2f\n",acc_l.X,acc_l.Y,acc_l.Z,acc_r.X,acc_r.Y,acc_r.Z,ultrasonic);
    // return;

    // printf("Running controller with events:\n");
    // printf("\t acc_l: %s, (%f,%f,%f)\n", acc_l_event?"true":"false", acc_l.X, acc_l.Y, acc_l.Z);
    // printf("\t acc_r: %s, (%f,%f,%f)\n", acc_r_event?"true":"false", acc_r.X, acc_r.Y, acc_r.Z);
    // printf("\t ultrasonic: %s, %f\n", ultrasonic_event?"true":"false", ultrasonic);

    /* setting up input connections of pathplanningsm */
    pathplanningsm->battery_level = battery_level;
    pathplanningsm->battery_level_event = battery_level_event;
    
    pathplanningsm->ultrasonic = ultrasonic;
    pathplanningsm->ultrasonic_event  = ultrasonic_event;
    
    pathplanningsm->charging_event  = charging_event;

    pathplanningsm->displacement_in = displacementsm->displacement_out;
    pathplanningsm->displacement_in_event = displacementsm->displacement_out_event;
    displacementsm->displacement_out_ack_event = pathplanningsm->displacement_in_ack_event;

    /* setting up input connections of inputduplicationsm1 */
    inputduplicationsm1->i = acc_r;
    inputduplicationsm1->i_event = acc_r_event;

    /* setting up input connections of inputduplicationsm2 */
    inputduplicationsm2->i = acc_l;
    inputduplicationsm2->i_event = acc_l_event;

    /* setting up input connections of cleansm */
    cleansm->clean = pathplanningsm->clean;
    cleansm->clean_event = pathplanningsm->clean_event;
    pathplanningsm->clean_ack_event = cleansm->clean_ack_event;
    
    /* setting up input connections of midlevelsm */
    midlevelsm->move_forward_event = true;//pathplanningsm->move_forward_event;
    pathplanningsm->move_forward_ack_event = midlevelsm->move_forward_ack_event;

    midlevelsm->turn = pathplanningsm->turn;
    midlevelsm->turn_event = false; //pathplanningsm->turn_event;
    pathplanningsm->turn_ack_event = midlevelsm->turn_ack_event;

    midlevelsm->stop_event = pathplanningsm->stop_event;
    pathplanningsm->stop_ack_event = false;//midlevelsm->stop_ack_event;

    midlevelsm->angle_adjustment = pid1->err_output;
    midlevelsm->angle_adjustment_event = pid1->err_output_event;
    pid1->err_ack_event = midlevelsm->angle_adjustment_ack_event;

    midlevelsm->speed_adjustment = pid2->err_output;
    midlevelsm->speed_adjustment_event = pid2->err_output_event;
    pid2->err_ack_event = midlevelsm->speed_adjustment_ack_event;

    /* setting up input connections of angularspeedsm */
    angularspeedsm->acc_r = inputduplicationsm1->o2;
    angularspeedsm->acc_r_event = inputduplicationsm1->o2_event;
    inputduplicationsm1->o_ack2_event = angularspeedsm->acc_r_ack_event;

    angularspeedsm->acc_l = inputduplicationsm2->o2;
    angularspeedsm->acc_l_event = inputduplicationsm2->o2_event;
    inputduplicationsm2->o_ack2_event = angularspeedsm->acc_l_ack_event;

    /* setting up input connection of speedsm1 */
    speedsm1->acc = inputduplicationsm1->o1;
    speedsm1->acc_event = inputduplicationsm1->o1_event;
    inputduplicationsm1->o_ack1_event = speedsm1->acc_ack_event;

    /* setting up input connection of speedsm2 */
    speedsm2->acc = inputduplicationsm2->o1;
    speedsm2->acc_event = inputduplicationsm2->o1_event;
    inputduplicationsm2->o_ack1_event = speedsm2->acc_ack_event;

    /* setting up input connections PID */
    pid1->actual = angularspeedsm->angle;
    pid1->actual_event = angularspeedsm->angle_event;
    angularspeedsm->angle_ack_event = pid1->actual_ack_event;

    pid1->target = midlevelsm->target_angle;
    pid1->target_event = midlevelsm->target_angle_event;
    midlevelsm->target_angle_ack_event = pid1->target_ack_event;

    /* setting up input connections of inputduplicationsm3 */
    inputduplicationsm3->i = speedsm1->sp;
    inputduplicationsm3->i_event = speedsm1->sp_event;
    speedsm1->sp_ack_event = inputduplicationsm3->i_ack_event;

    /* setting up input connections of linearspeedsm */
    linearspeedsm->speed_l = speedsm2->sp;
    linearspeedsm->speed_l_event = speedsm2->sp_event;
    speedsm2->sp_ack_event = linearspeedsm->speed_l_ack_event;

    linearspeedsm->speed_r = inputduplicationsm3->o2;
    linearspeedsm->speed_r_event = inputduplicationsm3->o2_event;
    inputduplicationsm3->o_ack2_event = linearspeedsm->speed_r_ack_event;

    /* setting up input connections of displacementsm */
    displacementsm->displacement_in = pathplanningsm->displacement_out;
    displacementsm->displacement_in_event = pathplanningsm->displacement_out_event;
    pathplanningsm->displacement_out_ack_event = displacementsm->displacement_in_ack_event;

    displacementsm->sp = inputduplicationsm3->o1;
    displacementsm->sp_event = inputduplicationsm3->o1_event;
    inputduplicationsm3->o_ack1_event = displacementsm->sp_ack_event;

    /* setting up input connections PID */
    pid2->actual = linearspeedsm->linear_speed;
    pid2->actual_event = linearspeedsm->linear_speed_event;
    linearspeedsm->ls_ack_event = pid2->actual_ack_event;

    pid2->target = midlevelsm->target_speed;
    pid2->target_event = midlevelsm->target_speed_event;
    midlevelsm->target_speed_ack_event = pid2->target_ack_event;


    // resets output events after passing them onto the corresponding input events

    /* setting up input connections of pathplanningsm */
    // There is a deadlock in the code. I'm trying to prevent it
    // by making the displacement_out_event of DisplacementSM's always available.
    // displacementsm->displacement_out_event = false;
    pathplanningsm->displacement_in_ack_event = false;

    /* setting up input connections of cleansm */
    pathplanningsm->clean_event = false;
    cleansm->clean_ack_event = false;
    
    /* setting up input connections of midlevelsm */
    pathplanningsm->move_forward_event = false;
    midlevelsm->move_forward_ack_event = false;

    pathplanningsm->turn_event = false;
    midlevelsm->turn_ack_event = false;

    pathplanningsm->stop_event = false;
    midlevelsm->stop_ack_event = false;

    // There is a deadlock in the code. I'm trying to prevent it
    // by making the err_output_event of PID's always available.
    // pid1->err_output_event = false;
    midlevelsm->angle_adjustment_ack_event = false;

    // There is a deadlock in the code. I'm trying to prevent it
    // by making the err_output_event of PID's always available.
    // pid2->err_output_event = false;
    midlevelsm->speed_adjustment_ack_event = false;

    /* setting up input connections of angularspeedsm */
    inputduplicationsm1->o2_event = false;
    angularspeedsm->acc_r_ack_event = false;

    inputduplicationsm2->o2_event = false;
    angularspeedsm->acc_l_ack_event = false;

    /* setting up input connection of speedsm1 */
    inputduplicationsm1->o1_event = false;
    speedsm1->acc_ack_event = false;

    /* setting up input connection of speedsm2 */
    inputduplicationsm2->o1_event = false;
    speedsm2->acc_ack_event = false;

    /* setting up input connections PID */
    angularspeedsm->angle_event = false;
    pid1->actual_ack_event = false;

    midlevelsm->target_angle_event = false;
    pid1->target_ack_event = false;

    /* setting up input connections of inputduplicationsm3 */
    speedsm1->sp_event = false;
    inputduplicationsm3->i_ack_event = false;

    /* setting up input connections of linearspeedsm */
    speedsm2->sp_event = false;
    linearspeedsm->speed_l_ack_event = false;

    inputduplicationsm3->o2_event = false;
    linearspeedsm->speed_r_ack_event = false;

    /* setting up input connections of displacementsm */
    pathplanningsm->displacement_out_event = false;
    displacementsm->displacement_in_ack_event = false;

    inputduplicationsm3->o1_event = false;
    displacementsm->sp_ack_event = false;

    /* setting up input connections PID */
    linearspeedsm->linear_speed_event = false;
    pid2->actual_ack_event = false;

    midlevelsm->target_speed_event = false;
    pid2->target_ack_event = false;

    // printf("pathplanningsm\n");
    pathplanningsm->step();
    // printf("inputduplicationsm1\n");
    inputduplicationsm1->step();
    // printf("inputduplicationsm2\n");
    inputduplicationsm2->step();
    // printf("cleansm\n");
    cleansm->step();
    // printf("midlevelsm\n");
    midlevelsm->step();
    // printf("angularspeedsm\n");
    angularspeedsm->step();
    // printf("speedsm1\n");
    speedsm1->step();
    // printf("speedsm2\n");
    speedsm2->step();
    // printf("pid1\n");
    pid1->step();
    // printf("inputduplicationsm3\n");
    inputduplicationsm3->step();
    // printf("linearspeedsm\n");
    linearspeedsm->step();
    // printf("displacementsm\n");
    displacementsm->step();
    // printf("pid2\n");
    pid2->step();
    printf("------------------------------\n");

    // printf("\n\n");
    // cout << "Press any key to continue . . .\n";
    // cin.get();
    // cin.clear();
}