%%/Title: Program to study the voice recognition application for Embedded Universe Project
%%/Description: Takes multiple samples of the "PIOU" sounds 
%%/              and try to figure out which patterns are useful in the sounds

clear all
close all
clc

%% Parameters to change ***manually
Nb_sample = 9;  %Depends on how many samples are analyzed
L = 705530;       %Depends on the data sample having the most of data

% Parameters just below can be changed for tweaks for analysis
freq_sep = 1000; % Minimum freq separation between main peaks (200) (100)
Npeaks = 5;    % Number of main peaks per sample to analyze (20)    (5)

%% Sample data load ***(Add an audioread and a padarray per sample)
% Loading
[data1,Fs] = audioread('piouSamples\Piou1.wav\Piou1.wav');
[data2,Fs] = audioread('piouSamples\Piou2.wav\Piou2.wav');
[data3,Fs] = audioread('piouSamples\Piou3.wav\Piou3.wav');
%[data4,Fs] = audioread('piouSamples\Piou4.wav\Piou4.wav');
%[data5,Fs] = audioread('piouSamples\Piou5.wav\Piou5.wav');
[data6,Fs] = audioread('piouSamples\Piou6.wav\Piou6.wav');
[data7,Fs] = audioread('piouSamples\Piou7.wav\Piou7.wav');
[data8,Fs] = audioread('piouSamples\Piou8.wav\Piou8.wav');
[data9,Fs] = audioread('piouSamples\Piou9.wav\Piou9.wav');
[data10,Fs] = audioread('piouSamples\Piou10.wav\Piou10.wav');
[data11,Fs] = audioread('piouSamples\Piou11.wav\Piou11.wav');
[data12,Fs] = audioread('piouSamples\Piou12.wav\Piou12.wav');
[data13,Fs] = audioread('piouSamples\Piou13.wav\Piou13.wav');
[data14,Fs] = audioread('piouSamples\Piou14.wav\Piou14.wav');
[data15,Fs] = audioread('piouSamples\Piou15.wav\Piou15.wav');
[data16,Fs] = audioread('piouSamples\Piou16.wav\Piou16.wav');
[data17,Fs] = audioread('piouSamples\Piou17.wav\Piou17.wav');
[data18,Fs] = audioread('piouSamples\Piou18.wav\Piou18.wav');
[data19,Fs] = audioread('piouSamples\Piou19.wav\Piou19.wav');
[data20,Fs] = audioread('piouSamples\Piou20.wav\Piou20.wav');
[data21,Fs] = audioread('piouSamples\Piou21.wav\Piou21.wav');
[data22,Fs] = audioread('piouSamples\Piou22.wav\Piou22.wav');

%Padding zeros
data1 = padarray(data1,L-length(data1),'post');
data2 = padarray(data2,L-length(data2),'post');
data3 = padarray(data3,L-length(data3),'post');
% data4 = padarray(data4,L-length(data4),'post');
%data5 = padarray(data5,L-length(data5),'post');
data6 = padarray(data6,L-length(data6),'post');
data7 = padarray(data7,L-length(data7),'post');
data8 = padarray(data8,L-length(data8),'post');
data9 = padarray(data9,L-length(data9),'post');
data10 = padarray(data10,L-length(data10),'post');
data11 = padarray(data11,L-length(data11),'post');
data12 = padarray(data12,L-length(data12),'post');
data13 = padarray(data13,L-length(data13),'post');
data14 = padarray(data14,L-length(data14),'post');
data15 = padarray(data15,L-length(data15),'post');
data16 = padarray(data16,L-length(data16),'post');
data17 = padarray(data17,L-length(data17),'post');
data18 = padarray(data18,L-length(data18),'post');
data19 = padarray(data19,L-length(data19),'post');
data20 = padarray(data20,L-length(data20),'post');
data21 = padarray(data21,L-length(data21),'post');
data22 = padarray(data22,L-length(data22),'post');


%% Array organization for analysis ***(Add an array of data per sample)
tab = [data1 data2 data3 data6 data7 data8 data9 data10 data11 ];

%% FFT
plotArray = zeros(L,Nb_sample*2);

for i = 1:Nb_sample
    plotArray(:,2*i) = fft(tab(:,i));
end

% Cutting a half of the data because of the symetrical result
plotArray = plotArray(1:L/2,:);
L = L/2;
f = Fs*(0:L-1)/L;

for i = 1:Nb_sample
    plotArray(:,2*i-1) = plotArray(:,2*i-1) + f';
end

% Plot separately
for i = 1:Nb_sample
    figure()
    plot(plotArray(:,2*i-1),plotArray(:,2*i))
    title('FFT of the chosen sound')
    xlabel('f(Hz)')
    ylabel('|X(f)|')
end

% Find specific peaks
abs_plotArray = plotArray;
for i = 1:Nb_sample
    abs_plotArray(:,2*i) = abs(plotArray(:,2*i));
end

pkFreqArray = zeros(Npeaks*Nb_sample,1);
for i = 1:Nb_sample
    figure()
    findpeaks(abs_plotArray(:,2*i),abs_plotArray(:,2*i-1),'MinPeakDistance',freq_sep,'SortStr','descend','NPeaks',Npeaks)
    
    [pk,pkloc] = findpeaks(abs_plotArray(:,2*i),abs_plotArray(:,2*i-1),'MinPeakDistance',freq_sep,'SortStr','descend','NPeaks',Npeaks);
    x = i*Npeaks-(Npeaks-1);
    y = i*Npeaks;
    pkFreqArray(x:y) = pkloc';
end

% Sort the frequencies of the main peaks of all the chosen samples
pkFreqArray = sort(pkFreqArray,'descend');

% Show on a graph an approximate of a potential bandpass for the selected
% fragment of the sound "PIOU"
figure()
stem(pkFreqArray,ones(length(pkFreqArray),1),'r')
ylim([0 2])

