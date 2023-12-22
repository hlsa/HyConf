#include "DropOp.hpp"
#include "SandwichMakerPlatform.hpp"

DropOp::DropOp(SandwichMakerPlatform* _p) {
    p = _p;
}

DropOp::~DropOp() {

}

void DropOp::execute() {
    p->active = false;
}