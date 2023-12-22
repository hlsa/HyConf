#include "OutputLeftMotorOp.hpp"
#include "VacuumCleanerPlatform.hpp"

using namespace gazebo;

OutputLeftMotorOp::OutputLeftMotorOp(double _x, VacuumCleanerPlatform *_p)
{
    x = _x;
    p = _p;
}

OutputLeftMotorOp::~OutputLeftMotorOp()
{
}

void OutputLeftMotorOp::execute()
{
    //p->leftF = x/100;
    double leftF = x/100;
    printf("applying force %f to left actuator\n", leftF);
    p->left->SetForce(0,leftF);
}
