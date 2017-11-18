%%/Title: Program to study the voice recognition application for Embedded Universe Project
%%/Description: Takes multiple samples of the "PIOU" sounds 
%%/              and try to figure out which patterns are useful in the sounds

clear all
close all
clc

%% Parameters to change ***manually
% Parameters just below can be changed for analysis's tweaks
freq_sep = 1000; % Minimum freq separation between main peaks (200) (100)
nPeaks = 5;    % Number of main peaks per sample to analyze (20)    (5)

%% Constants folder path names
Piou = 'piouSamples\CleanedPiou.wav\';
Ppppiii = 'piouSamples\Ppppiii.wav\';
iiiou = 'piouSamples\iiiou.wav\';
ouuuu = 'piouSamples\ouuuu.wav\';

%% Sample data load
[data, Fs] = loadAllWavSamples(Piou);

%% FFT
[L,nbSamples] = size(data);
fftArray = zeros(L,nbSamples*2); %Preallocation for speed

for i = 1:nbSamples
    fftArray(:,2*i) = fft(data(:,i));
end

% Cutting a half of the data because of the symetrical result
fftArray = fftArray(1:L/2,:);
L = L/2;
f = Fs*(0:L-1)/L;

for i = 1:nbSamples
    fftArray(:,2*i-1) = fftArray(:,2*i-1) + f';
end

%% Plot separately (Uncomment to see each sample's FFT)
% for i = 1:nbSamples
%     figure()
%     plot(fftArray(:,2*i-1),fftArray(:,2*i))
%     title('FFT of the chosen sound')
%     xlabel('f(Hz)')
%     ylabel('|X(f)|')
% end

%% Find specific maximum peaks (Only the harmonics strong enough in magnitude  for recognition)
absFFTArray = fftArray;
for i = 1:nbSamples
    absFFTArray(:,2*i) = abs(fftArray(:,2*i));
end

pkFreqArray = zeros(nPeaks*nbSamples,1); %Preallocation for speed
pkFreqArraySamples = zeros(nPeaks,nbSamples); %Preallocation for speed
for i = 1:nbSamples
    %Uncomment to see each main peaks of the main harmonics of each sample
%     figure()
%     findpeaks(absFFTArray(:,2*i),absFFTArray(:,2*i-1),'MinPeakDistance',freq_sep,'SortStr','descend','NPeaks',nPeaks)
    
    [pk,pkloc] = findpeaks(absFFTArray(:,2*i),absFFTArray(:,2*i-1),'MinPeakDistance',freq_sep,'SortStr','descend','NPeaks',nPeaks);
    x = i*nPeaks-(nPeaks-1);
    y = i*nPeaks;
    pkFreqArray(x:y) = pkloc';
    pkFreqArraySamples(:,i) = pkFreqArray(x:y);
end

% Sort the frequencies of the main peaks of all the chosen samples
pkFreqArray = sort(pkFreqArray,'descend');

% Show on a graph an approximate of a potential bandpass for the selected
% fragment of the sound "PIOU"
% Uncomment to see the main peaks stem graph of each samplein same colour
% figure()
% stem(pkFreqArray,ones(length(pkFreqArray),1),'r')
% ylim([0 2])

%% Find specific maximum peaks with different colour code for each sample
figure()
stem(pkFreqArraySamples')
title('Main harmonics of each sample')
xlabel('Nth sample')
ylabel('Frequency (Hz)')


