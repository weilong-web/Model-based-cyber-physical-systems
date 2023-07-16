clc
T = readtable('simulation.csv')
%%
clf
figure('Name','Roll');
T.estimated_roll = rad2deg(T.estimated_roll); 
hold on
plot(T.time, T.actual_roll);
plot(T.time, T.estimated_roll);
plot(T.time, T.reference_roll);
legend("r_{true}", "r_{est}", "r_{ref}");

figure('Name','Pitch');
T.estimated_pitch = rad2deg(T.estimated_pitch); 
hold on
plot(T.time, T.actual_pitch);
plot(T.time, T.estimated_pitch);
plot(T.time, T.reference_pitch);
legend("p_{true}", "p_{est}", "p_{ref}");