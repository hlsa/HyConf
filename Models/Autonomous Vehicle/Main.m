
function Main()
%% -- SETUP ENVIRONMENT -- %%
clc;
clear all;

arm.torque1 = 10;
arm.torque2 = 10;
arm.torque3 = 10;
arm.homeX = 0;
arm.homeY = 0;
arm.homeZ = 0;
arm.homeTheta = 0;


belt.speed = 0.5;
belt.height = 1;
belt.width = 2;
belt.length = 5;

defaultBread.length = 0.1;
defaultBread.width = 0.1;
defaultBread.height = 0.02;
defaultBread.fillingsHeight = 0.03;

laser.y = 0.28;

gravity = 9.81;

%% -- SETUP INPUTS -- %%

bread1.x = 0.547;
bread1.y = 1.076;
bread1.z = 0.505;
bread1.theta = 90.6;

bread2.x = 0.414;
bread2.y = 1.056;
bread2.z = 0.505;
bread2.theta = -17.1;

save();
evalin('base', 'load(''matlab.mat'')');

%% -- RUN SIMULATION -- %%
simOut = sim('robot','SimulationMode','normal');
bread1SIM = simOut.get('bread1SIM');
assignin('base','bread1SIM',bread1SIM);

end