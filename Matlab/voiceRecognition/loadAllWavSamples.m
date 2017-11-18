function [ data , Fs ] = loadAllWavSamples(folderPathName)
%loadAllWavSamples: Function that loads and output an array of discrete
%data of all the samples selected from a sound folder. Must first load the
%principal folder of samples "piouSamples" from the OneDrive repository
%(click the link "piouSamplesOneDrive.url" nd download the folder).
%Automatically does the padding zero.
%   Input : folderPathName = Path of the folder containg .wav samples         
%   Output: data = Array of all the discrete data of the samples, row by
%                   row for each sample.
%           Fs = Sample frequency of each sample (generally 44.1kHz)

    L1 = 0;

    listing = dir(folderPathName);
    
    nbSamples = length(listing(3:end));
    
    for i = 1:nbSamples
        [data,~] = audioread(strcat(folderPathName,listing(i+2).name));
        L2 = length(data);
        if(L1<L2)
            L1 = L2;
        end
    end

    data = zeros(L1,nbSamples);

    for i = 1:nbSamples
        [y,Fs] = audioread(strcat(folderPathName,listing(i+2).name));
        Ly = length(y);
        data(1:Ly,i) = data(1:Ly,i) + y(:,1); 
    end

end

