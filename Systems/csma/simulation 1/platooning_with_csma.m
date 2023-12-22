close all;
clear all;
warning off;
clc;

% http://www.control.lth.se/project/truetime/
% run("D:\Program Files\Matlab R2017b\truetime-2.0\init_truetime")

timestep = 1e-3;
t_end = 80;

T_MIN_THRESHOLD = 0.1;            % time threshold
T_MAX_THRESHOLD = 1;            % time threshold
D_THRESHOLD = 4;            % distance threshold
S_THRESHOLD = 0.5;          % speed threshold
N = 1000;
index_start = 1;

%% Car parameters
car.v_0 = 120/3.6;
car.delta = 4;
car.T = 1.5;
car.s_0 = 2.0;
car.a = 1.4;
car.b = 1.67;
car.l_avg = 5;

leader = car;
follower_1 = car;
follower_2 = car;

leader.v0 = 20;
leader.x0 = 150;

follower_1.v0 = 10;
% follower_1.x0 = leader.x0 - leader.l_avg - (follower_1.s_0+follower_1.v0*follower_1.T)/sqrt(1-(follower_1.v0/follower_1.v_0)^follower_1.delta);
% follower_1.x0 = leader.x0 - 41.445;
follower_1.x0 = -200;


follower_2.v0 = 10;
% follower_2.x0 = follower_1.x0 - follower_1.l_avg - (follower_2.s_0+follower_2.v0*follower_2.T)/sqrt(1-(follower_2.v0/follower_2.v_0)^follower_2.delta);
% follower_2.x0 = follower_1.x0 - 41.445;
follower_2.x0 = -300;

sim('platooning_with_csma_simulink')

disp('done')
%%
figure(1)
hold on
for i = 1:size(scheduler.Data,2)
    plot(scheduler.Time,scheduler.Data(:,i)-i+1)
end
legend('Leader','Follower 1','Follower 2')

%%
figure(2)


subplot(3,1,1)
hold on
plot(x_ddot_leader.Time(N:end),x_ddot_leader.Data(N:end),'LineWidth',2)
plot(x_ddot_follower_ideal.Time(N:end),x_ddot_follower_ideal.Data(N:end),'LineWidth',2)
plot(x_ddot_follower_1.Time(N:end),x_ddot_follower_1.Data(N:end),'LineWidth',2)
% plot(x_ddot_follower_2.Time(N:end),x_ddot_follower_2.Data(N:end),'LineWidth',2)
legend('Leader', 'Follower ideal', 'Follower 1');
ylabel('a (m/s^2)')
xlabel('t (s)')
% text(-10,0.3,'(a)','FontSize',16)
set(gca,'fontsize',16)

subplot(3,1,2)
hold on
plot(x_dot_leader.Time(N:end),x_dot_leader.Data(N:end),'LineWidth',2)
plot(x_dot_follower_ideal.Time(N:end),x_dot_follower_ideal.Data(N:end),'LineWidth',2)
plot(x_dot_follower_1.Time(N:end),x_dot_follower_1.Data(N:end),'LineWidth',2)
% plot(x_dot_follower_2.Time(N:end),x_dot_follower_2.Data(N:end),'LineWidth',2)
legend('Leader', 'Follower ideal', 'Follower 1');
ylabel('v (m/s)')
xlabel('t (s)')
% text(-10,20,'(b)','FontSize',16)
set(gca,'fontsize',16)

% subplot(3,1,3)
% hold on
% plot(x_leader.Time(N:end),x_leader.Data(N:end),'LineWidth',2)
% plot(x_follower_ideal.Time(N:end),x_follower_ideal.Data(N:end),'LineWidth',2)
% plot(x_follower_1.Time(N:end),x_follower_1.Data(N:end),'LineWidth',2)
% plot(x_follower_2.Time(N:end),x_follower_2.Data(N:end),'LineWidth',2)
% legend('Leader', 'Follower ideal', 'Follower 1', 'Follower 2');
% ylabel('x (m)')
% xlabel('t (s)')
% text(-10,800,'(c)','FontSize',16)
% set(gca,'fontsize',16)

subplot(3,1,3)
hold on
% plot(x_leader.Time(N:end),x_leader.Data(N:end),'LineWidth',2)
plot(x_leader.Time(N:end),x_leader.Data(N:end)-x_follower_ideal.Data(N:end),'LineWidth',2)
plot(x_leader.Time(N:end),x_leader.Data(N:end)-x_follower_1.Data(N:end),'LineWidth',2)
legend('Relative distance ideal', 'Relative distance model');
ylabel('x (m)')
xlabel('t (s)')
% text(-10,800,'(c)','FontSize',16)
set(gca,'fontsize',16)

x0=10;
y0=10;
width=1200;
height=600;
set(gcf,'units','points','position',[x0,y0,width,height])