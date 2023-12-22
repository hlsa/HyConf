#include "SandwichMakerBehaviour.hpp"
#include "SandwichMakerPlatform.hpp"
#include "InterceptOp.hpp"
#include "PIDOp.hpp"
#include "FIFO.hpp"
#include <math.h>
#include <iostream>
#include "Operation.hpp"

using namespace std;

SandwichMakerBehaviour::SandwichMakerBehaviour(SandwichMakerPlatform *_p)
{
    p = _p;
    interceptop = new InterceptOp(p);
    pidop = new PIDOp(p);
}

SandwichMakerBehaviour::~SandwichMakerBehaviour()
{
}

void SandwichMakerBehaviour::step()
{
    while (simDetectionBehaviourStm_step() == CONTINUE)
    {
    }
}

STEP_RETURN SandwichMakerBehaviour::go_home()
{
    
    // I'm not considering the time it takes to lift due to the
    // assumption that it is extremely fast
    p->lift();
    STEP_RETURN r = pidop->step(HOME_X, HOME_Y);
    
    while (r == CONTINUE)
    {
        r = pidop->step(HOME_X, HOME_Y);
    }
    if (r == TERMINATED) {
        pidop->reset();
    }

    return r;
}

float get_x(float t, float r)
{
    return r * cos(t);
}

float get_y(float t, float r)
{
    return r * sin(t);
}

STEP_RETURN SandwichMakerBehaviour::pid(float p_theta, float p_radius)
{
    t_theta = p_theta;
    t_radius = p_radius;
    switch (pid_state)
    {
    case 0:
    {
        p_e_theta = t_theta - p->c_theta;
        p_e_radius = t_radius - p->c_radius;
        i_e_radius = 0.0;
        i_e_theta = 0.0;
        d_e_radius = 0.0;
        d_e_theta = 0.0;
        pid_state++;
        return WAIT;
        break;
    }
    case 1:
    {
        float e_theta = t_theta - p->c_theta;
        float e_radius = t_radius - p->c_radius;
        d_e_theta = (e_theta - p_e_theta) / p->step_size;
        d_e_radius = (e_radius - p_e_radius) / p->step_size;
        i_e_theta += (e_theta - p_e_theta) * p->step_size;
        i_e_radius += (e_radius - p_e_radius) * p->step_size;
        if (-epsilon_theta < e_theta && e_theta < epsilon_theta && -epsilon_radius < e_radius && e_radius < epsilon_radius)
        {
            pid_state++;
        }
        else
        {
            p->v_theta = (Pt * e_theta + It * i_e_theta + Dt * d_e_theta);
            p->v_radius = (Pr * e_radius + Ir * i_e_radius + Dr * d_e_radius);
        }
        p_e_theta = e_theta;
        p_e_radius = e_radius;
        return WAIT;
        break;
    }
    default:
    {
        p->v_theta = 0.0;
        p->v_radius = 0.0;
        return TERMINATED;
        break;
    }
    }
}

//This function corresponds function UpdateY1 in RoboSim. This function updates the y coordinate of the robot while moving toward shape1.
//moves out of control -- not a good function
//arm moves distance height1 vertically
double SandwichMakerBehaviour::UpdateY1_1(double origin_y1, double belt_speed, double vertical_speed, double height, double sh1TS, double sh2TS)
{
    double updatedY;
    updatedY = belt_speed * (height / vertical_speed + sh1TS + sh2TS) + origin_y1;
    return updatedY;
}

STEP_RETURN SandwichMakerBehaviour::simDetectionBehaviourStm_step()
{
    if (simDectectionBehaviourStm_state == TURNING_ON)
    {
        if (p->on_event)
        {
            simDectectionBehaviourStm_state = OPERATION;
            return CONTINUE;
        }
        else
        {
            return WAIT;
        }
    }
    else
    {
        if (p->off())
        {
            simDectectionBehaviourStm_state = TURNING_ON;
            return CONTINUE;
        }
        else
        {
            // execute substates
            return OPERATION_step();
        }
    }
}

