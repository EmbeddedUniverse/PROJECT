%%/Title: Program to study the voice recognition application for Embedded Universe Project
%%/Description: Takes multiple samples of the "PIOU" sounds 
%%/              and try to figure out which patterns are useful in the sounds

clear all
close all
clc
%% Constants folder path names
Piou = 'piouSamples\Piou.wav\';
Ppppiii = 'piouSamples\Ppppiii.wav\';
iiiou = 'piouSamples\iiiou.wav\';
ouuuu = 'piouSamples\ouuuu.wav\';

%% Parameters to change ***manually
nbSamples = 20;  %Depends on how many samples are analyzed

% Parameters just below can be changed for tweaks for analysis
freq_sep = 1000; % Minimum freq separation between main peaks (200) (100)
Npeaks = 5;    % Number of main peaks per sample to analyze (20)    (5)

%% Sample data load
[data, Fs] = loadAllWavSamples(nbSamples, Piou);
%% FFT
[L,~] = size(data);
plotArray = zeros(L,nbSamples*2-1);

for i = 1:nbSamples
    plotArray(:,2*i) = fft(data(:,i));
end

% Cutting a half of the data because of the symetrical result
plotArray = plotArray(1:L/2,:);
L = L/2;
f = Fs*(0:L-1)/L;

for i = 1:nbSamples
    plotArray(:,2*i-1) = plotArray(:,2*i-1) + f';
end

% Plot separately
for i = 1:nbSamples
    figure()
    plot(plotArray(:,2*i-1),plotArray(:,2*i))
    title('FFT of the chosen sound')
    xlabel('f(Hz)')
    ylabel('|X(f)|')
end

% Find specific peaks
abs_plotArray = plotArray;
for i = 1:nbSamples
    abs_plotArray(:,2*i) = abs(plotArray(:,2*i));
end

pkFreqArray = zeros(Npeaks*nbSamples,1);
for i = 1:nbSamples
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

