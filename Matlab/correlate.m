    function [ R ] = correlate( x, refSignal )
% Function : autocorrelate 
% Input : entry signal x and refSignal
% Output : vector R (result of the correlation)
%   This function autocorrelate a signal with itself. It's made to
%   recognize a pattern repeated in the signal.


length_x = length(x);
length_refSignal = length(refSignal);

if length_x < length_refSignal
   x_norm = [x zeros((length_refSignal-length_x),1)]
   refSignal_norm = refSignal;
elseif length_x > length_refSignal
   refSignal_norm = [refSignal, zeros(1,(length_x-length_refSignal))];
   x_norm = x;
else
   x_norm = x;
   refSignal_norm = refSignal;
end

length_signal = (length(refSignal_norm));

R = zeros(1, 2*length_signal-1);
temp = zeros(length_signal, length(R));

i=1;
while i<= length_signal && i < 500
        temp(i,:)=[zeros(1, i-1),x_norm(i)*fliplr(refSignal_norm),zeros(1,length_signal-i)];
        R = R+temp(i,:);
        i = i+1;
end

