#include "LowerOp.hpp"
#include "SandwichMakerPlatform.hpp"

LowerOp::LowerOp(float HEIGHT, SandwichMakerPlatform* _p) {
    height = HEIGHT;
    p = _p;
}

LowerOp::~LowerOp() {

}

void LowerOp::execute() {
    if (height==p->HEIGHT_TO_SHAPE1)
        p->pz =  (p->pz) - p->HEIGHT_TO_SHAPE1;
    else // if (HEIGHT==HEIGHT_TO_SHAPE2)
        p->pz = (p->pz) - p->HEIGHT_TO_SHAPE2;
}