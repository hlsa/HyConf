#pragma once

#include "framework/Output.hpp"
#include <gazebo/gazebo.hh>

using namespace gazebo;

class VacuumCleanerPlatform;

class OutputRightMotorOp : public Output
{
    VacuumCleanerPlatform *p;

public:
    double x, y;
    int skip = 0;
    const int MAX = 10;
    OutputRightMotorOp(double _x, VacuumCleanerPlatform *_p);
    ~OutputRightMotorOp();
    void execute();
};
