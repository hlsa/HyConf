#pragma once

#include "framework/Output.hpp"

class SandwichMakerPlatform;

class MoveOp: public Output {
    SandwichMakerPlatform* p;
public:
    double x, y;
    MoveOp(double _x, double _y, SandwichMakerPlatform* _p);
    ~MoveOp();
    void execute();
};