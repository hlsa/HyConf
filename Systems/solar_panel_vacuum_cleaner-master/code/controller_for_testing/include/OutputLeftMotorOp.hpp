#pragma once

#include "framework/Output.hpp"

class VacuumCleanerPlatform;

class OutputLeftMotorOp: public Output {
    VacuumCleanerPlatform* p;
public:
    double x, y;
    OutputLeftMotorOp(double _x, VacuumCleanerPlatform* _p);
    ~OutputLeftMotorOp();
    void execute();
};