#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "../Definitions.hpp"

// #define CLEANSM

class VacuumCleanerPlatform;

class CleanSM : public Behaviour
{
    enum STATES
    {
            DONT_CLEAN, CLEAN
    };

    VacuumCleanerPlatform *p;
    
    // behaviour specific variables -- CAN CHANGE THESE
    bool clean_var;

    // CONSTANT
    double voltage = 3.0;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = DONT_CLEAN;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

public:
    /* input events */
    bool clean;
    bool clean_event = false;

    /* output events */
    bool clean_ack_event = false;

    CleanSM(VacuumCleanerPlatform *_p);
    ~CleanSM();
    void step();

    STEP_RETURN smallstep();
};
