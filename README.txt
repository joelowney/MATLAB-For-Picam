MATLAB-for-PICAM is a simple software package that allows Princeton Instruments cameras to be used via MATLAB on Windows 7 64-bit by way of the Princeton Instruments PICAM Library. Copyright (C) 2014 Joe Lowney. The copyright holder can be reached at joelowney@gmail.com

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.

Use Princeton Instruments cameras via MATLAB on Windows 7 64-bit by way of PICAM.

-----------------------------------------------------

Purpose: The purpose of this software package is to be able to capture images on the Princeton Instruments PIXIS 1024 from Matlab on a Windows 7 64 bit. To use a 64 bit operating system with the PIXIS camera you must use the PICAM drivers. IMPORTANT: This code was written for a very specific experiment. It is not my intention that you use the exact code, but rather my hope is that it will provide a useful starting point for your applications that require controlling a Princeton Instruments Camera via MATLAB on a Windows 7 64-bit platform.

Summary: I created an executeable titled 'ConfigAndCapture' which talks to the PIXIS camera via the PICAM drivers. The important feature of this executeable is that it takes several arguments to specify the Region of Interest (ROI), number of frames, and the desired exposure time. This executeable can be reconfigured to take different arguments if needed. Doing so would require recompiling. IMPORTANT: the executeable file is configured to set the camera to be triggered externally. If you do not want this to be the case then you will need to make appropriate changes in the .cpp file.

Functional Summary: In MATLAB you can run the CaptureFrames.m script. This script contains variables to specify the ROI, number of frames, and the exposure time. The MATLAB script calls CaptureFrames.bat which in turn calls the executeable with the necessary arguments.

Requirements: You will need the PICAM drivers available from Princeton Instruments for free. You will probably need to recompile the ConfigAndCapture.cpp file to meet your specific needs.
