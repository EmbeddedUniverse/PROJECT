clc
clear all
close all

targetFs = 16000;

load mtlb %Non Piou Signal
[P,Q] = rat(targetFs / Fs);
xmtlb = resample(mtlb, P, Q);
[signal, inputFs] = audioread('PiouTest.wav');


pla = audioplayer(xmtlb, targetFs);
plah = audioplayer(mtlb, Fs);
play(pla);