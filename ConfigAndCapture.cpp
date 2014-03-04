////////////////////////////////////////////////////////////////////////////////
// Configure Sample
// - demonstrates camera setup including:
//   - changing common camera parameters
//   - reading temperature directly from hardware
//   - changing exposure time during acquisition
// - uses the first camera found if any or creates a demo camera
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include "picam.h"
#include <process.h>
#include "stdio.h"
#define NO_TIMEOUT  -1
#define TIMEOUT 10000
using namespace std;

// - prints any picam enum
void PrintEnumString( PicamEnumeratedType type, piint value )
{
    const pichar* string;
    Picam_GetEnumerationString( type, value, &string );
    std::cout << string;
    Picam_DestroyString( string );
}

// - prints the camera identity
void PrintCameraID( const PicamCameraID& id )
{
    // - print the model
    PrintEnumString( PicamEnumeratedType_Model, id.model );

    // - print the serial number and sensor
    std::cout << " (SN:" << id.serial_number << ")"
              << " ["    << id.sensor_name   << "]" << std::endl;
}

// - prints error code
void PrintError( PicamError error )
{
    if( error == PicamError_None )
        std::cout << "Succeeded" << std::endl;
    else
    {
        std::cout << "Failed (";
        PrintEnumString( PicamEnumeratedType_Error, error );
        std::cout << ")" << std::endl;
    }
}

