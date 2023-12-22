#include "VacuumCleanerPlatform.hpp"
#include <iostream>
#include <math.h>
#include <string.h>
#include "OutputLeftMotorOp.hpp"
#include "OutputRightMotorOp.hpp"
#include "VacuumOp.hpp"
#include "BrushOp.hpp"
#include "Definitions.hpp"

using namespace std;

extern "C"
{
#include "extApi.h"
}

VacuumCleanerPlatform::VacuumCleanerPlatform()
{
}

VacuumCleanerPlatform::~VacuumCleanerPlatform()
{
}

/************************************************************/
/****** INPUT AUXILIARY FUNCTIONS AND READ SENSORS **********/
/************************************************************/


void VacuumCleanerPlatform::ultrasonicSetup()
{
	printf("Setting up ultrasonic sensor\n");
	simxUChar detectionState = 0;
	simxFloat* detectedPoint;
	detectedPoint = (simxFloat*)malloc(3*sizeof(simxFloat));
	simxInt* detectedObjectHandle;
	detectedObjectHandle = (simxInt*)malloc(sizeof(simxInt));
	simxFloat* detectedSurfaceNormalVector;
	detectedSurfaceNormalVector = (simxFloat*)malloc(3*sizeof(simxFloat));
	simxReadProximitySensor(clientID,proximitySensor,&detectionState,detectedPoint,detectedObjectHandle,detectedSurfaceNormalVector,simx_opmode_streaming);
	if (detectionState == 0) {// && past1 == 0 && past2 == 0 && past3 == 0) {
		// printf("NOT DETECTED\n");
		ultrasonic_event = true;
		ultrasonic = 1000;
	} else {
		// printf("DETECTED\n");
		ultrasonic_event = true;
		ultrasonic = detectedPoint[2];
	}
	delete detectedPoint;
	delete detectedObjectHandle;
	delete detectedSurfaceNormalVector;
}

void VacuumCleanerPlatform::readUltrasonic()
{
	simxUChar detectionState = 0;
	simxFloat* detectedPoint;
	detectedPoint = (simxFloat*)malloc(3*sizeof(simxFloat));
	simxInt* detectedObjectHandle;
	detectedObjectHandle = (simxInt*)malloc(sizeof(simxInt));
	simxFloat* detectedSurfaceNormalVector;
	detectedSurfaceNormalVector = (simxFloat*)malloc(3*sizeof(simxFloat));
	simxInt r1 = simxReadProximitySensor(clientID,proximitySensor,&detectionState,detectedPoint,detectedObjectHandle,detectedSurfaceNormalVector,simx_opmode_buffer);
	if (r1 == simx_return_ok) {
		// printf("READ: %u %f %f %f\n", detectionState, detectedPoint[0], detectedPoint[1], detectedPoint[2]);
		// cout << detectionState << detectedPoint[0] << detectedPoint[1] << detectedPoint[2] << endl;
		if (detectionState == 0) {// && past1 == 0 && past2 == 0 && past3 == 0) {
			// printf("NOT DETECTED\n");
			// if the robot can't detect an obstacle, it is because it is over the edge. I'm using 1000 to make sure it is greater than the height of the robot.
			ultrasonic_event = true;
			ultrasonic = 1000.0;
		} else {
			// printf("DETECTED\n");
			ultrasonic_event = true;
			ultrasonic = detectedPoint[2];
		}
		// past3 = past2;
		// past2 = past1;
		// past1 = detectionState;
	} else {
		printf("NOT READ\n");
	}
	delete detectedPoint;
	delete detectedObjectHandle;
	delete detectedSurfaceNormalVector;
}

void VacuumCleanerPlatform::leftAccSetup() {
	printf("Setting up left accelerometer\n");
	simxFloat x;
	simxFloat y;
	simxFloat z;
	bool event = true;
	if (simxGetFloatSignal(clientID, "left-x-accel", &x, simx_opmode_streaming) != simx_return_ok) {
		event = event && false;
	}
	if (simxGetFloatSignal(clientID, "left-y-accel", &y, simx_opmode_streaming) != simx_return_ok) {
		event = event && false;
	}
	if (simxGetFloatSignal(clientID, "left-z-accel", &z, simx_opmode_streaming) != simx_return_ok) {
		event = event && false;
	}
	if (event) {
		acc_l_event = true;
		acc_l.X = y;
		acc_l.Y = -x;
		acc_l.Z = z;
		// printf("left - X-Accel: %f, Y-Accel: %f, Z-Accel: %f\n", x,y,z);
	} else {
		printf("failed to read left accelerometer\n");
	}
}

void VacuumCleanerPlatform::readLeftAcc() {
	simxFloat x;
	simxFloat y;
	simxFloat z;
	bool event = true;
	if (simxGetFloatSignal(clientID, "left-x-accel", &x, simx_opmode_buffer) != simx_return_ok) {
		event = event && false;
	}
	if (simxGetFloatSignal(clientID, "left-y-accel", &y, simx_opmode_buffer) != simx_return_ok) {
		event = event && false;
	}
	if (simxGetFloatSignal(clientID, "left-z-accel", &z, simx_opmode_buffer) != simx_return_ok) {
		event = event && false;
	}
	if (event) {
		acc_l_event = true;
		acc_l.X = y;
		acc_l.Y = -x;
		acc_l.Z = z;
		// printf("left - X-Accel: %f, Y-Accel: %f, Z-Accel: %f\n", x,y,z);
	} else {
		printf("failed to read left accelerator\n");
	}
}

