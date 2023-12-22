% =========================================================================
% Title         : TJte_closeness_degree.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : Conformance degree with (T,tau,epsilon)-closeness) 
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================


function [result,t,e] = TJte_closeness_degree(testcase,acumen,TJte_closeness_mode,selected_var,selected_var_degree,var_degree,delta)


% Determine conformance degree var
if strcmp(selected_var_degree,'epsilon')
    e = var_degree;
    t = 0;
else
    t = var_degree;
    e = 0;
end

% Load implementation and model data
if acumen.instances > 1
    X2 = acumen.time;
    index=find(ismember(acumen.var,selected_var));
    Y2 = acumen.data2(index,:);
else
    X2 = acumen.time;
    index=find(ismember(acumen.var,selected_var));
    Y2 = acumen.data1(index,:);
end

if TJte_closeness_mode == 0
    X1 = acumen.time;
    index=find(ismember(acumen.var,selected_var));
    Y1 = acumen.data1(index,:);
else
    X1 = testcase.time;
    index=find(ismember(testcase.var,selected_var));
    Y1 = testcase.data(index,:);
end

result = 0;
while (result == 0)
    % Check ((T,J,(tau,epsilon))-closeness) --

    % Conformance is standard true, unless otherwise proven
    result = true;
    res = 10^-6; %Compensator for simulation time deviation (rounding)

    % Iteration 1 ([X1,Y1] w.r.t. [X2,Y2])
    for(k=1:1:max(size(X1)))
        % Check if conformance still holds
        if(result == false)
            break;
        end

        % Construct min/max time boundaries of X1 instance
        time_min = max(0,X1(1,k)-t);
        time_max = min(max(X1),X1(1,k)+t);

        % Find corresponding time matrix instances of X2 
        time_min_index = find(X2 >= time_min-res,1,'first');
        time_max_index = find(X2 <= time_max+res,1,'last');

        % Conformance check Y1 w.r.t. Y2
        for(i=time_min_index:1:time_max_index)
            if(abs(Y1(1,k)-Y2(1,i)) <= e)
                result = true;
                break;
            else
                result = false;
                iteration = 1;
            end
        end    
    end


    % Iteration 2 ([X2,Y2] w.r.t. [X1,Y1])
    for(k=1:1:max(size(X2)))
        % Check if conformance still holds
        if(result == false)
            break;
        end

        % Construct min/max time boundaries of X1 instance
        time_min = max(0,X2(1,k)-t);
        time_max = min(max(X2),X2(1,k)+t);

        % Find corresponding time matrix instances of X2 
        time_min_index = find(X1 >= time_min-res,1,'first');
        time_max_index = find(X1 <= time_max+res,1,'last');

        % Compare Y1 instance with Y2 instances
        for(i=time_min_index:1:time_max_index)
            if(abs(Y2(1,k)-Y1(1,i)) <= e)
                result = true;
                break;
            else
                result = false;
                iteration = 2;
            end
        end
    end 
    
    % Increase tau (t) or (epsilon) with step delta
    if strcmp(selected_var_degree,'epsilon')
        t = t + delta;
    else
        e = e + delta;
    end
       
end
    