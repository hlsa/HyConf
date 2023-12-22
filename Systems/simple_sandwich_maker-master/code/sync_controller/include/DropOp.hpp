#pragma once

#include "framework/Output.hpp"

class SandwichMakerPlatform;

class DropOp: public Output{
    SandwichMakerPlatform* p;    
public:
    DropOp(SandwichMakerPlatform* _p);
    ~DropOp();
    void execute();
};