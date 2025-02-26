clc; clear;
clear all;
close all;
% Lab 4, Part 4

%% Create simulated data
% uncomment, run this portion once and save as file then comment it back
% only perform action once

%[Acc, Mag, wGyro, Eul] = CreateTrajectoryData(0.02, 'true'); % generate trajectory data
%save("simulated_trajectory_data.mat");
%load('simulated_trajectory_data.mat');

% outputs a 3 × n vector for:
%  - accelerometer          floating point numbers with unit norm
%  - magnetometer           floating point numbers with unit norm
%  - gyroscope              scaled integers
%  - true Euler angles      degrees

%%
% convert deg to rad
% div by scale factor 131


%%%%%%%%%%%%%%%%%%noise free, forward and exp integration%%%%%%%%%%%%%%

%choosing from foward and exp integration is changing the flag in
%IntegrateOpenLoop.m

deltaT = 0.1; %different delta than d

[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(deltaT, 0); %without noise
t_array = 0:deltaT:(length(Eul)-1)*deltaT;


% creating rotation matrix
phi = Eul(1,3);
theta = Eul(1,2);
psi = Eul(1,1);
%xyz
Rminus = [(cos(theta)*cos(psi)) (cos(theta)*sin(psi)) (-sin(theta)); 
    (sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi)) (sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi)) (sin(phi)*cos(theta)); 
    (cos(phi)*sin(theta)*cos(psi)-sin(phi)*sin(psi)) (cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi)) (cos(phi)*cos(theta))];

%fills 0s of skew-symmetric matrix with 1s, serves as "empty matrix" 
fill = [1 0 0; 0 1 0; 0 0 1];
%for foward integreation
eul_forward = [];
Rminus = fill;

%getting bias from average of gyro data
Gyrobias_x = mean(wGyro(:, 1));
Gyrobias_y = mean(wGyro(:, 2));
Gyrobias_z = mean(wGyro(:, 3));


%reminder: phi = x, theta = y, psi = z
%converting true euler to rads
phi = deg2rad(Eul(1,1));
theta = deg2rad(Eul(1,2));
psi = deg2rad(Eul(1,3));

for i = 1:length(Eul)
    %extracting each row or wGyro with each iterations of i
    gyro_row = wGyro(i, 1:3);
    %subracting bias from data and converting to rads
    gyro_row(1) = (deg2rad(gyro_row(1) - Gyrobias_x)/131);
    gyro_row(2) = (deg2rad(gyro_row(2) - Gyrobias_y)/131);
    gyro_row(3) = (deg2rad(gyro_row(3) - Gyrobias_z)/131);
    %forward integration
    Rplus = IntegrateOpenLoop(Rminus, gyro_row, deltaT);
    % euler angles
    phi = rad2deg(atan2(Rplus(2,3), Rplus(3,3)));
    theta = rad2deg(-asin(Rplus(1,3)));
    psi = rad2deg(atan2(Rplus(1,2),Rplus(1,1)));
    %foward integeration eul angles matrix
    eul_forward(i, 1:3) = [psi, theta, phi];
    Rminus = Rplus;
end

figure(1);
subplot(3,2,1);
plot(t_array, eul_forward(:,1))
hold on;
plot(t_array,Eul(:,1))
legend('Exponential', 'True');
title('Z Axis');

subplot(3,2,2);
plot(t_array, abs(eul_forward(:,1) - Eul(:,1)))
title('Z Axis Error');

subplot(3,2,3);
plot(t_array, eul_forward(:,2))
hold on;
plot(t_array,Eul(:,2))
legend('Exponential', 'True');
title('Y Axis');
subplot(3,2,4);
plot(t_array, abs(eul_forward(:,2) - Eul(:,2)))
title('Y Axis Error');


subplot(3,2,5);
plot(t_array, eul_forward(:,3))
hold on;
plot(t_array,Eul(:,3))
legend('Exponential', 'True');
title('X Axis');
subplot(3,2,6);
plot(t_array, abs(eul_forward(:,3) - Eul(:,3)))
title('X Axis Error');


%%%%%%%%%%%%%%%%%%noisy, forward and exp integration%%%%%%%%%%%%%%



deltaT = 0.1; %different delta than d

[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(deltaT, 1); %with noise
t_array = 0:deltaT:(length(Eul)-1)*deltaT;


% creating rotation matrix
phi = Eul(1,3);
theta = Eul(1,2);
psi = Eul(1,1);
%xyz
Rminus = [(cos(theta)*cos(psi)) (cos(theta)*sin(psi)) (-sin(theta)); 
    (sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi)) (sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi)) (sin(phi)*cos(theta)); 
    (cos(phi)*sin(theta)*cos(psi)-sin(phi)*sin(psi)) (cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi)) (cos(phi)*cos(theta))];

%fills 0s of skew-symmetric matrix with 1s, serves as "empty matrix" 
fill = [1 0 0; 0 1 0; 0 0 1];
%for foward integreation
eul_forward = [];
Rminus = fill;

%getting bias from average of gyro data
Gyrobias_x = mean(wGyro(:, 1));
Gyrobias_y = mean(wGyro(:, 2));
Gyrobias_z = mean(wGyro(:, 3));


%reminder: phi = x, theta = y, psi = z
%converting true euler to rads
phi = deg2rad(Eul(1,1));
theta = deg2rad(Eul(1,2));
psi = deg2rad(Eul(1,3));

for i = 1:length(Eul)
    %extracting each row or wGyro with each iterations of i
    gyro_row = wGyro(i, 1:3);
    %subracting bias from data and converting to rads
    gyro_row(1) = (deg2rad(gyro_row(1) - Gyrobias_x)/131);
    gyro_row(2) = (deg2rad(gyro_row(2) - Gyrobias_y)/131);
    gyro_row(3) = (deg2rad(gyro_row(3) - Gyrobias_z)/131);
    %forward integration
    Rplus = IntegrateOpenLoop(Rminus, gyro_row, deltaT);
    % euler angles
    phi = rad2deg(atan2(Rplus(2,3), Rplus(3,3)));
    theta = rad2deg(-asin(Rplus(1,3)));
    psi = rad2deg(atan2(Rplus(1,2),Rplus(1,1)));
    %foward integeration eul angles matrix
    eul_forward(i, 1:3) = [psi, theta, phi];
    Rminus = Rplus;
end

figure(2);
subplot(3,2,1);
plot(t_array, eul_forward(:,1))
hold on;
plot(t_array,Eul(:,1))
legend('Exponential', 'True');
title('Z Axis');

subplot(3,2,2);
plot(t_array, abs(eul_forward(:,1) - Eul(:,1)))
title('Z Axis Error');

subplot(3,2,3);
plot(t_array, eul_forward(:,2))
hold on;
plot(t_array,Eul(:,2))
legend('Exponential', 'True');
title('Y Axis');
subplot(3,2,4);
plot(t_array, abs(eul_forward(:,2) - Eul(:,2)))
title('Y Axis Error');


subplot(3,2,5);
plot(t_array, eul_forward(:,3))
hold on;
plot(t_array,Eul(:,3))
legend('Exponential', 'True');
title('X Axis');
subplot(3,2,6);
plot(t_array, abs(eul_forward(:,3) - Eul(:,3)))
title('X Axis Error');
