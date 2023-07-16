clc; clear all; close all;
load FlightData.mat
%% Linearization
syms phi theta dphi dtheta dpsi real
syms wx wy wz wdx wdy wdz real
syms T1 T2 T3 T4 real

d = 0.046;
dist = d*cos(pi/4);
lift = 1.9796e-9; %Lift constant
drag = 2.5383e-11;%Drag constant
Jx = 1.1463e-5;
Jy = 1.6993e-5;
Jz = 2.9944e-5;

% syms Jx Jy Jz drag lift dist

w = [wx;wy;wz];
x = [phi; theta; w];
u = [T1; T2; T3; T4];

tauz = drag * (T2+T4-T1-T3)/lift;
taux = dist*(T4+T3-T2-T1);
tauy = dist*(T3 + T2 - T1 - T4);

Tau = [taux; tauy; tauz];

J = diag([Jx, Jy, Jz]);

w_dot = inv(J)*(-cross(w,J*w)+Tau);

rot_mat_inv = [1, 0, sin(theta);
    0, cos(phi), -sin(phi)*cos(theta);
    0, sin(phi), cos(phi)*cos(theta)]';

der_euler = rot_mat_inv*w;

%non linear
f_nl = [der_euler(1:2,:);w_dot]; 

A_non_operating = jacobian(f_nl,x);
B_non_operating = jacobian(f_nl,u);

A = double(subs(A_non_operating, [phi,theta,wx,wy,wz],[0,0,0,0,0]));
g = 9.81;
thrust_converter = 0.06/4*g/65536;
B = double(subs(B_non_operating, [phi,theta,wx,wy,wz],[0,0,0,0,0]));


%% Discretization

sys = ss(A,B,zeros(1,size(A,1)),0);

T = 1e-3;

sys_d = c2d(sys,T);

discrete_matrix = expm([A,B;zeros(4,5),zeros(4,4)]*T);
Ad = discrete_matrix(1:5,1:5);
Bd = discrete_matrix(1:5,6:end);
A = sys_d.A;
B = sys_d.B;

Qx = diag([30, 30, 10, 10, 10])*1;
Qu = diag([1 1 1 1])*1;

[L,S,P] = dlqr(A,B,Qx,Qu);
disp(L)

%% From Project_1
h = T;
alpha = 1;
gamma = alpha/(h+alpha);

radi = 0.4;
