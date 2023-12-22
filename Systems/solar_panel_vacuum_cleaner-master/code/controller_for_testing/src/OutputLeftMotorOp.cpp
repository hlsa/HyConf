#include "OutputLeftMotorOp.hpp"
#include "VacuumCleanerPlatform.hpp"

OutputLeftMotorOp::OutputLeftMotorOp(double _x, VacuumCleanerPlatform* _p) {
    x = _x;
    p = _p;
}

OutputLeftMotorOp::~OutputLeftMotorOp() {

}

void OutputLeftMotorOp::execute() {
    p->leftV=x;
}