void SetFltParameter(PicamHandle camera, PicamParameter parameter, piflt floatval)
{
	PicamError error;
	pibln committed;
	piflt CurrentFltVal;
	piflt DefaultFltVal;

	std::cout << "= = = = = = = = = " << std::endl;

	// Display Current Val
	std::cout << "Getting value of ";
	PrintEnumString(PicamEnumeratedType_Parameter, parameter);
	std::cout << ": ";
	error = Picam_GetParameterFloatingPointValue(camera, parameter, &CurrentFltVal);
	PrintError(error);
	if(error == PicamError_None)
	{
		PrintEnumString(PicamEnumeratedType_Parameter, parameter);
		std::cout << " is set to: " << CurrentFltVal << std::endl;
	}

	// Display read value
	pibln readable;
	std::cout << "Checking if value is readable from hardware: ";
	error = Picam_CanReadParameter( camera, parameter, &readable );
	PrintError(error);
	std::cout << "Readability is: " << readable << std::endl;

	// Display Default Val
	std::cout << "Reading Default Value: ";
	error = Picam_GetParameterFloatingPointDefaultValue(camera, parameter, &DefaultFltVal);
	PrintError(error);
	if(error == PicamError_None)
	{
		PrintEnumString(PicamEnumeratedType_Parameter, parameter);
		std::cout << " default value is: " << DefaultFltVal << std::endl;
	}


	// Get Constraint Types
	PicamConstraintType ConstraintType;
	std::cout << "Getting Constraint Type: ";
	error = Picam_GetParameterConstraintType(camera, parameter, &ConstraintType);
	PrintError(error);
	if(error == PicamError_None)
	{
		std::cout << "Constraint Type is: ";\
		PrintEnumString(PicamEnumeratedType_ConstraintType, ConstraintType);
		std::cout << std::endl;
	}
	if(ConstraintType == PicamConstraintType_Range)
	{
		const PicamRangeConstraint* RecConstraint;
		const PicamRangeConstraint* ReqConstraint;
		std::cout << "Getting Range Constraints: ";
		// error = Picam_GetParameterRangeConstraint( camera, parameter, PicamConstraintCategory category, const PicamRangeConstraint** constraint );
		error = Picam_GetParameterRangeConstraint( camera, parameter, PicamConstraintCategory_Required, &ReqConstraint );
		PrintError(error);
		error = Picam_GetParameterRangeConstraint( camera, parameter, PicamConstraintCategory_Recommended, &RecConstraint );
		PrintError(error);
		std::cout << "Max Possible Val is: " << ReqConstraint-> maximum << std::endl;
		std::cout << "Min Possible val is: " << ReqConstraint-> minimum << std::endl;
		std::cout << "Possible Increment is: " << ReqConstraint -> increment << std::endl;

		std::cout << "Max Recommended Val is: " << RecConstraint-> maximum << std::endl;
		std::cout << "Min Recommended val is: " << RecConstraint-> minimum << std::endl;
		std::cout << "Recommended Increment is: " << RecConstraint -> increment << std::endl;

		std::cout << "Destroying range constraints: ";
		error = Picam_DestroyRangeConstraints(RecConstraint);
		PrintError(error);
		error = Picam_DestroyRangeConstraints(ReqConstraint);
		PrintError(error);
	}
	if(ConstraintType == PicamConstraintType_Collection)
	{
		const PicamCollectionConstraint* RecConstraint;
		const PicamCollectionConstraint* ReqConstraint;
		std::cout << "Getting Collection Constraints: ";
		// error = Picam_GetParameterCollectionConstraint( camera, parameter, PicamConstraintCategory_Required, &constraint );
		error = Picam_GetParameterCollectionConstraint( camera, parameter, PicamConstraintCategory_Required, &ReqConstraint );
		PrintError(error);
		error = Picam_GetParameterCollectionConstraint( camera, parameter, PicamConstraintCategory_Recommended, &RecConstraint );
		PrintError(error);

		std::cout << "Number of allowable values is: " << ReqConstraint -> values_count << std::endl;
		std::cout << "Allowable values are: ";
		piint maxval = ReqConstraint->values_count;
		int count = 0;
		for ( count = 0; count <= maxval -1; count++)
		{	
			std::cout << ReqConstraint->values_array[count] << " ";
		}
		
		std::cout << std::endl << "Number of recommended values is: " << RecConstraint -> values_count << std::endl;
		std::cout << "Allowable values are: ";
		count = 0;
		for ( count = 0; count <= maxval -1; count++)
		{	
			std::cout << RecConstraint->values_array[count] << " ";
		}

		std::cout << std::endl;
		std::cout << "Destroying collection constraints: ";
		error = Picam_DestroyCollectionConstraints(RecConstraint);
		PrintError(error);
		error = Picam_DestroyCollectionConstraints(ReqConstraint);
		PrintError(error);
	}

	
	// Check if settable
	std::cout << "Checking if parameter is settable to value: ";
	pibln settable;
	error = Picam_CanSetParameterFloatingPointValue( camera, parameter, floatval, &settable);
	PrintError(error);
	std::cout << "Settability is: " << settable << std::endl;
	

	// Set Val
	std::cout << "Setting  ";
	PrintEnumString(PicamEnumeratedType_Parameter, parameter);
	std::cout << " to: " << floatval << std::endl;
	//    std::cout << "Setting " << ExposureTime << " ms exposure time: ";
    error = 
        Picam_SetParameterFloatingPointValue(
            camera,
            parameter,
            floatval );
    PrintError( error );

    // - show that the modified parameters need to be applied to hardware
    Picam_AreParametersCommitted( camera, &committed );
    if( committed )
        std::cout << "Parameters have not changed.  Not going to commit." << std::endl;
    else
	{
        std::cout << "Parameters have been modified" << std::endl;

		// - apply the changes to hardware
		std::cout << "Commit to hardware: ";
		const PicamParameter* failed_parameters;
		piint failed_parameters_count;
		error =
			Picam_CommitParameters(
				camera,
				&failed_parameters,
				&failed_parameters_count );
		PrintError( error );
		if( failed_parameters_count > 0 )
		{
			std::cout << "THE FOLLOWING PARAMETERS ARE INVALID: " << std::endl;
			for( piint i = 0; i < failed_parameters_count; ++i )
			{
				std::cout << "FAILED TO COMMIT PARAMETER: ";
				std::cout << "    ";
				PrintEnumString(
					PicamEnumeratedType_Parameter,
					failed_parameters[i] );
				std::cout << std::endl;
			}
		}

	// Read Val Again
	std::cout << "Reading value of ";
	PrintEnumString(PicamEnumeratedType_Parameter, parameter);
	std::cout << ": ";
	error = Picam_GetParameterFloatingPointValue(camera, parameter, &CurrentFltVal);
	PrintError(error);
	if(error == PicamError_None)
	{
		PrintEnumString(PicamEnumeratedType_Parameter, parameter);
		std::cout << " is set to: " << CurrentFltVal << std::endl;
	}

	}

	std::cout << std::endl;
}

