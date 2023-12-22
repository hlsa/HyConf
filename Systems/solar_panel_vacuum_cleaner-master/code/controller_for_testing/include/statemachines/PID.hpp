#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "../Definitions.hpp"

// #define PIDSM

class VacuumCleanerPlatform;

class PID : public Behaviour
{
    enum STATES
    {
        WAIT_TARGET, WAIT_ACTUAL, RESET, UPDATE, WAITING, WAIT_ERR_ACK
    };

    VacuumCleanerPlatform *p;
    
    // behaviour specific variables -- CAN CHANGE THESE
    double prior_error;
    double integral;
    double error;
    double derivative;
    double out;
    double target_value;
    double actual_value;
    double old_target;
    
    // CONSTANT
    const double kp = 2;
    const double ki = 0.01;
    const double kd = 0.01;
    const double c_step = 1;

    // implicit clocks used to model wait statements
    double clock = 0;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = WAIT_TARGET;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

public:
    /* input events */
    double target;
    bool target_event = false;
    double actual;
    bool actual_event = false;
    bool err_ack_event = false;

    /* output events */
    double err_output;
    bool err_output_event = false;
    bool target_ack_event = false;
    bool actual_ack_event = false;

    PID(VacuumCleanerPlatform *_p);
    ~PID();
    void step();

    STEP_RETURN smallstep();
};
