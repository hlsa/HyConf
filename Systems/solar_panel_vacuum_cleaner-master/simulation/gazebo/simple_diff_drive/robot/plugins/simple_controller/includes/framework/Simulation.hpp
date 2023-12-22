#pragma once

class Platform;
class Behaviour;

class Simulation {
    Platform* p;
    Behaviour* b;
public:
    Simulation(Platform* _p, Behaviour* _b);
    ~Simulation();
    void step();
};