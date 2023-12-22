clear all;
warning off;

tic

timestep = 0.001;

a_factor = 1;
a_max = 1.0 * a_factor;     % maximum acceleration of the leading car
a_min = -0.5 * a_factor;    % minimum acceleration of the leading car

T_THRESHOLD = 1;            % time threshold
D_THRESHOLD = 4;            % distance threshold
S_THRESHOLD = 0.5;          % speed threshold

N = 1000;                   % moving average parameter

plot_figures = true;

%% Car parameters
% car.v_0 = 120;
% car.delta = 4;
% car.T = 1.5;
% car.s_0 = 2.0;
% car.a = 1.4;
% car.b = 2.0;
% car.l_avg = 4.8;

car.v_0 = 120/3.6;
car.delta = 4;
car.T = 1.6;
car.s_0 = 2.0;
car.a = 1.4;
car.b = 1.67;
car.l_avg = 5;

%% Truck parameters
truck.v_0 = 80/3.6;
truck.delta = 4;
truck.T = 1.7;
truck.s_0 = 2.0;
truck.a = 0.7;
truck.b = 1.0;
truck.l_avg = 16;

%% Setup the platoon
leader = car;
follower_1 = car;
follower_2 = car;
follower_3 = car;

%% Initial positions/velocities
leader.x0 = 150;
leader.v0 = 20;

follower_1.x0 = leader.x0 - leader.l_avg - leader.v0 * follower_1.T - follower_1.s_0;
follower_1.v0 = 20;

follower_2.x0 = follower_1.x0 - follower_1.l_avg - follower_1.v0 * follower_2.T - follower_2.s_0;
follower_2.v0 = 20;

follower_3.x0 = follower_2.x0 - follower_2.l_avg - follower_2.v0 * follower_3.T - follower_3.s_0;
follower_3.v0 = 20;

%% Start the simulation
disp('Simulation started')
sim('car_following_model_multiple_vehicles')
disp('Simulation done')

t_start = 10 / timestep + 1;

%% Plot the results

if plot_figures
    figure(1)
    subplot(3,1,1)
    hold on
    plot(x_ddot_leader.Time(t_start:end),x_ddot_leader.Data(t_start:end),'LineWidth',2)
%     plot(x_ddot_follower_ideal.Time(t_start:end),x_ddot_follower_ideal.Data(t_start:end),'LineWidth',2)
    plot(x_ddot_follower_1.Time(t_start:end),x_ddot_follower_1.Data(t_start:end),'LineWidth',2)
    plot(x_ddot_follower_2.Time(t_start:end),x_ddot_follower_2.Data(t_start:end),'LineWidth',2)
    plot(x_ddot_follower_3.Time(t_start:end),x_ddot_follower_3.Data(t_start:end),'LineWidth',2)
    axis([10 70 -1 1])
    set(gca,'fontsize',12)
    title('Acceleration')
    ylabel('a (m/s^2)')
%     legend('Leader','Following 1 (ideal)','Following 1','Following 2','Following 3','Location','northwest')
    legend('Leader','Following 1','Following 2','Following 3','Location','northwest')

    subplot(3,1,2)
    hold on
    plot(x_dot_leader.Time(t_start:end),x_dot_leader.Data(t_start:end),'LineWidth',2)
%     plot(x_dot_follower_ideal.Time(t_start:end),x_dot_follower_ideal.Data(t_start:end),'LineWidth',2)
    plot(x_dot_follower_1.Time(t_start:end),x_dot_follower_1.Data(t_start:end),'LineWidth',2)
    plot(x_dot_follower_2.Time(t_start:end),x_dot_follower_2.Data(t_start:end),'LineWidth',2)
    plot(x_dot_follower_3.Time(t_start:end),x_dot_follower_3.Data(t_start:end),'LineWidth',2)
    axis([10 70 15 30])
    set(gca,'fontsize',12)
    title('Velocity')
    ylabel('v (m/s)')
%     legend('Leader','Following 1 (ideal)','Following 1','Following 2','Following 3','Location','northwest')
    legend('Leader','Following 1','Following 2','Following 3','Location','northwest')

    subplot(3,1,3)
    hold on
    plot(x_leader.Time(t_start:end),x_leader.Data(t_start:end),'LineWidth',2)
%     plot(x_follower_ideal.Time(t_start:end),x_follower_ideal.Data(t_start:end),'LineWidth',2)
    plot(x_follower_1.Time(t_start:end),x_follower_1.Data(t_start:end),'LineWidth',2)
    plot(x_follower_2.Time(t_start:end),x_follower_2.Data(t_start:end),'LineWidth',2)
    plot(x_follower_3.Time(t_start:end),x_follower_3.Data(t_start:end),'LineWidth',2)
    title('Position')
    set(gca,'fontsize',12)
    ylabel('x (m)')
    xlabel('t (s)')
%     legend('Leader','Following 1 (ideal)','Following 1','Following 2','Following 3','Location','northwest')
    legend('Leader','Following 1','Following 2','Following 3','Location','northwest')
        
%     figure(2)
%     hold on
%     plot(x_leader.Time(t_start:end),x_leader.Data(t_start:end)-x_follower_1.Data(t_start:end),'LineWidth',2)
%     plot(x_leader.Time(t_start:end),x_follower_1.Data(t_start:end)-x_follower_2.Data(t_start:end),'LineWidth',2)
%     plot(x_leader.Time(t_start:end),x_follower_2.Data(t_start:end)-x_follower_3.Data(t_start:end),'LineWidth',2)
%     legend('Distance leader and follower','Distance follower 1 and follower 2','Distance follower 2 and follower 3')
end

toc


