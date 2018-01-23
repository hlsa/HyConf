close all; clear all; clc;

run('Models/platooning_MI.m')

tau_list = 0.1:0.1:3;
delta_epsilon = 0.01;

load('../data/1_epsilon.mat');
epsilons = epsilon_list;
clear epsilon_list

for tau_i = 1:length(tau_list)
    tau = tau_list(tau_i)
    epsilon = epsilons(tau_i) + 0.1;
    run('conformance_check.m')
    
    if result == 0
        while result ~= 1
            epsilon = epsilon + delta_epsilon;
            run('conformance_check.m');
        end
    
    else
        while result ~= 0
            epsilon = epsilon - delta_epsilon;
            run('conformance_check.m');
        end
        epsilon = epsilon + delta_epsilon;
    end
    
    epsilon_list(tau_i) = epsilon;
end
%%
figure(1)
plot(tau_list,epsilon_list, 'LineWidth', 2)
xlabel('\tau')
ylabel('\epsilon')