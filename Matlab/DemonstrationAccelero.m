close all
clear all
clc

fclose('all');
serialLink = serial('COM4','BaudRate',57600); % Change to appropriate port
fopen(serialLink);

Fs = 20;

% sampleSize = 100;
% t = -(sampleSize-1):0;
% bufferX = zeros(sampleSize);
% bufferY = zeros(sampleSize);
% bufferZ = zeros(sampleSize);
% 
% push = @(new, buffer) [buffer(2:length(buffer)), new];
% 
% figure('Name', 'DonneesAccelero');
% plot(t, bufferX, t, bufferY, t, bufferZ)
% ylim([-2,2]);

%for 1second of information at 100Hz, number of data is 100
i=1;
N=1000;

buffX = zeros(1,N);
buffY = zeros(1,N);
buffZ = zeros(1,N);

for a=1:3
    a
    pause(1);
end

fileIDx = fopen('ReferenceSignalX.dat','w');
fileIDy = fopen('ReferenceSignalY.dat','w');
fileIDz = fopen('ReferenceSignalZ.dat','w');
fprintf(fileIDx, '#define NUMREFDATA %d\n', N);
fprintf(fileIDx, 'float REFERENCESIGNALX[NUMREFDATA] = {\n');
fprintf(fileIDy, '#define NUMREFDATA %d\n', N);
fprintf(fileIDy, 'float REFERENCESIGNALY[NUMREFDATA] = {\n');
fprintf(fileIDz, '#define NUMREFDATA %d\n', N);
fprintf(fileIDz, 'float REFERENCESIGNALZ[NUMREFDATA] = {\n');

while(i<N-1)
    sample = fscanf(serialLink,'x=%f y=%f z=%f');
    buffX(1,i) = sample(1);
    buffY(1,i) = -(sample(2)+1);
    buffZ(1,i) = sample(3);
    fprintf(fileIDx,'   %f,  \n', sample(1));
    fprintf(fileIDy,'   %f,  \n', (-sample(2)));
    fprintf(fileIDz,'   %f,  \n', sample(3));
    i = i+1;
end
sample = fscanf(serialLink,'x=%f y=%f z=%f');
fprintf(fileIDx,'   %f,  };\n', sample(1));
fprintf(fileIDy,'   %f,  };\n', (-sample(2)));
fprintf(fileIDz,'   %f,  };\n', sample(3));

fclose(fileIDx);
fclose(fileIDy);
fclose(fileIDz);

buffX(1,N) = sample(1);
buffY(1,N) = -sample(2);
buffZ(1,N) = sample(3);

figure()
hold on
%subplot(3,1,1)
plot(1:N,buffX);
%subplot(3,1,2)
plot(1:N,buffY);
%subplot(3,1,3)
plot(1:N,buffZ);
hold off

% while()
%     sample = fscanf(serialLink,'x=%f y=%f z=%f')
%     bufferX = push(sample(1), bufferX);
%     bufferY = push(sample(2), bufferY);
%     bufferZ = push(sample(3), bufferZ);
%     fprintf(fileID,'    [  %d,  %d,  %d  ]\n', bufferX(1,i), bufferY(1,i), bufferZ(1,i));
%     plot(t, bufferX, t, bufferY, t, bufferZ);
%     ylim([-2,2]);
%     drawnow;
% end

fclose(serialLink);