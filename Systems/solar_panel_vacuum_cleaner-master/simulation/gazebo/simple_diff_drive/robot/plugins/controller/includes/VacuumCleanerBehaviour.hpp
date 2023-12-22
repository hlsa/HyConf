#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "Definitions.hpp"

class AngularSpeedSM;
class CleanSM;
class DisplacementSM;
class InputDuplicationSM;
class LinearSpeedSM;
class MidLevelSM;
class PathPlanningSM;
class PID;
class SpeedSM;
class VacuumCleanerPlatform;

class VacuumCleanerBehaviour : public Behaviour
{
    VacuumCleanerPlatform *p = nullptr;

    AngularSpeedSM *angularspeedsm = nullptr;
    CleanSM *cleansm = nullptr;
    DisplacementSM *displacementsm = nullptr;
    InputDuplicationSM *inputduplicationsm1 = nullptr;
    InputDuplicationSM *inputduplicationsm2 = nullptr;
    InputDuplicationSM *inputduplicationsm3 = nullptr;
    LinearSpeedSM *linearspeedsm = nullptr;
    MidLevelSM *midlevelsm = nullptr;
    PathPlanningSM *pathplanningsm = nullptr;
    PID *pid1 = nullptr;
    PID *pid2 = nullptr;
    SpeedSM *speedsm1 = nullptr;
    SpeedSM *speedsm2 = nullptr;

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

    // int warm_up_counter = 0;
    // const int WARM_UP = 100;
public:

    VacuumCleanerBehaviour(VacuumCleanerPlatform *_p);
    ~VacuumCleanerBehaviour();
    void step();
};
