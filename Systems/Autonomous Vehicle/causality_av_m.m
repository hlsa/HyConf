function Main()
%% -- SETUP ENVIRONMENT -- %%
clc;
clear all;

car.initialAcceleration = 0;
car.initialSpeed = 10;
car.breaks = 0.4;

lidar.lowRange = 40;
lidar.highRange = 20;
lidar.currentRange = lidar.highRange;

battery.decay = 1;
battery.initialCharge = 10;
battery.criticalThreshold = 5;
battery.change1 = 0;
battery.change2 = 5;

pedestrian.position = 80;

environment.gravity = 9.8;
environment.rain = 0; %% Rain value should be 0 or 1 or 2. Set to 0 for no rain, 1 for medium rain and 2 for maximum rain.

save();
evalin('base', 'load(''matlab.mat'')');

%% -- RUN SIMULATION -- %%
simOut = sim('causality_av_s','SimulationMode','normal');
simulation = simOut.get('simulation');
assignin('base','simulation',simulation);

end