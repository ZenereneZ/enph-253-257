k = 205;
L = 0.3;
a = 0.01;
c = 900;
roh = 2700;
dx = 0.01;
dt = 0.1;
P = 10;
kc = 5;
Tamb = 20;
sigma = 5.67E-8;
epsilon = 1;

x_left = linspace(0, 2.9, 30);
x_n = linspace(0.005, 0.295, 30);
T_n = ones(1,30) * 27.5;
simSensors = zeros(6638, 6);
t = 1:6638;
for i = 0:66379
    if(mod(int32(i/3000), 2) == 1) 
        P = 0;
    else
        P = 10;
    end
    dT_nfirst = -k*(T_n(1) - T_n(2))*dt/(c*roh*dx^2);
    dT_nlast = k*(T_n(end-1) - T_n(end))*dt/(c*roh*dx^2);
    dT_convectivefirst = -2*kc*(T_n(1) - Tamb)*dt/(c*roh*a);
    dT_convectivelast = -2*kc*(T_n(end) - Tamb)*dt/(c*roh*a);
    T_n(1) = T_n(1) + (dT_nfirst + dT_convectivefirst + P*dt/(c*roh*pi*a^2*dx));
    T_n(end) = T_n(end) + (dT_nlast + dT_convectivelast);

    for j = 2:29
        dT_n = (k/(c*roh))*dt*(T_n(j-1)-2*T_n(j)+T_n(j+1))/dx^2;
        dT_radiative = - (2/(c*roh*a))*dt*(kc*(T_n(j)-Tamb)+epsilon*sigma*(((T_n(j)+273.15)^4)-(Tamb+273.15)^4));
        T_n(j) = (dT_n + dT_radiative);
    end
    if (mod(i, 10) == 0)
        time = i/10 + 1;
        simSensors(time, 1) = T_n(2);
        simSensors(time, 2) = T_n(7);
        simSensors(time, 3) = T_n(12);
        simSensors(time, 4) = T_n(17);
        simSensors(time, 5) = T_n(22);
        simSensors(time, 6) = T_n(27);
    end
end
for i = 1:6
    plot(t, simSensors(:, i));
end
