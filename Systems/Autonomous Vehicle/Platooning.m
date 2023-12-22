function [] = Platooning()
    clc;

   
    figure('units','centimeters','position',[25,15,20,10])  
    hold on
    
    grid on
    xlim([0 9]);
    ylim([0 5]);
    ax = gca
    ax.XAxis.FontSize = 15;
    ax.YAxis.FontSize = 15;
    ax.FontWeight = 'bold';
    xticks([0:0.1:9]);
    yticks([0:0.1:5]);
    
   
    critical_epsilon_example();
    %robotic_arm_angle();
    %robotic_arm_angle_fix();
    %robotic_arm_coordinates();
    %robotic_arm_coordinates_fix();
    %suspension1();
    %suspension1_fix();
    %suspension2();
    %suspension2_fix();

    hold off
    
end






function [] = critical_epsilon_example()
    l1 = fplot(@(x) x, [0,4], 'r', 'DisplayName','$y_1$', 'LineWidth',2);
         fplot(@(x) (-x+8), [4,8], 'r', 'DisplayName','$y_1$', 'LineWidth',2);
    l2 = fplot(@(x) x-0.2, [0,4.2], 'b', 'DisplayName','$y_2$', 'LineWidth',2);
         fplot(@(x) -x + 8.2, [4.2,8.2], 'b', 'DisplayName','$y_2$', 'LineWidth',2);
    hleg = legend([l1 l2],'location','best', 'Interpreter','latex');
    set(hleg,'FontSize',15);
    
end

function [] = robotic_arm_angle()
    a = 110;
    b = 100;
    p = 129;
    e = 0;
    l1 = fplot(@(x) p, [0,500], 'DisplayName','$p1.\Theta$', 'LineWidth',2);
    l2 = fplot(@(x) e, [0,500], 'r', 'DisplayName','$ee.\Theta$', 'LineWidth',2);
    hleg = legend([l1 l2],'location','best', 'Interpreter','latex');
    set(hleg,'FontSize',15);
end

function [] = robotic_arm_angle_fix()
    a = 110;
    b = 100;
    p = 129;
    e = 0;
    e2 = 203.42
    l1 = fplot(@(x) p, [0,500],'DisplayName','$p1.\Theta$','LineWidth',2);
    l2 = fplot(@(x) e, [0,150], 'r', 'DisplayName','$ee.\Theta$','LineWidth',2);
    fplot(@(x) a*rob_angle((x-193)/b)+85, [150,300], 'r', 'LineWidth',2);
    fplot(@(x) e2, [300,500], 'r', 'LineWidth',2);
    hleg = legend([l1 l2],'location','best', 'Interpreter','latex');
    set(hleg,'FontSize',15);
end

function y = rob_angle(x)
    y = x^2 + sin(3*x);
end

function [] = robotic_arm_coordinates()
    p1x = 0.91;
    p2x = 0.75;
    p1y = 0;
    p2y = 0;
    p1z = 1.7;
    p2z = 0.4;  
    
    l1 = fplot(@(x) (0.8*x + p1x), [0,12],'b', 'DisplayName','p1.x', 'LineWidth',2);
    l2 = fplot(@(x) (0.8*x + p2x), [0,12],'g', 'DisplayName','p2.x', 'LineWidth',2);
    l3 = fplot(@(x) p1y, [0,12],'r', 'DisplayName','p1.y', 'LineWidth',2);
    l4 = fplot(@(x) p2y, [0,12],'c', 'DisplayName','p2.y', 'LineWidth',2);
    l5 = fplot(@(x) p1z, [0,12],'m', 'DisplayName','p1.z', 'LineWidth',2);
    l6 = fplot(@(x) p2z, [0,12],'y', 'DisplayName','p2.z', 'LineWidth',2);

    hleg = legend([l1 l2 l3 l4 l5 l6],'location','best');
end

