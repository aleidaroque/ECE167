clear all
close all




run("ACC_readings.m")

meanXpos = mean(posX);
meanXneg = mean(negX);

meanYpos = mean(posY);
meanYneg = mean(negY);

meanZpos = mean(posZ);
meanZneg = mean(negZ);

aX = [meanXpos(1) 1; meanXneg(1) 1];
bX = [1; -1];

a_X = aX\bX
% scale: 0.0010, bias: -0.0134

aY = [meanYpos(2) 1; meanYneg(2) 1];
bY = [1; -1];

a_Y = aY\bY
% scale: 0.0010, bias: 0.0879

aZ = [meanZpos(3) 1; meanZneg(3) 1];
bZ = [1; -1];

a_Z = aZ\bZ
% scale: 0.0010, bias: 0.0091

run("tumble_readings.m");

x_acc = (0.0010)*(tumble_acc(:, 1)) - 0.0134;
y_acc = (0.0010)*(tumble_acc(:, 2)) + 0.0879;
z_acc = (0.0010)*(tumble_acc(:, 3)) + 0.0091;

% uncalibrated plot
figure(1);
scatter3(tumble_acc(:, 1), tumble_acc(:, 2), tumble_acc(:, 3));
title('Accelerometer: Uncalibrated');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;

% calibrated plot
figure(2);
scatter3(x_acc, y_acc, z_acc);
title('Accelerometer: Calibrated');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

run("MAG_readings.m")

meanXmax = mean(maxX);
meanXmin = mean(minX);

meanYmax = mean(maxY);
meanYmin = mean(minY);

meanZmax = mean(maxZ);
meanZmin = mean(minZ);

aX = [meanXmax(1) 1; meanXmin(1) 1];
bX = [1; -1];

m_X = aX\bX
% scale: 0.0018, bias: -0.8823

aY = [meanYmax(2) 1; meanYmin(2) 1];
bY = [1; -1];

m_Y = aY\bY
% scale: 0.0017, bias: -0.4828

aZ = [meanZmax(3) 1; meanZmin(3) 1];
bZ = [1; -1];

m_Z = aZ\bZ
% scale: 0.0017, bias: -0.8729


x_mag = (0.0018)*(tumble_mag(:, 1)) - 0.8823;
y_mag = (0.0017)*(tumble_mag(:, 2)) - 0.4828;
z_mag = (0.0017)*(tumble_mag(:, 3)) - 0.8729;

% uncalibrated plot
figure(3);
scatter3(tumble_mag(:, 1), tumble_mag(:, 2), tumble_mag(:, 3));
title('Magnetometer: Uncalibrated');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;

% calibrated plot
figure(4);
scatter3(x_mag, y_mag, z_mag);
title('Magnetometer: Calibrated');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;
