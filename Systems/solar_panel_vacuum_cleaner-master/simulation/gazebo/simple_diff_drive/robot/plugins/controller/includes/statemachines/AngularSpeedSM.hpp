#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "../Definitions.hpp"

#define ANGULARSPEEDSM

class VacuumCleanerPlatform;

class AngularSpeedSM : public Behaviour
{
    enum STATES
    {
        WAIT_AL, WAIT_AR, WAIT_INPUT, UPDATE_VALUE, WAITING, WAIT_ANGLE_ACK
    };

    VacuumCleanerPlatform *p;
    
    // behaviour specific variables -- CAN CHANGE THESE
    double speed;
    double turn_angle;
    Direction direction;
    TripleAxis al;
    TripleAxis ar;

    // CONSTANT
    double t = 1;
    double acc_distance = 0.094; //m -- distance between the accelerometers
    
    // implicit clocks used to model wait statements
    double clock = 0;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = WAIT_INPUT;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

public:
    /* input events */
    TripleAxis acc_r;
    bool acc_r_event = false;
    TripleAxis acc_l;
    bool acc_l_event = false;
    bool angle_ack_event = false;

    /* output event */
    double angle;
    bool angle_event = false;
    bool acc_r_ack_event = false;
    bool acc_l_ack_event = false;
    
    AngularSpeedSM(VacuumCleanerPlatform *_p);
    ~AngularSpeedSM();
    void step();

    STEP_RETURN smallstep();
};
