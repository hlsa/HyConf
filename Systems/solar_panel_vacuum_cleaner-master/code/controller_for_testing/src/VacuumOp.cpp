#include "VacuumOp.hpp"
#include "VacuumCleanerPlatform.hpp"
#include <stdio.h>

VacuumOp::VacuumOp(double _x, VacuumCleanerPlatform* _p) {
    x = _x;
    p = _p;
}

VacuumOp::~VacuumOp() {

}

void VacuumOp::execute() {
    // printf("Vacuum set to %f\n", x);
}