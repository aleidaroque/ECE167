clear all
close all

run("EllipseXYData.m")

% length of X data
samples = length(Xmeas);

% Construct the A matrix
A = [Xmeas Ymeas.^2 Ymeas ones(samples, 1)];

% Construct the b vector
b = -Xmeas.^2;

% Solve for z
z = A \ b;
% resulting matrix:
% -0.7923
%  0.2658
%  0.1801
% -1.0349

% Extract the parameters
a = 1.10558;
b = 2.14445;
x_0 = 0.39615;
y_0 = -0.33878;

% Calculate the calibrated X and Y data
calibrated_X = 1/a.*(Xmeas - x_0);
calibrated_Y = 1/b.*(Ymeas - y_0);

% Plot the original data
figure;
plot(Xmeas, Ymeas, 'ro'); 
hold on;

% Plot the calibrated data
plot(calibrated_X, calibrated_Y, 'bo');

% Add legend and labels
xlim([-3 3]);
ylim([-3 3]);
legend('Original Data', 'Calibrated Data');
xlabel('X data');
ylabel('Y data');
title('Least Squares Data Calibration');