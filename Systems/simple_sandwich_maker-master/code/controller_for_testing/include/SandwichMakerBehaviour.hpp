#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"

class SandwichMakerPlatform;
class InterceptOp;
class PIDOp;

enum STATES
{
    FINDING_FIRST_SHAPE,
    FOUND_FIRST_SHAPE_X,
    FOUND_FIRST_SHAPE_Y,
    FOUND_FIRST_SHAPE_TS,
    FOUND_SECOND_SHAPE_X,
    FOUND_SECOND_SHAPE_Y,
    FOUND_SECOND_SHAPE_TS,
    S_LOWER_ON_FIRST_SHAPE,
    D_LOWER_ON_FIRST_SHAPE,
    S_PICK_FIRST_SHAPE,
    D_PICK_FIRST_SHAPE,
    S_LIFT_FIRST_SHAPE,
    D_LIFT_FIRST_SHAPE,
    GO_TO_SECOND_SHAPE,
    S_LOWER_ON_SECOND_SHAPE,
    D_LOWER_ON_SECOND_SHAPE,
    S_DROP_ON_SECOND_SHAPE,
    D_DROP_ON_SECOND_SHAPE,
    S_LIFT_ON_SECOND_SHAPE,
    D_LIFT_ON_SECOND_SHAPE,
    GO_TO_HOME_POSITION
};

class SandwichMakerBehaviour : public Behaviour
{
    SandwichMakerPlatform *p;
    InterceptOp *interceptop = nullptr;
    PIDOp *pidop = nullptr;

    // behaviour specific variables -- CAN CHANGE THESE
    double slice_x;
    double slice_y;
    double slice_ts;
    double shape1_x;
    double shape1_y;
    double shape1_ts;
    double shape2_x;
    double shape2_y;
    double shape2_ts;
    double lift_ts;
    double x, y;

    // implicit clocks used to model sinceEntry conditions
    int clock_FOUND_FIRST_SHAPE_TS;
    int clock_D_LOWER_ON_FIRST_SHAPE;
    int clock_D_PICK_FIRST_SHAPE;
    int clock_D_LIFT_FIRST_SHAPE;
    int clock_D_LOWER_ON_SECOND_SHAPE;
    int clock_D_DROP_ON_SECOND_SHAPE;
    int clock_D_LIFT_ON_SECOND_SHAPE;

    double T = 0;
    double t = 0;
    double aux = 0;
    int flag = 0;

    //The constants are not defined as const because of the parameters of remoteApi functions.
    double BELT_SPEED = 0.132; //(m/s) //0.132
    double ARM_SPEED = 0.132;  //(m/s) NEEDS UPDATING //
    double HOME_X = 0.0;
    double HOME_Y = 0.0;
    double HOME_Z = 0.235;
    double HOME_R = 0.0;
    double DELAY_DISCARD = 3.0; //(ms) // We do not know anything about the actual value of Delay_DISCARD.
    double WAIT_PICK = 0.0;     // (s) //  We do not know anything about the actual value of WAIT_PICK.
    double WAIT_DROP = 0.0;     // (s) // We do not know anything about the actual value of WAIT_DROP.

    // these are defined here as taking 0 time units because of the assumption that it is really fast.
    // a more realistic version should use the values calculated in the platform
    double WAIT_LOWER = 0.0;
    double WAIT_LIFT = 0.0;

    //Home position for arm is : (x=0, y=250, z=100) // We do not use the home position of arm in this program.
    //Operation area: L 500mm (y in [0 500]), W 300mm (x in [-150, +150]).

    double UpdateY1_1(double origin_y1, double belt_speed, double vertical_speed, double height, double sh1TS, double sh2TS);

    // state for intercept operation
    // int intercept_state = 0;
    // double intercept_y = 0;
    // double intercept_x = 0;

    int pid_state = 0;
    // target theta and radius
    float t_theta = 0, t_radius = 0;
    float Pt = 6.2, It = 0.2, Dt = 0.4;
    float Pr = 5.2, Ir = 0.2, Dr = 0.4;
    //float e_theta, e_radius; // current error
    float p_e_theta, p_e_radius; // previous error
    float d_e_theta, d_e_radius; // derivative
    float i_e_theta, i_e_radius; // integration
    float epsilon_theta = 0.05, epsilon_radius = 0.01;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = FINDING_FIRST_SHAPE;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

public:
    // shared variables
    double c_theta = 0.0;
    double c_radius = 0.0;

    SandwichMakerBehaviour(SandwichMakerPlatform *_p);
    ~SandwichMakerBehaviour();
    void step();

    STEP_RETURN simDetectionBehaviourStm_step();
};