void VacuumCleanerPlatform::rightAccSetup() {
	printf("Setting up right accelerometer\n");
	simxFloat x;
	simxFloat y;
	simxFloat z;
	bool event = true;
	if (simxGetFloatSignal(clientID, "right-x-accel", &x, simx_opmode_streaming) != simx_return_ok) {
		event = event && false;
	}
	if (simxGetFloatSignal(clientID, "right-y-accel", &y, simx_opmode_streaming) != simx_return_ok) {
		event = event && false;
	}
	if (simxGetFloatSignal(clientID, "right-z-accel", &z, simx_opmode_streaming) != simx_return_ok) {
		event = event && false;
	}
	if (event) {
		acc_r_event = true;
		acc_r.X = y;
		acc_r.Y = -x;
		acc_r.Z = z;
		// printf("right - X-Accel: %f, Y-Accel: %f, Z-Accel: %f\n", x,y,z);
	} else {
		printf("failed to read right accelerometer\n");
	}
}

void VacuumCleanerPlatform::readRightAcc() {
	simxFloat x;
	simxFloat y;
	simxFloat z;
	bool event = true;
	if (simxGetFloatSignal(clientID, "right-x-accel", &x, simx_opmode_buffer) != simx_return_ok) {
		event = event && false;
	}
	if (simxGetFloatSignal(clientID, "right-y-accel", &y, simx_opmode_buffer) != simx_return_ok) {
		event = event && false;
	}
	if (simxGetFloatSignal(clientID, "right-z-accel", &z, simx_opmode_buffer) != simx_return_ok) {
		event = event && false;
	}
	if (event) {
		acc_r_event = true;
		acc_r.X = y;
		acc_r.Y = -x;
		acc_r.Z = z;
		// printf("right - X-Accel: %f, Y-Accel: %f, Z-Accel: %f\n", x,y,z);
	} else {
		printf("failed to read right accelerator\n");
	}	
}

    void VacuumCleanerPlatform::readCharging() {
		charging_event = false;
	}
    void VacuumCleanerPlatform::chargingSetup() {
		printf("Setting up charging sensor\n");
	}
    void VacuumCleanerPlatform::readBatteryLevel() {
		battery_level_event = true;
		battery_level = 100.0;
	}
    void VacuumCleanerPlatform::batteryLevelSetup() {
		printf("Setting up battery level sensor\n");
	}

void VacuumCleanerPlatform::add_output(Output *o)
{
	if (output_index < NOUTPUTS)
	{
		outputs[output_index++] = o;
	}
}

void VacuumCleanerPlatform::readSensors()
{
	// reset events
	ultrasonic_event = false;
	acc_l_event = false;
	acc_r_event = false;
	charging_event = false;
	battery_level_event = false;

	// Read events from hardware
	readUltrasonic();
	readLeftAcc();
	readRightAcc();
	readCharging();
	readBatteryLevel();
}

/************************************************************/
/***** OUTPUT AUXILIARY FUNCTIONS AND WRITE ACTUATORS *******/
/************************************************************/

// This function corresponds the operation output_left_motor in RoboSim.
void VacuumCleanerPlatform::output_left_motor(double x)
{
	OutputLeftMotorOp *op = new OutputLeftMotorOp(x, this);
	add_output(op);
}

// This function corresponds the operation output_right_motor in RoboSim.
void VacuumCleanerPlatform::output_right_motor(double x)
{
	OutputRightMotorOp *op = new OutputRightMotorOp(x, this);
	add_output(op);
}

// This function corresponds the operation brush in RoboSim.
void VacuumCleanerPlatform::brush(double x)
{
	BrushOp *op = new BrushOp(x, this);
	add_output(op);
}

// This function corresponds the operation vacuum in RoboSim.
void VacuumCleanerPlatform::vacuum(double x)
{
	VacuumOp *op = new VacuumOp(x, this);
	add_output(op);
}

void VacuumCleanerPlatform::writeActuators()
{
	// execute operation in order.
	// each operation updates the platform mapping's outputs
	for (int i = 0; i < output_index; i++)
	{
		outputs[i]->execute();
	}

	// change the frame of reference of any variables (pz in this case)
	// send the platform mapping's outputs to V-REP
	simxSetJointTargetVelocity(clientID, leftMotor, leftV, simx_opmode_oneshot);
	simxSetJointTargetVelocity(clientID, rightMotor, rightV, simx_opmode_oneshot);
	// simxSetJointMaxForce(clientID, leftMotor, leftV, simx_opmode_oneshot);
	// simxSetJointMaxForce(clientID, rightMotor, rightV, simx_opmode_oneshot);

	// simxSetIntegerSignal(clientID, "gripper_active", active ? 1 : 0, simx_opmode_oneshot);
	// simxSetFloatSignal(clientID, "h", h, simx_opmode_oneshot);

	// free outputs and reset index
	for (int i = 0; i < output_index; i++)
	{
		delete outputs[i];
	}
	output_index = 0;
}