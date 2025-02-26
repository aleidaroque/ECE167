clear all;
close all;
clc;

% Define inertial reference vectors
gravityInertial = [0; 0; -1]; % Earth's gravity vector in inertial frame
magneticFieldInertial = [1; 0; 0]; % Earth's magnetic field vector in inertial frame

% Simulation parameters
deltaT = 0.1; % Time step in seconds
[accelerometerData, magnetometerData, gyroData, trueEulerAngles] = CreateTrajectoryData(deltaT, 1); % Generate simulated sensor data

%save("simulated_data_p5.mat");
%load('simulated_data_p5.mat');


timeVector = 0:deltaT:(length(gyroData) - 1) * deltaT; % Time vector for plotting

% Calculate initial gyro bias as the mean of the first second of data
gyroBias = mean(gyroData(1:1 / deltaT, :), 1);

% Initialize the rotation matrix to identity (no rotation)
rotationMatrix = eye(3);

% Initialize storage for estimated Euler angles
estimatedEulerAngles = zeros(length(trueEulerAngles), 3);

% Process each data point to estimate Euler angles
for i = 1:length(trueEulerAngles)
    % Adjust gyro readings based on bias and scale factor
    gyroReading = deg2rad(gyroData(i, :) - gyroBias) / 131;
    
    % Update rotation matrix and gyro bias using the closed-loop integrator
    [rotationMatrix, gyroBias] = IntegrateClosedLoop(rotationMatrix, gyroBias, gyroReading, ...
                                                     magnetometerData, accelerometerData, ...
                                                     magneticFieldInertial, gravityInertial, deltaT);
    
    % Convert rotation matrix to Euler angles
    pitch = rad2deg(-asin(rotationMatrix(1, 3)));
    roll = rad2deg(atan2(rotationMatrix(2, 3), rotationMatrix(3, 3)));
    yaw = rad2deg(atan2(rotationMatrix(1, 2), rotationMatrix(1, 1)));
    estimatedEulerAngles(i, :) = [yaw, pitch, roll];
end

% Plotting the estimated Euler angles
figure
plot(timeVector, estimatedEulerAngles(:, 1), 'b')
hold on; 

plot(timeVector, estimatedEulerAngles(:, 2), 'r')
hold on; 

plot(timeVector, estimatedEulerAngles(:, 3), 'g')
title('Euler Angle Closed Loop');
grid on;
legend('Yaw (Z axis)', 'Pitch (Y axis)', 'Roll (X axis)');

% Pretty plot of attitude data function
PrettyPlotAttitudeData(deltaT, accelerometerData, magnetometerData, gyroData, estimatedEulerAngles)
