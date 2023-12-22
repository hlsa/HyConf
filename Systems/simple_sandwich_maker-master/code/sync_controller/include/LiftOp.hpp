#pragma once

#include "framework/Output.hpp"

class SandwichMakerPlatform;

class LiftOp: public Output {
    SandwichMakerPlatform* p;
public:
    void execute();
    LiftOp(SandwichMakerPlatform* _p);
    ~LiftOp();
};