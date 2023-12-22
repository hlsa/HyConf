# Sandwich Maker case study
This repository contains all the artifacts developed in the sandwich maker case study. These are structured as follows:

## models 
These are different models designed during the development of the case study:
   
   1. **robochart** This is the robochart model of the controller.
   2. **robosim** This is the robosim model derived from the robochart model above.
   3. **p-model** This is the physical models of the robot.
   4. **simulink** This is the simulink model used for test generation.
   5. **sdf** This is the sdf model derived from the p-model above.
   6. **maxima** This includes various maxima models used to calculate the dynamics of a simple multi body system. The plan is to extend it to calculate the equations of the arm automatically (instead of manually as done in the documentation.)
   7. **scad** This is a 3d-model of the arm described in the input language of the scad tool.
   
## documentation
These are documents that explain different aspects this project.
   1. **diagrams** This provides explainations of the RoboChart, RoboSim and PhysMod (p-model) diagrams
   2. **dynamics** This documents the calculations performed to derive the dynamic equations of the robotic arm.
## code
   1. **controller_for_testing** This is the latest controller that is being used to run the automatically generated tests. It is based on **sync_controller_estimated_positions** removing the button to allow headless execution of tests.
   2. **original_controller** This is the first implementation assuming joint position sensors and direct access to clock. This implementation tries to use timing to synchronise the client and server, but due to the simulation time not matching the client time, the controller runs faster than simulation. For this reason, this controller cannot use the cycle to estimate positions.
   3. **sync_controller** This implementation uses the CoppeliaSim API to synchronise the client and server.
   4. **sync_controller_estimated_positions** This is a modified version of **sync_controller** that estimates the position of the end-effector instead of using joint position sensors.

## simulation
   1. **scenes** These are various CoppeliaSim scenes used to simulate the sandwich maker. The relevant scene for testing is *sandwich_machine_for_test.ttt*. The remaining scenes are variations and include scenes where some slices are missing, and where slices are continously generated.
## testing
   1. **tests** This folder contains the automatically generated tests.
   2. **results** This folder contains the results of the generated tests produced automatically by the testing scene above.
## images
   This folder includes generated images of the various diagram to facilitate inclusion in other documents. *Notice that these images may be outdated. I'm in the process of reviewing all models and will regenerate the figures.*
## videos
   This folder includes videos the simulations.


# Instructions to run tests

## Setup
1) Put the tests in the folder *testing/tests*
2) Open in CoppeliaSim the scene *simulation/scenes/sandwich_maker_for_test.ttt*
   1) Double click the script icon next to the Base element in the Scene hierarchy. This will open a text editor.
   2) change the variable basepath to contain the path to the folder *testing* of the git repository on your local file system
3) Open a terminal and cd into the *code/controller_for_testing* folder in the git repository.
   1) set up an environment variable VREP_PATH to point to the programming folder in the CoppeliaSim installation dir, or hard code it in the file *simple_sandwich_maker.pro*.
   2) run *qmake*
   3) run *make*

## Test execution
1) Double click the script icon next to the Base element in the Scene hierarchy. This will open a text editor.
   1) change the variable filename to contain the test (e.g., test1.csv, test1.txt)
   2) close the script editor
2) Click the start/resume simulation button in CoppeliaSim (this will start the simulation and pause it)
3) Open a terminal and cd into the controller_no_button
   1) run "make run" (this will start the controller and resume the simulation)
   2) wait until the simulation stops
4) Once the simulation has stopped, look for the results in the results folder. The file name will be the same
as the input name.

_If you don't want the simulation to stop, you can comment line 79 and uncomment line 80 in the Base script (in CoppeliaSim).
This will pause the simulation when the first slice reaches the end of the conveyor belt. In this case, the results will only be
written to disk when you stop the simulation using the "Stop simulation" button in CoppeliaSim._
