% =========================================================================
% Title         : JSON_parse.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : Convert + save JSON data
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================

function [acumen] = JSON_parse(data,acumen,m)


for k=1:1:max(size(acumen.var))

    % Select variable
    selected_var = char(acumen.var(1,k));


    % Convert selected variable to data structure format --
    if max(size(strfind(selected_var,''''))) > 1
        der='2';
        selected_var2 = selected_var(1:(max(size(selected_var))-2));
    else
        if max(size(strfind(selected_var,''''))) > 0
            der='1';
            selected_var2 = selected_var(1:(max(size(selected_var))-1));
        else
            der='0';
            selected_var2 = selected_var;
        end
    end
    var_tmp = strcat(selected_var2,'/',der);
    
    % Acquire variables
        str_tmp = char(data);           
        
        % Extract timestep location
        [index_time_begin,index_time_end]=JSON_search(str_tmp,'"0.0":','time/0');

        % Extract timestep value
        acumen_time_tmp = str_tmp(index_time_begin:index_time_end);
        acumen.time(1,m) = str2double(acumen_time_tmp);

        for l=1:1:acumen.instances
            % Extract variables
            [var_begin,var_end]=JSON_search(str_tmp,strcat('"0.',num2str(l),'":'),var_tmp);

            if l == 1
                acumen.data1(k,m) = str2double(str_tmp(var_begin:var_end));
            else
                acumen.data2(k,m) = str2double(str_tmp(var_begin:var_end));
            end
        end
end
