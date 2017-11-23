clc
clear all
close all

buffSize = 8192;
subSize = 512;
overlap = 150;
tFs = 16000;

tonalEnd = round(subSize*(1000/tFs));
highEnd = round(subSize*(5000/tFs));

window = hamming(512)';
subC = floor((buffSize / (subSize-overlap)))-1;

sample = zeros(1,buffSize);
[signal, Fs] = audioread('Piou1.wav');

nSignal = changeFS(signal, Fs, tFs);

sample(1:size(nSignal,1)) = nSignal;


subSamples = zeros(subC,subSize);
hammed = zeros(subC,subSize);
fftRes = zeros(subC,subSize/2+1);
total = zeros(subC,1);
bands = zeros(subC,3);

index = 1;
start = 1;
while index <= subC
    subSamples(index,:) = sample(start:start+subSize-1);
    
    hammed(index,:) = subSamples(index,:) .* window;
    
    ffiou = fft(hammed(index,:));
    fftRes(index,:) = abs(ffiou(1:((subSize/2)+1)));
    
    total(index,1) = sum(fftRes(index,:));
    
    %Tonal frequencies associated with open mouth speach
    bands(index,1) = sum(fftRes(index,1:tonalEnd)) /  total(index,1);
    bands(index,2) = sum(fftRes(index,tonalEnd:highEnd)) /  total(index,1);
    bands(index,3) = sum(fftRes(index,highEnd:end)) /  total(index,1);
    
    
    figure()
    subplot(2,1,1)
    plot(1:subSize, hammed(index,:))
    subplot(2,1,2)
    plot(tFs/subSize*(0:subSize/2), fftRes(index,:))
   
    index = index +1;
    start = start+subSize-overlap;
end