void SetIntParameter(PicamHandle camera, PicamParameter parameter, piint intval)
{
	PicamError error;
	pibln committed;
	std::cout << "= = = = = = = = = " << std::endl;
	// Display Current Val
	piint CurrentIntVal;
	std::cout << "Reading value of ";
	PrintEnumString(PicamEnumeratedType_Parameter, parameter);
	std::cout << ": ";
	error = Picam_GetParameterIntegerValue(camera, parameter, &CurrentIntVal);
	PrintError(error);
	if(error == PicamError_None)
	{
		PrintEnumString(PicamEnumeratedType_Parameter, parameter);
		std::cout << " is set to: " << CurrentIntVal << std::endl;
	}

	// Display read value
	pibln readable;
	std::cout << "Checking if value is readable from hardware: ";
	error = Picam_CanReadParameter( camera, parameter, &readable );
	PrintError(error);
	std::cout << "Readability is: " << readable << std::endl;

	// Display Default Val
	piint DefaultIntVal;
	std::cout << "Reading Default Value: ";
	error = Picam_GetParameterIntegerDefaultValue(camera, parameter, &DefaultIntVal);
	PrintError(error);
	if(error == PicamError_None)
	{
		PrintEnumString(PicamEnumeratedType_Parameter, parameter);
		std::cout << " default value is: " << DefaultIntVal << std::endl;
	}

	// Get Constraint Types
	PicamConstraintType ConstraintType;
	std::cout << "Getting Constraint Type: ";
	error = Picam_GetParameterConstraintType(camera, parameter, &ConstraintType);
	PrintError(error);
	if(error == PicamError_None)
	{
		std::cout << "Constraint Type is: ";\
		PrintEnumString(PicamEnumeratedType_ConstraintType, ConstraintType);
		std::cout << std::endl;
	}
	if(ConstraintType == PicamConstraintType_Range)
	{
		const PicamRangeConstraint* ReqConstraint;
		const PicamRangeConstraint* RecConstraint;;
		std::cout << "Getting Range Constraints: ";
		// error = Picam_GetParameterRangeConstraint( camera, parameter, PicamConstraintCategory category, const PicamRangeConstraint** constraint );
		error = Picam_GetParameterRangeConstraint( camera, parameter, PicamConstraintCategory_Required, &ReqConstraint );
		PrintError(error);
		error = Picam_GetParameterRangeConstraint( camera, parameter, PicamConstraintCategory_Recommended, &RecConstraint );
		std::cout << "Max Allowable Val is: " << ReqConstraint-> maximum << std::endl;
		std::cout << "Min Allowable val is: " << ReqConstraint-> minimum << std::endl;
		std::cout << "Allowable Increment is: " << ReqConstraint -> increment << std::endl;
		std::cout << "Max Recommended Val is: " << RecConstraint-> maximum << std::endl;
		std::cout << "Min Recommended val is: " << RecConstraint-> minimum << std::endl;
		std::cout << "Recommended Increment is: " << RecConstraint -> increment << std::endl;

		std::cout << "Destroying range constraints: ";
		error = Picam_DestroyRangeConstraints(RecConstraint);
		PrintError(error);
		error = Picam_DestroyRangeConstraints(ReqConstraint);
		PrintError(error);
	}
	if(ConstraintType == PicamConstraintType_Collection)
	{
		const PicamCollectionConstraint* ReqConstraint;
		const PicamCollectionConstraint* RecConstraint;
		std::cout << "Getting Collection Constraints: ";
		// error = Picam_GetParameterCollectionConstraint( camera, parameter, PicamConstraintCategory_Required, &constraint );
		error = Picam_GetParameterCollectionConstraint( camera, parameter, PicamConstraintCategory_Required, &ReqConstraint );
		PrintError(error);
		error = Picam_GetParameterCollectionConstraint( camera, parameter, PicamConstraintCategory_Recommended, &RecConstraint );
		PrintError(error);
		std::cout << "Number of permissible values is: " << ReqConstraint -> values_count << std::endl;
		std::cout << "Permissible values are: ";
		piint maxval = ReqConstraint->values_count;
		int count = 0;
		for ( count = 0; count <= maxval -1; count++)
		{	
			std::cout << ReqConstraint->values_array[count] << " ";
		}
		std::cout << std::endl;
		std::cout << "Number of recommended values is: " << RecConstraint -> values_count << std::endl;
		std::cout << "Recommended values are: ";
		maxval = RecConstraint -> values_count;
		count = 0;
		for ( count = 0; count <= maxval - 1; count++)
		{
			std::cout << RecConstraint -> values_array[count] << " ";
		}

		std::cout << std::endl;
		std::cout << "Destroying collection constraints: ";
		error = Picam_DestroyCollectionConstraints(RecConstraint);
		PrintError(error);
		error = Picam_DestroyCollectionConstraints(ReqConstraint);
		PrintError(error);
	}

	// Check if settable
	std::cout << "Checking if parameter is settable to value: ";
	pibln settable;
	error = Picam_CanSetParameterIntegerValue( camera, parameter, intval, &settable);
	PrintError(error);
	std::cout << "Settability is: " << settable << std::endl;

	// Set Parameter
	std::cout << "Setting Parameter: ";
	PrintEnumString(PicamEnumeratedType_Parameter, parameter);
	std::cout << " to: " << intval << std::endl;
	//    std::cout << "Setting " << ExposureTime << " ms exposure time: ";
    error = 
		Picam_SetParameterIntegerValue(
            camera,
            parameter,
            intval );
    PrintError( error );

    // - show that the modified parameters need to be applied to hardware
    Picam_AreParametersCommitted( camera, &committed );
    if( committed )
        std::cout << "Parameters have not changed.  Not going to commit." << std::endl;
    else
	{
        std::cout << "Parameters have been modified." << std::endl;

		// - apply the changes to hardware
		std::cout << "Commit to hardware: ";
		const PicamParameter* failed_parameters;
		piint failed_parameters_count;
		error =
			Picam_CommitParameters(
				camera,
				&failed_parameters,
				&failed_parameters_count );
		PrintError( error );
		if( failed_parameters_count > 0 )
		{
			std::cout << "The following parameters are invalid:" << std::endl;
			for( piint i = 0; i < failed_parameters_count; ++i )
			{
				std::cout << "FAILED TO COMMIT PARAMETER: ";
				std::cout << "    ";
				PrintEnumString(
					PicamEnumeratedType_Parameter,
					failed_parameters[i] );
				std::cout << std::endl;
			}
		}
	}

	// Read Val AGain
	std::cout << "Reading value of ";
	PrintEnumString(PicamEnumeratedType_Parameter, parameter);
	std::cout << ": ";
	error = Picam_GetParameterIntegerValue(camera, parameter, &CurrentIntVal);
	PrintError(error);
	if(error == PicamError_None)
	{
		PrintEnumString(PicamEnumeratedType_Parameter, parameter);
		std::cout << " is set to: " << CurrentIntVal << std::endl;
	}
	std::cout << std::endl;
}
// - Set configuration.
// - Need to mimic most (preferably all) settings from Winview.  Still learning how this all maps.
//void Configure( PicamHandle camera, piint ExposureTime )
void Configure( PicamHandle camera, piflt ExposureTime )
{
	SetFltParameter(camera, PicamParameter_ExposureTime, ExposureTime);
	
	SetFltParameter(camera, PicamParameter_AdcSpeed, 2);  
	SetIntParameter(camera, PicamParameter_AdcAnalogGain, PicamAdcAnalogGain_High);
	SetIntParameter(camera, PicamParameter_CleanUntilTrigger, 1);
	
	/*
	typedef enum PicamTriggerResponse
	{
		PicamTriggerResponse_NoResponse               = 1,
		PicamTriggerResponse_ReadoutPerTrigger        = 2,
		PicamTriggerResponse_ShiftPerTrigger          = 3,
		PicamTriggerResponse_ExposeDuringTriggerPulse = 4,
		PicamTriggerResponse_StartOnSingleTrigger     = 5
	} PicamTriggerResponse; 
	*/
	SetIntParameter(camera, PicamParameter_TriggerResponse, PicamTriggerResponse_ReadoutPerTrigger);

	/*
	typedef enum PicamTriggerDetermination
	{
		PicamTriggerDetermination_PositivePolarity = 1,
		PicamTriggerDetermination_NegativePolarity = 2,
		PicamTriggerDetermination_RisingEdge       = 3,
		PicamTriggerDetermination_FallingEdge      = 4
	} PicamTriggerDetermination;
	*/
	SetIntParameter(camera, PicamParameter_TriggerDetermination, PicamTriggerDetermination_PositivePolarity);

	SetIntParameter(camera, PicamParameter_CleanCycleCount, 1);
	SetIntParameter(camera, PicamParameter_CleanSectionFinalHeight, 4);
	SetIntParameter(camera, PicamParameter_CleanSectionFinalHeightCount, 250);
	SetFltParameter(camera, PicamParameter_SensorTemperatureSetPoint, -70);
	SetIntParameter(camera, PicamParameter_ReadoutControlMode, PicamReadoutControlMode_FullFrame);
	// In WinView we set this to 1 (assuming it corresponds to strips per clean, which is what I was told by Rob Alan).  When we set it to anything less than 8 it kills the PICAM.  So we're using 8.
	SetIntParameter(camera, PicamParameter_CleanCycleHeight, 8);

	/*
	typedef enum PicamReadoutControlMode
	{
		PicamReadoutControlMode_FullFrame       = 1,  
		PicamReadoutControlMode_FrameTransfer   = 2,
		PicamReadoutControlMode_Interline       = 5,
		PicamReadoutControlMode_Kinetics        = 3,
		PicamReadoutControlMode_SpectraKinetics = 4,
		PicamReadoutControlMode_Dif             = 6
	} PicamReadoutControlMode;
	*/
	SetIntParameter(camera, PicamParameter_ReadoutControlMode, PicamReadoutControlMode_FullFrame);
}

