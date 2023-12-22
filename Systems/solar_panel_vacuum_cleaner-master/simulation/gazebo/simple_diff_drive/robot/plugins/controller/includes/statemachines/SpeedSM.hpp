#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "../Definitions.hpp"

#define SPEEDSM

class VacuumCleanerPlatform;

class SpeedSM : public Behaviour
{
    enum STATES
    {
        WAIT_INPUT, UPDATE_SPEED, WAITING, WAIT_SP_ACK
    };

    VacuumCleanerPlatform *p;
    
    // behaviour specific variables -- CAN CHANGE THESE
    TripleAxis acceleration;
    TripleAxis speed;

    // CONSTANT
    double t = 1;
    
    // implicit clocks used to model wait statements
    double clock = 0;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = WAIT_INPUT;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

public:
    /* input event */
    bool acc_event = false;
    TripleAxis acc;
    bool sp_ack_event = false;
    /* output event */
    bool sp_event = false;
    TripleAxis sp;
    bool acc_ack_event = false;
    
    SpeedSM(VacuumCleanerPlatform *_p);
    ~SpeedSM();
    void step();

    STEP_RETURN smallstep();
};
