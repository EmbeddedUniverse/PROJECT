close all
clear all
clc

fclose('all');
serialLink = serial('COM4','BaudRate',57600); % Change to appropriate port
fopen(serialLink);

Fs = 20;

sampleSize = 40;
t = -(sampleSize-1):0;
bufferX = zeros(sampleSize);
bufferY = zeros(sampleSize);
bufferZ = zeros(sampleSize);

push = @(new, buffer) [buffer(2:length(buffer)), new];

figure('Name', 'DonneesAccelero');
plot(t, bufferX, t, bufferY, t, bufferZ)
ylim([-2,2]);

while(~isempty(findobj('type', 'figure', 'name', 'DonneesAccelero')))
    sample = fscanf(serialLink,'x=%f y=%f z=%f')
    bufferX = push(sample(1), bufferX);
    bufferY = push(sample(2), bufferY);
    bufferZ = push(sample(3), bufferZ);
    plot(t, bufferX, t, bufferY, t, bufferZ);
    ylim([-2,2]);
    drawnow;
end

fclose(serialLink);