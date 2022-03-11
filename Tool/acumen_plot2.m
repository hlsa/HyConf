% =========================================================================
% Title         : acumen_plot.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : Plot specific variable from data struc [acumen]
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================

%function [X1,Y1,X2,Y2]=JSON_plot4(testcase,acumen,tooling_mode,selected_var,selected_output)


testcase = handles.testcase;
acumen = handles.acumen;
    

contents=cellstr(get(handles.plotvar,'String')); 
selected_var = contents{get(handles.plotvar,'Value')};
contents2=cellstr(get(handles.selectoutput,'String'));
selected_output = contents2{get(handles.selectoutput,'Value')};

handles.tooling_mode = get(handles.popupmenu_toolingmode,'Value');
tooling_mode = handles.tooling_mode;

% Output selection --
if strcmp(selected_output,'Model output')
    special = 1;
else
    if strcmp(selected_output,'Implementation output')
        special = 2;
    else
        special = 3;
    end    
end

% Plot data --
if special == 1
    if tooling_mode == 1
        % Load data
        X1 = testcase.time;
        index=find(ismember(testcase.var,selected_var));
        Y1 = testcase.data(index,:);
    elseif tooling_mode == 2
        % Load data
        X1 = acumen.time;
        index=find(ismember(acumen.var,selected_var));
        Y1 = acumen.data1(index,:);
    end
    
    if size(X1) == size(Y1)

        % Plot specified variable
        plot(X1,Y1,'b','linewidth',2);
        grid on;
        xlabel('Time');
        ylabel('Value');
        title(selected_var);
        if tooling_mode == 1
            legend('Model [Matlab]');
        elseif tooling_mode == 2
            legend('Model [Acumen]');
        end
    end
end

if special == 2
    if acumen.instances > 1
        X1 = acumen.time;
        index=find(ismember(acumen.var,selected_var));
        Y1 = acumen.data2(index,:);
    else
        X1 = acumen.time;
        index=find(ismember(acumen.var,selected_var));
        Y1 = acumen.data1(index,:);
    end

    if size(X1) == size(Y1)

        % Plot specified variable
        plot(X1,Y1,'r','linewidth',2);
        grid on;
        xlabel('Time');
        ylabel('Value');
        legend('Implementation [Acumen]');
        title(selected_var);
    end
end

if special == 3
    if tooling_mode == 1
        % Load data
        X1 = testcase.time;
        index=find(ismember(testcase.var,selected_var));
        Y1 = testcase.data(index,:);
    elseif tooling_mode == 2
        % Load data
        X1 = acumen.time;
        index=find(ismember(acumen.var,selected_var));
        Y1 = acumen.data1(index,:);
    end
    
    if acumen.instances > 1
        X2 = acumen.time;
        index=find(ismember(acumen.var,selected_var));
        Y2 = acumen.data2(index,:);
    else
        X2 = acumen.time;
        index=find(ismember(acumen.var,selected_var));
        Y2 = acumen.data1(index,:);
    end
    
    if size(X1) == size(Y1)


        % Plot specified variable
        plot(X1,Y1,'b','linewidth',2);
        hold on;
        plot(X2,Y2,'k','linewidth',2);
        hold off;
        grid on;
        xlabel('Time');
        ylabel('Value');
        title(selected_var);
        if tooling_mode == 1
            legend('Model [Matlab]','Implementation [Acumen]');            
        elseif tooling_mode == 2
            legend('Model [Acumen]','Implementation [Acumen]');
        end
    end
end


