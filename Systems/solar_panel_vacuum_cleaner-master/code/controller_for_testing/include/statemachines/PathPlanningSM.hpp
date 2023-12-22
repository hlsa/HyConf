#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "../Definitions.hpp"

// #define PATHPLANNINGSM

class VacuumCleanerPlatform;

class PathPlanningSM : public Behaviour
{
    enum STATES
    {
        RESUME_ENTRY1,RESUME_ENTRY2,RESUME_ENTRY3,
        RESUME_EXIT1,RESUME_EXIT2,RESUME_EXIT3,

        GO_UP_ENTRY1, GO_UP_EXIT,

        GO_RIGHT_ENTRY1,GO_RIGHT_ENTRY2,GO_RIGHT_ENTRY3,GO_RIGHT_EXIT,
        GO_DOWN_ENTRY1,GO_DOWN_ENTRY2,GO_DOWN_EXIT1,GO_DOWN_EXIT2,GO_DOWN_EXIT3,GO_DOWN_EXIT4,
        
        GO_RIGHT_AGAIN, CHECK_END, CHECK_BATTERY, CHECK_BATTERY_EXIT,
        RETURN_ENTRY1,RETURN_ENTRY2,RETURN_ENTRY3,RETURN_ENTRY4,RETURN_ENTRY5,
        DOCK_ENTRY1,DOCK_ENTRY2,DOCK_ENTRY3,
        SLEEP,SLEEP_EXIT1,SLEEP_EXIT2,SLEEP_EXIT3,
        WAITING
    };

    VacuumCleanerPlatform *p;
    
    // behaviour specific variables -- CAN CHANGE THESE
    double cycles;
    double u;
    double d;
    double b;

    // CONSTANT
    const double cliff = 0.3; // the height of the robot is 0.27
    const double nozzle = 0.1;
    const double battery_low = 20.0; //20%
    const double sleep_time = 100;
    
    // implicit clocks used to model wait statements
    double clock = 0;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = RESUME_ENTRY1;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

public:
    /* input events */
    double displacement_in;
    bool displacement_in_event = false;
    double battery_level;
    bool battery_level_event = false;
    double ultrasonic;
    bool ultrasonic_event = false;
    bool charging_event = false;

    bool clean_ack_event = false;
    bool displacement_out_ack_event = false;
    bool move_forward_ack_event = false;
    bool turn_ack_event = false;
    bool stop_ack_event = false;

    /* output events */
    double displacement_out;
    bool displacement_out_event = false;
    bool clean;
    bool clean_event = false;
    bool stop_event = false;
    bool move_forward_event = false;
    Direction turn;
    bool turn_event = false;
    
    bool displacement_in_ack_event = false;
    bool battery_level_ack_event = false;
    bool ultrasonic_ack_event = false;
    bool charging_ack_event = false;

    PathPlanningSM(VacuumCleanerPlatform *_p);
    ~PathPlanningSM();
    void step();

    STEP_RETURN smallstep();
};