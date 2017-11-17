%% Convert for data manipulation in another software (.m4 to .wav) (Audacity, for instance)

clear all
close all
clc

% Sample data load
[data,Fs] = audioread('piouSamples\Piou.m4a\Piou22.m4a');

% Sample data write
audiowrite('piouSamples\Piou22.wav\Piou22.wav',data,Fs);
