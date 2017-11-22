clc
clear all
close all

goodSampleName = 'TEST_PEW_GOOD';
badSampleName = 'TEST_PEW_BAD';

signalLength = 8192;
samplingFrequency = 16000;

% BadSignal
load mtlb

[signal, inputFs] = audioread('../Ressources/AudioSample/classicPew.wav');


fsAdjustedGood = changeFS(signal, inputFs, samplingFrequency);
fsAdjustedBad = changeFS(mtlb, Fs, samplingFrequency);

alignedGood = alignSignal(fsAdjustedGood);