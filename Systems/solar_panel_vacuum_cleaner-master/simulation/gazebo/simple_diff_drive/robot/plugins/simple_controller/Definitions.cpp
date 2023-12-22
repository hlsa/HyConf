#include "Definitions.hpp"

double get_linear_speed(double spl, double spr) {
    return (spl+spr)/2;
}

double get_sign(double a) {
    if (a >= 0) return 1.0;
    else return -1.0;
}

Orientation turn_left(Orientation x) {
    switch (x) {
        case Orientation_up: return Orientation_right;
        case Orientation_right: return Orientation_down;
        case Orientation_down: return Orientation_left;
        case Orientation_left: return Orientation_up;
    }
}
Orientation turn_right(Orientation x) {
    switch (x) {
        case Orientation_up: return Orientation_left;
        case Orientation_left: return Orientation_down;
        case Orientation_down: return Orientation_right;
        case Orientation_right: return Orientation_up;
    }
}