function [] = robotic_arm_coordinates_fix()
    p1x = 0.91;
    p2x = 0.75;
    p1y = 0;
    p2y = 0;
    p1z = 1.7;
    p2z = 0.4;  
    
    l2 = fplot(@(x) (0.8*x + p2x), [0,12],'g', 'DisplayName','p2.x', 'LineWidth',2);
    l1 = fplot(@(x) (0.8*x + p1x), [0,2.34],'b', 'DisplayName','p1.x', 'LineWidth',2);
    fplot(@(x) 2.778, [2.34,2.94],'b', 'DisplayName','p1.x', 'LineWidth',2);
    fplot(@(x) ra_s1(x), [2.94,4],'b', 'DisplayName','p1.x', 'LineWidth',2);
    fplot(@(x) (0.8*x + p2x), [4,12],'b', 'DisplayName','p1.x', 'LineWidth',2);
    
    l4 = fplot(@(x) p2y, [0,12],'c', 'DisplayName','p2.y', 'LineWidth',2);
    l3 = fplot(@(x) p1y, [0,2.34],'r', 'DisplayName','p1.y', 'LineWidth',2);
    fplot(@(x) (2.5*x)-5.85, [2.34,2.94],'r', 'DisplayName','p1.y', 'LineWidth',2);
    fplot(@(x) ra_s2(x), [2.94,4],'r', 'DisplayName','p1.y', 'LineWidth',2);
    fplot(@(x) -(2.5*x)+11.25, [4,4.5],'r', 'DisplayName','p1.y', 'LineWidth',2);
    fplot(@(x) p2y, [4.5,12],'r', 'DisplayName','p1.y', 'LineWidth',2);
    
    l6 = fplot(@(x) p2z, [0,12],'y', 'DisplayName','p2.z', 'LineWidth',2);
    l5 = fplot(@(x) p1z, [0,2.4],'m', 'DisplayName','p1.z', 'LineWidth',2);
    fplot(@(x) ra_s3(x), [2.4,3.237],'m', 'DisplayName','p1.z', 'LineWidth',2);
    fplot(@(x) p2z, [3.237,12],'m', 'DisplayName','p1.z', 'LineWidth',2);

    hleg = legend([l1 l2 l3 l4 l5 l6],'location','best');
end

function y = ra_s1(x)
a = 23.6;
b = 20;
c = 5.62;
d = -2.6;
y = 3*(((x-d)/a)^2+sin(b*((x-d)/a)))+c;
end

function y = ra_s2(x)
a = 12;
b = 20;
c = -1;
d = -1.3;
y = 3*(((x-d)/a)^2+sin(b*((x-d)/a)))+c;
end

function y = ra_s3(x)
a = 20;
b = 24;
c = -2.35;
d = -9.4;
y = 3*(((x-d)/a)^2+sin(b*((x-d)/a)))+c;
end


function [] = suspension1()
    a = 6.1;
    b = 3.7;
    c = 5;
    d = 7.1;
    z = 5.57;

    fplot(@(x) (3*((x/9)^2+sin(3*(x/8.4)))+1), [0,15], 'LineWidth',2);
    fplot(@(x) 6, [0,15], 'LineWidth',2);
    fplot(@(x) 4, [0,15], 'LineWidth',2);
    fplot(@(x) (3*((x/a)^2+sin(b*(x/a)))+c), [0,8], 'r', 'LineWidth',2);
    fplot(@(x) (3*(((x-d)/a)^2+sin(b*((x-d)/a)))+z), [8,15], 'r', 'LineWidth',2);
end

function [] = suspension1_fix()
    a = 6.1;
    b = 3.7;
    c = 5;
    d = 6.5;
    z = 4.638;

    fplot(@(x) (3*((x/9)^2+sin(3*(x/8.4)))+1), [0,15], 'LineWidth',2);
    fplot(@(x) 5, [0,15], 'LineWidth',2);
    fplot(@(x) 4, [0,15], 'LineWidth',2);
    fplot(@(x) (3*((x/a)^2+sin(b*(x/a)))+c), [0,8], 'r', 'LineWidth',2);
    fplot(@(x) (3*(((x-d)/a)^2+sin(b*((x-d)/a)))+z), [8,15], 'r', 'LineWidth',2);
end

