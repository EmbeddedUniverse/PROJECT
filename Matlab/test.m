% test.m

% This file is use to test algorithms (functions) of all sort in Matlab
% before an implementation on an another platform. 

%% Test - Autocorrelation matlab's function
clear all
close all
clc
% Constant(s)
number_of_data_input = 20;

% Maximal
x1 = ones(number_of_data_input,1);

% Minimal
x2 = zeros(number_of_data_input,1);

% Nominal
x3 = rand(number_of_data_input,1);

% Test
R1 = xcorr(x1);
R2 = xcorr(x2);
R3 = xcorr(x3);

k = -(number_of_data_input-1):(number_of_data_input-1);

figure()
subplot(3,1,1)
plot(k , R1)
subplot(3,1,2)
plot(k , R2)
subplot(3,1,3)
plot(k , R3)

%% Test - Autocorrelation recoded
clear all
close all
clc
% Constant(s)
number_of_data_input = 20;

% Maximal
x1 = ones(number_of_data_input,1);

% Minimal
x2 = zeros(number_of_data_input,1);

% Nominal
x3 = rand(number_of_data_input,1);

% Test
R1 = autocorrelate(x1);
R2 = autocorrelate(x2);
R3 = autocorrelate(x3);

k = -(number_of_data_input-1):(number_of_data_input-1);

figure()
subplot(3,1,1)
plot(k , R1)
subplot(3,1,2)
plot(k , R2)
subplot(3,1,3)
plot(k , R3)

%% Test - Autocorrelation functions comparison (Matlab and recoded version)
clear all
close all
clc
% Constant(s)
number_of_data_input = 20;

% Maximal
x1 = ones(number_of_data_input,1);

% Minimal
x2 = zeros(number_of_data_input,1);

% Nominal
x3 = rand(number_of_data_input,1);

% Test
R1_xcorr = xcorr(x1);
R2_xcorr = xcorr(x2);
R3_xcorr = xcorr(x3);

R1_recoded = autocorrelate(x1);
R2_recoded = autocorrelate(x2);
R3_recoded = autocorrelate(x3);

k = -(number_of_data_input-1):(number_of_data_input-1);

figure()
subplot(3,1,1)
plot(k , R1_xcorr, 'r', k , R1_recoded, 'b--o')
subplot(3,1,2)
plot(k , R2_xcorr, 'r', k , R2_recoded, 'b--o')
subplot(3,1,3)
plot(k , R3_xcorr, 'r', k , R3_recoded, 'b--o')