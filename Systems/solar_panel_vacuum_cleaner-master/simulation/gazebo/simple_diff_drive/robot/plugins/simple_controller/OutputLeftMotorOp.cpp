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
    this->p->jointController->SetVelocityTarget(
    this->p->left->GetScopedName(), x);
}
