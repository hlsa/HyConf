#pragma once

enum STEP_RETURN
{
    TERMINATED,
    CONTINUE,
    WAIT
};

class Operation {
public:
    virtual ~Operation() {};
    // virtual STEP_RETURN step() = 0;
};