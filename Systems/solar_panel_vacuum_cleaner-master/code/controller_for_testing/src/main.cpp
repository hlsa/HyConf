
// Make sure to have the server side running in V-REP!
// Start the server from a child script with following command:
// simExtRemoteApiStart(portNumber) -- starts a remote API server service on the specified port

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

using namespace std;

extern "C"
{
#include "extApi.h"
}

#include "VacuumCleanerPlatform.hpp"
#include "OutputLeftMotorOp.hpp"
#include "OutputRightMotorOp.hpp"
#include "VacuumCleanerBehaviour.hpp"
#include "Simulation.hpp"
#include "FIFO.hpp"

#include "TestBehaviour.hpp"

int main(int argc, char *argv[])
{
    cout << "Starting" << endl;
    int portNb = 0;
    VacuumCleanerPlatform *p = new VacuumCleanerPlatform();
    // VacuumCleanerBehaviour *b = new VacuumCleanerBehaviour(p);
    TestBehaviour *b = new TestBehaviour(p);
    Simulation *sim = new Simulation((Platform *)p, (Behaviour *)b);
    cout << "Reading arguments" << endl;
    if (argc >= 5)
    {
        portNb = atoi(argv[1]);
        p->leftMotor = atoi(argv[2]);
        p->rightMotor = atoi(argv[3]);
        p->proximitySensor = atoi(argv[4]);
    }
    else
    {
        printf("Indicate following arguments: 'portNumber leftMotor rightMotor proximitySensor'!\n");
        extApi_sleepMs(5000);
        return 0;
    }

    cout << "Initialising client" << endl;

    p->clientID = simxStart((simxChar *)"127.0.0.1", portNb, true, true, 2000, 5);

    if (p->clientID != -1)
    {
        simxSynchronous(p->clientID, true);
        // simxUChar **newmsg;
        // newmsg = (simxUChar **)malloc(100 * sizeof(simxUChar));
        // simxInt *nmsg;
        // nmsg = (simxInt *)malloc(sizeof(simxInt));
        // simxInt value;
        // simxFloat fvalue;
        // simxGetFloatSignal(p->clientID, "liftTS", &fvalue, simx_opmode_streaming);
        // simxGetIntegerSignal(p->clientID, "on", &value, simx_opmode_streaming);
        // simxGetStringSignal(p->clientID, "bread1", newmsg, nmsg, simx_opmode_streaming);
        // simxGetStringSignal(p->clientID, "bread2", newmsg, nmsg, simx_opmode_streaming);
        p->ultrasonicSetup();
        p->leftAccSetup();
        p->rightAccSetup();
        p->batteryLevelSetup();
        p->chargingSetup();
        simxStartSimulation(p->clientID, simx_opmode_oneshot);
        cout << "Starting simulation" << endl;
        while (simxGetConnectionId(p->clientID) != -1)
        {
            sim->step();
            simxSynchronousTrigger(p->clientID);
            // cout << "Step" << endl;
        }
        simxFinish(p->clientID);
    }
    else
    {
        printf("Failed to start simulation\n");
    }
    printf("Simulation finished\n");
    delete p;
    delete b;
    return (0);
}