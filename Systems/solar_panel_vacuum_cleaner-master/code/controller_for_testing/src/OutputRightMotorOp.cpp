#include "OutputRightMotorOp.hpp"
#include "VacuumCleanerPlatform.hpp"

OutputRightMotorOp::OutputRightMotorOp(double _x, VacuumCleanerPlatform* _p) {
    x = _x;
    p = _p;
}

OutputRightMotorOp::~OutputRightMotorOp() {

}

void OutputRightMotorOp::execute() {
    p->rightV=x;
}