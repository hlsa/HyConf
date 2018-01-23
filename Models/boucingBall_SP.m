% =========================================================================
% Title         : TestCaseGeneration_BoostConverter.m
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

%function [testcase] = TestCaseGeneration_BoostConverter(acumen,conformance)

% User interface connection
acumen = handles.acumen;
conformance = handles.conformance;


% MODEL DECLARATION --
% Global model parameters
x1 = 5;
x2 = 0;


% Init test case generation --
i = 1;
k = 0;

% Test case generation algorithm --
while (k <= acumen.endtime)

    % Generate input sample 
	if x1 <= 0 && x2 <= 0
       x2 = -0.75 * x2;
    end
	
		
    % Update testcase structure
    testcase.data(1,i) = x1;
    testcase.data(2,i) = x2;
    testcase.time(1,i) = k;
    testcase.var = {'x1' 'x2'};
    testcase.inputvalue(1,i) = x1;
    testcase.inputvar = {'x1'};    
    
	
	k = k + acumen.timestep;
    i = i + 1;
	
end

% User interface connection
handles.testcase = testcase;
