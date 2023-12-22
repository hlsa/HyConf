#pragma once

#include "Operation.hpp"

class SandwichMakerPlatform;

class InterceptOp: public Operation {
    enum InterceptOp_STATE {
        INITIAL, STEP, FINAL, CHECK_ERROR
    };

    SandwichMakerPlatform *p;
    InterceptOp_STATE state;
    float e_theta, e_radius;
    float p_e_theta, p_e_radius;
    float d_theta, d_radius;
    float i_theta, i_radius;
    float t_x, t_y;
    float t_theta, t_radius;
    
    float Pt = 6.2, It = 0.2, Dt = 0.4;
    float Pr = 5.2, Ir = 0.2, Dr = 0.4;
    float epsilon_theta = 0.05, epsilon_radius = 0.01;

    float theta(float x, float y);
    float radius(float x, float y);

    public:
        ~InterceptOp();
        InterceptOp(SandwichMakerPlatform *p);
        STEP_RETURN step(float x, float y);
        void reset();
};