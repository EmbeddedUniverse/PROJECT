close all
clear all
clc

fclose('all');
serialLink = serial('COM4','BaudRate',57600); % Change to appropriate port
fopen(serialLink);

[que, delimiterOut] = importdata('test_haut_bas_100Hz_autocorr.dat');

Fs = 20;

%for 1 second of information at 100Hz, number of data is 100
i=1;
N=1000;

buffX = zeros(1,N);
buffY = zeros(1,N);
buffZ = zeros(1,N);

for a=1:3
    a
    pause(1);
end

while(i<N-1)
    sample = fscanf(serialLink,'x=%f y=%f z=%f');
    buffX(1,i) = sample(1);
    buffY(1,i) = sample(2)+1;
    buffZ(1,i) = sample(3);
    i = i+1;
end

refx = que(:,1)-1';
refy = que(:,2)-1';
refz = que(:,3)-1';

buffX = buffX';
buffY = buffY';
buffZ = buffZ';

autocorr = xcorr(refz, refz);
corrX = xcorr(refz, buffX);
corrY = xcorr((-refz), buffY);
corrZ = xcorr(refz, buffZ);

corrXmax=0;
corrYmax=0;
corrZmax=0;

for i = 1:length(corrX)
 if corrXmax<corrX(i)
     corrXmax=corrX(i);
 end
 
 if corrYmax<corrY(i)
     corrYmax=corrY(i);
 end
 
 if corrZmax<corrZ(i)
     corrZmax=corrZ(i);
 end
    
end


figure()
subplot(4,1,1)
hold on 
plot(95.*ones(2000))
plot(corrX);
title("X")
hold off

subplot(4,1,2)
hold on
plot(95.*ones(2000))
plot(corrY);
title("Y")
hold off

subplot(4,1,3)
hold on
plot(95.*ones(2000))
plot(corrZ);
title("Z")
hold off

subplot(4,1,4)
hold on 
plot(autocorr);
plot(80.*ones(120))
title("Auto")
hold off
fclose(serialLink);

figure 
plot(refz)