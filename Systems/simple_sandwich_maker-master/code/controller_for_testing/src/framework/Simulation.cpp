#include "Simulation.hpp"
#include "Platform.hpp"
#include "Behaviours.hpp"

#include <stdio.h>

Simulation::Simulation(Platform* _p, Behaviour* _b) {
    p = _p;
    b = _b;
}

Simulation::~Simulation() {

}

void Simulation::step() {
    p->readSensors();
    b->step();
    p->writeActuators();
}