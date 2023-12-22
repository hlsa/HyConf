# This is the implementation of the controller used in testing the robotic arm.

1. It removes the button to facilitate automation of tests;
2. It uses explicit synchronisation to delimit the simulation cycle (instead of time);
3. It estimates the position of the arm instead o using a sensor. This introduces
errors that increase with time, and invariably lead to failure.

# Compilation and execution instructions

In order to regenerate the Makefile, use the command 'qmake'.
In order to compile the client, run 'make'.
In order to run the simulation, follow these steps:
1. Load the scene simple_bullet_remote.ttt
2. Start the simulation in the vrep window
3. Start the client program with the parameters shown in the Lua console
For example 'simple_bullet_remoteApi 8888 19 17 18'

Once the simulation is started, it is imediately paused. Once the client
starts, it resumes the simulation.


