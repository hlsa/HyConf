#include "OutputRightMotorOp.hpp"
#include "VacuumCleanerPlatform.hpp"
#include <gazebo/gazebo.hh>


using namespace gazebo;

OutputRightMotorOp::OutputRightMotorOp(double _x, VacuumCleanerPlatform *_p)
{
    x = _x;
    p = _p;
}

OutputRightMotorOp::~OutputRightMotorOp()
{
}

void OutputRightMotorOp::execute()
{
    p->rightF = x/100;
}
