#include "InterceptOp.hpp"
#include "SandwichMakerPlatform.hpp"
#include <math.h>
#include <stdio.h>


InterceptOp::InterceptOp(SandwichMakerPlatform *_p) {
    p = _p;
}

InterceptOp::~InterceptOp() {
    printf("InterceptOp destroyed\n");
}

float InterceptOp::theta(float x, float y) {
    double aux = sqrt(x * x + y * y);
    return acos(x / aux);

}
float InterceptOp::radius(float x, float y) {
    double aux = sqrt(x * x + y * y);
    return aux - 0.32;
}

void InterceptOp::reset() {
    state = INITIAL;
}


STEP_RETURN InterceptOp::step(float _x, float _y) {
    switch (state) {
        case INITIAL:
            t_x = _x + 0.47;
            t_y = _y;
            p_e_theta = 0.0; p_e_radius = 0.0;
            d_theta = 0.0; d_radius = 0.0;
            i_theta = 0.0; i_radius = 0.0;
            t_theta = theta(t_x, t_y);
            t_radius = radius(t_x, t_y);
            p_e_theta = t_theta - p->c_theta;
            p_e_radius = t_radius - p->c_radius;
            state = STEP;
            return CONTINUE;
        case STEP:
            t_theta = theta(t_x, t_y);
            t_radius = radius(t_x, t_y);
            e_theta = t_theta - p->c_theta;
            e_radius = t_radius - p->c_radius;
            d_theta = (e_theta - p_e_theta)/p->step_size;
            d_radius = (e_radius - p_e_radius)/p->step_size;
            i_theta = i_theta + (e_theta - p_e_theta)*p->step_size;
            i_radius = i_radius + (e_radius - p_e_radius)*p->step_size;
            p->v_theta = (Pt*e_theta+It*i_theta+Dt*d_theta);
            p->v_radius = (Pr*e_radius+Ir*i_radius+Dr*d_radius);
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
                t_y = t_y + p->beltVelocity*p->step_size;
                state = STEP;
            }
            return CONTINUE;
        case FINAL:
            return TERMINATED;
        default:
            return WAIT;
    }
}