// - reads the temperature and temperature status directly from hardware
void ReadTemperature( PicamHandle camera )
{
    PicamError error;

    // - read temperature
    std::cout << "Read sensor temperature: ";
    piflt temperature;
	
    error =
        Picam_ReadParameterFloatingPointValue(
            camera,
            PicamParameter_SensorTemperatureReading,
            &temperature );
    PrintError( error );
    if( error == PicamError_None )
    {
        std::cout << "    " << "Temperature is "
                  << temperature << " degrees C" << std::endl;
    }

    // - read temperature status
    std::cout << "Read sensor temperature status: ";
    PicamSensorTemperatureStatus status;
    error =
        Picam_ReadParameterIntegerValue(
            camera,
            PicamParameter_SensorTemperatureStatus,
            reinterpret_cast<piint*>( &status ) );
    PrintError( error );
    if( error == PicamError_None )
    {
        std::cout << "    " << "Status is ";
        PrintEnumString( PicamEnumeratedType_SensorTemperatureStatus, status );
        std::cout << std::endl;
    }
}

void AcquireROI(PicamHandle camera, string FullFilePath, int x0, int y0, int dx, int dy, int NFrames)
{
	PicamError					err;			 /* Error Code			*/
	PicamAvailableData			dataFrame;		 /* Data Struct			*/
	PicamAcquisitionErrorsMask	acqErrors;		 /* Errors				*/
	const PicamRois				*region;		 /* Region of interest  */
	const PicamParameter		*paramsFailed;	 /* Failed to commit    */
	piint						failCount;		 /* Count of failed	    */
	const PicamRoisConstraint  *constraint;		 /* Constraints			*/
	
	/* Variables to compute central region in image */
	//	piint halfHeight, halfWidth, totalWidth, totalHeight;
	piint totalWidth, totalHeight;

	/* Get dimensional constraints */
	err = Picam_GetParameterRoisConstraint(	camera, 
											PicamParameter_Rois, 
											PicamConstraintCategory_Required, 
											&constraint);	
	/* Error check */
	if (err == PicamError_None)
	{		
		/* Get width and height from constraints */
		totalWidth = (piint)constraint->width_constraint.maximum;
		totalHeight= (piint)constraint->height_constraint.maximum;

		if( x0 > -1 && x0 < totalWidth && dx > 0 && x0 + dx < totalWidth +1 && y0 > -1 && y0 < totalHeight && dy > 0 && y0 + dy < totalHeight + 1)
			std::cout << "Valid ROI choice. " << std::endl;
		else
		{
			std::cout << "ERROR: Invalid ROI choice. " << std::endl;
			if( x0 <0 )
				std::cout << "Choice of x0 (" << x0 << ") is invalid.  x0 must be between the values of 0 and " << totalWidth - 1<< std::endl;
			if( y0 < 0 )
				std::cout << "Choice of y0 (" << y0 << ") is invalid.  y0 must be between the values of 0 and " << totalHeight - 1 << std::endl;
			if( x0 > totalWidth -1 )
				std::cout << "Choice of x0 (" << x0 << ") is invalid.  x0 must be between the values of 0 and " << totalWidth - 1 << std::endl;
			if( y0 > totalHeight - 1)
				std::cout << "Choice of y0 (" << y0 << ") is invalid.  y0 must be between the values of 0 and " << totalHeight - 1 << std::endl;
			if( dx < 1)
				std::cout << "Choice of dx (" << dx << ") is invalid.  dx must be between the values of 1 and " << totalWidth << std::endl;
			if( dy < 1)
				std::cout << "Choice of dy (" << dy << ") is invalid.  dy must be between the values of 1 and " << totalHeight << std::endl;
			if(x0+dx > totalWidth)
				std::cout << "Invalid x-parameters.  x0 + dx (" << x0 + dx << ") must be less than or equal to sensor width (" << totalWidth  << ")" <<  std::endl;
			if(y0 + dy > totalHeight)
				std::cout << "Invalid y-parameters.  y0 + dy (" << y0 + dy << ") must be less than or equal to sensor height (" << totalHeight << ")" << std::endl;
		}

		/* Clean up constraints after using constraints */
		Picam_DestroyRoisConstraints(constraint);


		/* Get the orinal ROI */
		err = Picam_GetParameterRoisValue(	camera, 
											PicamParameter_Rois, 
											&region);
		/* Error check */
		if (err == PicamError_None)
		{
			/* Modify the region */
			if (region->roi_count == 1) 
			{
				/* The absolute size of the ROI */
				// region->roi_array[0].height		= halfHeight;
				// region->roi_array[0].width		= halfWidth;
				region->roi_array[0].height		= dy;
				region->roi_array[0].width		= dx;

				/* The offset into the chip of the ROI (1/4th) */
				// region->roi_array[0].x			= halfWidth  / 2;
				// region->roi_array[0].y			= halfHeight / 2;
				region->roi_array[0].x			= x0;
				region->roi_array[0].y			= y0;

				/* The vertical and horizontal binning */
				// region->roi_array[0].x_binning	= 1;
				// region->roi_array[0].y_binning	= 1;
				region->roi_array[0].x_binning	= 1;
				region->roi_array[0].y_binning	= 1;

				// Print our ROI settings
				std::cout << "x0 = : " << region->roi_array[0].x << std::endl;
				std::cout << "y0 = : " << region->roi_array[0].y << std::endl;
				std::cout << "dx = : " << region->roi_array[0].width << std::endl;
				std::cout << "dy = : " << region->roi_array[0].height << std::endl;
				std::cout << "xbin = : " << region->roi_array[0].x_binning << std::endl;
				std::cout << "ybin = : " << region->roi_array[0].y_binning << std::endl;
				std::cout << "NFrames = : " << NFrames << std::endl;
			}

			std::cout << "Setting ROIs: ";
			/* Set the region of interest */
			err = Picam_SetParameterRoisValue(	camera, 
												PicamParameter_Rois, 
												region);
			/* Error check */
			if (err == PicamError_None)
			{
				/* Commit ROI to hardware */
				err = Picam_CommitParameters(	camera, 
												&paramsFailed, 
												&failCount);
                Picam_DestroyParameters(paramsFailed);
				PrintError(err);

				/* Error check */
				if (err == PicamError_None)
				{
					piint readoutstride = 0;
					Picam_GetParameterIntegerValue( camera, PicamParameter_ReadoutStride, &readoutstride );

					std::cout << "ReadoutStride is: " << readoutstride << "\n";

					piflt readoutTime = 0;
					err = Picam_GetParameterFloatingPointValue( camera, PicamParameter_ReadoutTimeCalculation, &readoutTime );
					if(err == PicamError_None)
						std::cout << "Readout time is: " << readoutTime << "msec" << std::endl;
					else
						std::cout << "Error getting readoutTime." << std::endl;

					/* Acquire 1 frame of data with a timeout */
					err = Picam_Acquire(camera, NFrames, NO_TIMEOUT, &dataFrame, &acqErrors);
					if (err  == PicamError_None) 
					{
						/* Get the bit depth */
						piint depth;
						Picam_GetParameterIntegerValue(	camera, 
														PicamParameter_PixelBitDepth,  
														&depth);	



						const char * FullFilePathChar  = FullFilePath.c_str();
						FILE *pFile;
						pFile = fopen( FullFilePathChar, "wb");

						if( pFile )
						{
							std::cout << "Opened file successfully.  Preparing to write \n";
							fwrite( dataFrame.initial_readout, 1, NFrames * (readoutstride), pFile );
							fclose( pFile );
						}
						else
						{
							std::cout << "FAILED TO OPEN FILE: " << FullFilePathChar << " \n";
						}
					}
					PrintError(err);
				}				
			}	
			/* Free the regions */
			Picam_DestroyRois(region);
		}
	} 	
}

