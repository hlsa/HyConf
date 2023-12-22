% =========================================================================
% Title         : ACMfile_import.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : Import simulation and system variables from .acm file
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================

%function [acumen] = ACMfile_import(model_path)

% User interface connection
model_path = handles.implementation_path;

% Read in file and determine length text
acmfile = fileread(model_path);
End_text = max(size(acmfile));


% EXTRACT SYSTEM VARIABLES
% ========================

% Detect the system section
index_inputs_begin2 = strfind(acmfile,'SYSTEM variables [BEGIN]');
index_inputs_end2 = strfind(acmfile,'SYSTEM variables [END]');

% Determine first variable index
Begin_var = index_inputs_begin2 + max(size('SYSTEM variables [BEGIN]')) + 1;

k=1;
while ((Begin_var+3) < index_inputs_end2)

% Determine end of variabel name by locating the first = sign 
End_var_tmp = min(strfind(acmfile(Begin_var:End_text),'='));
End_var = Begin_var + End_var_tmp - 2;

% Save variable
acumen.var(1,k)= cellstr(acmfile(Begin_var:End_var));
k=k+1;

% Determine start %index of next variable 
Begin_var = Begin_var + min(strfind(acmfile(Begin_var:End_text),','));
end




% EXTRACT SIMULATION VARIABLES
% ============================

% Detect the system section
index_inputs_begin3 = strfind(acmfile,'SIM variables [BEGIN]');
index_inputs_end3 = strfind(acmfile,'SIM variables [END]');

% Determine first variable index
Begin_var = index_inputs_begin3 + max(size('SIM variables [BEGIN]')) + 1;

k=1;
while ((Begin_var+3) < index_inputs_end3)

% Determine end of variable name by locating the first = sign 
Begin_var_tmp = Begin_var + min(strfind(acmfile(Begin_var:End_text),'='));

% Save variable
if k == 1
	End_var_tmp = Begin_var_tmp - 1 + min(strfind(acmfile(Begin_var_tmp:End_text),','));
    acumen.timestep = str2double(acmfile(Begin_var_tmp:End_var_tmp));
end
if k == 2
	End_var_tmp = Begin_var_tmp + 3;
    acumen.endtime = str2double(acmfile(Begin_var_tmp:End_var_tmp));
end
k=k+1;

% Determine start %index of next variable 
Begin_var = Begin_var + min(strfind(acmfile(Begin_var:End_text),','));
end

% Extract number of model instances
acumen.instances = max(size(strfind(acmfile,'create')));

acumen.time = []; % BUGFIX
acumen.data1(k-1,1) = 0; % BUGFIX
acumen.data2(k-1,1) = 0; % BUGFIX

% User interface connection
handles.acumen = acumen;
   