#pragma once

#include "framework/Output.hpp"

class VacuumCleanerPlatform;

class BrushOp : public Output
{
    VacuumCleanerPlatform *p;

public:
    double x;
    BrushOp(double _x, VacuumCleanerPlatform *_p);
    ~BrushOp();
    void execute();
};