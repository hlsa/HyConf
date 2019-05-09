% =========================================================================
% Title         : JSON_search.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : Search for begin/end index variable (in JSON string)
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================

function [index_var_begin,index_var_end] = JSON_search(str_temp,instance_input,var2)

% Find location of 4 strings in str_tmp
index0=strfind(str_temp,instance_input);
index1=strfind(str_temp,var2);
index2=strfind(str_temp,'"value":');
index3=strfind(str_temp,'}'); 

% Look for specified instance
for w=1:1:max(size(index1))
   if ((index1(1,w)-index0)>0) 
        index_var_name = index1(1,w);
        break;
   end
end

% Look for beginning specified variable value (of specified instance)
for w=1:1:max(size(index2))
   if (index2(1,w)-index_var_name>0) 
        index_var_begin = index2(1,w)+8;
        break;
   end
end

% Look for end specified variable value (of specified instance)
for w=1:1:max(size(index3))
   if (index3(1,w)-index_var_begin>0) 
        index_var_end = index3(1,w)-1;
        break;
   end
end