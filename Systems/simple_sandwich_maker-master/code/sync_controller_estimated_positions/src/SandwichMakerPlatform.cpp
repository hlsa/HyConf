#include "SandwichMakerPlatform.hpp"
#include <iostream>
#include <math.h>
#include <string.h>

#include "MoveOp.hpp"
#include "PickOp.hpp"
#include "DropOp.hpp"
#include "LowerOp.hpp"
#include "LiftOp.hpp"

using namespace std;

extern "C"
{
#include "extApi.h"
}

SandwichMakerPlatform::SandwichMakerPlatform()
{
}

SandwichMakerPlatform::~SandwichMakerPlatform()
{
}

/************************************************************/
/****** INPUT AUXILIARY FUNCTIONS AND READ SENSORS **********/
/************************************************************/

bool SandwichMakerPlatform::on()
{
	return on_event;
}

bool SandwichMakerPlatform::off()
{
	return off_event;
}

bool SandwichMakerPlatform::is_home()
{
	double x = (c_radius + 0.32) * cos(c_theta) - 0.47;
	double y = sqrt((c_radius + 0.32) * (c_radius + 0.32) - x * x);
	if (fabs(x) < 0.001 && fabs(y) < 0.001)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SandwichMakerPlatform::readLiftTS()
{
	simxFloat value = 0;
	simxInt r1 = simxGetFloatSignal(clientID, "liftTS", &value, simx_opmode_buffer);
	if (r1 == 0)
	{
		simxClearFloatSignal(clientID, "liftTS", simx_opmode_oneshot);
		liftTS.event = true;
		liftTS.value = value;
	}
}

void SandwichMakerPlatform::readButton()
{
	simxInt value = 0;
	simxInt r1 = simxGetIntegerSignal(clientID, "on", &value, simx_opmode_buffer);
	if (r1 == 0)
	{
		if (value == 0)
		{
			on_event = false;
			off_event = true;
		}
		else
		{
			on_event = true;
			off_event = false;
		}
	}
}

void SandwichMakerPlatform::readCamera1()
{
	simxUChar **newmsg;
	newmsg = (simxUChar **)malloc(100 * sizeof(simxUChar));
	simxInt *nmsg;
	nmsg = (simxInt *)malloc(sizeof(simxInt));
	simxInt r1 = simxGetStringSignal(clientID, "bread1", newmsg, nmsg, simx_opmode_buffer);
	if (r1 == 0 && *nmsg > 0)
	{
		char *value = strtok((char *)*newmsg, " ");
		int n;
		sscanf(value, "A%d", &n);
		value = strtok(NULL, " ");
		while (value != NULL)
		{
			// read x value
			sscanf(value, "x=%lf", &(lShapeX.value));
			lShapeX.event = true;
			// get next token
			value = strtok(NULL, " ");
			// read y value
			sscanf(value, "y=%lf", &(lShapeY.value));
			lShapeY.event = true;
			value = strtok(NULL, " ");
			// read timestamp
			sscanf(value, "t=%lf", &(lShapeTS.value));
			lShapeTS.event = true;

			// get next token
			value = strtok(NULL, " ");
		}
	}
	free(newmsg);
	free(nmsg);
}

void SandwichMakerPlatform::readCamera2()
{
	simxUChar **newmsg;
	newmsg = (simxUChar **)malloc(100 * sizeof(simxUChar));
	simxInt *nmsg;
	nmsg = (simxInt *)malloc(sizeof(simxInt));
	simxInt r2 = simxGetStringSignal(clientID, "bread2", newmsg, nmsg, simx_opmode_buffer);
	if (r2 == 0 && *nmsg > 0)
	{
		char *value = strtok((char *)*newmsg, " ");
		int n;
		sscanf(value, "B%d", &n);
		value = strtok(NULL, " ");
		while (value != NULL)
		{
			// read x value
			sscanf(value, "x=%lf", &(rShapeX.value));
			rShapeX.event = true;
			// get next token
			value = strtok(NULL, " ");
			// read y value
			sscanf(value, "y=%lf", &(rShapeY.value));
			rShapeY.event = true;
			value = strtok(NULL, " ");
			// read timestamp
			sscanf(value, "t=%lf", &(rShapeTS.value));
			rShapeTS.event = true;
			// get next token
			value = strtok(NULL, " ");
		}
	}
	free(newmsg);
	free(nmsg);
}

void SandwichMakerPlatform::add_output(Output *o)
{
	if (output_index < NOUTPUTS)
	{
		outputs[output_index++] = o;
	}
}

void SandwichMakerPlatform::readSensors()
{
	// forget previous events
	lShapeX.event = false;
	lShapeY.event = false;
	lShapeTS.event = false;
	rShapeX.event = false;
	rShapeY.event = false;
	rShapeTS.event = false;

	on_event = false;
	off_event = true;

	liftTS.event = false;

	if (isnan(c_theta))
	{
		c_theta = 0.0;
	}
	if (isnan(c_radius))
	{
		c_radius = 0.0;
	}

	// Read button
	readButton();
	readLiftTS();
	// Read messages from camera.
	readCamera1();
	readCamera2();
	//printf("Cameras: %d, %d\n", lShapeX.event, rShapeX.event);
}

/************************************************************/
/***** OUTPUT AUXILIARY FUNCTIONS AND WRITE ACTUATORS *******/
/************************************************************/

// This function corresponds the operation move in RoboSim.
void SandwichMakerPlatform::move(double x, double y)
{
	MoveOp *op = new MoveOp(x, y, this);
	add_output(op);
}
// This function corresponds the operation pick in RoboSim.
void SandwichMakerPlatform::pick()
{
	PickOp *op = new PickOp(this);
	add_output(op);
}
// This function corresponds the operation drop in RoboSim.
void SandwichMakerPlatform::drop()
{
	DropOp *op = new DropOp(this);
	add_output(op);
}
// This function corresponds the operation lower in RoboSim.
void SandwichMakerPlatform::lower(float HEIGHT)
{
	LowerOp *op = new LowerOp(HEIGHT, this);
	add_output(op);
}

// This function corresponds the operation lift in RoboSim.
void SandwichMakerPlatform::lift()
{
	LiftOp *op = new LiftOp(this);
	add_output(op);
}

void SandwichMakerPlatform::writeActuators()
{
	// execute operation in order.
	// each operation updates the platform mapping's outputs
	for (int i = 0; i < output_index; i++)
	{
		outputs[i]->execute();
	}

	// change the frame of reference of any variables (pz in this case)
	// send the platform mapping's outputs to V-REP
	simxSetJointTargetVelocity(clientID, rj, v_theta, simx_opmode_oneshot);
	simxSetJointTargetVelocity(clientID, pj1, v_radius, simx_opmode_oneshot);
	double h = 0.235 - pz;
	simxSetJointTargetPosition(clientID, pj2, h, simx_opmode_oneshot);
	simxSetIntegerSignal(clientID, "gripper_active", active ? 1 : 0, simx_opmode_oneshot);

	simxSetFloatSignal(clientID, "h", h, simx_opmode_oneshot);
	// free outputs and reset index
	for (int i = 0; i < output_index; i++)
	{
		delete outputs[i];
	}
	output_index = 0;
	c_theta = c_theta + v_theta * step_size;
	c_radius = c_radius + v_radius * step_size;
}