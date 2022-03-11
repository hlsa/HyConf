        
model = handles.matlab.model;
implementation = handles.matlab.implementation;

contents=cellstr(get(handles.plotvar,'String')); 
selected_var = contents{get(handles.plotvar,'Value')};

index_selected_var_model = find(ismember(model.variables,selected_var));
index_selected_var_implementation = find(ismember(implementation.variables,selected_var));

% contents2=cellstr(get(handles.selectoutput,'String'));
% selected_output = contents2{get(handles.selectoutput,'Value')}
selected_output = get(handles.selectoutput,'Value');


tooling_mode = get(handles.popupmenu_toolingmode,'Value');

if selected_output == 1 % plot the model output
    X1 = model.time;
    Y1 = model.data(index_selected_var_model,:);
    plot(X1,Y1,'b','linewidth',2);
    grid on;
    xlabel('Time');
    ylabel('Value');
    title(selected_var);
    legend('Model [Matlab]');
elseif selected_output == 2 % plot the implementation output
    X1 = implementation.time;
    Y1 = implementation.data(index_selected_var_implementation,:);
    plot(X1,Y1,'r','linewidth',2);
    grid on;
    xlabel('Time');
    ylabel('Value');
    title(selected_var);
    legend('Implementation [Matlab]');
else % plot both the model and the implementation output
    X1 = model.time;
    Y1 = model.data(index_selected_var_model,:);
    X2 = implementation.time;
    Y2 = implementation.data(index_selected_var_implementation,:);
    plot(X1,Y1,'b','linewidth',2);
    hold on;
    plot(X2,Y2,'r','linewidth',2);
    hold off;
    grid on;
    xlabel('Time');
    ylabel('Value');
    title(selected_var);
    legend('Model [Matlab]','Implementation [Matlab]');
end

% 
% plot(X1,Y1,'b','linewidth',2);
% % hold on;
% % plot(X2,Y2,'k','linewidth',2);
% hold off;
% grid on;
% xlabel('Time');
% ylabel('Value');
% title(selected_var);
% if tooling_mode == 3
%     legend('Model [Matlab]','Implementation [Matlab]');            
% end