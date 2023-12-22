#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "../Definitions.hpp"

// #define MIDLEVELSM

class VacuumCleanerPlatform;

class MidLevelSM : public Behaviour
{
    enum STATES
    {
        GO_UP_ENTRY0, GO_UP_ENTRY1, GO_UP_ENTRY2, GO_UP_ENTRY3, GO_UP_ENTRY4, GO_UP_ENTRY5, GO_UP_DURING,
        STOP_ENTRY0, STOP_ENTRY1, STOP_ENTRY2, STOP_ENTRY3, STOP_ENTRY4, STOP_ENTRY5, STOP_DURING,
        TURN_RIGHT_ENTRY0, TURN_RIGHT_ENTRY1, TURN_RIGHT_ENTRY2, TURN_RIGHT_ENTRY3, TURN_RIGHT_ENTRY4, TURN_RIGHT_ENTRY5, TURN_RIGHT_DURING,
        TURN_LEFT_ENTRY0, TURN_LEFT_ENTRY1, TURN_LEFT_ENTRY2, TURN_LEFT_ENTRY3, TURN_LEFT_ENTRY4, TURN_LEFT_ENTRY5, TURN_LEFT_DURING,
        GO_SIDE_ENTRY0, GO_SIDE_ENTRY1, GO_SIDE_ENTRY2, GO_SIDE_ENTRY3, GO_SIDE_ENTRY4, GO_SIDE_ENTRY5, GO_SIDE_DURING,
        GO_DOWN_ENTRY0, GO_DOWN_ENTRY1, GO_DOWN_ENTRY2, GO_DOWN_ENTRY3, GO_DOWN_ENTRY4, GO_DOWN_ENTRY5, GO_DOWN_DURING
    };

    VacuumCleanerPlatform *p;
    
    // behaviour specific variables
    double lspeed;
    double rspeed;
    double angle;
    double speed_adj;
    double angle_adj;
    Direction direction;
    Orientation orientation;

    // CONSTANT
    const double c_step = 10;
    const double climb_speed = 0.1;
    const double descent_speed = 0.5;
    const double side_speed = 0.5;
    const double turn_speed = 0.01;
    /* not all of these constants seem to be used */
    const double forward_angle = 0.1;
    const double left_angle = 0.1;
    const double right_angle = 0.1;
    const double movesp = 0.4;
    const double stopsp = 0.0;

    // STATE MACHINE CONTROL VARIABLES
    STATES state = GO_UP_ENTRY0;
    // I'm using this variable to decide whether I'm entering or continuing with the execution of a state.
    // This must be set to true when a state is entered, and false, when it is exited.
    bool in_state = false;

    // clock for sinceEntry conditions
    int clock = 0;

public:
    /* input events */
    bool move_forward_event = false;
    Direction turn;
    bool turn_event = false;
    double angle_adjustment;
    bool angle_adjustment_event = false;
    double speed_adjustment;
    bool speed_adjustment_event = false;
    bool stop_event = false;

    bool target_speed_ack_event = false;
    bool target_angle_ack_event = false;

    /* output events */
    double target_speed = false;
    bool target_speed_event = false;
    double target_angle = false;
    bool target_angle_event = false;

    bool move_forward_ack_event = false;
    bool turn_ack_event = false;
    bool angle_adjustment_ack_event = false;
    bool speed_adjustment_ack_event = false;
    bool stop_ack_event = false;

    MidLevelSM(VacuumCleanerPlatform *_p);
    ~MidLevelSM();
    void step();

    STEP_RETURN smallstep();
    char* getStateName();
};
