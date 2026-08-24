/**********************************************************************************
 *  Copyright 2011-2021 National Instruments Corporation.  All Rights Reserved.   *
 **********************************************************************************/

/*****************************************************************************
 *  Agilent Technologies B2900 Series Precision Source/Measure Unit Instrument 
 *  Driver Sample Program.
 * 
 *  Developed with LabWindows/CVI 12.0
 *  Original Release: Nov 2012
 *
 *  Purpose:  This sample program initializes the instrument, configure 
 *            the instrument's output user defined waveform.
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
 *  Note:     This example is only supported by B2961B, B2962B, B2961B and B2962B.
 *********************************************************************************/

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
#define AGB2900_EXAMPLE_LAN_RES_ADDR "B2961B"


/* -------------------------------------------------------------------------- */
/* ============================== Main Entry ================================ */
/* -------------------------------------------------------------------------- */
int main()
{
    ViSession agb2900;
    ViStatus  error = VI_SUCCESS;

    ViChar    szMsg[1024];
	ViReal64 ListOutputData[4] = {0.1, 0.2, 0.3, 0.1};
	ViBoolean ListTriggerOutput[4] = {VI_TRUE,VI_TRUE,VI_FALSE,VI_TRUE};


    /* Initialize */
    CheckErr(agb2900_Initialize(AGB2900_EXAMPLE_LAN_RES_ADDR,
                                  VI_TRUE, VI_TRUE,
                                  &agb2900));
	/* Configure the output protection */
	CheckErr(agb2900_ConfigureOutputProtection(agb2900,
									           AGB2900_INSTRUMENT_CHANNEL_1, 
                                               VI_TRUE, 
                                               1E-4, 
   											   2));

    /* Configure the source to output constant 2V */
    CheckErr(agb2900_ConfigureSource(agb2900,
                                      AGB2900_INSTRUMENT_CHANNEL_1,
									  AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,
									  AGB2900_SOURCE_OUTPUT_FUNCTION_DC,
									  2,
									  2,
									  2,
									  VI_TRUE));
   /* Configure the user defined waveform */ 	
    CheckErr(agb2900_ConfigureWaveformUserDefined(agb2900,
		   							  			  AGB2900_INSTRUMENT_CHANNEL_1,
												  AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,
												  1E-5,
												  AGB2900_LIST_OUTPUT_DATA_MODE_NEW,
												  ListOutputData,
												  4,
												  VI_TRUE,
												  AGB2900_TRIGGER_OUTPUTPORT_EXT1,
												  AGB2900_LIST_TRIGGER_OUTPUT_MODE_NEW,
												  ListTriggerOutput,
												  4));
	/* Enable the output */
    CheckErr(agb2900_EnableOutput(agb2900, AGB2900_INSTRUMENT_CHANNEL_1, VI_TRUE));
	
	sprintf(szMsg, "Press <Enter> to continue");
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
