clc
clear all
close all

targetFs = 16000;
segmentLength = 256;
segmentOverlap = 100;
sumThreshold = 1;
firK = -0.97;

load mtlb %Non Piou Signal
cmtlb = changeFS(mtlb, Fs, targetFs);
[signal, inputFs] = audioread('PiouTest.wav');
cSignal = changeFS(signal, inputFs, targetFs);

fSignal = filter([1,firK], 1, cSignal); % Small FIR filter to help amplify high frequency

figure(1);
subplot(2,1,1)
plot(0:size(cSignal,1)-1, cSignal);
subplot(2,1,2)
plot(0:size(cSignal,1)-1, fSignal);
title('Effect of FIR filter');
% pla = audioplayer(fSignal, targetFs);
% play(pla);


% Segmentation
segments = zeros(floor(size(fSignal,1)/(segmentLength-segmentOverlap))-1,segmentLength);
inputArrayIndex = 1;
for i = 1:size(segments,1)
    
    segments(i, :) = fSignal(inputArrayIndex:inputArrayIndex+segmentLength-1)';
    
    inputArrayIndex = inputArrayIndex + segmentLength - segmentOverlap;
end

% Selecting segments to process
first = 0;
for i = 1:size(segments,1)
    if (sum(abs(segments(i, :))) > sumThreshold)
        first = i;
        break
    end
end
last = size(segments,1);
for i = first+1:size(segments,1)-1
    if (sum(abs(segments(i, :))) < sumThreshold)
        last = i-1;
        break
    end
end
chosenSegments= segments(first:last,:);

% Applying windows to the hamming windows to the segments
window = hamming(segmentLength)';
wSegments = zeros(size(chosenSegments));
for i = 1:size(chosenSegments,1)
    wSegments(i,:) = chosenSegments(i,:).*window;
end

% m = abs(fft(wSegments(8,:)));
% plot(1:256, m)

% Saving hlf