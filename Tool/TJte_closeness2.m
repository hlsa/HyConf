% =========================================================================
% Title         : TJte_closeness.m
% Author        : Arend Aerts
% Project       : Model-Based testing tool for hybrid systems in Acumen
% Date          : 10-04-2015
% Description   : Determine conformance with(T,tau,epsilon)-closeness) 
%
%                                ---
%
% Copyright (©) 2010-2014, Acumen Individual Contributors (AIC)
% All rights reserved.
%
% See LICENSE-AIC for licensing details. 
% =========================================================================


%function [conformance] = TJte_closeness(testcase,acumen,TJte_closeness_mode,conformance)
disp('Start conformance testing')
testcase = handles.testcase;
acumen = handles.acumen;
tooling_mode = handles.tooling_mode;
TJte_closeness_mode = handles.tooling_mode;

contents1=cellstr(get(handles.popupmenu25,'String'));
tmp1 = contents1{get(handles.popupmenu25,'Value')};
conformance.var = {tmp1}; 
tmp2=cellstr(get(handles.edit11,'String'));
conformance.tau = str2double(char(tmp2)); 
tmp3=cellstr(get(handles.edit12,'String'));
tmp4=cellstr(get(handles.edit13,'String'));
conformance.epsilon = str2double(char(tmp3)) + str2double(char(tmp4));

model = handles.matlab.model;
implementation = handles.matlab.implementation;

