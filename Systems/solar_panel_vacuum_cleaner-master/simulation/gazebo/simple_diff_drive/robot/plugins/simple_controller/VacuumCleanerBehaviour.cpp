#include "VacuumCleanerBehaviour.hpp"
#include "VacuumCleanerPlatform.hpp"

// #include "PIDOp.hpp"
#include "FIFO.hpp"
#include <math.h>
#include <iostream>
#include "Operation.hpp"
#include "consts.hpp"

struct TripleAxis;

using namespace std;
using namespace gazebo;

VacuumCleanerBehaviour::VacuumCleanerBehaviour(VacuumCleanerPlatform *_p)
{
    p = _p;
}

VacuumCleanerBehaviour::~VacuumCleanerBehaviour()
{
}

void VacuumCleanerBehaviour::step()
{
    // read from the platform -- for multicontroller model, we need to figure out exactly where this is done.
    // Ideally, it should be done at the level of a Module, but in the current frameworks this is not available,
    // so it is being done here. In this model, it is not too much of a problem as there is a single controller.

    if (step_cnt == 0)
    {
        acc_l_event = p->acc_l_event;
        acc_l = p->acc_l;

        acc_r_event = p->acc_r_event;
        acc_r = p->acc_r;

        ultrasonic_event = p->ultrasonic_event;
        ultrasonic = p->ultrasonic;

        charging_event = p->charging_event;

        battery_level_event = p->battery_level_event;
        battery_level = p->battery_level;
    }
    else if (step_cnt < N - 1)
    {
        acc_l_event = p->acc_l_event;
        acc_l.X += p->acc_l.X;
        acc_l.Y += p->acc_l.Y;
        acc_l.Z += p->acc_l.Z;

        acc_r_event = p->acc_r_event;
        acc_r.X += p->acc_r.X;
        acc_r.Y += p->acc_r.Y;
        acc_r.Z += p->acc_r.Z;

        ultrasonic_event = p->ultrasonic_event;
        ultrasonic += p->ultrasonic;

        charging_event = p->charging_event;

        battery_level_event = p->battery_level_event;
        battery_level += p->battery_level;
    }
    else
    {
        acc_l_event = p->acc_l_event;
        acc_l.X += p->acc_l.X;
        acc_l.Y += p->acc_l.Y;
        acc_l.Z += p->acc_l.Z;

        acc_r_event = p->acc_r_event;
        acc_r.X += p->acc_r.X;
        acc_r.Y += p->acc_r.Y;
        acc_r.Z += p->acc_r.Z;

        acc_l.X /= N;
        acc_l.Y /= N;
        acc_l.Z /= N;

        acc_r.X /= N;
        acc_r.Y /= N;
        acc_r.Z /= N;
        printf("step\n");
        printf("base: (%f,%f,%f),(%f,%f,%f)\n",p->base_lX,p->base_lY,p->base_lZ,p->base_rX,p->base_rY,p->base_rZ);
        printf("raw:  (%f,%f,%f),(%f,%f,%f),%f\n", acc_l.X, acc_l.Y, acc_l.Z, acc_r.X, acc_r.Y, acc_r.Z, ultrasonic);

        acc_l.X -= p->base_lX;
        acc_l.Y -= p->base_lY;
        acc_l.Z -= p->base_lZ;
        acc_r.X -= p->base_rX;
        acc_r.Y -= p->base_rY;
        acc_r.Z -= p->base_rZ;

        ultrasonic_event = p->ultrasonic_event;
        ultrasonic += p->ultrasonic;
        ultrasonic /= N;

        charging_event = p->charging_event;

        battery_level_event = p->battery_level_event;
        battery_level += p->battery_level;
        battery_level /= N;

        printf("norm: (%f,%f,%f),(%f,%f,%f),%f\n", acc_l.X, acc_l.Y, acc_l.Z, acc_r.X, acc_r.Y, acc_r.Z, ultrasonic);

        aspeed = sqrt((abs(acc_r.Y - acc_l.Y))/(2*ACC_DIST));
        double readL = WHL_RADIUS*p->left->GetVelocity(0);
        double readR = WHL_RADIUS*p->right->GetVelocity(0);
        lspeed = (readL+readR)/2;
        dist = dist + lspeed * STEP_SIZE * N;
        theta = theta + aspeed * STEP_SIZE * N;

        printf("Angular movement: theta = %.5f, and aspeed = %.5f\n", theta, aspeed);
        printf("Linear movement: dist = %.5f, and lspeed = %.5f\n", dist, lspeed);
        printf("Ultrasonic: %f\n", ultrasonic);
        printf("Battery: %f\%\n", p->battery);
        printf("Cleaning cycle: %d\n", cycles);
        
        /* ANGLE PID CONTROL */

        switch(st) {
            case TURN1:
                printf("TURN1\n");
                if (turn(3.1415 / 2))
                    st = STOP1;
                break;
            case STOP1:
                printf("STOP1\n");
                if (stop())
                    st = MOVE_UP2;
                break;
            case MOVE_OUT1a:
                printf("MOVE OUT OF STATION\n");
                if (dist >= ROBOT_LENGTH) {
                    st = STOP1a;
                } else {
                    move();
                }
                break;
            case STOP1a:
                printf("STOP1a\n");
                if (stop()) {
                    if (cycles == 0) {
                        printf("cycles == 0\n");
                        st = MOVE_UP2;
                    } else {
                        printf("cycles == %d\n", cycles);
                        st = RESUME9a;
                    }
                }
                break;
            case MOVE_UP2:
                printf("MOVE UP\n");
                if (ultrasonic_event && ultrasonic >= EDGE_HEIGHT) {
                    printf("EDGE FOUND\n");
                    st = STOP2;
                } else {
                    move();
                }
                break;
            case STOP2:
                printf("STOP2\n");
                if (stop()) {
                    st = TURN3;
                }
                break;
            case TURN3:
                printf("TURN3\n");
                if (turn(3.1415 / 2))
                    st = STOP3;
                break;
            case STOP3:
                printf("STOP3\n");
                if (stop())
                    st = MOVE_RIGHT_TOP4;
                break;
            case MOVE_RIGHT_TOP4:
                printf("MOVE RIGHT AT TOP\n");
                if (ultrasonic_event && ultrasonic >= EDGE_HEIGHT) {
                    printf("EDGE FOUND AT TOP OF PANEL - GO DOWN AND CROSS THE BRIDGE\n");
                    st = STOP10;
                } else if (dist >= ROBOT_LENGTH) {
                    st = STOP4;
                } else {
                    move();
                }
                break;
            case STOP4:
                printf("STOP4\n");
                if (stop()) {
                    st = TURN5;
                }
                break;
            case TURN5:
                printf("TURN5\n");
                if (turn(3.1415 / 2))
                    st = STOP5;
                break;
            case STOP5:
                printf("STOP5\n");
                if (stop())
                    st = MOVE_DOWN6;
                break;
            case MOVE_DOWN6:
                printf("MOVE DOWN\n");
                if (ultrasonic_event && ultrasonic >= EDGE_HEIGHT) {
                    printf("EDGE FOUND\n");
                    st = STOP6;
                } else {
                    move();
                }
                break;
            case STOP6:
                printf("STOP6\n");
                if (stop()) {
                    st = TURN7;
                }
                break;
            case TURN7:
                printf("TURN7\n");
                if (turn(-3.1415 / 2))
                    st = STOP7;
                break;
            case STOP7:
                printf("STOP7\n");
                if (stop())
                    st = MOVE_RIGHT_BOTTOM8;
                break;
            case MOVE_RIGHT_BOTTOM8:
                printf("MOVE_RIGHT_BOTTOM8\n");
                if (ultrasonic_event && ultrasonic >= EDGE_HEIGHT) {
                    printf("EDGE FOUND AT BOTTOM OF PANEL - RETURN TO CHARGING STATION\n");
                    st = STOP15;
                } else if (dist >= ROBOT_LENGTH) {
                    st = STOP8;
                } else {
                    move();
                }
                break;
            case STOP8:
                printf("STOP8\n");
                if (stop()) {
                    if (p->battery <= BATTERY_LOW) {
                        st = TURN16;
                    } else {
                        cycles++;
                        st = TURN9;
                    }
                }
                break;
            case TURN9:
                printf("TURN9\n");
                if (turn(-3.1415 / 2))
                    st = STOP9;
                break;
            case STOP9:
                printf("STOP9\n");
                if (stop())
                    st = MOVE_UP2;
                break;
            /* END OF BASIC CASE - up and down */
            case STOP10:
                printf("STOP10\n");
                if (stop())
                    st = TURN10a;
                break;
            case TURN10a:
                printf("TURN10a\n");
                if (turn(3.1415 / 2))
                    st = STOP10a;
                break;
            case STOP10a:
                printf("STOP10a\n");
                if (stop())
                    st = MOVE_DOWN11;
                break;
            case MOVE_DOWN11:
                printf("MOVE_DOWN11\n");
                if (ultrasonic_event && ultrasonic >= EDGE_HEIGHT) {
                    printf("EDGE FOUND\n");
                    st = STOP11;
                } else {
                    move();
                }
                break;
            case STOP11:
                printf("STOP11\n");
                if (stop()) {
                    st = TURN7;
                }
                break; 
            case STOP15:
                printf("STOP15\n");
                if (stop()) {
                    cycles = 0;
                    st = TURN16;
                }
                break;
            case TURN16:
                printf("TURN16\n");
                if (turn(-3.1415))
                    st = STOP16;
                break;
            case STOP16:
                printf("STOP16\n");
                if (stop())
                    st = MOVE_LEFT17;
                break;  
            case MOVE_LEFT17:
                printf("MOVE_LEFT17\n");
                if (ultrasonic_event && ultrasonic >= EDGE_HEIGHT) {
                    printf("EDGE FOUND AT BOTTOM OF PANEL ON THE LEFT\n");
                    st = STOP17;
                } else {
                    move();
                }
                break;
            case STOP17:
                printf("STOP17\n");
                if (stop()) {
                    st = TURN18;
                }
                break;
            case TURN18:
                printf("TURN18\n");
                if (turn(-3.1415 / 2))
                    st = STOP18;
                break;
            case STOP18:
                printf("STOP18\n");
                if (stop())
                    st = MOVE_DOWN19;
                break;
            case MOVE_DOWN19:
                printf("MOVE DOWN18\n");
                if (ultrasonic_event && ultrasonic >= EDGE_HEIGHT) {
                    printf("EDGE FOUND\n");
                    st = STOP19;
                } else {
                    move();
                }
                break;
            case STOP19:
                printf("STOP19\n");
                if (stop()) {
                    st = TURN20;
                }
                break;
            case TURN20:
                printf("TURN20\n");
                if (turn(3.1415 / 2))
                    st = STOP20;
                break;
            case STOP20:
                printf("STOP20\n");
                if (stop()) {
                    st = WAIT;
                    T = 0;
                }
                break;
            case WAIT:
                printf("WAIT");
                if (T >= CHARGING_TIME) {
                    st = TURN1;
                } else {
                    T++;
                }
                break;
            case RESUME9a:
                printf("RESUME9a\n");
                if (turn(3.1415 / 2))
                    st = STOP9a;
                break;
            case STOP9a:
                printf("STOP9a\n");
                if (stop())
                    st = MOVE_RIGHT_BOTTOM9b;
                break;
            case MOVE_RIGHT_BOTTOM9b:
                printf("MOVE_RIGHT_BOTTOM9b\n");
                if (ultrasonic_event && ultrasonic >= EDGE_HEIGHT) {
                    printf("EDGE FOUND AT BOTTOM OF PANEL - RETURN TO CHARGING STATION\n");
                    st = STOP15;
                } else if (dist >= cycles*ROBOT_LENGTH) {
                    st = STOP9b;
                } else {
                    move();
                }
                break;
            case STOP9b:
                printf("STOP9b\n");
                if (stop()) {
                    st = TURN9c;
                }
                break;
            case TURN9c:
                printf("TURN9c\n");
                if (turn(-3.1415 / 2))
                    st = STOP9c;
                break;
            case STOP9c:
                printf("STOP9c\n");
                if (stop())
                    st = MOVE_UP2;
                break;
        }
    }
    step_cnt = (step_cnt+1)%N;
}