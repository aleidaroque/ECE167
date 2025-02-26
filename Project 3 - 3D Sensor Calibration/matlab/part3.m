clear all
close all

run("GYR_readings.m")

%% 10s READING

bias_10s = mean(r_10s)

%% 10m READING

% get minimum and maximum values

maxX = max(r_10m(:, 1));
minX = min(r_10m(:, 1));

maxY = max(r_10m(:, 2));
minY = min(r_10m(:, 2));

maxZ = max(r_10m(:, 3));
minZ = min(r_10m(:, 3));

% Initial estimate of the bias for each axis

aX = [maxX(1) 1; minX(1) 1];
bX = [1; -1];

X = aX\bX
% scale: 0.0039, bias: 0.0020

aY = [maxY(1) 1; minY(1) 1];
bY = [1; -1];

Y = aY\bY
% scale: 0.0039, bias: 0.0020

aZ = [maxZ(1) 1; minZ(1) 1];
bZ = [1; -1];

Z = aZ\bZ
% scale: 0.0039, bias: 0.0020


% subtract biases from each axis
drift_bias = r_10m - bias_10s;

% time axis
t = 100E-3; % 100 ms per sample
samples = length(r_10m);
time = 0:t:(samples-1)*t;

drift = cumtrapz(time, drift_bias);

% plotting gyroscope data
figure;

plot(time, drift(:, 1), 'r');
hold on;
plot(time, drift(:, 2), 'g');
hold on;
plot(time, drift(:, 3), 'b');


xlabel('Time');
ylabel('Degrees');
legend('X', 'Y', 'Z');
title('Gyroscope Bias vs Time');
grid on;

