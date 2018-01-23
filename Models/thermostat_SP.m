% =========================================================================
% Title         : TestCaseGeneration_Thermostat.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : Test case generation for conformance testing 
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================

%function [testcase,tmp] = TestCaseGeneration_Thermostat(acumen,conformance)

% User interface connection
acumen = handles.acumen;
conformance = handles.conformance;



% MODEL DECLARATION --
% Global model parameters
t_low = 70; 
t_high = 80;

% Mode dependent parameters 
% mode q1 - ON
q1.dynamics = c2d(ss(-1,1,1,0),acumen.timestep);
q1.inputspec = [26 35];%[20 30];        

% mode q2 - OFF
q2.dynamics = c2d(ss(-1,1,1,0),acumen.timestep);
q2.inputspec = [15 19]; %[15 25];      

% INIT model
x_0 = 0;
t_0 = 0;      
mode_0 = 1;


% Init test case generation --
i = 1;
k = 0;
mode_old = 0;
s_check = false;
k_old = -100;

% Test case generation algorithm --
while (k <= acumen.endtime)
    % Update inputspec if mode change
    if mode_0 ~= mode_old
        switch mode_0
            case 1
                offset = max(q1.inputspec);
                sample_constant = abs(min(q1.inputspec)-max(q1.inputspec)); 
                mode_old = 1;
            case 2 
                offset = max(q2.inputspec);
                sample_constant = abs(min(q2.inputspec)-max(q2.inputspec)); 
                mode_old = 2;
        end
    end
    
    % Generate input sample (mode dependent)
    if k <= 5
        window = 0.1;%rand(1);
    else
    if k <= 10
        window = 0.6;%rand(1);
    else
        window = 1;%rand(1);
    end
    end
    input = offset-(sample_constant*window); 
    
    % Update testcase structure
    testcase.data(1,i) = t_0;
    testcase.data(2,i) = mode_0;
    testcase.data(3,i) = input;
    testcase.inputvalue(1,i) = input;
    testcase.time(1,i) = k;
    testcase.var = {'t' 'mode' 'tminmax'};
    testcase.inputvar = {'tminmax'};
    tmp(1,i) = window;
    
    
    % Apply input sample and determine output 
    switch mode_0
        case 1
            t_0 = q1.dynamics.a*t_0+q1.dynamics.b*input;
        case 2
            t_0 = q2.dynamics.a*t_0+q2.dynamics.b*input;
    end
        
    % Check guards / update mode 
    if (mode_0 == 1) & (t_0 > t_high)
        mode_0 = 2;
    else
        if (mode_0 == 2) & (t_0 < t_low)
            mode_0 = 1;
        end   
    end
    
    
    % Sensitivity check
    for m=1:1:max(size(conformance.var))
        if char(conformance.var(1,m)) == 't'
            t_0_min = t_0 - conformance.epsilon(1,m);
            t_0_max = t_0 + conformance.epsilon(1,m);

            if (mode_0 == 1) & ((t_0_min >= t_high) | (t_0_max >= t_high))
                s_check = true;
                break;
            else
                if (mode_0 == 2) & ((t_0_min <= t_low) | (t_0_max <= t_low))
                    s_check = true;
                    break;
                end   
            end
        end
    end
    
    if s_check == true
        t_0 = testcase.data(1,i);
        mode_0 = testcase.data(2,i);
        s_check = false;
        k_old = -100;
    else
        k = k + acumen.timestep;
        i=i+1;
        
        if k < acumen.endtime
            % Update test case structure 
            testcase.out_state(1,i) = t_0;
            testcase.out_state(2,i) = mode_0;
            testcase.data(3,i) = input;
        end
    end
end

% User interface connection
handles.testcase = testcase;