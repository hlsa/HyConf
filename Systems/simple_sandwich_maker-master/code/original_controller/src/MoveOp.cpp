#include "MoveOp.hpp"
#include "SandwichMakerPlatform.hpp"

MoveOp::MoveOp(double _x, double _y, SandwichMakerPlatform* _p) {
    x = _x;
    y = _y;
    p = _p;
}

MoveOp::~MoveOp() {

}

void MoveOp::execute() {
    p->px=x;
    p->py=y;
}