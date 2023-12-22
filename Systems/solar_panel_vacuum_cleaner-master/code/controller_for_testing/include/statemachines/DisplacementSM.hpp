#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "../Definitions.hpp"

// #define DISPLACEMENTSM

class VacuumCleanerPlatform;

class DisplacementSM : public Behaviour
{
    enum STATES
    {
        WAIT_INPUT, UPDATE_DISPLACEMENT, WAIT_DISPLACEMENT_ACK, WAITING
    };

    VacuumCleanerPlatform *p;
    
    // behaviour specific variables -- CAN CHANGE THESE
    double d;
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
    /* input events */
    double displacement_in;
    bool displacement_in_event = false;
    bool displacement_out_ack_event;
    TripleAxis sp;
    bool sp_event = false;

    /* output events */
    double displacement_out;
    bool displacement_out_event = false;
    bool displacement_in_ack_event = false;
    bool sp_ack_event = false;

    DisplacementSM(VacuumCleanerPlatform *_p);
    ~DisplacementSM();
    void step();

    STEP_RETURN smallstep();
};
