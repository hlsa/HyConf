#pragma once

class Platform {
public:
    virtual ~Platform() {};
    virtual void readSensors() = 0;
    virtual void writeActuators() = 0;
};