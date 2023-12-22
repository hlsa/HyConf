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
    interceptop = new InterceptOp(p, this);
    pidop = new PIDOp(p, this);
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

float get_x(float t, float r)
{
    return r * cos(t);
}

float get_y(float t, float r)
{
    return r * sin(t);
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
    switch (state)
    {
    case FINDING_FIRST_SHAPE:
        // since there are no entry action, I don't need to check in_state here
        if (p->lShapeX.event)
        {
            shape1_x = p->lShapeX.value;
            state = FOUND_FIRST_SHAPE_X;
            in_state = false;
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
            state = FOUND_FIRST_SHAPE_Y;
            in_state = false;
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
            state = FOUND_FIRST_SHAPE_TS;
            in_state = false;
            return CONTINUE;
        }
        else
        {
            return CONTINUE;
        }
        break;
    case FOUND_FIRST_SHAPE_TS:
        if (!in_state)
        {
            // a state has not yet been entered
            // if a state has an entry action, it must terminate before in_state is set to true.
            // see state S_FOUND_SECOND_SHAPE_TS
            // reset implicit clock
            clock_FOUND_FIRST_SHAPE_TS = 0;
            in_state = true;
            return CONTINUE;
        }
        else
        {
            if (clock_FOUND_FIRST_SHAPE_TS <= DELAY_DISCARD && p->rShapeX.event)
            {
                shape2_x = p->rShapeX.value;
                in_state = false;
                state = FOUND_SECOND_SHAPE_X;
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
                in_state = false;
                state = FINDING_FIRST_SHAPE;
                return CONTINUE;
            }
        }

        break;
    case FOUND_SECOND_SHAPE_X:
        if (p->rShapeY.event)
        {
            shape2_y = p->rShapeY.value;
            state = FOUND_SECOND_SHAPE_Y;
            in_state = false;
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
            state = FOUND_SECOND_SHAPE_TS;
            in_state = false;
            return CONTINUE;
        }
        else
        {
            return CONTINUE;
        }
        break;
    case FOUND_SECOND_SHAPE_TS:
        if (!in_state)
        {
            // account for delay between reading shape1 and starting the intercept operation
            STEP_RETURN r = interceptop->step(shape1_x, shape1_y + (shape2_ts - shape1_ts) * p->beltVelocity);
            if (r == TERMINATED)
            {
                interceptop->reset();
                in_state = true;
                return CONTINUE;
            }
            else
            {
                return r;
            }
        }
        else
        {
            in_state = false;
            state = S_LOWER_ON_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    case S_LOWER_ON_FIRST_SHAPE:
        if (!in_state)
        {
            p->pick();
            p->lower(p->HEIGHT_TO_SHAPE1);
            in_state = true;
            return CONTINUE;
        }
        else
        {
            in_state = false;
            state = D_LOWER_ON_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    case D_LOWER_ON_FIRST_SHAPE:
        if (!in_state)
        {
            clock_D_LOWER_ON_FIRST_SHAPE = 0;
            in_state = true;
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
                in_state = false;
                state = S_PICK_FIRST_SHAPE;
                return CONTINUE;
            }
        }
        break;
    case S_PICK_FIRST_SHAPE:
        if (!in_state)
        {
            p->pick();
            in_state = true;
            return CONTINUE;
        }
        else
        {
            in_state = false;
            state = D_PICK_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    case D_PICK_FIRST_SHAPE:
        if (!in_state)
        {
            clock_D_PICK_FIRST_SHAPE = 0;
            in_state = true;
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
                in_state = false;
                state = S_LIFT_FIRST_SHAPE;
                return CONTINUE;
            }
        }
        break;
    case S_LIFT_FIRST_SHAPE:
        if (!in_state)
        {
            p->lift();
            in_state = true;
            return CONTINUE;
        }
        else
        {
            in_state = false;
            state = D_LIFT_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    case D_LIFT_FIRST_SHAPE:
        if (!in_state)
        {
            clock_D_LIFT_FIRST_SHAPE = 0;
            in_state = true;
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
                in_state = false;
                state = GO_TO_SECOND_SHAPE;
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
        if (!in_state)
        {
            // need to account for movement and time since the timestamp was obtained
            STEP_RETURN r = interceptop->step(shape2_x, shape2_y + (lift_ts - shape2_ts) * p->beltVelocity);
            if (r == TERMINATED)
            {
                in_state = true;
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
            in_state = false;
            state = S_LOWER_ON_SECOND_SHAPE;
            return CONTINUE;
        }
        break;
    case S_LOWER_ON_SECOND_SHAPE:
        if (!in_state)
        {
            p->lower(p->HEIGHT_TO_SHAPE2);
            p->drop();
            in_state = true;
            return CONTINUE;
        }
        else
        {
            in_state = false;
            state = D_LOWER_ON_SECOND_SHAPE;
            return CONTINUE;
        }
        break;
    case D_LOWER_ON_SECOND_SHAPE:
        if (!in_state)
        {
            clock_D_LOWER_ON_SECOND_SHAPE = 0;
            in_state = true;
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
                in_state = false;
                state = S_DROP_ON_SECOND_SHAPE;
                return CONTINUE;
            }
        }
        break;
    case S_DROP_ON_SECOND_SHAPE:
        if (!in_state)
        {
            p->drop();
            in_state = true;
            return CONTINUE;
        }
        else
        {
            in_state = false;
            state = D_DROP_ON_SECOND_SHAPE;
            return CONTINUE;
        }
        break;
    case D_DROP_ON_SECOND_SHAPE:
        if (!in_state)
        {
            clock_D_DROP_ON_SECOND_SHAPE = 0;
            in_state = true;
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
                in_state = false;
                state = S_LIFT_ON_SECOND_SHAPE;
                return CONTINUE;
            }
        }
        break;
    case S_LIFT_ON_SECOND_SHAPE:
        if (!in_state)
        {
            p->lift();
            in_state = true;
            return CONTINUE;
        }
        else
        {
            in_state = false;
            state = D_LIFT_ON_SECOND_SHAPE;
            return CONTINUE;
        }
        break;
    case D_LIFT_ON_SECOND_SHAPE:
        if (!in_state)
        {
            clock_D_LIFT_ON_SECOND_SHAPE = 0;
            in_state = true;
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
                in_state = false;
                state = GO_TO_HOME_POSITION;
                return CONTINUE;
            }
        }
        break;
    case GO_TO_HOME_POSITION:
        if (!in_state)
        {
            STEP_RETURN r = pidop->step(HOME_X, HOME_Y);
            if (r == TERMINATED)
            {
                pidop->reset();
                in_state = true;
                return CONTINUE;
            }
            else
            {
                return r;
            }
        }
        else
        {
            in_state = false;
            state = FINDING_FIRST_SHAPE;
            return CONTINUE;
        }
        break;
    default:
        printf("This state should not be reachable");
        return WAIT;
        break;
    }
}