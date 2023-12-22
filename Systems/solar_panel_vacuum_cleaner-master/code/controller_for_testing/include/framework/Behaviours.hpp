#pragma once

class Behaviour {
public:
    virtual ~Behaviour() {};
    virtual void step() = 0;
};