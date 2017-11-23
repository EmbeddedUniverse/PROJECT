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
xxx = changeFS(mtlb, Fs, samplingFrequency)';
fsAdjustedBad = xxx(1:sampleLength);

% Good Signals
[signals, inputFs] = loadAllWavSamples('../Ressources/AudioSample/');
nbGoodSample = size(signals,2);
fsAdjustedGood = zeros(nbGoodSample,sampleLength);

for i = 1:nbGoodSample
    x = changeFS( (signals(:, i))' , inputFs, samplingFrequency);
    fsAdjustedGood(i,1:size(x,2)) = x;
end

player = audioplayer(fsAdjustedBad(1,:), samplingFrequency);
play(player);

fileID = fopen('../Ressources/TestVoiceSamples.dat','w');
fprintf(fileID, '// This is an output of generateTestSignals.m\r\n\r\n');
fprintf(fileID, '#define %s %d\r\n', sampleLengthName, sampleLength);
fprintf(fileID, '#define %s %d\r\n', samplingFrequencyName, samplingFrequency);
fprintf(fileID, '#define %s %d\r\n', nbGoodSampleName, nbGoodSample);
fprintf(fileID, '#define %s %d\r\n', nbBadSampleName, nbBadSample);

% Good samples
fprintf(fileID, '\r\nfloat %s [%s][%s] = \r\n{\r\n', goodSampleName, nbGoodSampleName, sampleLengthName);
for i = 1:size(fsAdjustedGood, 1)
    fprintf(fileID, '\t{');
    for j = 1:(size(fsAdjustedGood, 2) - 1)
        fprintf(fileID, '%e, ', fsAdjustedGood(i,j));
    end
    fprintf(fileID, '%e},\r\n', fsAdjustedGood(i,size(fsAdjustedGood, 2)));
end
fprintf(fileID, '};\r\n');

% Bad Samples
fprintf(fileID, '\r\nfloat %s [%s][%s] = \r\n{\r\n', badSampleName, nbBadSampleName, sampleLengthName);
for i = 1:size(fsAdjustedBad, 1)
    fprintf(fileID, '\t{');
    for j = 1:(size(fsAdjustedBad, 2) - 1)
        fprintf(fileID, '%e, ', fsAdjustedBad(i,j));
    end
    fprintf(fileID, '%e},\r\n', fsAdjustedBad(i,size(fsAdjustedBad, 2)));
end
fprintf(fileID, '};\r\n');

fclose(fileID);