#pragma once

#include "framework/Output.hpp"

class VacuumCleanerPlatform;

class OutputRightMotorOp: public Output {
    VacuumCleanerPlatform* p;
public:
    double x, y;
    OutputRightMotorOp(double _x, VacuumCleanerPlatform* _p);
    ~OutputRightMotorOp();
    void execute();
};