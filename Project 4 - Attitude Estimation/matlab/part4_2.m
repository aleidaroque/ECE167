clear all;
close all;

run("lab4tumbledata.m");

deltaT = 0.1; %different delta than d
%t_array = 0:deltaT:(length(Eul)-1)*deltaT;
theta = 0;
psi = 0;
phi = 0;

% creating rotation matrix

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
Gyrobias_x = mean(tensec_data(:, 1));
Gyrobias_y = mean(tensec_data(:, 2));
Gyrobias_z = mean(tensec_data(:, 3));


%reminder: phi = x, theta = y, psi = z
%converting true euler to rads

for i = 1:length(tenmin_data)
    %extracting each row or wGyro with each iterations of i
    gyro_row = tenmin_data(i, 1:3);
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
    fprintf("phi: %f, theta: %f, psi: %f\n", phi, theta, psi);
    Rminus = Rplus;
end

fprintf("Z Bias: %f ", Gyrobias_z);
fprintf("Y Bias: %f ", Gyrobias_y);
fprintf("X Bias: %f ", Gyrobias_x);