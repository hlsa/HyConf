#pragma once

#include "framework/Output.hpp"

class VacuumCleanerPlatform;

class OutputLeftMotorOp : public Output
{
    VacuumCleanerPlatform *p;

public:
    double x, y;
    int skip = 0;
    const int MAX = 10;
    OutputLeftMotorOp(double _x, VacuumCleanerPlatform *_p);
    ~OutputLeftMotorOp();
    void execute();
};