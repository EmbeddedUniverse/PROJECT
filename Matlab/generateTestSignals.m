clc
clear all
close all

addpath(genpath('voiceRecognition'));

goodSampleName = 'TEST_PEW_GOOD';
nbGoodSampleName = 'NB_TEST_PEW_GOOD';
badSampleName = 'TEST_PEW_BAD';
nbBadSampleName = 'NB_TEST_PEW_BAD';
nbBadSample = 1;

sampleLengthName = 'TEST_SAMPLE_LENGTH';
sampleLength = 8192;
samplingFrequencyName = 'TEST_SAMPLE_FREQUENCY';
samplingFrequency = 16000;

% BadSignal
load mtlb
fsAdjustedBad = changeFS(mtlb, Fs, samplingFrequency);

% Good Signals
[signals, inputFs] = loadAllWavSamples('../Ressources/AudioSample/');
nbGoodSample = size(signals,2);
fsAdjustedGood = zeros(nbGoodSample,sampleLength);

for i = 1:nbGoodSample
    x = changeFS( (signals(:, i))' , inputFs, samplingFrequency);
    fsAdjustedGood(i,1:size(x,2)) = x;
end

% player = audioplayer(fsAdjustedGood(5,:), samplingFrequency);
% play(player);

fileID = fopen('../Ressources/TestVoiceSamples.dat','w');
fprintf(fileID, '#define %s %d\r\n', sampleLengthName, sampleLength);
fprintf(fileID, '#define %s %d\r\n', samplingFrequencyName, samplingFrequency);
fprintf(fileID, '#define %s %d\r\n', nbGoodSampleName, nbGoodSample);
fprintf(fileID, '#define %s %d\r\n', nbBadSampleName, nbBadSample);

% Good samples
fprintf(fileID, '\r\nfloat %s [%s][%s] = \r\n{\r\n', goodSampleName, nbGoodSampleName, sampleLengthName);

fclose(fileID);