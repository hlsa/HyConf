#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "Definitions.hpp"

// #define LINEARSPEEDSM

class VacuumCleanerPlatform;

class LinearSpeedSM : public Behaviour
{
    enum STATES
    {
        WAIT_INPUT, WAIT_LEFT, WAIT_RIGHT, SEND_OUTPUT, WAIT_OUTPUT_ACK
    };
    
    VacuumCleanerPlatform *p;
    
    // behaviour specific variables -- CAN CHANGE THESE
    TripleAxis spl;
    TripleAxis spr;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = WAIT_INPUT;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

public:
    /* input events */
    bool speed_l_event = false;
    bool speed_r_event = false;
    TripleAxis speed_l, speed_r;
    bool ls_ack_event = false;

    /* output event */
    bool linear_speed_event = false;
    double linear_speed;
    bool speed_l_ack_event = false;
    bool speed_r_ack_event = false;

    LinearSpeedSM(VacuumCleanerPlatform *_p);
    ~LinearSpeedSM();
    void step();

    STEP_RETURN smallstep();
};
