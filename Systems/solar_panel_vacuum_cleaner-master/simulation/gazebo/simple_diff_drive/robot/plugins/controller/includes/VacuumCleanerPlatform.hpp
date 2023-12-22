#pragma once

#include "framework/Platform.hpp"
#include "Definitions.hpp"
#include <gazebo/physics/physics.hh>
#include <gazebo/sensors/sensors.hh>

class FIFO;
class Output;
class OutputLegtMotorOp;
class OutputRightMotorOp;
class VacuumOp;
class BrushOp;

// const int NINPUTS = 1 // ultrasonic, chargin, battery_level, acc_l, acc_r
const int NOUTPUTS = 4; // output_left_motor and output_right_motor, brush, vacuum

using namespace gazebo;

class VacuumCleanerPlatform : public Platform
{
    Output *outputs[NOUTPUTS];
    int output_index = 0;
    void add_output(Output *o);

public:
    // Platform variables
    physics::JointPtr left, right;
    sensors::ImuSensorPtr acc_left, acc_right;
    sensors::RaySensorPtr ultrasonic_sensor;

    float step_size = 0.05;

    double base_lX = 0;
    double base_lY = 0;
    double base_lZ = 0;
    double base_rX = 0;
    double base_rY = 0;
    double base_rZ = 0;

    double leftF = 0.0;
    double rightF = 0.0;

    /************************************************************/
    /********************** PLATFORM SENSORS **********************/
    /************************************************************/

    double ultrasonic;
    bool ultrasonic_event;
    void readUltrasonic();
    // void ultrasonicSetup();

    TripleAxis acc_l;
    bool acc_l_event;
    void readLeftAcc();
    // void leftAccSetup();

    TripleAxis acc_r;
    bool acc_r_event;
    void readRightAcc();
    // void rightAccSetup();

    bool charging_event;
    void readCharging();
    // void chargingSetup();

    double battery_level;
    bool battery_level_event;
    void readBatteryLevel();
    // void batteryLevelSetup();

    /************************************************************/
    /********************* PLATFORM ACTUATORS **********************/
    /************************************************************/

    VacuumCleanerPlatform();
    ~VacuumCleanerPlatform();

    /************************************************************/
    /****** INPUT AUXILIARY FUNCTIONS AND WRITE ACTUATORS *******/
    /* PLATFORM MAPPING */
    /************************************************************/

    void readSensors();

    /************************************************************/
    /***** OUTPUT AUXILIARY FUNCTIONS AND WRITE ACTUATORS *******/
    /* PLATFORM MAPPING */
    /************************************************************/

    void output_left_motor(double x);
    void output_right_motor(double x);
    void brush(double x);
    void vacuum(double x);
    void writeActuators();
};