function [] = suspension2()
    a = 25;
    b = 3.9;
    c = 5;
    d = 9.6;    
    z = 8.44;
    h2 = 4.08;
    k2 = 7.14;
    h = 2.72;
    k = 3.08;
    
    syms t;
    
    fplot(@(x) (sus2_f(x)/50), [0,2.236],'k', 'LineWidth',2);
    fplot(@(x) (2*sus2_f(x)+0.3), [2.236,5.08],'k', 'LineWidth',2);
    fplot(@(x) (sus2_f(x)/50), [5.08,8.1],'k', 'LineWidth',2);
    fplot(@(x) (-1*(sus2_f(x)^2)), [8,10.1],'k', 'LineWidth',2);
    fplot(@(x) (sus2_f(x)/50), [10.1,15],'k', 'LineWidth',2);
    
    fplot(@(x) (3*((x/a)^2+sin(b*(5*x/a)))+c), [0,5.974],'r', 'LineWidth',2);
    fplot(@(x) (2.177), [5.974,15],'r', 'LineWidth',2);
    
    fplot(@(x) (6.742), [0,2.7],'b', 'LineWidth',2);
    fplot(@(x) (int(sus2_f2(t),t,0,x-4.5)+5), [2.7,6.371],'b', 'LineWidth',2);
    fplot(@(x) (3.254), [6.371,15],'b', 'LineWidth',2);
    
    fplot(@(x) (2.3), [0,2.3],'m', 'LineWidth',2);
    fplot(@(x) (sus2_f3(x-h)+k), [2.3,4.9],'m', 'LineWidth',2);
    fplot(@(x) (sus2_f3(0.9*x-h2)+k2), [4.9,5.894],'m', 'LineWidth',2);
    fplot(@(x) (8.131), [5.894,15],'m', 'LineWidth',2);



    
end

function y = sus2_f(x)
    syms k;
    y = symsum((((-0.84)^k)*(cos(((1.16^k)*pi*(x+9))))),k,2,20)+1;
end

function y = sus2_f2(x)
    y = 0.4*(x^2)-1.4;
end

function y = sus2_f3(x)
    y = x^2+sin(3*x);
end


function [] = suspension2_fix()
    a2 = 14.5;
    b2 = 5.94;
    c2 = 5.57;
    a = 25;
    b = 3.9;
    c = 5;
    d = 9.6;    
    z = 8.44;
    h2 = 4.08;
    k2 = 7.14;
    h = 2.72;
    k = 3.08;
    
    syms t;
    
    fplot(@(x) (sus2_f(x)/50), [0,2.236],'k', 'LineWidth',2);
    fplot(@(x) (2*sus2_f(x)+0.3), [2.236,5.08],'k', 'LineWidth',2);
    fplot(@(x) (sus2_f(x)/50), [5.08,8.1],'k', 'LineWidth',2);
    fplot(@(x) (-1*(sus2_f(x)^2)), [8,10.1],'k', 'LineWidth',2);
    fplot(@(x) (sus2_f(x)/50), [10.1,15],'k', 'LineWidth',2);
    
    fplot(@(x) (3*((x/a)^2+sin(b*(5*x/a)))+c), [0,5.974],'r', 'LineWidth',2);
    fplot(@(x) (2.177), [5.974,7],'r', 'LineWidth',2);
    fplot(@(x) ((3*((x/a2)^2+sin(b2*(6*x/a2)))+c2)/1.5), [7,8.321],'r', 'LineWidth',2);
    fplot(@(x) (6.371), [8.321,15],'r', 'LineWidth',2);

    
    fplot(@(x) (6.742), [0,2.7],'b', 'LineWidth',2);
    fplot(@(x) (int(sus2_f2(t),t,0,x-4.5)+5), [2.7,6.371],'b', 'LineWidth',2);
    fplot(@(x) (3.254), [6.371,8.17],'b', 'LineWidth',2);
    fplot(@(x) (2*cos((2*pi*(x+2.3))/3)+5.25), [8.17,11.17],'b', 'LineWidth',2);
    fplot(@(x) (3.254), [11.17,15],'b', 'LineWidth',2);


    
    fplot(@(x) (2.3), [0,2.3],'m', 'LineWidth',2);
    fplot(@(x) (sus2_f3(x-h)+k), [2.3,4.9],'m', 'LineWidth',2);
    fplot(@(x) (sus2_f3(0.9*x-h2)+k2), [4.9,5.894],'m', 'LineWidth',2);
    fplot(@(x) (8.131), [5.894,8.095],'m', 'LineWidth',2);
    fplot(@(x) (((x-10)^2+sin(3*(x-6)))+4.5), [8.095,11.374],'m', 'LineWidth',2);
    fplot(@(x) (5.986), [11.374,15],'m', 'LineWidth',2);


end