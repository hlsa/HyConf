#include "LiftOp.hpp"
#include "SandwichMakerPlatform.hpp"
#include <math.h>

extern "C" {
    #include "extApi.h"
}

LiftOp::LiftOp(SandwichMakerPlatform* _p) {
    p = _p;
}

LiftOp::~LiftOp() {

}

void LiftOp::execute() {
    p->pz = 0.235;	
}