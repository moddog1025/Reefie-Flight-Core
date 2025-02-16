clc;
clear;
close all;

%% -------------------- PARAMETERS --------------------
F_avg          = 400;    % Average thrust (N)
F_max          = 500;    % Max thrust (N)
t_burn         = 10;      % Burn time (s)
p1             = 0.1;     % Percentage of burn time in phase 1 (ramp-up)
p2             = 0.1;     % Percentage of burn time in phase 2 (ramp-down to avg)
p3             = 0.7;     % Percentage of burn time in phase 3 (constant avg)
p4             = 0.1;     % Percentage of burn time in phase 4 (ramp-down to 0)
m              = 20;      % Mass (kg)
g              = 9.81;    % Gravity (m/s²)
drag_accel     = -10.5;   % Combined gravity + drag during coast (m/s²)
descent_main_v = 23;      % Descent velocity before disreef (m/s)
descent_slow_v = 5;       % Descent velocity after disreef (m/s)
disreef_alt    = 365;     % Disreef altitude (m)
dt             = 0.01;    % Time step for simulation (s)
poll_freq      = 20;      % Barometer poll frequency (Hz)

%% -------------------- TIME INTERVALS FOR THRUST --------------------
% Define phase endpoints
t1 = p1 * t_burn;              % End of ramp-up phase
t2 = t1 + p2 * t_burn;           % End of ramp-down to F_avg
t3 = t2 + p3 * t_burn;           % End of constant thrust phase
t4 = t_burn;                   % Motor burnout time

%% -------------------- POWERED ASCENT --------------------
t_ascent = 0:dt:t4;
F = zeros(size(t_ascent));

for i = 1:length(t_ascent)
    t_curr = t_ascent(i);
    if t_curr < t1
        % Phase 1: Ramp-up from 0 to F_max
        F(i) = (F_max/t1) * t_curr;
    elseif t_curr < t2
        % Phase 2: Ramp-down from F_max to F_avg
        F(i) = F_max - ((F_max - F_avg)/(t2-t1)) * (t_curr - t1);
    elseif t_curr < t3
        % Phase 3: Constant thrust at F_avg
        F(i) = F_avg;
    else
        % Phase 4: Ramp-down from F_avg to 0
        F(i) = F_avg - (F_avg/(t4-t3)) * (t_curr - t3);
    end
end

% Acceleration during powered ascent (thrust force minus gravity)
a_ascent = F/m - g;
% Integrate acceleration to get velocity and altitude
v_ascent = cumtrapz(t_ascent, a_ascent);
y_ascent = cumtrapz(t_ascent, v_ascent);

%% -------------------- COAST PHASE (AFTER MOTOR BURNOUT) --------------------
% Create a sufficiently long time vector for coast to capture apogee.
t_coast = t4:dt:500;  
a_coast = drag_accel * ones(size(t_coast)); % Constant deceleration (gravity + drag)
v_coast = v_ascent(end) + cumtrapz(t_coast, a_coast);
y_coast = y_ascent(end) + cumtrapz(t_coast, v_coast);

% Find apogee: first time when upward velocity drops to zero or below.
apogee_idx = find(v_coast <= 0, 1);
t_apogee = t_coast(apogee_idx);
y_apogee = y_coast(apogee_idx);

%% -------------------- MAIN DESCENT (Constant descent_main_v) --------------------
% Calculate the time when altitude reaches the disreef altitude.
t_disreef = t_apogee + (y_apogee - disreef_alt)/descent_main_v;
t_descent_main = t_apogee:dt:t_disreef;
y_descent_main = y_apogee - descent_main_v*(t_descent_main - t_apogee);

%% -------------------- FINAL DESCENT (Constant descent_slow_v) --------------------
% Calculate landing time (when altitude reaches 0).
t_ground = t_disreef + disreef_alt/descent_slow_v;
t_descent_slow = t_disreef:dt:t_ground;
y_descent_slow = disreef_alt - descent_slow_v*(t_descent_slow - t_disreef);

%% -------------------- FINAL DATA ARRAYS --------------------
% Use coast data up to apogee only.
t_coast_trim = t_coast(t_coast <= t_apogee);
y_coast_trim = y_coast(t_coast <= t_apogee);

% Concatenate all phases: Powered Ascent, Coast, Main Descent, and Final Descent.
t_final = [t_ascent, t_coast_trim, t_descent_main, t_descent_slow];
y_final = [y_ascent, y_coast_trim, y_descent_main, y_descent_slow];

% Remove any duplicate time values to ensure uniqueness for interpolation.
[t_final, uniqueIdx] = unique(t_final, 'stable');
y_final = y_final(uniqueIdx);

%% -------------------- BAROMETER SIMULATION & CSV OUTPUT --------------------
% Create a new time vector that simulates barometer readings at poll_freq.
t_poll = t_final(1):1/poll_freq:t_final(end);
% Interpolate the altitude to match the barometer's polling times.
y_poll = interp1(t_final, y_final, t_poll);

% Create a table with time and altitude data.
T = table(t_poll', y_poll', 'VariableNames', {'Time_s', 'Altitude_m'});
% Write the table to a CSV file.
writetable(T, 'altitude_data.csv');

%% -------------------- PLOT ALTITUDE vs TIME --------------------
figure;
plot(t_final, y_final, 'b', 'LineWidth', 2);
xlabel('Time (s)');
ylabel('Altitude (m)');
title('Rocket Altitude vs Time');
grid on;
hold on;

% Mark transition points
xline(t4, 'k--', 'Motor Burnout');
xline(t_apogee, 'r--', 'Apogee');
xline(t_disreef, 'g--', 'Disreef');
xline(t_ground, 'm--', 'Landing');

legend('Altitude Curve', 'Motor Burnout', 'Apogee', 'Disreef', 'Landing', 'Location', 'Best');
hold off;