STEP_RETURN SandwichMakerBehaviour::OPERATION_step()
{
    switch (OPERATION_state)
    {
    case FINDING_FIRST_SHAPE:
        // since there are no entry action, I don't need to check OPERATION_in_state here
        if (p->lShapeX.event)
        {
            shape1_x = p->lShapeX.value;
            OPERATION_state = FOUND_FIRST_SHAPE_X;
            OPERATION_in_state = false;
            return CONTINUE;
        }
        else
        {
            return WAIT;
        }
        break;
    case FOUND_FIRST_SHAPE_X:
        if (p->lShapeY.event)
        {
            shape1_y = p->lShapeY.value;
            OPERATION_state = FOUND_FIRST_SHAPE_Y;
            OPERATION_in_state = false;
            return CONTINUE;
        }
        else
        {
            // this models the fact that there are no alternative transitions that force an exec, so
            // if the event lShapeY is not available, the step diverges, reflecting the deadline 0
            // in the RoboChart model
            return CONTINUE;
        }
        break;
    case FOUND_FIRST_SHAPE_Y:
        if (p->lShapeTS.event)
        {
            shape1_ts = p->lShapeTS.value;
            OPERATION_state = FOUND_FIRST_SHAPE_TS;
            OPERATION_in_state = false;
            return CONTINUE;
        }
        else
        {
            return CONTINUE;
        }
        break;
    case FOUND_FIRST_SHAPE_TS:
        if (!OPERATION_in_state)
        {
            // a state has not yet been entered
            // if a state has an entry action, it must terminate before OPERATION_in_state is set to true.
            // see state S_FOUND_SECOND_SHAPE_TS
            // reset implicit clock
            clock_FOUND_FIRST_SHAPE_TS = 0;
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            if (clock_FOUND_FIRST_SHAPE_TS <= DELAY_DISCARD && p->rShapeX.event)
            {
                shape2_x = p->rShapeX.value;
                OPERATION_in_state = false;
                OPERATION_state = FOUND_SECOND_SHAPE_X;
                return CONTINUE;
            }
            else if (clock_FOUND_FIRST_SHAPE_TS <= DELAY_DISCARD && !p->rShapeX.event)
            {
                // if the delay has not yet been reached, and the event has not been observed, increment
                // clock and wait a cycle
                clock_FOUND_FIRST_SHAPE_TS++;
                return WAIT;
            }
            else
            {
                // otherwise: delay has been violated
                // go to first state of OPERATION, not sure if time should pass here
                OPERATION_in_state = false;
                OPERATION_state = FINDING_FIRST_SHAPE;
                return CONTINUE;
            }
        }

        break;
    case FOUND_SECOND_SHAPE_X:
        if (p->rShapeY.event)
        {
            shape2_y = p->rShapeY.value;
            OPERATION_state = FOUND_SECOND_SHAPE_Y;
            OPERATION_in_state = false;
            return CONTINUE;
        }
        else
        {
            return CONTINUE;
        }
        break;
    case FOUND_SECOND_SHAPE_Y:
        if (p->rShapeTS.event)
        {
            shape2_ts = p->rShapeTS.value;
            OPERATION_state = FOUND_SECOND_SHAPE_TS;
            OPERATION_in_state = false;
            return CONTINUE;
        }
        else
        {
            return CONTINUE;
        }
        break;
    case FOUND_SECOND_SHAPE_TS:
        if (!OPERATION_in_state)
        {
            // account for delay between reading shape1 and starting the intercept operation
            STEP_RETURN r = interceptop->step(shape1_x, shape1_y + (shape2_ts - shape1_ts) * p->beltVelocity);
            if (r == TERMINATED)
            {
                interceptop->reset();
                OPERATION_in_state = true;
                return CONTINUE;
            }
            else
            {
                return r;
            }
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = S_LOWER_ON_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    case S_LOWER_ON_FIRST_SHAPE:
        if (!OPERATION_in_state)
        {
            p->pick();
            p->lower(p->HEIGHT_TO_SHAPE1);
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = D_LOWER_ON_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    case D_LOWER_ON_FIRST_SHAPE:
        if (!OPERATION_in_state)
        {
            clock_D_LOWER_ON_FIRST_SHAPE = 0;
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            if (clock_D_LOWER_ON_FIRST_SHAPE <= WAIT_LOWER)
            {
                clock_D_LOWER_ON_FIRST_SHAPE++;
                return WAIT;
            }
            else
            {
                OPERATION_in_state = false;
                OPERATION_state = S_PICK_FIRST_SHAPE;
                return CONTINUE;
            }
        }
        break;
    case S_PICK_FIRST_SHAPE:
        if (!OPERATION_in_state)
        {
            p->pick();
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = D_PICK_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    case D_PICK_FIRST_SHAPE:
        if (!OPERATION_in_state)
        {
            clock_D_PICK_FIRST_SHAPE = 0;
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            if (clock_D_PICK_FIRST_SHAPE <= WAIT_PICK)
            {
                clock_D_PICK_FIRST_SHAPE++;
                return WAIT;
            }
            else
            {
                OPERATION_in_state = false;
                OPERATION_state = S_LIFT_FIRST_SHAPE;
                return CONTINUE;
            }
        }
        break;
    case S_LIFT_FIRST_SHAPE:
        if (!OPERATION_in_state)
        {
            p->lift();
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = D_LIFT_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    case D_LIFT_FIRST_SHAPE:
        if (!OPERATION_in_state)
        {
            clock_D_LIFT_FIRST_SHAPE = 0;
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            // There is a problem here where the value received by liftTS is
            // too old. Adjusting by one cycle helps, but it is not clear
            // why. I need to investigate the state machine and the V-REP
            // model.
            if (clock_D_LIFT_FIRST_SHAPE >= WAIT_LIFT && p->liftTS.event)
            {
                lift_ts = p->liftTS.value + p->step_size;
                OPERATION_in_state = false;
                OPERATION_state = GO_TO_SECOND_SHAPE;
                return CONTINUE;
            }
            else
            {
                clock_D_LIFT_FIRST_SHAPE++;
                return WAIT;
            }
        }
        break;
    case GO_TO_SECOND_SHAPE:
        if (!OPERATION_in_state)
        {
            // need to account for movement and time since the timestamp was obtained
            STEP_RETURN r = interceptop->step(shape2_x, shape2_y + (lift_ts - shape2_ts) * p->beltVelocity);
            if (r == TERMINATED)
            {
                OPERATION_in_state = true;
                interceptop->reset();
                return CONTINUE;
            }
            else
            {
                return r;
            }
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = S_LOWER_ON_SECOND_SHAPE;
            return CONTINUE;
        }
        break;
    case S_LOWER_ON_SECOND_SHAPE:
        if (!OPERATION_in_state)
        {
            p->lower(p->HEIGHT_TO_SHAPE2);
            p->drop();
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = D_LOWER_ON_SECOND_SHAPE;
            return CONTINUE;
        }
        break;
    case D_LOWER_ON_SECOND_SHAPE:
        if (!OPERATION_in_state)
        {
            clock_D_LOWER_ON_SECOND_SHAPE = 0;
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            if (clock_D_LOWER_ON_SECOND_SHAPE <= WAIT_LOWER)
            {
                clock_D_LOWER_ON_SECOND_SHAPE++;
                return WAIT;
            }
            else
            {
                OPERATION_in_state = false;
                OPERATION_state = S_DROP_ON_SECOND_SHAPE;
                return CONTINUE;
            }
        }
        break;
    case S_DROP_ON_SECOND_SHAPE:
        if (!OPERATION_in_state)
        {
            p->drop();
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = D_DROP_ON_SECOND_SHAPE;
            return CONTINUE;
        }
        break;
    case D_DROP_ON_SECOND_SHAPE:
        if (!OPERATION_in_state)
        {
            clock_D_DROP_ON_SECOND_SHAPE = 0;
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            if (clock_D_DROP_ON_SECOND_SHAPE <= WAIT_DROP)
            {
                clock_D_DROP_ON_SECOND_SHAPE++;
                return WAIT;
            }
            else
            {
                OPERATION_in_state = false;
                OPERATION_state = S_LIFT_ON_SECOND_SHAPE;
                return CONTINUE;
            }
        }
        break;
    case S_LIFT_ON_SECOND_SHAPE:
        if (!OPERATION_in_state)
        {
            p->lift();
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = D_LIFT_ON_SECOND_SHAPE;
            return CONTINUE;
        }
        break;
    case D_LIFT_ON_SECOND_SHAPE:
        if (!OPERATION_in_state)
        {
            clock_D_LIFT_ON_SECOND_SHAPE = 0;
            OPERATION_in_state = true;
            return CONTINUE;
        }
        else
        {
            if (clock_D_LIFT_ON_SECOND_SHAPE <= WAIT_LIFT)
            {
                clock_D_LIFT_ON_SECOND_SHAPE++;
                return WAIT;
            }
            else
            {
                OPERATION_in_state = false;
                OPERATION_state = GO_TO_HOME_POSITION;
                return CONTINUE;
            }
        }
        break;
    case GO_TO_HOME_POSITION:
        if (!OPERATION_in_state)
        {
            STEP_RETURN r = pidop->step(HOME_X, HOME_Y);
            if (r == TERMINATED)
            {
                pidop->reset();
                OPERATION_in_state = true;
                return CONTINUE;
            }
            else
            {
                return r;
            }
        }
        else
        {
            OPERATION_in_state = false;
            OPERATION_state = FINDING_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    default:
        printf("This state should not be reachable");
        return WAIT;
        break;
    }
}