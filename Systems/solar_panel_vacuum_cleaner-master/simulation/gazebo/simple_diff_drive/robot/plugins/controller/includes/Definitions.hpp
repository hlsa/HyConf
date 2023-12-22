#ifndef DEFINITIONS_H
#define DEFINITIONS_H
enum Direction {
    Direction_left, Direction_right
};
enum Orientation {
    Orientation_up, Orientation_right, Orientation_down, Orientation_left
};
struct TripleAxis {
    double X;
    double Y;
    double Z;
};
double get_linear_speed(double spl, double spr);
double get_sign(double a);
Orientation turn_left(Orientation x);
Orientation turn_right(Orientation x);
#endif /* DEFINITIONS_H */
