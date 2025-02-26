% part 4_2

clear all
close all

run("tumble_readings.m")



AscaleFactor = 0.5*(2^15-1);        % conversion of g's to bits
HscaleFactor = 1/0.15;              % uT/bits

A = tumble_acc;
M = tumble_mag;

A_scaled = A/AscaleFactor;
M_scaled = M * HscaleFactor;

a_x = A_scaled(:, 1);
a_y = A_scaled(:, 2);
a_z = A_scaled(:, 3);

m_x = M_scaled(:, 1);
m_y = M_scaled(:, 2);
m_z = M_scaled(:, 3);

kstep = 5;

plotFlag = 0;

%% calibrating data

[Atilde_A,Btilde_A] = CalibrateEllipsoidData3D(a_x, a_y, a_z, kstep, plotFlag);

[Atilde_M,Btilde_M] = CalibrateEllipsoidData3D(m_x, m_y, m_z, kstep, plotFlag);


%% correcting data

[Xcorr_A, Ycorr_A, Zcorr_A] = CorrectEllipsoidData3D(a_x, a_y, a_z, Atilde_A, Btilde_A);

[Xcorr_M, Ycorr_M, Zcorr_M] = CorrectEllipsoidData3D(m_x, m_y, m_z, Atilde_M, Btilde_M);

%% plotting data 

figure(1);
scatter3(a_x, a_y, a_z);
hold on;
scatter3(Xcorr_A, Ycorr_A, Zcorr_A);
%hold on;
legend('Pre-Calibration', 'Post-Calibration');
xlabel('X');
ylabel('Y');
zlabel('Z');
title('Accelerometer');
grid on;


figure(2);
scatter3(m_x, m_y, m_z);
hold on;
scatter3(Xcorr_M, Ycorr_M, Zcorr_M);
legend('Pre-Calibration', 'Post-Calibration');
xlabel('X');
ylabel('Y');
zlabel('Z');
title('Magnetometer')
grid on;




