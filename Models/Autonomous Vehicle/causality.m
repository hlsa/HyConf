%BAD BREAKS BAD BATTERY

% fplot(0.2,[0 10],'b')
% hold on
% fplot(@(x) 10-x,[0 10],'g')
% hold on
% fplot(10,[0 7],'r')
% hold on
% fplot(@(x) (log(x-6.5)/log(0.8))+ 6.9,[7 10],'r')
% hold off
% grid on
% xlim([0 10])
% ylim([0 10])

%GOOD BREAKS BAD BATTERY

% fplot(0.8,[0 10],'b')
% hold on
% fplot(@(x) 10-x,[0 10],'g')
% hold on
% fplot(10,[0 7],'r')
% hold on
% fplot(@(x) (log(x-6)/log(0.9))+ 10,[7 10],'r')
% hold off
% grid on
% xlim([0 10])
% ylim([0 10])


%BAD BREAKS GOOD BATTERY 

% fplot(0.2,[0 10],'b')
% hold on
% fplot(@(x) 10-x/2,[0 10],'g')
% hold on
% fplot(10,[0 6],'r')
% hold on
% fplot(@(x) (log(x-5.5)/log(0.8))+ 6.9,[6 10],'r')
% hold off
% grid on
% xlim([0 10])
% ylim([0 10])


%GOOD BREAKS GOOD BATTERY 

% fplot(0.8,[0 10],'b')
% hold on
% fplot(@(x) 10-x/2,[0 10],'g')
% hold on
% fplot(10,[0 6],'r')
% hold on
% fplot(@(x) (log(x-5)/log(0.9))+ 10,[6 10],'r')
% hold off
% grid on
% xlim([0 10])
% ylim([0 10])



% fplot(@(x) 10-x,[4 6],'g')
% grid on
% xlim([0 10])
% ylim([0 10])


% fplot(@(x) 10-x,[4 6],'g')
% grid on
% xlim([0 10])
% ylim([0 10])

%fplot(@(x) 10-x,[0 4],'g')
%hold on
fplot(@(x) -(x-5)^2+7 ,[0 10],'g')
%hold on
%fplot(@(x) 10-x,[7 10],'g')
%hold off
grid on
xlim([0 10])
ylim([0 10])







%%%%%%%%%%%%%%%%%



%fplot(@(x) log(x)/log(0.5))
%grid on

%fplot(20,[0,5],'b')
%hold on
%fplot(10,[5,15],'b')
%hold off
%grid on




%dist = [2.3, 9.7, 4.2, 5.6, 1.1, 8.8, 7.9, 2.1, 9.2, 5.1, 8.0];
%bend = [0,0,0,0,0,0,0,0,1,0,0];
%comp = sin(t)+4;
%plot(t,bend,t,dist,t,comp)
%legend('bend','dist','comp')

    