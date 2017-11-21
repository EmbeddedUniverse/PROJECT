function [ adjustedSignal ] = changeFS( original, Fs, newFs )
%CHANGEFS Summary of this function goes here
%   Detailed explanation goes here
[P,Q] = rat(newFs / Fs);
adjustedSignal = resample(original, P, Q);

end

