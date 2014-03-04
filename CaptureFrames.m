clear all; close all;

%%%% This routine captures an image sequence from a Princeton Instruments Camera by calling an executeable which uses the PICAM Library.
%%%% We pass several parameters to the executeable:
%%%% FileDir --- The directory in which to save the image files 
%%%% FileName --- The filename to save as
%%%% int2str(x0) --- The start pixel in the x-direction (relevant for using ROI).
%%%% int2str(y0) --- The start pixel in the y-direction (relevant for using ROI).
%%%% int2str(dx) --- The width in x-direction (relevant for using ROI).
%%%% int2str(dy) --- The width in y-direction (relevant for using ROI).
%%%% num2str(DT) --- The exposure time in milliseconds
%%%% int2str(NFrames)] --- The number of frames to capture


display(['Beginning acquisition at: ' datestr(now,'yyyy-mm-dd HH:MM:SS')]);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Image Acquisition Params %%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ROI Parameters
tic;
dx = 1024;
dy = 1024;
x0 = 0;
y0 = 0;

% Number of Frames
NFrames = 5;

% Exposure time in msec
DT = 6;

%%%%%%%%%%%%%%%
%%% Options %%%
%%%%%%%%%%%%%%%

%% Create Tiff File
% Default: do not create tiff file (to save disk space)
% In the future you may want to create TIFFs for compatibility reason.  Or
% modify the executeable to save TIFFs (not sure how to do that yet).
CreateTiffFile = true;

%% Display Images
% Default: do not display images (allow processing function to display
% images)
DisplayImages = false;
CheckNoise = false;
%% File Naming Parameters
today = datestr(now,'yyyy-mm-dd');
year = datestr(now,'yyyy');


RootFileDir = ['C:\SPECIFY ROOT DIRECTORY TO SAVE YOUR DATA HERE\' year '\'];

FileDir = [RootFileDir today '\'];


%%%%%%%%%%%%%%%%%
%%% Execution %%%
%%%%%%%%%%%%%%%%%

%%%
% Determine file name
%%%
[ FileName TiffName FilePath TiffPath filenamecounter] = NewFilePath( FileDir );

display(['Waiting for acquisition of ' FilePath]);

doscmd = ['start /MIN CaptureFrames.bat ' FileDir ' ' FileName ' ' int2str(x0) ' ' int2str(y0) ' ' int2str(dx) ' ' int2str(dy) ' ' num2str(DT) ' ' int2str(NFrames)];

[status,stdout]  = dos(doscmd);

disp('Waiting for file to arrive... ')
while(exist(FilePath) == 0)
    pause(0.75)
end

% Load raw data into Matlab.
FileID = fopen(FilePath);
for kk = 1:NFrames
    ImageMatrix(:,:,kk) = fread(FileID, [dx, dy], '*uint16')';
end
fclose(FileID);

% Optionally write TIFF file
if(CreateTiffFile)
    for kk = 1:NFrames
        imwrite(ImageMatrix(:,:,kk),TiffPath,'writemode','append');
    end  
end
   
display(['Completed acquisition of ' TiffName  ' at: ' datestr(now,'yyyy-mm-dd HH:MM:SS')]);