#pragma once

#include "framework/Output.hpp"

class SandwichMakerPlatform;

class PickOp: public Output{
    SandwichMakerPlatform* p;
public:
    PickOp(SandwichMakerPlatform* _p);
    ~PickOp();
    void execute();
};