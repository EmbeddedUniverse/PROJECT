%%/Title:       voiceRecognitionStudy
%%/Description: Program to study the voice recognition application for Embedded Universe Project
%%/             Takes multiple samples of the "PIOU" sounds and try to figure out which patterns
%%/             are useful in the sounds

clear all
close all
clc

%% Parameters to change ***manually
% Parameters just below can be changed for analysis's tweaks
freq_sep = 300; % Minimum freq separation between main peaks (200) (100)
nPeaks = 20;    % Number of main peaks per sample to analyze (20)    (5)

%Place helpful visual lines to check if points are between them in figure 1
lowLimit =6800;
highLimit = 7800; 

%% Constants folder path names
Piou = 'piouSamples\CleanedPiou.wav\';
Ppppiii = 'piouSamples\Ppppiii.wav\';
iiiou = 'piouSamples\iiiou.wav\';
ouuuu = 'piouSamples\ouuuu.wav\';

%% Sample data load
[data, Fs] = loadAllWavSamples(Ppppiii);

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
hold on
plot(1:1:length(pkFreqArraySamples),ones(length(pkFreqArraySamples),1)*lowLimit,'r',1:1:length(pkFreqArraySamples),ones(length(pkFreqArraySamples),1)*highLimit,'r')
stem(pkFreqArraySamples')
title(strcat('Main harmonics of each sample (',num2str(nPeaks),' peaks, ',num2str(freq_sep),'Hz of seperation for each peaks)'))
xlabel('Nth sample')
ylabel('Frequency (Hz)')
hold off


figure()
hold on
for i = 1:length(pkFreqArraySamples)
    stem(pkFreqArraySamples(:,i),ones(length(pkFreqArraySamples(:,i)),1))
end
title(strcat('Main harmonics with different colour for each sample (',num2str(nPeaks),' peaks, ',num2str(freq_sep),'Hz of seperation for each peaks)'))
xlabel('Frequency (Hz)')
ylabel('Magnitude')
ylim([0 2])
hold off

%Calculate the mean and the standard deviation of the harmonics per
%separation band (see variable's value of "freq_sep" at the top of the script)
meanPkFreqArraySamples = zeros(i,1);
stdPkFreqArraySamples = zeros(i,1);

% Same but tries with a logarithmic representation (base 10)
logMeanPkFreqArraySamples = zeros(i,1);
logStdPkFreqArraySamples = zeros(i,1);

for i = 1:nPeaks
    meanPkFreqArraySamples(i) = mean(pkFreqArraySamples(i,:));
    stdPkFreqArraySamples(i) = std(pkFreqArraySamples(i,:));
    
    logMeanPkFreqArraySamples(i) = mean(log10(pkFreqArraySamples(i,:)));
    logStdPkFreqArraySamples(i) = std(log10(pkFreqArraySamples(i,:)));
end