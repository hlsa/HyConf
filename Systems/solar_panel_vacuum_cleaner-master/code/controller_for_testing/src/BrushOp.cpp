#include "BrushOp.hpp"
#include "VacuumCleanerPlatform.hpp"
#include <stdio.h>

BrushOp::BrushOp(double _x, VacuumCleanerPlatform* _p) {
    x = _x;
    p = _p;
}

BrushOp::~BrushOp() {

}

void BrushOp::execute() {
    // printf("Brush set to %f\n", x);
}