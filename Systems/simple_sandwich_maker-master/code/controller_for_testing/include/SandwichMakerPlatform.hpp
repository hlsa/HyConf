#pragma once

#include "framework/Platform.hpp"
class FIFO;
class Output;
class MoveOp;
class ActivateGripperOp;
class DeactivateGripperOp;
class LowerOp;
class LiftOp;

const int NINPUTS = 4;
const int NOUTPUTS = 6;

struct DoubleEvent
{
    bool event;
    double value;
};

class SandwichMakerPlatform : public Platform
{

    Output *outputs[NOUTPUTS];
    int output_index = 0;

    void add_output(Output *o);

    void readLiftTS();
    void readCamera1();
    void readCamera2();
    void readButton();

public:
    /// Platform constants
    double HEIGHT_TO_SHAPE1 = 0.224;                       // We do not know anything about the actual value of height.
    double HEIGHT_TO_SHAPE2 = 0.224;                       // We do not know anything about the actual value of height.
    float WAIT_LOWER1 = HEIGHT_TO_SHAPE1 / VERTICAL_SPEED; // (s) // Although we can calculate WAIT_LOWER1 based on HEIGHT_TO_SHAPE1 and ARM_SPEED,
    //we define it as a constant in this program because in the RoboSim model we use WAIT_LOWER1 as
    // a constant in [sinceEntry(D_LOWER_ON_FIRST_SHAPE)<=WAIT_LOWER1].

    float WAIT_LOWER2 = HEIGHT_TO_SHAPE2 / VERTICAL_SPEED; // (s) // Although we can calculate WAIT_LOWER2 based on HEIGHT_TO_SHAPE2 and ARM_SPEED,
                                                           //we define it as a constant in this program because in the RoboSim model we use WAIT_LOWER2 as
                                                           // a constant in [sinceEntry(ON_SECOND_SHAPE)>WAIT_LOWER2].
    float VERTICAL_SPEED = 1.0;                            // m/s

    // Platform variables
    int clientID;
    int rj;
    int pj1;
    int pj2;
    double beltVelocity = 0.102; // m/s
    // speed of the vertical prismatic joint
    double hv = 1; // m/s
    // avoid duplication of events -- INTERNAL
    float step_size = 0.05;
    
    /************************************************************/
    /********************** PLATFORM SENSORS **********************/
    /************************************************************/
    // input events on and off
    bool on_event = false;
    bool off_event = true;

    // input events x and y for cameras 1 and 2 (currently implemented
    // in terms of a single sensor)
    DoubleEvent lShapeX, lShapeY, lShapeTS, rShapeX, rShapeY, rShapeTS;
    DoubleEvent liftTS;

    /************************************************************/
    /********************* PLATFORM ACTUATORS **********************/
    /************************************************************/

    // position control of the arm -- changing these variables makes the arm move to that position
    double px = 0;
    double py = 0;
    double pz = 0.235;
    // activate and deactivate gripper
    bool active = false;

    // I believe the PLATFORM ACTUATORS should be
    // theta_v, radius_v to control the velocity of the arm in the x-y plane
    // pz to control the height of second prismatic joint
    float v_theta = 0.0, v_radius = 0.0;

    SandwichMakerPlatform();
    ~SandwichMakerPlatform();

    /************************************************************/
    /****** INPUT AUXILIARY FUNCTIONS AND WRITE ACTUATORS *******/
    /* PLATFORM MAPPING */
    /************************************************************/

    bool on();
    bool off();

    void readSensors();

    /************************************************************/
    /***** OUTPUT AUXILIARY FUNCTIONS AND WRITE ACTUATORS *******/
    /* PLATFORM MAPPING */
    /************************************************************/

    void move(double x, double y);
    void lower(float HEIGHT);
    void lift();
    void pick();
    void drop();

    void writeActuators();
};