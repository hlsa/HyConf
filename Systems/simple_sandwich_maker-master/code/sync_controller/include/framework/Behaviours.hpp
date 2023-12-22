#pragma once

enum STEP_RETURN {
    TERMINATED, CONTINUE, WAIT
};

class Behaviour {
public:
    virtual ~Behaviour() {};
    virtual void step() = 0;
};