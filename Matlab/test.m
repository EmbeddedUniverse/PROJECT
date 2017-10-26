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
x1 = ones(1,number_of_data_input);

% Minimal
x2 = 2*ones(1,number_of_data_input-4);

% Nominal
x3 = rand(1,number_of_data_input);

% Test
R1 = correlate(x1,x1);
R2 = correlate(x2,x2);
R3 = correlate(x3,x3);


figure()
subplot(3,1,1)
plot(1:39,R1)
subplot(3,1,2)
plot(1:31,R2)
subplot(3,1,3)
plot(1:39,R3)

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

R1_recoded = correlate(x1',x1');
R2_recoded = correlate(x2',x2');
R3_recoded = correlate(x3',x3');

k = -(number_of_data_input-1):(number_of_data_input-1);

figure()
subplot(3,1,1)
plot(k , R1_xcorr, 'r', k , R1_recoded, 'b--o')
title('Autocorrelation with constant pulse input value')
xlabel('k delay')
ylabel('Correlation')
legend('Matlab','Recoded')
subplot(3,1,2)
plot(k , R2_xcorr, 'r', k , R2_recoded, 'b--o')
title('Autocorrelation with minimal input value')
xlabel('k delay')
ylabel('Correlation')
legend('Matlab','Recoded')
subplot(3,1,3)
plot(k , R3_xcorr, 'r', k , R3_recoded, 'b--o')
title('Autocorrelation with random input values')
xlabel('k delay')
ylabel('Correlation')
legend('Matlab','Recoded')

%% Test - Autocorrelation functions comparison (Matlab and recoded-Matlab and recoded-C version)
clear all
close all
clc

formatSpec = '%f';

pre_autocorrelation_data_C_file = fopen('pre_autocorrelation_data_C.txt','r');
pre_autocorrelation_data_C = fscanf(pre_autocorrelation_data_C_file,formatSpec);

autocorrelation_results_C_file = fopen('autocorrelation_results_C.txt','r');
autocorrelation_results_C = fscanf(autocorrelation_results_C_file,formatSpec);

fclose(pre_autocorrelation_data_C_file);
fclose(autocorrelation_results_C_file);

% Number of signal data in input
number_of_data_input = length(pre_autocorrelation_data_C);

% Test
R_xcorr = xcorr(pre_autocorrelation_data_C);

R_recoded = autocorrelate(pre_autocorrelation_data_C);

R_C = autocorrelation_results_C;

k = -(number_of_data_input-1):(number_of_data_input-1);

figure()
plot(k , R_xcorr, 'r', k , R_recoded, 'b--o', k , R_C, 'g--x')
title('Autocorrelation comparison with Matlab and C algorythms')
xlabel('k delay')
ylabel('Correlation')
legend('Matlab','Recoded', 'C')