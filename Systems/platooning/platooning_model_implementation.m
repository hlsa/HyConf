clear all; 
warning off;

tic

stepsize = 0.001;

a_factor = 0.5;
a_max = 1 * a_factor;      % maximum acceleration of the leading car
a_min = -0.5 * a_factor;   % minimum acceleration of the leading car

T_MIN_THRESHOLD = 0.1;   % minimum time threshold
T_MAX_THRESHOLD = 1;   % maximum time threshold
D_THRESHOLD = 4;   % distance threshold
S_THRESHOLD = 0.5; % speed threshold

N = 1000; % moving average parameter

plot_figures = true;

%% car parameters
v_0 = 120/3.6;
T = 1.5;
s_0 = 2.0;
a = 1.4;
b = 2.0;
delta = 4;
l_avg = 4.8;

% fprintf('Simulation started, a=%.2f ',a)
disp('Simulation started')
sim('car_following_model')
disp('Simulation done')

start_time = 10;
start_time_index = start_time / stepsize + 1;


model.variables = {'x','v','a'};
model.data(1,:) = x_follower_model.Data(start_time_index:end)';
model.data(2,:) = x_dot_follower_model.Data(start_time_index:end)';
model.data(3,:) = x_ddot_follower_model.Data(start_time_index:end)';
model.time = x_ddot_follower_model.Time(start_time_index:end)';

implementation.variables = {'x','v','a'};
implementation.data(1,:) = x_follower_implementation.Data(start_time_index:end)';
implementation.data(2,:) = x_dot_follower_implementation.Data(start_time_index:end)';
implementation.data(3,:) = x_ddot_follower_implementation.Data(start_time_index:end)';
implementation.time = x_ddot_follower_implementation.Time(start_time_index:end)';

handles.matlab.model = model;
handles.matlab.implementation = implementation;

%% Plot the results

if plot_figures
    figure(1)
    subplot(3,1,1)
    hold on
    plot(x_ddot_leader.Time(start_time_index:end),x_ddot_leader.Data(start_time_index:end),'LineWidth',2)
    plot(x_ddot_follower_model.Time(start_time_index:end),x_ddot_follower_model.Data(start_time_index:end),'LineWidth',2)
    plot(x_ddot_follower_implementation.Time(start_time_index:end),x_ddot_follower_implementation.Data(start_time_index:end),'LineWidth',2)
    axis([10 70 -1 1.5])
    set(gca,'fontsize',12)
    title('Acceleration')
    ylabel('a (m/s^2)')
    legend('Leader','Following car model','Following car implementation','Location','northwest')
    % xlabel('t (s)')

    subplot(3,1,2)
    hold on
    plot(x_dot_leader.Time(start_time_index:end),x_dot_leader.Data(start_time_index:end),'LineWidth',2)
    plot(x_dot_follower_model.Time(start_time_index:end),x_dot_follower_model.Data(start_time_index:end),'LineWidth',2)
    plot(x_dot_follower_implementation.Time(start_time_index:end),x_dot_follower_implementation.Data(start_time_index:end),'LineWidth',2)
    axis([10 70 10 35])
    set(gca,'fontsize',12)
    title('Velocity')
    ylabel('v (m/s)')
    legend('Leader','Following car model','Following car implementation','Location','northwest')
    % xlabel('t (s)')

    subplot(3,1,3)
    hold on
    plot(x_leader.Time(start_time_index:end),x_leader.Data(start_time_index:end),'LineWidth',2)
    plot(x_follower_model.Time(start_time_index:end),x_follower_model.Data(start_time_index:end),'LineWidth',2)
    plot(x_follower_implementation.Time(start_time_index:end),x_follower_implementation.Data(start_time_index:end),'LineWidth',2)
    title('Position')
    set(gca,'fontsize',12)
    ylabel('x (m)')
    xlabel('t (s)')
    legend('Leader','Following car model','Following car implementation','Location','northwest')
end
% 
% 
% 
% x0=10;
% y0=10;
% width=1200;
% height=600;
% set(gcf,'units','points','position',[x0,y0,width,height])
% saveas(gcf,'position_velocity_acceleration_leader_follower_model_follower_implementation.png')





%% Messages send

send_times = [];
temp = 0;

for i = 1:length(messages_sent.Time)
    if messages_sent.Data(i) ~= temp
        send_times = [send_times messages_sent.Time(i)];
        temp = messages_sent.Data(i);
    end
end

figure(2)
plot(diff(send_times))


%% Stable distance
% % v = x_dot_leader.Data(10001:end);
% 
% v = 0:0.1:30;
% 
% % v = 20
% s_stable = (s_0+v.*T)./sqrt(1-(v./v_0).^delta);
% 
% % difference = x_leader.Data(10001:end)-x_follower_model.Data(10001:end) - l_avg;
% figure(2)
% hold on
% % plot(x_leader.Time(10001:end),difference,'LineWidth',2)
% % plot(x_leader.Time(10001:end),s_stable,'LineWidth',2)
% plot(v,s_stable,'LineWidth',2)
% % legend('Real distance','Optimal distance')
% xlabel('v (m/s)')
% ylabel('s_{stable} (m)')
% set(gca,'fontsize',12)




%% Plot the results
% figure(1)
% subplot(3,1,1)
% hold on
% plot(x_ddot_leader.Time,x_ddot_leader.Data)
% plot(x_ddot_follower_implementation.Time,x_ddot_follower_implementation.Data)
% plot(x_ddot_follower_model.Time,x_ddot_follower_model.Data)
% legend('Leader','Follower (implementation)','Follower (model)')
% 
% subplot(3,1,2)
% hold on
% plot(x_dot_leader.Time,x_dot_leader.Data)
% plot(x_dot_follower_implementation.Time,x_dot_follower_implementation.Data)
% plot(x_dot_follower_model.Time,x_dot_follower_model.Data)
% legend('Leader','Follower (implementation)','Follower (model)')
% 
% subplot(3,1,3)
% hold on
% plot(x_leader.Time,x_leader.Data)
% plot(x_follower_implementation.Time,x_follower_implementation.Data)
% plot(x_follower_model.Time,x_follower_model.Data)
% legend('Leader','Follower (implementation)','Follower (model)')
% 
toc





