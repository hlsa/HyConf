#pragma once

#include "framework/Output.hpp"

class SandwichMakerPlatform;

class LowerOp: public Output{
    SandwichMakerPlatform* p;
    float height;
public:
    LowerOp(float HEIGHT,SandwichMakerPlatform* _p);
    ~LowerOp();
    void execute();
};