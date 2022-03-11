% =========================================================================
% Title         : JSON_update.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : save JSON data + modify JSON data 
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================

function [str2,acumen,k] = JSON_update(str1,acumen,testcase,k)

 
% Convert + save data to Matlab struct
[acumen]=JSON_parse(str1,acumen,k);


% Modify simulator state 
if false && ~isempty(testcase.inputvar)
    str1_tmp=char(str1);
    for l=1:1:acumen.instances

        % Determine and select number of instances
        instance = strcat('"0.',num2str(l),'":');  
        index = round((acumen.time(1,k)*(1/acumen.timestep))+1);
        for m=1:1:max(size(testcase.inputvar))

            % Determine inputvar and corresponding value
            inputvar = strcat(char(testcase.inputvar(1,m)),'/0'); 
            inputvalue = num2str(testcase.inputvalue(m,index));
          
            % Detect index and modify variable (input) value
            [var_begin,var_end]=JSON_search(str1_tmp,instance,inputvar);
            var_begin2 = var_begin - 1;
            var_end2 = var_end + 1;
            str1_tmp = strcat(str1_tmp(1:var_begin2),inputvalue,str1_tmp(var_end2:max(size(str1_tmp))));
        end
    end
    % Convert char string to java object
    str2 = javaObject('java.lang.String',str1_tmp);
else
    str2 = str1;
end


% Check for simulation end --
disp(acumen.time(1,k));
disp(acumen.endtime - acumen.timestep);
if acumen.time(1,k) >= acumen.endtime - acumen.timestep
    k = 0; 
else
    %acumen.endtime = acumen.endtime - acumen.timestep;
    k = k+1;
end