# This is the implementation of the controller used in testing the vacuum cleaner.

1. It uses explicit synchronisation to delimit the simulation cycle (instead of time);

# Compilation and execution instructions

In order to regenerate the Makefile, use the command 'qmake'.
In order to compile the client, run 'make'.
In order to run the simulation, follow these steps:
1. Load the scene initial.ttt
2. Start the simulation in the CoppeliaSim window
3. Start the client program with the parameters shown in the Lua console
For example 'vacuum_cleaner_remoteApi 8888 25 27 24'

Once the simulation is started, it is imediately paused. Once the client
starts, it resumes the simulation.


