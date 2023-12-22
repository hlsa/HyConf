#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "../Definitions.hpp"

#define INPUTDUPLICATIONSM

class VacuumCleanerPlatform;

class InputDuplicationSM : public Behaviour
{
    enum STATES
    {
        WAIT_INPUT, WAIT_ACK1, WAIT_ACK2
    };

    VacuumCleanerPlatform *p;
    
    // variables
    TripleAxis x;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = WAIT_INPUT;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

public:
    /* input event */
    TripleAxis i;
    bool i_event = false;
    bool o_ack1_event = false;
    bool o_ack2_event = false;
    /* output event */
    TripleAxis o1;
    bool o1_event = false;
    TripleAxis o2;
    bool o2_event = false;
    bool i_ack_event = false;

    
    InputDuplicationSM(VacuumCleanerPlatform *_p);
    ~InputDuplicationSM();
    void step();

    STEP_RETURN smallstep();
};
