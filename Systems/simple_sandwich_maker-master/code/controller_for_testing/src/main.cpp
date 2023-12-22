
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

#include "SandwichMakerPlatform.hpp"
#include "MoveOp.hpp"
#include "SandwichMakerBehaviour.hpp"
#include "Simulation.hpp"
#include "FIFO.hpp"

int main(int argc, char *argv[])
{
    cout << "Starting" << endl;
    int portNb = 0;
    SandwichMakerPlatform *p = new SandwichMakerPlatform();
    SandwichMakerBehaviour *b = new SandwichMakerBehaviour(p);
    Simulation *sim = new Simulation((Platform *)p, (Behaviour *)b);
    if (argc >= 5)
    {
        portNb = atoi(argv[1]);
        p->rj = atoi(argv[2]);
        p->pj1 = atoi(argv[3]);
        p->pj2 = atoi(argv[4]);
    }
    else
    {
        printf("Indicate following arguments: 'portNumber revoluteJoint PrismaticJoint PrimaticJoint'!\n");
        extApi_sleepMs(5000);
        return 0;
    }

    p->clientID = simxStart((simxChar *)"127.0.0.1", portNb, true, true, 2000, 5);

    if (p->clientID != -1)
    {
        simxSynchronous(p->clientID, true);
        simxUChar **newmsg;
        newmsg = (simxUChar **)malloc(100 * sizeof(simxUChar));
        simxInt *nmsg;
        nmsg = (simxInt *)malloc(sizeof(simxInt));
        simxInt value;
        simxFloat fvalue;
        simxGetFloatSignal(p->clientID, "liftTS", &fvalue, simx_opmode_streaming);
        simxGetIntegerSignal(p->clientID, "on", &value, simx_opmode_streaming);
        simxGetStringSignal(p->clientID, "bread1", newmsg, nmsg, simx_opmode_streaming);
        simxGetStringSignal(p->clientID, "bread2", newmsg, nmsg, simx_opmode_streaming);
        simxStartSimulation(p->clientID, simx_opmode_oneshot);
        while (simxGetConnectionId(p->clientID) != -1)
        {
            sim->step();
            simxSynchronousTrigger(p->clientID);
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