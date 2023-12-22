#include "PIDOp.hpp"
#include "SandwichMakerPlatform.hpp"
#include "SandwichMakerBehaviour.hpp"
#include <math.h>
#include <stdio.h>

PIDOp::PIDOp(SandwichMakerPlatform *_p, SandwichMakerBehaviour *_b) {
    p = _p;
    b = _b;
}

float PIDOp::theta(float x, float y) {
    double aux = sqrt(x * x + y * y);
    return acos(x / aux);

}
float PIDOp::radius(float x, float y) {
    double aux = sqrt(x * x + y * y);
    return aux - 0.32;
}

void PIDOp::reset() {
    state = INITIAL;
}

STEP_RETURN PIDOp::step(float _x, float _y) {
    switch (state) {
        case INITIAL:
            t_x = _x + 0.47;
            t_y = _y;
            p_e_theta = 0.0; p_e_radius = 0.0;
            d_theta = 0.0; d_radius = 0.0;
            i_theta = 0.0; i_radius = 0.0;
            t_theta = theta(t_x, t_y);
            t_radius = radius(t_x, t_y);
            p_e_theta = t_theta - b->c_theta;
            p_e_radius = t_radius - b->c_radius;
            state = STEP;
            return CONTINUE;
        case STEP:
            e_theta = t_theta - b->c_theta;
            e_radius = t_radius - b->c_radius;
            d_theta = (e_theta - p_e_theta)/p->step_size;
            d_radius = (e_radius - p_e_radius)/p->step_size;
            i_theta = i_theta + (e_theta - p_e_theta)*p->step_size;
            i_radius = i_radius + (e_radius - p_e_radius)*p->step_size;
            p->v_theta = (Pt*e_theta+It*i_theta+Dt*d_theta);
            p->v_radius = (Pr*e_radius+Ir*i_radius+Dr*d_radius);
            b->c_theta = b->c_theta + p->v_theta * p->step_size;
	    b->c_radius = b->c_radius + p->v_radius * p->step_size;
            state = CHECK_ERROR;
            return WAIT;
        case CHECK_ERROR:
            if (abs(e_theta) < epsilon_theta && abs(e_radius) < epsilon_radius) {
                p->v_theta = 0.0;
                p->v_radius = 0.0;
                state = FINAL;
            } else {
                p_e_theta = e_theta;
                p_e_radius = e_radius;
                state = STEP;
            }
            return CONTINUE;
        case FINAL:
            return TERMINATED;
        default:
            return WAIT;
    }
}