int main(int argc, char *argv[])
{
	
	// Handle arguments.  Convert string types to int types.
	string FileDir = string(argv[1]);
	string FileName = string(argv[2]);

	string x0Str = string(argv[3]);
	int x0 = atoi(x0Str.c_str());

	string y0Str = string(argv[4]);
	int y0 = atoi(y0Str.c_str());

	string dxStr = string(argv[5]);
	int dx = atoi(dxStr.c_str());

	string dyStr = string(argv[6]);
	int dy = atoi(dyStr.c_str());

	string dtStr = string(argv[7]);
	// int dt = atoi(dtStr.c_str());
	piflt dt = ::atof(dtStr.c_str());

	string NFramesStr = string(argv[8]);
	int NFrames = atoi(NFramesStr.c_str());

	if(argc == 9)
	{
		std::cout << "============" << std::endl;
		std::cout << "Inputs: " << std::endl;
		std::cout << "------------" << std::endl;
		cout << "FileDir: " << FileDir << "\n";
		cout << "Filename: " << FileName << "\n";
		cout << "x0: " << x0 << "\n";
		cout << "y0: " << y0 << "\n";
		cout << "dx: " << dx << "\n";
		cout << "dy: " << dy << "\n";
		cout << "dt: " << dt << "\n";

		// Echo all of the arguments so compiler doesn't whine about unused arguments
//		cout << FileDir << FileName << x0 << y0 << dx << dy << dt << NFrames << "\n";

		std::string FullFilePath = FileDir + FileName;
		cout << "Full File Path: " << FullFilePath << "\n";
	}
	else
	{
		cout << "Incorrect Number of Arguments.  Expecting 8 arguments: Save Directory, Save Name, x-pixel start, y-pixel start, Nx, Ny, exposure time (msec), NFrames\n";
	}

	// Construct full file path.
	std::string FullFilePath = FileDir + FileName;

    // - set formatting options.  Not sure what this does.
    std::cout << std::boolalpha;

    Picam_InitializeLibrary();

    // - open the first camera if any or create a demo camera
    PicamHandle camera;
    PicamCameraID id;
    if( Picam_OpenFirstCamera( &camera ) == PicamError_None )
        Picam_GetCameraID( camera, &id );
    else
    {
		
        Picam_ConnectDemoCamera(
            PicamModel_Pixis1024BR,
            "12345",
            &id );
		
		Picam_OpenCamera( &id, &camera );
		
		
    }
    PrintCameraID( id );
    std::cout << std::endl;


    std::cout << "Configuration" << std::endl
              << "=============" << std::endl;
    Configure( camera, dt);
	std::cout << std::endl;

    std::cout << "Temperature" << std::endl
              << "===========" << std::endl;
    ReadTemperature( camera );
    std::cout << std::endl;

	// std::cout << "Acquisition" << std::endl
    //          << "=============" << std::endl;
    // Acquire( camera, FullFilePath, NFrames );
    // std::cout << std::endl;

	std::cout << "ROI Acquisition" << std::endl
			  << "=============" << std::endl;
	AcquireROI(camera, FullFilePath, x0, y0, dx, dy, NFrames);
	std::cout << std::endl;


    Picam_CloseCamera( camera );
    Picam_UninitializeLibrary();
}





