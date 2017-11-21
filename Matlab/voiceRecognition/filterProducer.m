%%/Title:       filterProducer
%%/Description: Program to produce filters for the voice recognition application for 
%%/             Embedded Universe Project.

%%/ NOTE: This script only builds elliptic/chebyshev bandpass filters with a 2Q13 representation for DSP 

clear all
close all
clc

%% Specifications of the filters (Can be changed manually)
%Number of filters (Will calculate the all the coefficiens 
%for the number of filters specified)
nbFilters = 2;
%Sampling frequency
fs = 16000;
%Order N needed to create the waterfall form
N = 6;
%Ripple in the bandpass according to specs
ripple_dB = 8;
%Stopband attenuation 
stopBandAtt_dB = 40;
%Cutoff frequencies for the filters (Each row is a filter and each column :
%left side is for high-passing from this frequency and the right side is to
%low-passing from this frequency.
fc = [500 1000; 4000 6000; 0 0; 0 0; 0 0; 0 0];

%% Parameters/Constants
%Nyquist frequency
fNyq = fs/2;

% 2Q13/Real converter
n2Q13_multiplier = 2^13;
n2Q13_divider = 1\n2Q13_multiplier;

%Number of coefficients of an second order IIR filter
nbCoeffsSecondOrder = 6;

%% FILTERS
sosArray = zeros(N/2*nbFilters,nbCoeffsSecondOrder);
globalGainArray = zeros(nbFilters,1);
for i = 1:nbFilters
    for y = 1:2
        %Filter order N=4 
        if(y == 1)
            [A,B,C,D] = ellip(N/2,ripple_dB, stopBandAtt_dB,fc(i,:)/fNyq);
        else
            [A,B,C,D] = cheby1(N/2,ripple_dB,fc(i,:)/fNyq,'bandpass');
        end

        [sos globalGain] = ss2sos(A,B,C,D, 'up', 'inf');
        
        % 2 parts of order N=2 as a waterfall
        if(N == 4)
            [b a] = sos2tf(sos, globalGain); 
            figure()
            freqz(b,a);
            title(strcat('Frequency response of the filter IIR Biquad for FILTER ',num2str(i)))
            figure()
            zplane(b,a);
            title(strcat('Poles and zeros of the filter IIR Biquad for FILTER ',num2str(i)))
        end
        
        if(y == 1)
            [b1,a1] = ss2tf(A,B,C,D);
        end
        
        if(y == 2)
            [b2,a2] = ss2tf(A,B,C,D);
            figure()
            freqz([b1,a1],[b2,a2]);
            title(strcat('Frequency response of the filter IIR Biquad for FILTER ',num2str(i)))
            figure()
            zplane([b1,a1],[b2,a2]);
            title(strcat('Poles and zeros of the filter IIR Biquad for FILTER ',num2str(i)))
            legend('Elliptic','Chebyshev')
        end
    end
    
    %Convert values in 2Q13 form (Also keeps poles inside of the unit-circle)
    for z = 1:N/2
        if (sqrt(sos(z,6)) >= 1)
            sos(z,6) = floor(sos(z,6)*n2Q13_multiplier)*n2Q13_divider;
        end
    end

    sos = round(sos*n2Q13_multiplier);
    globalGain = round(globalGain.*n2Q13_multiplier);

    sosArray((N/2*i-(N/2-1)):(N/2*i),:) = sosArray((N/2*i-(N/2-1)):(N/2*i),:) + sos;
    globalGainArray(i) = globalGainArray(i) + globalGain;
    
    % %% Write txt file of the coefficients values 
    % fileID = fopen('coeffsIIR.txt','w');
    % 
    % sos = [sos;sos2;sos3;sos4;sos5;sos6];
    % gain = [globalGain;gain_global2;gain_global3;gain_global4;gain_global5;gain_global6];
    % for i = 1:NbFilt
    %     fprintf(fileID, '//FILTER %d,\n',i);
    %     fprintf(fileID, '//Global gain\n');
    %     fprintf(fileID, '\t%d\n',gain(i));
    %     fprintf(fileID, '//Coefficients\n');  
    %     fprintf(fileID, '\t{%d, %d, %d, %d, %d, %d},\n',sos(2*i-1,1),sos(2*i-1,2),sos(2*i-1,3),sos(2*i-1,4),sos(2*i-1,5),sos(2*i-1,6));
    %     fprintf(fileID, '\t{%d, %d, %d, %d, %d, %d}',sos(2*i,1),sos(2*i,2),sos(2*i,3),sos(2*i,4),sos(2*i,5),sos(2*i,6));
    %     fprintf(fileID, '\n\n');
    % end
    % fclose(fileID);
end