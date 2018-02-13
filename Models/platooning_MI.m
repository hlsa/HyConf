% https://arxiv.org/pdf/0912.3613.pdf
dt = 0.01;
t_start = 0;
t_end = 60;

t = t_start:dt:t_end;

%% Simulation 1
% a_l = zeros(1,length(t));

%% Simulation 2
% for i = 1:length(t)
%     if i*dt <= 10
%         a_l(i) = 0;
%     elseif i*dt > 10 && i*dt <= 30
%         a_l(i) = 1;
%     elseif i*dt > 30 && i*dt <= 60
%         a_l(i) = 0;
%     elseif i*dt > 60 && i*dt <= 65
%         a_l(i) = 1.25;
%     elseif i*dt > 65 && i*dt <= 77
%         a_l(i) = 0;
%     elseif i*dt > 77 && i*dt <= 80
%         a_l(i) = -1.5;
%     elseif i*dt > 80 && i*dt <= 90
%         a_l(i) = 0;
%     elseif i*dt > 90 && i*dt <= 92
%         a_l(i) = -1;
%     elseif i*dt > 92 && i*dt <= 120
%         a_l(i) = 0;
%     elseif i*dt > 120 && i*dt <= 123
%         a_l(i) = -0.75;
%     elseif i*dt > 123 && i*dt <= 130
%         a_l(i) = 0;
%     elseif i*dt > 130 && i*dt <= 140
%         a_l(i) = 1;
%     elseif i*dt > 140
%         a_l(i) = 0;
%     end
% end

%% Simulation 3
a_max = 1;
a_min = -0.5;

for i = 1:length(t)
    if i*dt <= 10
        a_l(i) = 0;
    elseif i*dt > 10 && i*dt <= 15
        a_l(i) = (i*dt - 10)/5*a_max;
    elseif i*dt > 15 && i*dt <= 20
        a_l(i) = a_max;
    elseif i*dt > 20 && i*dt <= 25
        a_l(i) = a_max-(i*dt - 20)/5*a_max;
    elseif i*dt > 25 && i*dt <= 35
        a_l(i) = 0;
    elseif i*dt > 35 && i*dt <= 40
        a_l(i) = (i*dt - 35)/5*a_min;;
    elseif i*dt > 40 && i*dt <= 45
        a_l(i) = a_min;
    elseif i*dt > 45 && i*dt <= 50
        a_l(i) = a_min-(i*dt - 45)/5*a_min;;
    elseif i*dt > 50
        a_l(i) = 0;
    end
end


v_l = 10;
x_l = 100;
for i = 2:length(t)
    v_l(i) = v_l(i-1)+a_l(i-1)*dt;
    x_l(i) = x_l(i-1)+v_l(i-1)*dt+0.5*a_l(i-1)*dt^2;
end


%% Model parameters car
v_0 = 120; %m/s     desired speed
T = 1.5; %s         time headway
s_0 = 2.0; %m       minimum gap
a = 2.0; %m/s^2     acceleration
b = 2.0; %m/s^2     deceleration
delta = 4; %        acceleration exponent
l_avg = 4.8; %m     average car length


x_f(1) = 78.2;
v_f(1) = 10;
a_f(1) = 0;

for time = t(2:end)
    i = round(time*(1/dt)+1);
    
    s_star = s_0 + v_f(i-1)*T + ((v_f(i-1)*(v_f(i-1)-v_l(i-1))))/(2*sqrt(a*b));
    s = x_l(i-1)-x_f(i-1)-l_avg;
    
    a_f(i) = a*(1-((v_f(i-1)/v_0))^delta-(s_star/s)^2);
    v_f(i) = v_f(i-1)+a_f(i-1)*dt;
    x_f(i) = x_f(i-1)+v_f(i-1)*dt+0.5*a_f(i-1)*dt^2;
end

% small fix for the beginning

%% Plot the results
figure(1)
subplot(3,1,1)
hold on
plot(t,a_l)
plot(t,a_f)
title('Acceleration')
ylabel('a (m/s^2)')
xlabel('t (s)')
legend('Leader','Follower')
subplot(3,1,2)
hold on
plot(t,v_l)
plot(t,v_f)
title('Velocity')
ylabel('v (m/s)')
xlabel('t (s)')
legend('Leader','Follower')
subplot(3,1,3)
hold on
plot(t,x_l)
plot(t,x_f)
title('Position')
ylabel('x (m)')
xlabel('t (s)')
legend('Leader','Follower')
% 
% figure(2)
% plot(t,x_l-x_f-l_avg)
% title('Bumper-bumper distance')
% xlabel('t (s)')
% ylabel('distance (m)')
%%
figure(3)
plot([0 60],[0 0], 'k')

hold on
plot(t,a_l,'b','LineWidth',2)
plot([0,15],[a_max, a_max], 'k--','LineWidth',2)
plot([0,40],[a_min, a_min], 'k--','LineWidth',2)

axis([0,60,a_min-0.5,a_max+0.5])
text(7.0,1.2,'a_{max}','FontSize',12)
text(19,-0.3,'a_{min}','FontSize',12)
xlabel('time (s)')
ylabel('acceleration (m/s^2)')
title('Predescribed acceleration for the leading car','FontSize',20)
set(gca,'fontsize',16)
%%
model.variables = {'x','v','a'};
model.data(1,:) = x_l;
model.data(2,:) = v_l;
model.data(3,:) = a_l;
model.time = t;

implementation.variables = {'x','v','a'};
implementation.data(1,:) = x_f;
implementation.data(2,:) = v_f;
implementation.data(3,:) = a_f;
implementation.time = t;

handles.matlab.model = model;
handles.matlab.implementation = implementation;