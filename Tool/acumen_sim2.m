% =========================================================================
% Title         : Acumen_Sim.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 13-04-2015
% Description   : Start Acumen (server mode) and load model
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================

%function [acumen] = acumen_sim(acumen,model_path,Matlab_M_Testcase)

% User interface connection
acumen = handles.acumen;
testcase = handles.testcase;
model_path = handles.implementation_path;
acumen_path = handles.acumen_path;

% Temporary fixes
temporary_path = pwd;
cd('Acumen')
code = strcat('start /min java -jar acumen-14.12.01.jar listen "..\Models\thermostat_MI2.acm" "5000" && exit');
[status,results] = dos(code);
cd(temporary_path)

% 
% % Start server socket connection
% temp_path = sprintf('%s %s %s%s%s','start java -jar',acumen_path,'listen "',model_path,'" "5000" && exit')
% [status,results] = dos(temp_path);


% Iniate + execute socket connection Acumen 
[acumen]=JavaClient(acumen,testcase);


% User interface connection
handles.acumen = acumen;


