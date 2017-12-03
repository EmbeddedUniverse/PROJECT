clc
clear all
close all

buffSize = 8192;
subSize = 512;
overlap = 180;
tFs = 16000;

k = -0.5;

tonalEnd = round(subSize*(1000/tFs));
highEnd = round(subSize*(5000/tFs));

window = hamming(512)';
subC = floor((buffSize / (subSize-overlap)))-1;

sample = zeros(1,buffSize);
[signal, Fs] = audioread('Piou2.wav');

fsignal  = filter([1,k],1,signal);

nSignal = changeFS(fsignal, Fs, tFs);

sample(1:size(nSignal,1)) = nSignal;


subSamples = zeros(subC,subSize);
hammed = zeros(subC,subSize);
fftRes = zeros(subC,subSize/2+1);
total = zeros(subC,1);
bands = zeros(subC,3);
%p = 1, i = 2, ou = 3 , x= 0
phonem = zeros(subC,1);

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
    
    avg = mean(total);
    
    if total(index) < 0.10* total(1)
        phonem(index) = 0;
    elseif bands(index, 3) > 0.2
        phonem(index) = 1;
    elseif bands(index, 1) > 0.3
        if bands(index, 2) > 0.3
            phonem(index) = 2;
        else
            phonem(index) = 3;
        end
    else
        phonem(index) = 1;
    end
    
%     figure()
%     subplot(2,1,1)
%     plot(1:subSize, hammed(index,:))
%     subplot(2,1,2)
%     plot(tFs/subSize*(0:subSize/2), fftRes(index,:))
   
    index = index +1;
    start = start+subSize-overlap;
end

phonem'