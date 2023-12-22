#pragma once
#include "Platform.hpp"

class Output {
public:
    virtual ~Output() {};
    virtual void execute() = 0;
};