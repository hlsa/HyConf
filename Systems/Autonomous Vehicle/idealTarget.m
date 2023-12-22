function output = idealTarget()
clc;
    f(11,6,10,4);
    f(11,9,18,4);
    f(11,12,32,4);
    f(11,15,56,4);
    f(6,4,4,2);
    f(10,4,4,6);
    f(18,4,4,10);
    f(34,4,4,18);
    f(4,8,28,2);
    f(10,16,66,3);
    f(16,24,128,5);
    f(22,34,296,7);
    disp('break');
    f(11,6,10,8);
    f(11,9,18,8);
    f(11,12,32,8);
    f(11,15,56,8);
    f(6,4,4,4);
    f(10,4,4,8);
    f(18,4,4,14);
    f(34,4,4,26);
    f(4,8,28,3);
    f(10,16,66,6);
    f(16,24,128,10);
    f(22,34,296,15);
    disp('break');
    f(11,6,10,11);
    f(11,9,18,11);
    f(11,12,32,11);
    f(11,15,56,11);
    f(6,4,4,6);
    f(10,4,4,10);
    f(18,4,4,18);
    f(34,4,4,34);
    f(4,8,28,4);
    f(10,16,66,10);
    f(16,24,128,16);
    f(22,34,296,22);
     
    %syms position(t) speed(t);
    %initialCondition1 = position(0) == 0;
    %initialCondition2 = speed(0) == 10;
    %eqn2 = diff(speed,t) == 10 - else 0;
    %eqn1 = diff(position,t) == speed(t) 
    %sol = dsolve(eqn,initialCondition);
    %ezplot(sol,[0,10]);
end

function y = f(v,s,t,e)
    y = (e^3 + 20*(v + s) + t)/79;
    disp(sprintf('%10.1f',y));
end