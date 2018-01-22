%% This script will run all the functions of the gui without actually having
%% to click on them

close all; clear all; clc;

%% Initialize some paramaters

% Define workspace
cd('C:\Users\s132652\Google Drive\TUe\2017-2018\Internship\GIT\cps-conf-tool');

% Define location Acumen .jar file (runfile)
handles.acumen_path = 'C:\Users\s132652\Google Drive\TUe\2017-2018\Internship\GIT\cps-conf-tool\Acumen\acumen-14.12.01.jar';

% Define location for CORA installation folder
handles.cora_path = 'C:\Users\s132652\Google Drive\TUe\2017-2018\Internship\GIT\cps-conf-tool\CORA';

% Thermostat example
handles.model_path = 'C:\Users\s132652\Google Drive\TUe\2017-2018\Internship\GIT\cps-conf-tool\Models\thermostat_SP.m';
handles.implementation_path = 'C:\Users\s132652\Google Drive\TUe\2017-2018\Internship\GIT\cps-conf-tool\Models\thermostat_MI2.acm';

handles.conformance.var = {};            
handles.conformance.epsilon = [];          
handles.conformance.tau = []; 

run('ACMfile_import2.m');

%% Run test case generation
run(handles.model_path);

%% Run test execution
run('acumen_sim2.m');

%% Run conformance check
clc
conformance.var = 't';
conformance.tau = 0.01;
conformance.epsilon = 1;
run('conformance_check.m');

if conformance.result == 0
    while conformance.result ~= 1
        conformance.epsilon = conformance.epsilon + 0.01;
        run('conformance_check.m');
    end
else
    while conformance.result ~= 0
        conformance.epsilon = conformance.epsilon - 0.01;
        run('conformance_check.m');
    end
end

txt = sprintf('The critical epsilon is equal to %.2f \nwith tau being equal to %.4f', conformance.epsilon, conformance.tau);
disp(txt)