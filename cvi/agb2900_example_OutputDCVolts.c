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
 *            source to output constant voltage.
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
    ViReal64  rOutputValue;

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
	CheckErr(agb2900_ConfigureSourceAutoRange(agb2900,
											   AGB2900_INSTRUMENT_CHANNEL_1,
											   AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,
											   VI_TRUE,
											   2));
    /* Enable the output */
    CheckErr(agb2900_EnableOutput(agb2900, AGB2900_INSTRUMENT_CHANNEL_1, VI_TRUE));

    /* Read the output */
    CheckErr(agb2900_ReadSpotMeasurement(agb2900, AGB2900_INSTRUMENT_CHANNEL_1, 
										 AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE, &rOutputValue));

    sprintf(szMsg, "Source unit is outputing constant %lfV\nPress <Enter> to continue", rOutputValue);

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
