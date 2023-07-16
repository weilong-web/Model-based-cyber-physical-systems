clear all; 
close all; 
clc;

h = 0.01; 
%syms Jx Jy Jz real
Jx = 1.1463e-5;
Jy = 1.6993e-5;
Jz = 2.9944e-5;
J = [Jx,0,0; 0,Jy,0; 0,0,Jz];
m = 0.027;                  % Mass
g = 9.81;                   % Gravitational acceleration
d = 0.046;                  % Distance from center of mass to rotor axis
lift = 1.9796E-9;           % Lift constant b
b = 1.9796E-9;
drag =  2.5383E-11;         % Drag constant k
k = 2.5383E-11;

syms theta psi phi ctrl1 ctrl2 ctrl3 ctrl4 real

% torque in body frame
tau_x = d/sqrt(2) * (ctrl3 - ctrl2 - ctrl1 + ctrl4);
tau_y = d/sqrt(2) * (-ctrl4 + ctrl3 + ctrl2 - ctrl1);
tau_z = drag/lift * (ctrl2 + ctrl4 - ctrl1 - ctrl3);
tau = [tau_x; tau_y; tau_z];

syms w_r w_p w_y real

wb = [w_r; w_p; w_y];
dw = inv(J) * (cross(-wb, J * wb) + tau);

T = [1, 0, sin(theta);
     0, cos(phi), -sin(phi)*cos(theta);
     0, sin(phi), cos(phi)*cos(theta)];
w = T' * wb;

f = [wb(1:2); dw];
x = [phi; theta; w_r; w_p; w_y];
u = [ctrl1; ctrl2; ctrl3; ctrl4];
A = jacobian(f, x);
B = jacobian(f, u);


% operating point
x_0 = [0; 0; 0; 0; 0];
A_num = subs(A, x, x_0);
B_num = subs(B, x, x_0);


Ad = double(expm(A_num*h));
syms t s
eAt = ilaplace(inv(eye(size(Ad,1))*s - A_num));
Bd = double(int(eAt*B_num,t,0,h));

% State error penalty
%Q = 900.*eye(length(A_num));
%Q = eye(5)*100;
%Q = diag([10 10 1000 1000 1000 ])*10e6;
Q = diag([1000 1000 1 1 1]);

R = eye(4);
[K,~,~] = dlqr(Ad,Bd,Q,R)




















