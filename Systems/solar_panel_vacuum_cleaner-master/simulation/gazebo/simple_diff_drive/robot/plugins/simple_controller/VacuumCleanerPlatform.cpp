#include "VacuumCleanerPlatform.hpp"
#include <iostream>
#include <math.h>
#include <string.h>
#include "OutputLeftMotorOp.hpp"
#include "OutputRightMotorOp.hpp"
#include "VacuumOp.hpp"
#include "BrushOp.hpp"
#include "Definitions.hpp"
#include "consts.hpp"

using namespace std;

// extern "C"
// {
// #include "extApi.h"
// }

using namespace gazebo;

VacuumCleanerPlatform::VacuumCleanerPlatform()
{
}

VacuumCleanerPlatform::~VacuumCleanerPlatform()
{
}

/************************************************************/
/****** INPUT AUXILIARY FUNCTIONS AND READ SENSORS **********/
/************************************************************/

void VacuumCleanerPlatform::readUltrasonic()
{
	double range = this->ultrasonic_sensor->Range(0);
	ultrasonic_event = true;
	ultrasonic = range;
}

void VacuumCleanerPlatform::readLeftAcc()
{
	ignition::math::Vector3d acc = this->acc_left->LinearAcceleration();
	acc_l_event = true;
	acc_l.X = acc.X();
	acc_l.Y = acc.Y();
	acc_l.Z = -acc.Z();
}

void VacuumCleanerPlatform::readRightAcc()
{
	ignition::math::Vector3d acc = this->acc_right->LinearAcceleration();
	acc_r_event = true;
	acc_r.X = acc.X();
	acc_r.Y = acc.Y();
	acc_r.Z = -acc.Z();
}

void VacuumCleanerPlatform::readCharging()
{
	charging_event = false;
}

void VacuumCleanerPlatform::readBatteryLevel()
{
	battery_level_event = true;
	battery_level = 100.0;
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

	this->jointController->Update();

	// free outputs and reset index
	for (int i = 0; i < output_index; i++)
	{
		delete outputs[i];
	}
	output_index = 0;

	// update battery after usage
	battery -= STEP_SIZE*BATTERY_RATE;
}
