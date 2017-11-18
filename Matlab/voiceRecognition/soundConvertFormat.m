%%/Title: Program to convert.m4a files into uncleaned .wav files for
%%/Embedded Universe's voice recognition project
%%/Description: Takes multiple samples of the "PIOU" sounds 
%%/              and convert into uncleaned .wav files for audacity
clear all
close all
clc

% Verifying each .m4a file reachable to convert into .wav
listing = dir('piouSamples\Piou.m4a');

% Sample data load
for i = 1:length(listing(3:end))
    [data,Fs] = audioread(strcat('piouSamples\Piou.m4a\',listing(i+2).name));
    
    % Sample data write
    audiowrite(strcat('piouSamples\Piou.wav\Piou',num2str(i),'.wav'),data,Fs);
end