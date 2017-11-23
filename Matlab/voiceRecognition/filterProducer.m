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
%Number of filters (Will calculate the all the coefficiens for the number
%of filters specified)
nbFilters = 1;
%Sampling frequency
fs = 16000;
%Order N needed to create the waterfall form
N = 12; %Minimum order 4
%Ripple in the bandpass according to specs
ripple_dB = 3;
%Stopband attenuation
stopBandAtt_dB = 40;
%Cutoff frequencies for the filters (Each row is a filter and each column :
%left side is for high-passing from this frequency and the right side is to
%low-passing from this frequency.
fc = [3100 3900; 0 0; 0 0; 0 0; 0 0; 0 0];

%Flag whether the coefficients printed are for a elliptic filter or a
%chebyshev filter
flagFilter = ELLIPTIC;

%Print or don't print header
print = 1;

%Flag : Print a bandpass made directly or bandpass made of a lowpass and a
%highpass
printBandpass = 0;

%Frequency cutoff limits visually displayed on frequency response graph 
lowLimit = fc(1,1);
maxLowLimit = lowLimit - 100;

highLimit = fc(1,2);
maxHighLimit = highLimit - 100;

%% Parameter from spec above
%Nyquist frequency
fNyq = fs/2;
%% FILTERS

%Those arrays below will be useful for printing the values in a .h file for CCS
sosArray = zeros(N/2*nbFilters,nbCoeffsSecondOrder);
globalGainArray = zeros(nbFilters,1);

sosArrayB = zeros(N/2*nbFilters,nbCoeffsSecondOrder);
globalGainArrayB = zeros(nbFilters,1);


%Producing "nbFilters" filter(s)
for i = 1:nbFilters
    for y = ELLIPTIC:CHEBYSHEV
        %Filter order N
        if(y == ELLIPTIC)
            %Elliptic filter
            %Bandpass
            [A,B,C,D] = ellip(N/2,ripple_dB, stopBandAtt_dB,fc(i,:)/fNyq,'bandpass');
            %Lowpass
            [AL,BL,CL,DL] = ellip(N/2,ripple_dB, stopBandAtt_dB,fc(i,2)/fNyq,'low');
            %Highpass
            [AH,BH,CH,DH] = ellip(N/2,ripple_dB, stopBandAtt_dB,fc(i,1)/fNyq,'high');
        else
            %Chebyshev filter
            %Bandpass
            [A,B,C,D] = cheby1(N/2,ripple_dB,fc(i,:)/fNyq,'bandpass');
            %Lowpass
            [AL,BL,CL,DL] = cheby1(N/2,ripple_dB,fc(i,2)/fNyq,'low');
            %Highpass
            [AH,BH,CH,DH] = cheby1(N/2,ripple_dB,fc(i,1)/fNyq,'high');
        end
        
        %Bandpass
        [sos globalGain] = ss2sos(A,B,C,D, 'up', 'inf');
        %Lowpass
        [sosL globalGainL] = ss2sos(AL,BL,CL,DL, 'up', 'inf');
        %Highpass
        [sosH globalGainH] = ss2sos(AH,BH,CH,DH, 'up', 'inf');
        
        if(y == ELLIPTIC)
            %Elliptic filter
            %Bandpass
            [b1,a1] = sos2tf(sos, globalGain);
            
            %Lowpass
            [bL,aL] = sos2tf(sosL, globalGainL);
            %Highpass
            [bH,aH] = sos2tf(sosH, globalGainH);
            %Lowpass + Highpass = Bandpass
            hL1 = tf(bL,aL);
            hH1 = tf(bH,aH);
            hB1 = series(hL1,hH1);
            [bB1,aB1] = tfdata(hB1);
            bB1 = cell2mat(bB1);
            aB1 = cell2mat(aB1);
            [sosB,globalGainB] = tf2sos(bB1,aB1);
        end
        
        if(y == CHEBYSHEV)
            %Chebyshev filter
            %Bandpass
            [b2,a2] = sos2tf(sos, globalGain);
            
            %Lowpass
            [bL,aL] = sos2tf(sosL, globalGainL);
            %Highpass
            [bH,aH] = sos2tf(sosH, globalGainH);
            %Lowpass + Highpass = Bandpass
            hL2 = tf(bL,aL);
            hH2 = tf(bH,aH);
            hB2 = series(hL2,hH2);
            [bB2,aB2] = tfdata(hB2);
            bB2 = cell2mat(bB2);
            aB2 = cell2mat(aB2);
            [sosB,globalGainB] = tf2sos(bB2,aB2);
            
            %Comparing Elliptic filter with Chebyshev filter
            figure()
            freqz(b1,a1);
            hold on
            freqz(b2,a2);
            freqz(bB1,aB1);
            freqz(bB2,aB2);
            lines = findall(gcf,'type','line');
            set(lines(1),'color','red')
            set(lines(2),'color','blue')
            set(lines(3),'color','green')
            set(lines(4),'color','magenta')
            [o,~] = freqz(bB2,aB2);
            plot(ones(length(o),1)*lowLimit/fNyq,-(length(o)/2):1:(length(o)/2-1),'-k')
            plot(ones(length(o),1)*maxLowLimit/fNyq,-(length(o)/2):1:(length(o)/2-1),'-k')
            plot(ones(length(o),1)*highLimit/fNyq,-(length(o)/2):1:(length(o)/2-1),'-k')
            plot(ones(length(o),1)*maxHighLimit/fNyq,-(length(o)/2):1:(length(o)/2-1),'-k')
            hold off
            legend('Elliptic','Chebyshev','Elliptic Lowpass+Highpass','Chebyshev Lowpass+Highpass')
            title(strcat('Frequency response of the filter IIR (',num2str(N),'/2)quad for FILTER ',num2str(i)))
            figure()
            zplane([b1,a1])
            title(strcat('Poles and zeros of the filter IIR (',num2str(N),'/2)quad for ELLIPTIC FILTER ',num2str(i)))
            figure()
            zplane([b2,a2])
            title(strcat('Poles and zeros of the filter IIR (',num2str(N),'/2)quad for CHEBYSHEV FILTER ',num2str(i)))
            figure()
            zplane([bB1,aB1])
            title(strcat('Poles and zeros of the filter IIR (',num2str(N),'/2)quad for ELLIPTIC FILTER (LOW+HIGH) ',num2str(i)))
            figure()
            zplane([bB2,aB2])
            title(strcat('Poles and zeros of the filter IIR (',num2str(N),'/2)quad for CHEBYSHEV FILTER (LOW+HIGH) ',num2str(i)))
        end
            
        %Convert values in 2Q13 form (Also keeps poles inside of the unit-circle)
        for z = 1:N/2
            if (sqrt(sos(z,6)) >= 1)
                sos(z,6) = floor(sos(z,6)*n2Q13_multiplier)*n2Q13_divider;
            end
            if (sqrt(sosB(z,6)) >= 1)
                sos(z,6) = floor(sosB(z,6)*n2Q13_multiplier)*n2Q13_divider;
            end
        end
        if(y == flagFilter)
            sos = round(sos*n2Q13_multiplier);
            globalGain = round(globalGain.*n2Q13_multiplier);

            sosB = round(sosB*n2Q13_multiplier);
            globalGainB = round(globalGainB.*n2Q13_multiplier);

            %Filling the arrays for upcoming printing
            sosArray((N/2*i-(N/2-1)):(N/2*i),:) = sosArray((N/2*i-(N/2-1)):(N/2*i),:) + sos;
            globalGainArray(i) = globalGainArray(i) + globalGain;

            %Filling the arrays for upcoming printing
            sosArrayB((N/2*i-(N/2-1)):(N/2*i),:) = sosArrayB((N/2*i-(N/2-1)):(N/2*i),:) + sosB;
            globalGainArrayB(i) = globalGainArrayB(i) + globalGainB;
        end
        
        %% Write .h file of the coefficients values
        if(and(print == 1,i == 2))
            if(printBandpass == 1)
                if(y == flagFilter)
                    if(flagFilter ==1)
                        filterType = 'ELLIPTIC';
                    else
                        filterType = 'CHEBYSHEV';
                    end
                    fileID = fopen('coeffsIIR.txt','w');
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
                    fprintf(fileID, '\tconst short globalGain[nbSecondOrder];\n');
                    fprintf(fileID, '\tconst short coeffsIIR[nbSecondOrder][nbCoeffs];');
                    fprintf(fileID, '} secondOrdfilters; \n\n');

                    for h = 1:nbFilters
                        fprintf(fileID, '//FILTER %d\n',h);
                        fprintf(fileID, strcat('const secondOrdfilters sOFilters',num2str(h),' =  {'));
                        fprintf(fileID, strcat('{',num2str(globalGainArray(h)),'}, {'));
                        for b = 1:N/2
                            fprintf(fileID, '{');
                            for p = 1:nbCoeffsSecondOrder
                                if(p ~= nbCoeffsSecondOrder)
                                    fprintf(fileID, ' %d,',sosArray(length(sosArray)-(length(sosArray)-b*h),p));
                                else
                                    fprintf(fileID, ' %d}',sosArray(length(sosArray)-(length(sosArray)-b*h),p));
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
            else
                if(y == flagFilter)
                    if(flagFilter ==1)
                        filterType = 'ELLIPTIC';
                    else
                        filterType = 'CHEBYSHEV';
                    end
                    fileID = fopen('coeffsIIR.txt','w');
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
                    fprintf(fileID, '\tconst short globalGain[nbSecondOrder];\n');
                    fprintf(fileID, '\tconst short coeffsIIR[nbSecondOrder][nbCoeffs];');
                    fprintf(fileID, '} secondOrdfilters; \n\n');

                    for h = 1:nbFilters
                        fprintf(fileID, '//FILTER %d\n',h);
                        fprintf(fileID, strcat('const secondOrdfilters sOFilters',num2str(h),' =  {'));
                        fprintf(fileID, strcat('{',num2str(globalGainArrayB(h)),'}, {'));
                        for b = 1:N/2
                            fprintf(fileID, '{');
                            for p = 1:nbCoeffsSecondOrder
                                if(p ~= nbCoeffsSecondOrder)
                                    fprintf(fileID, ' %d,',sosArrayB(length(sosArrayB)-(length(sosArrayB)-b*h),p));
                                else
                                    fprintf(fileID, ' %d}',sosArrayB(length(sosArrayB)-(length(sosArrayB)-b*h),p));
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
end