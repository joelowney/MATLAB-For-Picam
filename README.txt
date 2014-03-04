Purpose:  The purpose of this software package is to be 
able to capture images on the Princeton Instruments PIXIS 1024 
from Matlab on a Windows 7 64 bit.  To use a 64 bit operating
system with the PIXIS camera you must use the PICAM drivers.

Summary:  We created an executeable titled 'ConfigAndCapture' 
which talks to the PIXIS camera via the PICAM drivers.  The 
important feature of this executeable is that it takes several 
arguments to specify the Region of Interest (ROI), number of frames,
and the desired exposure time.  This executeable can be reconfigured
to take different arguments if needed.  Doing so would require
recompiling.

Functional Summary:
In MATLAB you can run the CaptureFrames.m script.  This script 
contains variables to specify the ROI, number of frames, and 
the  exposure time.

The MATLAB script calls CaptureFrames.bat which in turn 
calls the executeable with the necessary arguments.  