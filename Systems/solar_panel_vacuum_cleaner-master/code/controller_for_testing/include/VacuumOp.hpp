#pragma once

#include "framework/Output.hpp"

class VacuumCleanerPlatform;

class VacuumOp: public Output {
    VacuumCleanerPlatform* p;
public:
    double x;
    VacuumOp(double _x, VacuumCleanerPlatform* _p);
    ~VacuumOp();
    void execute();
};