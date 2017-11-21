%%/Title:       filterProducer
%%/Description: Program to produce filters for the voice recognition application for 
%%/             Embedded Universe Project.

%%/ NOTE: This script only builds elliptic/chebyshev bandpass filters with a 2Q13 representation for DSP 

clear all
close all
clc
%% Constants
% 2Q13/Real converter
n2Q13_multiplier = 2^13;
n2Q13_divider = 1\n2Q13_multiplier;

%Number of coefficients of an second order IIR filter
nbCoeffsSecondOrder = 6;

%Define Elliptic and Chebyshev
ELLIPTIC = 1;
CHEBYSHEV = 2;

%% Specifications of the filters (Can be changed manually)
%Number of filters (Will calculate the all the coefficiens 
%for the number of filters specified)
nbFilters = 2;
%Sampling frequency
fs = 16000;
%Order N needed to create the waterfall form
N = 4;
%Ripple in the bandpass according to specs
ripple_dB = 8;
%Stopband attenuation 
stopBandAtt_dB = 40;
%Cutoff frequencies for the filters (Each row is a filter and each column :
%left side is for high-passing from this frequency and the right side is to
%low-passing from this frequency.
fc = [500 900; 4000 6000; 0 0; 0 0; 0 0; 0 0];

%Flag whether the coefficients printed are for a elliptic filter or a
%chebyshev filter
flagFilter = ELLIPTIC;

%Print or don't print header
print = 1;

%% Parameter from spec above
%Nyquist frequency
fNyq = fs/2;
%% FILTERS

%Those arrays below will be useful for printing the values in a .h file for CCS
sosArray = zeros(N/2*nbFilters,nbCoeffsSecondOrder);
globalGainArray = zeros(nbFilters,1);

%Producing "nbFilters" filter(s)
for i = 1:nbFilters
    for y = ELLIPTIC:CHEBYSHEV
        %Filter order N
        if(y == ELLIPTIC)
            %Elliptic filter
            [A,B,C,D] = ellip(N/2,ripple_dB, stopBandAtt_dB,fc(i,:)/fNyq);
        else
            %Chebyshev filter
            [A,B,C,D] = cheby1(N/2,ripple_dB,fc(i,:)/fNyq,'bandpass');
        end
        
        [sos globalGain] = ss2sos(A,B,C,D, 'up', 'inf');
        
        if(y == ELLIPTIC)
            %Elliptic filter
            [b1,a1] = sos2tf(sos, globalGain);
        end
        
        if(y == CHEBYSHEV)
            %Chebyshev filter
            [b2,a2] = sos2tf(sos, globalGain);
            
            %Comparing Elliptic filter with Chebyshev filter
            figure()
            freqz(b1,a1);
            hold on
            freqz(b2,a2);
            lines = findall(gcf,'type','line');
            set(lines(1),'color','red')
            set(lines(2),'color','blue')
            hold off
            legend('Elliptic','Chebyshev')
            title(strcat('Frequency response of the filter IIR (',num2str(N),'/2)quad for FILTER ',num2str(i)))
            figure()
            zplane([b1,a1])
            title(strcat('Poles and zeros of the filter IIR (',num2str(N),'/2)quad for ELLIPTIC FILTER ',num2str(i)))
            figure()
            zplane([b2,a2])
            title(strcat('Poles and zeros of the filter IIR (',num2str(N),'/2)quad for CHEBYSHEV FILTER ',num2str(i)))
        end
            
        %Convert values in 2Q13 form (Also keeps poles inside of the unit-circle)
        for z = 1:N/2
            if (sqrt(sos(z,6)) >= 1)
                sos(z,6) = floor(sos(z,6)*n2Q13_multiplier)*n2Q13_divider;
            end
        end

        sos = round(sos*n2Q13_multiplier);
        globalGain = round(globalGain.*n2Q13_multiplier);

        %Filling the arrays for upcoming printing
        sosArray((N/2*i-(N/2-1)):(N/2*i),:) = sosArray((N/2*i-(N/2-1)):(N/2*i),:) + sos;
        globalGainArray(i) = globalGainArray(i) + globalGain;

        %% Write .h file of the coefficients values
        if(print == 1)
            if(y == flagFilter)
                if(flagFilter ==1)
                    filterType = 'ELLIPTIC';
                else
                    filterType = 'CHEBYSHEV';
                end
                fileID = fopen('..\..\Includes\coeffsIIR.h','w');
                fprintf(fileID, '\n\n#ifndef INCLUDES_COEFFSIIR_H_\n');
                fprintf(fileID, '#define INCLUDES_COEFFSIIR_H_\n');
                fprintf(fileID, strcat('//********** ',filterType,' FILTERS **********\n'));
                fprintf(fileID, '//\tOrder = %d\n',N);
                fprintf(fileID, '//\tRepresentation in 2Q13\n');
                fprintf(fileID, '#define nbFilters %d\n',nbFilters);
                fprintf(fileID, '#define nbSecondOrder %d\n',N/2);
                fprintf(fileID, '#define nbCoeffs %d\n',nbCoeffsSecondOrder);

                fprintf(fileID, '\n//Second Order Filters Structure\n');
                fprintf(fileID, 'typedef struct {\n');
                fprintf(fileID, '\tconst int globalGain[nbSecondOrder];\n');
                fprintf(fileID, '\tconst int coeffsIIR[nbSecondOrder][nbCoeffs];');
                fprintf(fileID, '} secondOrdfilters; \n\n');

                for h = 1:nbFilters
                    fprintf(fileID, '//FILTER %d\n',h);
                    fprintf(fileID, strcat('const secondOrdfilters sOFilters',num2str(h),' =  {'));
                    fprintf(fileID, strcat('{',num2str(globalGainArray(h)),'}, {'));
                    for b = 1:N/2
                        fprintf(fileID, '{');
                        for y = 1:nbCoeffsSecondOrder
                            if(y ~= nbCoeffsSecondOrder)
                                fprintf(fileID, ' %d,',sosArray(b,y));
                            else
                                fprintf(fileID, ' %d}',sosArray(b,y));
                            end
                        end
                        if(b ~= N/2)
                            fprintf(fileID, ',\n');
                        else
                            fprintf(fileID, '}');
                        end
                    end
                    fprintf(fileID, '};\n\n');
                end

                fprintf(fileID, '\n//All the filters (how to call them)\n');
                fprintf(fileID, 'const secondOrdfilters* filters[nbFilters] = {');
                for v = 1:nbFilters
                    if(v == nbFilters)
                        fprintf(fileID, strcat('&sOFilters',num2str(v)));
                    else
                        fprintf(fileID, strcat('&sOFilters',num2str(v),', '));
                    end
                end
                fprintf(fileID, '}; \n\n');

                fprintf(fileID, '#endif /* INCLUDES_COEFFSIIR_H_ */\n');
                fclose(fileID);
            end
        end
    end
end