for m=1:1:max(size(conformance.var))  

    if tooling_mode == 1 || tooling_mode == 2
        % Load implementation and model data
        if acumen.instances > 1
            X2 = acumen.time;
            index=find(ismember(acumen.var,conformance.var(1,m)));
            Y2 = acumen.data2(index,:);
        else
            X2 = acumen.time;
            index=find(ismember(acumen.var,conformance.var(1,m)));
            Y2 = acumen.data1(index,:);
        end

        if TJte_closeness_mode == 2
            X1 = acumen.time;
            index=find(ismember(acumen.var,conformance.var(1,m)));
            Y1 = acumen.data1(index,:);
        elseif TJte_closeness_mode == 1
            X1 = testcase.time;
            index=find(ismember(testcase.var,conformance.var(1,m)));
            Y1 = testcase.data(index,:);
        end
    elseif tooling_mode == 3
        index_model=find(ismember(model.variables,conformance.var(1,m)));
        X1 = model.time;
        Y1 = model.data(index_model,:);
        
        index_implementation=find(ismember(implementation.variables,conformance.var(1,m)));
        X2 = implementation.time;
        Y2 = implementation.data(index_implementation,:);
    end


    % Check ((T,J,(tau,epsilon))-closeness) --

    % Conformance is standard true, unless otherwise proven
    conformance.result = true;
    res = 10^-6; %Compensator for simulation time deviation (rounding)

    % Iteration 1 ([X1,Y1] w.r.t. [X2,Y2])
    for(k=1:1:max(size(X1)))
        % Check if conformance still holds
        if(conformance.result == false)
            break;
        end

        % Construct min/max time boundaries of X1 instance
        time_min = max(0,X1(1,k)-conformance.tau(1,m));
        time_max = min(max(X1),X1(1,k)+conformance.tau(1,m));

        % Find corresponding time matrix instances of X2 
        time_min_index = find(X2 >= time_min-res,1,'first');
        time_max_index = find(X2 <= time_max+res,1,'last');

        % Conformance check Y1 w.r.t. Y2
        for(i=time_min_index:1:time_max_index)
            if(abs(Y1(1,k)-Y2(1,i)) <= conformance.epsilon(1,m))
                conformance.result = true;
                break;
            else
                conformance.result = false;
                ref_index = k;
                error_index_begin = time_min_index;
                error_index_end = i;
                conformance.iteration = 1;
            end
        end    
    end

    % Iteration 2 ([X2,Y2] w.r.t. [X1,Y1])
    for(k=1:1:max(size(X2)))
        % Check if conformance still holds
        if(conformance.result == false)
            break;
        end

        % Construct min/max time boundaries of X1 instance
        time_min = max(0,X2(1,k)-conformance.tau(1,m));
        time_max = min(max(X2),X2(1,k)+conformance.tau(1,m));

        % Find corresponding time matrix instances of X2 
        time_min_index = find(X1 >= time_min-res,1,'first');
        time_max_index = find(X1 <= time_max+res,1,'last');

        % Compare Y1 instance with Y2 instances
        for(i=time_min_index:1:time_max_index)
            if(abs(Y2(1,k)-Y1(1,i)) <= conformance.epsilon(1,m))
                conformance.result = true;
                break;
            else
                conformance.result = false;
                ref_index = k;
                error_index_begin = time_min_index;
                error_index_end = i;
                conformance.iteration = 2;
            end
        end
    end   


    % Plot (bad) conformance result --
    plotscaling = 2;

    if (conformance.result == false)
    plot(X1,Y1,'b','linewidth',2);
    hold on;
    plot(X2,Y2,'k','linewidth',2);
    
    contents=cellstr(get(handles.popupmenu9,'String')); 
    selected_view = contents{get(handles.popupmenu9,'Value')};

        if conformance.iteration == 1
            plot(X1(1,ref_index),Y1(1,ref_index),'r*','linewidth',5);
            plot(X2(1,error_index_begin),Y2(1,error_index_begin),'r*','linewidth',5);
            plot(X2(1,error_index_end),Y2(1,error_index_end),'r*','linewidth',5);

            plot([X1(1,ref_index)-conformance.tau(1,m) X1(1,ref_index)+conformance.tau(1,m)],[Y1(1,ref_index)-conformance.epsilon(1,m) Y1(1,ref_index)-conformance.epsilon(1,m)],'r--','linewidth',3);
            plot([X1(1,ref_index)-conformance.tau(1,m) X1(1,ref_index)+conformance.tau(1,m)],[Y1(1,ref_index)+conformance.epsilon(1,m) Y1(1,ref_index)+conformance.epsilon(1,m)],'r--' ,'linewidth',3);
            plot([X1(1,ref_index)-conformance.tau(1,m) X1(1,ref_index)-conformance.tau(1,m)],[Y1(1,ref_index)-conformance.epsilon(1,m) Y1(1,ref_index)+conformance.epsilon(1,m)],'r--','linewidth',3);
            plot([X1(1,ref_index)+conformance.tau(1,m) X1(1,ref_index)+conformance.tau(1,m)],[Y1(1,ref_index)-conformance.epsilon(1,m) Y1(1,ref_index)+conformance.epsilon(1,m)],'r--','linewidth',3);

            plot(X2(1,error_index_begin:error_index_end),Y2(1,error_index_begin:error_index_end),'r-','linewidth',3);
            hold off;
            grid on;
            xlabel('Time');
            ylabel('Value');
            if TJte_closeness_mode == 2
                legend('Model [Acumen]','Implementation [Acumen]','((T,J,(tau,epsilon))-closeness)');    
            elseif TJte_closeness_mode == 1
                legend('Model [Matlab]','Implementation [Acumen]','((T,J,(tau,epsilon))-closeness)');
            end
            title(strcat(conformance.var(1,m),'    (Iteration 1)'));

            if strcmp(selected_view,'Zoom')
                axis([max(0,X2(1,error_index_begin)-plotscaling*conformance.tau(1,m)) X2(1,error_index_end)+plotscaling*conformance.tau(1,m) min([Y2(1,error_index_begin),Y2(1,error_index_end),Y1(1,ref_index)-conformance.epsilon(1,m)])-plotscaling*conformance.epsilon(1,m) max([Y2(1,error_index_begin),Y2(1,error_index_end),Y1(1,ref_index)+conformance.epsilon(1,m)])+plotscaling*conformance.epsilon(1,m)])
            end
        else
            plot(X2(1,ref_index),Y2(1,ref_index),'r*','linewidth',5);
            plot(X1(1,error_index_begin),Y1(1,error_index_begin),'r*','linewidth',5);
            plot(X1(1,error_index_end),Y1(1,error_index_end),'r*','linewidth',5);

            plot([X2(1,ref_index)-conformance.tau(1,m) X2(1,ref_index)+conformance.tau(1,m)],[Y2(1,ref_index)-conformance.epsilon(1,m) Y2(1,ref_index)-conformance.epsilon(1,m)],'r--','linewidth',3);
            plot([X2(1,ref_index)-conformance.tau(1,m) X2(1,ref_index)+conformance.tau(1,m)],[Y2(1,ref_index)+conformance.epsilon(1,m) Y2(1,ref_index)+conformance.epsilon(1,m)],'r--' ,'linewidth',3);
            plot([X2(1,ref_index)-conformance.tau(1,m) X2(1,ref_index)-conformance.tau(1,m)],[Y2(1,ref_index)-conformance.epsilon(1,m) Y2(1,ref_index)+conformance.epsilon(1,m)],'r--','linewidth',3);
            plot([X2(1,ref_index)+conformance.tau(1,m) X2(1,ref_index)+conformance.tau(1,m)],[Y2(1,ref_index)-conformance.epsilon(1,m) Y2(1,ref_index)+conformance.epsilon(1,m)],'r--','linewidth',3);

            plot(X1(1,error_index_begin:error_index_end),Y1(1,error_index_begin:error_index_end),'r-','linewidth',3);
            hold off;
            grid on;
            xlabel('Time');
            ylabel('Value');
            if TJte_closeness_mode == 2
                legend('Model [Acumen]','Implementation [Acumen]','((T,J,(tau,epsilon))-closeness)');    
            elseif TJte_closeness_mode == 1
                legend('Model [Matlab]','Implementation [Acumen]','((T,J,(tau,epsilon))-closeness)');
            end

            title(strcat(conformance.var(1,m),'    (Iteration 2)'));

            if strcmp(selected_view,'Zoom')
                axis([max(0,X1(1,error_index_begin)-plotscaling*conformance.tau(1,m)) X1(1,error_index_end)+plotscaling*conformance.tau(1,m) min([Y1(1,error_index_begin),Y1(1,error_index_end),Y2(1,ref_index)-conformance.epsilon(1,m)])-plotscaling*conformance.epsilon(1,m) max([Y1(1,error_index_begin),Y1(1,error_index_end),Y2(1,ref_index)+conformance.epsilon(1,m)])+plotscaling*conformance.epsilon(1,m)])
            end
        end
    else
        conformance.iteration = 0;
    end
end

disp('Finished conformance testing')
    