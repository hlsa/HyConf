#include "PickOp.hpp"
#include "SandwichMakerPlatform.hpp"

PickOp::PickOp(SandwichMakerPlatform* _p) {
    p = _p;
}

PickOp::~PickOp() {

}

void PickOp::execute() {
    p->active = true;
}