#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "Definitions.hpp"
#include <iostream>

class VacuumCleanerPlatform;

class TestBehaviour : public Behaviour
{
    VacuumCleanerPlatform *p = nullptr;

    /* input events */
    double battery_level;
    bool battery_level_event = false;

    double ultrasonic;
    bool ultrasonic_event = false;
    
    bool charging_event = false;

    struct TripleAxis acc_l;
    bool acc_l_event = false;

    struct TripleAxis acc_r;
    bool acc_r_event = false;

    double sl = 0;
    double sr = 0;
    double target = 0;
public:

    TestBehaviour(VacuumCleanerPlatform *_p) {
        p = _p;
    }
    ~TestBehaviour() {}
    void step() {
        sl += p->acc_l.Y*p->step_size;

        sr += p->acc_r.Y*p->step_size;
        double as = sqrt(abs(sl-sr)/0.094);
        double das = sqrt(abs(10-5)/0.06);
        printf("angular speed = %f =? %f\n", as,das);
        printf("linear speed = (%f,%f) =? (%f,%f)\n", sl, sr, 1, 1);
        p->output_left_motor(1);
        p->output_right_motor(2);


    }
};
