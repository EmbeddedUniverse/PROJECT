function [ R ] = autocorrelate( x )
% Function : autocorrelate 
% Input : entry signal x 
% Output : vector R (result of the correlation)
%   This function autocorrelate a signal with itself. It's made to
%   recognize a pattern repeated in the signal.

length_x = length(x);
R = zeros(2*length_x-1, 1);

for k = -(length_x-1):(length_x-1)
    if k < 0            % advanced
        x_delayed = cat(1, x((1-k):length_x), zeros(-k, 1));
        R(length_x+k) = sum(x.*x_delayed);
    elseif k > 0        % retarded
        x_delayed = cat(1, zeros(k, 1), x(1:(length_x-k)));
        R(length_x+k) = sum(x.*x_delayed);
    elseif k == 0       % synchronized
        R(length_x) = sum(x.*x);
    end 
end

end

