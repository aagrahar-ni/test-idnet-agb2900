/**********************************************************************************
 *  Copyright 2011-2021 National Instruments Corporation.  All Rights Reserved.   *
 **********************************************************************************/

/*****************************************************************************
 *  Agilent Technologies B2900 Series Precision Source/Measure Unit Instrument 
 *  Driver Sample Program.
 * 
 *  Developed with LabWindows/CVI 9.0
 *  Original Release: July 2011
 *
 *  Purpose:  This sample program initializes the instrument, configure the
 *            source to output sweep voltage.
 *
 *            To run this sample program, do the following:
 *            1) Create a new project in LabWindows/CVI.
 *            2) Add this file to the project.  To add files to the project,
 *               select the Add Files To Project from the Edit menu of the
 *               Project window.
 *            3) Add one of the following files to the project:
 *               agb2900.fp, agb2900.c, or agb2900.lib.
 *            4) Check that the resource name used in agb2900_Initialize()
 *               is correct. If simulating, change flag to simulate.
 *            5) Run the project.
 *
 *            VCC or Borland Users:
 *            This example uses CVI's MessagePopup() to display the
 *            measured value.  Replace this MessagePopup() function with a
 *            printf() or similar display function.  Remove the include
 *            statement: #include <userint.h> from the source code.  Follow
 *            the directions for CVI users given above.
 *****************************************************************************/

#include <stdio.h>
#include <userint.h>
#include "agb2900.h"


/* -------------------------------------------------------------------------- */
/* ================================= Macros ================================= */
/* -------------------------------------------------------------------------- */
#ifndef CheckErr
#define CheckErr(fCall) \
        if (error = (fCall), (error = (error < 0) ? error : VI_SUCCESS)) \
        { \
            goto Error; \
        } \
        else
#else
    #error Macro: CheckErr re-definition.
#endif


/* -------------------------------------------------------------------------- */
/* ============================== Pre-processors ============================ */
/* -------------------------------------------------------------------------- */
#define AGB2900_EXAMPLE_LAN_RES_ADDR "B2912B" 


/* -------------------------------------------------------------------------- */
/* ============================== Main Entry ================================ */
/* -------------------------------------------------------------------------- */
int main()
{
    ViSession agb2900;
    ViStatus  error = VI_SUCCESS;

    ViChar    szMsg[1024];
    ViChar    *pcMsg = szMsg;
    ViReal64  rOutputValues[14];
    ViBoolean LXITriggers[8] = {VI_FALSE, VI_FALSE, VI_FALSE, VI_FALSE,
	                            VI_FALSE, VI_FALSE, VI_FALSE, VI_FALSE};
	ViInt32 i = 0;
	
    /* Initialize */
    CheckErr(agb2900_Initialize(AGB2900_EXAMPLE_LAN_RES_ADDR,
                                  VI_TRUE, VI_TRUE,
                                  &agb2900));
								  
	/* Configure the output protection */
	CheckErr(agb2900_ConfigureOutputProtection(agb2900,
									           AGB2900_INSTRUMENT_CHANNEL_1, 
                                               VI_TRUE, 
                                               20, 
   											   0.1));

    /* Configure the source */
    CheckErr(agb2900_ConfigureSource(agb2900,
                                      AGB2900_INSTRUMENT_CHANNEL_1,
									  AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,
									  AGB2900_SOURCE_OUTPUT_FUNCTION_DC,
									  20,
									  20,
									  20,
									  VI_TRUE));							  
	CheckErr(agb2900_ConfigureSourceAutoRange(agb2900,
											   AGB2900_INSTRUMENT_CHANNEL_1,
											   AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,
											   VI_TRUE,
											   20));

    /* Configure the sweep */
	CheckErr(agb2900_ConfigureSweep(agb2900,
                                     AGB2900_INSTRUMENT_CHANNEL_1,
									 AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,
									 VI_TRUE,
									 3,
									 10,
									 7,
									 AGB2900_SWEEP_MODE_LINEAR_SINGLE,
									 AGB2900_SWEEP_DIRECTION_UP));	

    /* Configure the measurement */
	CheckErr(agb2900_ConfigureMeasurement(agb2900,
                                           AGB2900_INSTRUMENT_CHANNEL_1,
										   AGB2900_MEASUREMENT_MODE_VOLTAGE,
										   20,
										   VI_FALSE,
										   0.1,
										   AGB2900_MEASUREMENT_RESISTANCE_MODE_MANUAL,
										   VI_TRUE));
										   
    /* Configure the measurement auto range*/
	CheckErr(agb2900_ConfigureMeasurementAutoRange(agb2900,
                                                    AGB2900_INSTRUMENT_CHANNEL_1,
										            AGB2900_MEASUREMENT_MODE_VOLTAGE,
												    VI_TRUE,
													0.2,
													200,
													90,
													AGB2900_AUTO_RANGE_MODE_NORMAL));
													
    /* Configure the trigger layer */
	CheckErr(agb2900_ConfigureTrigger(agb2900,
									   AGB2900_INSTRUMENT_CHANNEL_1,
									   AGB2900_DEVICE_ACTION_BOTH,
									   AGB2900_TRIGGER_SOURCE_AINT, 
									   14,
									   0,
									   5E-5, 
									   VI_FALSE,
									   LXITriggers,
									   8));													
											   
    /* Enable the output */
    CheckErr(agb2900_EnableOutput(agb2900, AGB2900_INSTRUMENT_CHANNEL_1, VI_TRUE));

    /* Initiate the measurement */
	CheckErr(agb2900_Initiate(agb2900,
                               AGB2900_INSTRUMENT_CHANNEL_1,
							   AGB2900_DEVICE_ACTION_BOTH));
							   
    /* Wait For Operation Complete */
	CheckErr(agb2900_WaitForOperationComplete(agb2900,
                                              AGB2900_INSTRUMENT_CHANNEL_1,
											  1E4));	
    /* Fetch the actual voltage */
    CheckErr(agb2900_FetchMeasurementMultiPoints(agb2900,
                                                  AGB2900_INSTRUMENT_CHANNEL_1,
												  AGB2900_DATA_MODE_VOLTAGE,
												  14,
												  rOutputValues));
												  
    pcMsg += sprintf(pcMsg, "Source unit is outputing sweep voltage. The actual voltage levels(V):");
	for(i = 0;i < 14;i++)
	{
        if(i%7 == 0)
        {
            pcMsg += sprintf(pcMsg, "\n"); 
        }
		pcMsg += sprintf(pcMsg, "%lf,", rOutputValues[i]); 
	}
	pcMsg += sprintf(pcMsg, "\nPress <Enter> to continue");

    /* Display a message as the finish */
    MessagePopup("Message", szMsg);

Error:
    if(VI_SUCCESS != error)
    {
        ViChar   szErrMsg[2048];
        ViChar * pcErrMsg = szErrMsg;
        ViInt32  iErrCode;

        /* Retrieve the error and display it */
        if(error == AGB2900_ERROR_INSTRUMENT_ERROR)
        {
            pcErrMsg += sprintf(pcErrMsg, "Instrument Error: ");
            agb2900_ErrorQuery(agb2900, &iErrCode, pcErrMsg);
        }
        else
            agb2900_ErrorMessage(agb2900, error, szErrMsg);

        MessagePopup("Error!", szErrMsg);
    }

    if(agb2900)
        agb2900_Close(agb2900);

    return 0;
}
