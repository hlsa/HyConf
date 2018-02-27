% clear all; 
warning off;

a_factor = 1;
a_max = 1 * a_factor;      % maximum acceleration of the leading car
a_min = -0.5 * a_factor;   % minimum acceleration of the leading car

T_THRESHOLD = 1;   % time threshold
D_THRESHOLD = 4;   % distance threshold
S_THRESHOLD = 0.5; % speed threshold

N = 1000; % moving average parameter

%% car parameters
v_0 = 120;
T = 1.5;
s_0 = 2.0;
a = 1.4;
b = 2.0;
delta = 4;
l_avg = 4.8;

fprintf('Simulation started, a=%.2f ',a)
sim('car_following_model')
disp('Simulation done')

model.variables = {'x','v','a'};
model.data(1,:) = x_follower_model.Data(10001:end)';
model.data(2,:) = x_dot_follower_model.Data(10001:end)';
model.data(3,:) = x_ddot_follower_model.Data(10001:end)';
model.time = x_ddot_follower_model.Time(10001:end)';

implementation.variables = {'x','v','a'};
implementation.data(1,:) = x_follower_implementation.Data(10001:end)';
implementation.data(2,:) = x_dot_follower_implementation.Data(10001:end)';
implementation.data(3,:) = x_ddot_follower_implementation.Data(10001:end)';
implementation.time = x_ddot_follower_implementation.Time(10001:end)';

handles.matlab.model = model;
handles.matlab.implementation = implementation;

%% Plot the results
% figure(1)
% subplot(3,1,1)
% hold on
% plot(x_ddot_leader.Time(10001:end),x_ddot_leader.Data(10001:end),'LineWidth',2)
% plot(x_ddot_follower_model.Time(10001:end),x_ddot_follower_model.Data(10001:end),'LineWidth',2)
% plot(x_ddot_follower_implementation.Time(10001:end),x_ddot_follower_implementation.Data(10001:end),'LineWidth',2)
% axis([10 70 -1 1.5])
% set(gca,'fontsize',12)
% title('Acceleration')
% ylabel('a (m/s^2)')
% legend('Leader','Following car model','Following car implementation','Location','northwest')
% % xlabel('t (s)')
% 
% subplot(3,1,2)
% hold on
% plot(x_dot_leader.Time(10001:end),x_dot_leader.Data(10001:end),'LineWidth',2)
% plot(x_dot_follower_model.Time(10001:end),x_dot_follower_model.Data(10001:end),'LineWidth',2)
% plot(x_dot_follower_implementation.Time(10001:end),x_dot_follower_implementation.Data(10001:end),'LineWidth',2)
% axis([10 70 10 35])
% set(gca,'fontsize',12)
% title('Velocity')
% ylabel('v (m/s)')
% legend('Leader','Following car model','Following car implementation','Location','northwest')
% % xlabel('t (s)')
% 
% subplot(3,1,3)
% hold on
% plot(x_leader.Time(10001:end),x_leader.Data(10001:end),'LineWidth',2)
% plot(x_follower_model.Time(10001:end),x_follower_model.Data(10001:end),'LineWidth',2)
% plot(x_follower_implementation.Time(10001:end),x_follower_implementation.Data(10001:end),'LineWidth',2)
% title('Position')
% set(gca,'fontsize',12)
% ylabel('x (m)')
% xlabel('t (s)')
% legend('Leader','Following car model','Following car implementation','Location','northwest')
% 
% 
% 
% x0=10;
% y0=10;
% width=1200;
% height=600;
% set(gcf,'units','points','position',[x0,y0,width,height])
% saveas(gcf,'position_velocity_acceleration_leader_follower_model_follower_implementation.png')














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
% toc





