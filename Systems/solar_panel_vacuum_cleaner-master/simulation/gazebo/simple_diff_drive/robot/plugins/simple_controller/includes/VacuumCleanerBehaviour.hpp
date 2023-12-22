#pragma once

#include "Behaviours.hpp"
#include "Operation.hpp"
#include "Definitions.hpp"
#include "VacuumCleanerPlatform.hpp"
#include "consts.hpp"

class VacuumCleanerBehaviour : public Behaviour
{
public:
    VacuumCleanerPlatform *p = nullptr;
    int step_cnt = 0;

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

    VacuumCleanerBehaviour(VacuumCleanerPlatform *_p);
    ~VacuumCleanerBehaviour();
    void step();

    double clean(double x)
    {
        if (x <= epsilon && x >= -epsilon)
        {
            return 0.0;
        }
        else
        {
            return x;
        }
    }

    double sign(double v)
    {
        if (v >= 0)
            return 1.0;
        else
            return -1.0;
    }

    double theta = 0.0;
    double aspeed = 0.0;

    double lspeed = 0.0;
    double dist = 0.0;

    double angleTarget = 0.0;
    double angleE = 0.0;
    double anglePE = 0.0;
    double angleP = 0.0;
    double angleI = 0.0;
    double angleD = 0.0;
    double angleO = 0.0;

    int cycles = 0;
    int T = 0;

    enum STATE {
        TURN1, STOP1, // 90 degrees clockwise
        MOVE_OUT1a, STOP1a, // move out of the charging station
        MOVE_UP2, STOP2, // move north
        TURN3, STOP3, // 90 degrees clockwise
        MOVE_RIGHT_TOP4, STOP4, // move east
        TURN5, STOP5, // 90 degrees clockwise
        MOVE_DOWN6, STOP6, // move south
        TURN7, STOP7, // 90 degrees anti-clockwise
        MOVE_RIGHT_BOTTOM8, STOP8, // move east
        TURN9, STOP9,  // 90 degrees anti-clockwise
        RESUME9a, STOP9a, // turn 90 degrees clockwise
        MOVE_RIGHT_BOTTOM9b, STOP9b, // move cycles*ROBOT_LENGTH
        TURN9c, STOP9c, // turn 90 degrees anti-clockwise and go to MOVE_UP2
        STOP10, TURN10a, STOP10a,// stop to go down and cross the bridge -- go to MOVE_UP2
        MOVE_DOWN11, STOP11, // move south
        STOP15, // stop to turn 180 degrees and go back to charging station
        TURN16, STOP16, // turn 180 degrees
        MOVE_LEFT17, STOP17, // return to base
        TURN18, STOP18, // turn 90 degreees anti-clockwise to enter the charging station
        MOVE_DOWN19, STOP19, // move south into the charging station
        TURN20, STOP20, // turn 90 degreees clockwise to position itself in charging station
        WAIT
    };


    STATE st = TURN1;

    bool turn(double a)
    {
        p->left->SetEffortLimit(0, -1);
        p->right->SetEffortLimit(0, -1);
        
        angleTarget = a;
        angleE = sign(angleTarget) * (abs(angleTarget) - abs(theta));
        if (angleE <= 0.01 && angleE >= -0.01)
        {
            printf("Reached\n");
            return true;
        }
        angleP = angleE;
        angleI += angleE * STEP_SIZE * 5;
        angleD = (angleE - anglePE) / (STEP_SIZE * 5);
        anglePE = angleE;
        angleO = CangleP * angleP + CangleI * angleI + CangleD * angleD;

        double normalisedO = 0;
        // if (angleO >= -0.02 && angleO <= 0.02)
        //     normalisedO = 0.0;
        // else
        if (angleO >= -2.0 && angleO <= 2.0)
            normalisedO = angleO;
        else if (angleO > 2.0)
            normalisedO = 2.0;
        else
            normalisedO = -2.0;

        printf("PID output is %f; normalised to %f\n", angleO, normalisedO);

        p->output_left_motor(normalisedO);
        p->output_right_motor(-normalisedO);
        return false;
    }

    bool stop()
    {
        if (abs(aspeed) <= 0.001 && abs(lspeed) <= 0.0001)
        {
            printf("Stopped\n");
            theta = 0;
            dist = 0;
            aspeed = 0.0;
            lspeed = 0.0;

            return true;
        }
        else
        {
            printf("Stopping angular speed %f and linear speed %f\n", aspeed, lspeed);
            p->output_left_motor(0.0);
            p->output_right_motor(0.0);
            return false;
        }
    }

    void move()
    {
        this->p->left->SetEffortLimit(0, 1.2); //0.07);
        this->p->right->SetEffortLimit(0, 1.2); //0.07);
        p->output_left_motor(2);//0.5);
        p->output_right_motor(2);//0.5);
    }
};
