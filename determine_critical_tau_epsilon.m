close all; clear all; clc;
tic
% h = waitbar(0,'Running simulations...');
%% varying tau

a = 1.0;
b = 1.0;
count = 1;

for a = 1:0.5:2
    for b = 1:0.5:2
    run('Models/platooning_MI.m')

        tau_list = 0.1:0.1:3.5;
        delta_epsilon = 0.01;

        for tau_i = 1:length(tau_list)
%             waitbar(tau_i / length(tau_list))
            tau = tau_list(tau_i);
            [count tau]
            epsilon = 0.1;
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
%         % Save the variables
        temp_path = pwd;
        cd ../data/sim3/
        
        tau_name = strcat('tau_',num2str(count));
        eval([tau_name '=tau_list;']);
        tau_save_name = strcat(tau_name,'.mat');
        save(tau_save_name,tau_name);

        epsilon_name = strcat('epsilon_',num2str(count));
        eval([epsilon_name '=epsilon_list;']);
        epsilon_save_name = strcat(epsilon_name,'.mat');
        save(epsilon_save_name,epsilon_name);
        
        count = count + 1;
        cd(temp_path)
        clearex('count','a','b')
    end
end

toc
% close(h)

%% varying epsilon
% epsilon_list = 0.1:0.01:0.45;
% delta_tau = 0.01;
% 

% 
% for epsilon_i = 1:length(epsilon_list)
%     waitbar(epsilon_i / length(epsilon_list))
%     epsilon = epsilon_list(epsilon_i)
%     tau = 0.8;
%     run('conformance_check.m')
%     
%     if result == 0
%         while result ~= 1
%             tau = tau + delta_tau;
%             run('conformance_check.m');
%         end
%     
%     else
%         while result ~= 0
%             tau = tau - delta_tau;
%             run('conformance_check.m');
%         end
%         tau = tau + delta_tau;
%     end
%     tau_list(epsilon_i) = tau;
% end
% toc
% close(h)
%%
% figure(1)
% plot(tau_list,epsilon_list,'-o', 'LineWidth', 2)
% xlabel('\tau')
% ylabel('\epsilon')