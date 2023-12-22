#pragma once

const double STEP_SIZE = 0.001; // small step
const int N = 10; // big step
const int WARM_UP = 100; // initial steps used to normalise sensors
const double epsilon = 0.000000001;

// Platform constants
const double WHL_DIST = 0.04; // distance between the wheel and the centre of the robot
const double ACC_DIST = 0.045; // distance betwen the acceleromters and the centre of the robot
const double BATTERY_RATE = 0.1; // 1% usage per second
const double BRIDGE_LENGTH = 0.2;
const double ROBOT_LENGTH = 0.1;
const double EDGE_HEIGHT = 0.3;
const int CHARGING_TIME = floor(10.0/STEP_SIZE);
const double WHL_RADIUS = 0.01;

// angle PID constants
const double CangleP = 0.9;
const double CangleI = 0.0000;
const double CangleD = 0.000;

// velocity PID constants
const double VELOCITY_P = 2;
const double VELOCITY_I = 0.01;
const double VELOCITY_D = 0.01;

// software constants
const double BATTERY_LOW = 10.0; //%


