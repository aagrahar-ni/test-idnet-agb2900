/*****************************************************************************
 *  2011-2021, National Instruments, Corporation.  All Rights Reserved.      *
 *****************************************************************************/

/*****************************************************************************
 *  Agilent Technologies B2900 Series Precision Source/Measure Unit Instrument Driver                               
 *  LabWindows/CVI9.0 Instrument Driver                                     
 *  Original Release:                              
 *  By: Jiang Li, National Instruments, Shanghai, China                              
 *                                    
 *                                                                           
 *  Modification History:                                                    
 *     May 2011  - Instrument Driver Created.  
 *     Nov 2012  - Modified by Xudong Yong, National Instruments, Shanghai, China
 *                 Add B2961A and B2962A support
 *     Apr 2013  - Modified by Xudong Yong, National Instruments, Shanghai, China
 *                 Add notes for Arbitrary Waveform group
 *     Mar 2021  - Modified by  Shashikumar BS, NI, Banglore, India 
 *     May 2026  - Modified by Anurag Agrahari, NI, Banglore, India
 *                Add model support for B2901CL, B2901C, B2902C, B2910CL, B2911C, B2912C, B2961C, B2962C models.
 *****************************************************************************/

#include <utility.h>
#include <visa.h>
#include <ansi_c.h>
#include "agb2900.h"

/* ========================================================================== */
/* --------------------------- Pre-processors ------------------------------- */
/* ========================================================================== */
#define AGB2900_REVISION         "Rev 1.2, 03/2021, CVI 12.0"  /* Instrument driver revision */
#define AGB2900_MANF_ID          0x0                           /* Instrument manufacturer ID */
#define AGB2900_MODEL_CODE       0x0                           /* Instrument model code      */
#define BUFFER_SIZE              64L                           /* I/O buffer size            */
#define BUFFER_SIZE_LARGE        512L                          /* Large buffer size          */
#define MSG_BUF_SIZE             256L                          /* IVI message buffer size    */

#define RECTIFY_CMD(str)         str"\n"

/* -------------------------------------------------------------------------- */
/*                        Instrument related constants                        */
/* -------------------------------------------------------------------------- */
#define AGB2900_SAVE_RECALL_STATE_NUM_MIN                            (0)
#define AGB2900_SAVE_RECALL_STATE_NUM_MAX                            (9)

#define AGB2900_TRIGGER_COUNT_MIN                                    (1)
#define AGB2900_TRIGGER_COUNT_MAX                                    (100000)
#define AGB2900_TRIGGER_DELAY_MIN                                    (0)
#define AGB2900_TRIGGER_DELAY_MAX                                    (100000)
#define AGB2900_TRIGGER_TIMER_INTERVAL_MIN                           (1E-5)
#define AGB2900_TRIGGER_TIMER_INTERVAL_MAX                           (1E5)

#define  AGB2900_PATTERN_BITS_MIN                                    (0)
#define  AGB2900_PATTERN_BITS_MAX                                    (16383)

#define  AGB2900_LIMIT_TEST_NUMBER_MIN                               (1)
#define  AGB2900_LIMIT_TEST_NUMBER_MAX                               (12)

#define  AGB2900_LIMIT_TEST_LIMIT_MIN                                (-9.999999E+20)
#define  AGB2900_LIMIT_TEST_LIMIT_MAX                                (9.999999E+20)

#define  AGB2900_VOLTAGE_RANGE_MIN                                   (-210)
#define  AGB2900_VOLTAGE_RANGE_MAX                                   (210)

#define  AGB2900_CURRENT_RANGE_MIN                                   (-3.03)
#define  AGB2900_CURRENT_RANGE_MAX                                   (3.03)

#define  AGB2900_MEASURMENT_RESISTANCE_RANGE_MIN                     (2)
#define  AGB2900_MEASURMENT_RESISTANCE_RANGE_MAX                     (2E8)

#define  AGB2900_POWER_LIFE_CYCLE_VALUE_MIN                          (4.8E-4)
#define  AGB2900_POWER_LIFE_CYCLE_VALUE_MAX                          (120)

#define  AGB2900_AUTO_RANGE_THRESHOLD_RATE_MIN                       (11)
#define  AGB2900_AUTO_RANGE_THRESHOLD_RATE_MAX                       (90)

#define  AGB2900_WAIT_TIME_OFFSET_MIN                                (0)
#define  AGB2900_WAIT_TIME_OFFSET_MAX                                (1)

#define  AGB2900_WAIT_TIME_GAIN_MIN                                  (0)
#define  AGB2900_WAIT_TIME_GAIN_MAX                                  (100)

#define  AGB2900_TRACE_BUFFER_SIZE_MIN                               (1)
#define  AGB2900_TRACE_BUFFER_SIZE_MAX                               (100000)

#define  AGB2900_PIN_OUTPUT_DATA_MIN                                 (0)
#define  AGB2900_PIN_OUTPUT_DATA_MAX                                 (16383)

#define  AGB2900_PIN_NUMBER_MIN                                      (1)
#define  AGB2900_PIN_NUMBER_MAX                                      (14)

#define  AGB2900_TRIGGER_PLUS_WIDTH_MIN                              (1E-5)
#define  AGB2900_TRIGGER_PLUS_WIDTH_MAX                              (1E-2)

#define  AGB2900_SOURCE_PLUS_WIDTH_MIN                               (5E-5)
#define  AGB2900_SOURCE_PLUS_WIDTH_MAX                               (1E5)

#define  AGB2900_SOURCE_PLUS_DELAY_MIN                               (0.0)
#define  AGB2900_SOURCE_PLUS_DELAY_MAX                               (99999.9)

#define  AGB2900_INTERLOCK_THRESHOLD_LEVEL_MIN                       (0)
#define  AGB2900_INTERLOCK_THRESHOLD_LEVEL_MAX                       (42)

#define  AGB2900_SWEEP_STEP_MIN                                      (1)
#define  AGB2900_SWEEP_STEP_MAX                                      (100000)

#define  AGB2900_CUTOFF_FREQUENCY_MIN                                (31.83)
#define  AGB2900_CUTOFF_FREQUENCY_MAX                                (31831)

#define AGB2900_LIMIT_TEST_DATA_OFFSET_MIN                           (0)	
#define AGB2900_LIMIT_TEST_DATA_SIZE_MIN                             (1)

#define AGB2900_LXI_TRIGGER_MIN                                      (1)	
#define AGB2900_LXI_TRIGGER_MAX                                      (8)

#define AGB2900_TIMEOUT_MIN                                          (1)
#define AGB2900_TIMEOUT_MAX                                          (4294967295) 

#define AGB2900_DATA_SIZE_MIN                                        (1)
#define AGB2900_DATA_SIZE_MAX                                        (4294967295) 

#define AGB2900_WAVE_NUMBER_MIN                                      (1)
#define AGB2900_WAVE_NUMBER_MAX                                      (10000)

#define AGB2900_TIME_RANGE_MIN                                       (0)
#define AGB2900_TIME_RANGE_MAX                                       (1000)

#define AGB2900_STEP_TIME_MIN                                        (1E-5)
#define AGB2900_STEP_TIME_MAX                                        (1000)

#define AGB2900_FREQUENCY_MIN                                        (1E-3)
#define AGB2900_FREQUENCY_MAX                                        (1E4)

#define AGB2900_PHASE_MARKER_MIN                                     (0)
#define AGB2900_PHASE_MARKER_MAX                                     (360)

#define AGB2900_SHUNT_RESISTANCE_LEVEL_MIN                           (0.2)
#define AGB2900_SHUNT_RESISTANCE_LEVEL_MAX                           (2E9)

#define AGB2900_SERIES_RESISTANCE_LEVEL_MIN                          (-2E5)
#define AGB2900_SERIES_RESISTANCE_LEVEL_MAX                          (+2E5)

/* ========================================================================== */
/* --------------------------- Macro Functions ------------------------------ */
/* ========================================================================== */

/* --------------------------- Error Checking ------------------------------- */
#ifndef CheckErr
#define CheckErr(fCall) \
        if (status = (fCall), (status = (status < 0) ? status : VI_SUCCESS)) \
        { \
            goto Error; \
        } \
        else
#else
    #error Validation Macro: CheckErr re-definition.
#endif

#ifndef CheckErrRaw
#define CheckErrRaw(fCall) \
        status = (fCall), (status = (status < 0) ? status : VI_SUCCESS)
#else
    #error Validation Macro: CheckErrRaw re-definition.
#endif

/* --------------------------- Invalid Param Reporting ---------------------- */
#ifndef CheckParam
#define CheckParam(fCall, errorCode) \
        if (fCall) \
        { \
            status = (errorCode); \
            goto Error; \
        } \
        else
#else
    #error Validation Macro: CheckParam re-definition.
#endif

/* --------------------------- Boolean value validation --------------------- */
#ifndef IsInvalidBooleanVal
    #define IsInvalidBooleanVal(bValue) (bValue != VI_TRUE && bValue != VI_FALSE)
#else
    #error Validation Macro: IsInvalidBooleanVal re-definition.
#endif

/* --------------------------- Numeric value validation --------------------- */
#ifndef IsInvalidNumericVal
    #define IsInvalidNumericVal(val, min, max) (val < min || val > max)
#else
    #error Validation Macro: IsInvalidNumericVal re-definition.
#endif

/* --------------------------- Pointer validation --------------------------- */
#ifndef IsInvalidPtr
    #define IsInvalidPtr(ptr) (ptr == NULL)
#else
    #error Validation Macro: IsInvalidPtr re-definition.
#endif

/* --------------------------- Utilities ------------------------------------ */
#ifndef IsDoingLowResMeasurement
    #define IsDoingLowResMeasurement() \
            (gbDoingLowResMeasure == VI_TRUE ? AGB2900_ERROR_DC_PWR_LOCKED : VI_SUCCESS)
#else
    #error Functional Macro: IsDoingLowResMeasurement re-definition.
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifndef ArraySize
#define ArraySize(array) \
        (sizeof(array) / sizeof(array[0]))
#else
    #error Macro: ArraySize re-definition.
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifndef SetNewTimeout
    #define SetNewTimeout(iNew, piOld) \
            CheckErr(viGetAttribute(vi, VI_ATTR_TMO_VALUE, piOld)); \
            CheckErr(viSetAttribute(vi, VI_ATTR_TMO_VALUE, iNew < 0 ? 1 : iNew))
#else
    #error Functional Macro: SetNewTimeout re-definition.
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifndef SetOldTimeout
    #define SetOldTimeout(old) \
            CheckErr(viSetAttribute(vi, VI_ATTR_TMO_VALUE, old))
#else
    #error Functional Macro: SetOldTimeout re-definition.
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Invoke CheckStatus anyway to ensure the event register is cleared after a  *
 * command execution so that its status will not impact the following command *
 * execution. If the status before calling CheckStatus is already with ERROR, *
 * returns the status as what it is. Otherwise, the result of CheckStatus will*
 * be returned.                                                               */
#ifndef VerifyStatus
    #define VerifyStatus(status) \
            if(vi != VI_NULL) \
            {\
                ViStatus error = CheckStatus(vi); \
                if(status == VI_SUCCESS) \
                    status = error; \
            }
#else
    #error Functional Macro: VerifyStatus re-definition.
#endif

/* ========================================================================== */
/* --------------------------- Instrument Commands -------------------------- */
/* ========================================================================== */
static ViString gpstrSaveRecallActions[]   = {"*SAV", "*RCL"};

static ViString gpstrSwitchTable[]         = {"OFF", "ON"};

static ViString gpstrChanList[]            = {"", "(@1)", "(@2)", "(@1,2)","(@1:2)","(@2,1)","(@2:1)"};

static ViString gpstrDeviceAction[]        = {"ACQ", "TRAN", "ALL"}; 

static ViString gpstrTriggerSource[]       = {"AINT", "BUS", "TIMER", "INT1", "INT2",
                                              "LAN", "EXT1", "EXT2", "EXT1", "EXT3",
                                              "EXT4", "EXT5", "EXT6", "EXT7", "EXT8",
                                              "EXT9", "EXT10", "EXT11", "EXT12", 
                                              "EXT13", "EXT14"};
											 
static ViString gpstrTriggerSignal[]       = {"EXT1", "EXT2", "EXT1", "EXT3", "EXT4",
                                              "EXT5", "EXT6", "EXT7", "EXT8", "EXT9",
                                              "EXT10", "EXT11", "EXT12", "EXT13", 
                                              "EXT14", "LAN", "INT1", "INT2"};											 

static ViString gpstrTriggerBypass[]       = {"ONCE", "OFF"};

static ViString gpstrTriggerTiming[]       = {"BEF", "AFT", "BOTH"};

static ViString gpstrTriggerLocation[]     = {"TRIG", "ARM", "SENS", "SOUR"};

static ViString gpstrOperationMode[]       = {"GRAD", "SORT"};

static ViString gpstrResultUpdateMode[]    = {"IMM", "END"};

static ViString gpstrTestType[]            = {"COMP", "LIM"};

static ViString gpstrFeedMode[]            = {"VOLT", "CURR", "RES", "MATH"};

static ViString gpstrJudgementCriteria[]   = {"IN", "OUT"};

static ViString gpstrMeasurementMode[]     = {"VOLT", "CURR", "RES"};

static ViString gpstrResistanceMode[]      = {"MAN", "AUTO"};

static ViString gpstrAutoRangeMode[]       = {"NORM", "RES", "SPE"};

static ViString gpstrWaitTimeMode[]        = {"SOUR", "SENS"};

static ViString gpstrBufferDataType[]      = {"MATH", "LIM", "SENS"};

static ViString gpstrBufferControl[]       = {"NEXT", "NEV"};

static ViString gpstrTimeRule[]            = {"DELT", "ABS"};

static ViString gpstrIOFuction[]           = {"DINP", "DIO", "HVOL", "TINP", "TOUT"};

static ViString gpstrFunctionPolarity[]    = {"POS", "NEG"};

static ViString gpstrTriggerType[]         = {"EDGE", "LEV"};

static ViString gpstrPinNumber[]           = {"EXT1", "EXT2", "EXT1", "EXT3", "EXT4",
                                              "EXT5", "EXT6", "EXT7", "EXT8", "EXT9",
                                              "EXT10", "EXT11", "EXT12", "EXT13", 
                                              "EXT14", "NONE"};	

static ViString gpstrDataMode[]            = {"VOLT","CURR", "RES", "SOUR", "STAT", 
                                              "TIME"};


static ViString gpstrStatisticalOperation[]= { "SDEV", "PKPK", "MEAN", "MIN", "MAX"};
										  
static ViString gpstrSourceOutputMode[]    = {"VOLT", "CURR"};

static ViString gpstrListSweep[]           = {"FIX", "LIST"};  

static ViString gpstrSweepScale[]          = {"LIN", "LOG"};

static ViString gpstrSweepStair[]          = {"SING", "DOUB"};

static ViString gpstrSweepDirection[]      = {"UP", "DOWN"};

static ViString gpstrSweepRangingMode[]    = {"BEST", "FIX", "AUTO"};

static ViString gpstrOutputFunction[]      = {"PLUS", "DC"};

static ViString gpstrLowTerminalState[]    = {"FLO", "GRO"};

static ViString gpstrOutputOffMode[]       = {"ZERO", "HIZ", "NORM"};

static ViString gpstrLXIEvent[]            = {"LAN0", "LAN1", "LAN2", "LAN3", 
                                              "LAN4", "LAN5", "LAN6", "LAN7"}; 

static ViString gpstrLXIEventEdge[]        = {"RISE", "FALL"}; 

static ViString gpstrLXIEventDetection[]   = {"RISE", "FALL", "HIGH", "LOW"}; 

static ViString gpstrLXIEventSource[]      = {"WaitingForAcquireArm1", "WaitingForTransitionArm1", 
                                              "WaitingForAcquireArm2", "WaitingForTransitionArm2",
                                              "WaitingForAcquireTrigger1", "WaitingForTransitionTrigger1", 
                                              "WaitingForAcquireTrigger2", "WaitingForTransitionTrigger2", 
                                              "Measuring1", "Settling1", "Measuring2", "Settling2"};  

static ViString gpstrLXIEventType[]        = {"ARM", "TRIG"};

static ViString gpstrLXIEventSlope[]       = {"NEG", "POS"};    

static ViString gpstrLXIEventDrive[]       = {"OFF", "NORM", "WOR"};

 static ViString gpstrExternalFilterType[]  = {"LNF", "ULNF" , "HCULNF"};

static ViString gpstrResistanceOperationMode[]  = {"FIX", "EMUL"};

static ViString gpstrTriggerOutputPort[]   = {"EXT1", "EXT2", "EXT3", "EXT4", "EXT5", 
                                              "EXT6", "EXT7", "EXT8", "EXT9", "EXT10", 
                                              "EXT11", "EXT12", "EXT13", "EXT14", "INT1", "INT2"}; 
static ViString gpstrWaveformShape[]       = {"EXP", "RAMP", "SIN", "SQU", "TRAP", "TRI", "UDEF"};



/* ========================================================================== */
/* --------------------------- Global Typedefs ------------------------------ */
/* ========================================================================== */
/* agb2900_StringValPair is used in the agb2900_ErrorMessage function */
typedef struct agb2900_StringValPair
{
   ViStatus stringVal;
   ViString stringName;
} agb2900_TStringValPair;


/* ========================================================================== */
/* --------------------------- Global Variables ----------------------------- */
/* ========================================================================== */
static ViBoolean gbDoingLowResMeasure = VI_FALSE;
static ViInt32 giInstrIndex = 1;    /* Instrument Index to identify single 



channel or 2 channel */
static ViInt32 userData = -1;    /* Record the model number of current instrument */


/* ========================================================================== */
/* --------------------------- Utility Functions (Non-Exported) ------------- */
/* ========================================================================== */
static ViStatus CheckStatus(ViSession vi);
static ViStatus DefaultInstrSetup(ViSession vi);
static ViStatus UpdateLowResMeasurementStatus(ViSession vi);
static ViReal64 RectifyManualRange(ViReal64 ManualRange,
                                   ViInt32  Measurement);
static void ComposeCmdForMeasurementViaPath(ViInt32   Measurement,
                                            ViBoolean AutoRange,
                                            ViReal64  ManualRange,
                                            ViInt32   AbsoluteResolution,
                                            ViChar *  CmdBuf);
static ViInt32 StringToReal64Array(ViChar  DataStr[],
                                   ViInt32 ResultDataSize,
                                   ViReal64 ResultData[],
                                   ViChar  Delimiter);
static ViChar * BuildFlattenStr(ViReal64  Array[],
                                ViInt32  iArraySize,
                                ViChar * Format,
                                ViChar * Delimiter,
                                ViChar   FlattenStr[],
                                ViInt32  BufSize);

/* ========================================================================== */
/* --------------------------- Exported APIs -------------------------------- */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*                       Initialization / Close Functions                     */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_Initialize
 * Description: Establishes communication with the instrument and optionally
 *              performs an instrument identification query and/or an instrument
 *              reset.  It also places the instrument in a default state needed
 *              for other instrument driver operations. Therefore, call this
 *              function before calling other instrument driver functions for
 *              this instrument. Generally, you need to call the this function
 *              only once at the beginning of an application.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_Initialize(ViRsrc      ResourceName,
                                     ViBoolean   IdQuery,
                                     ViBoolean   ResetDevice,
                                     ViSession * vi)
{
    ViStatus  status = VI_SUCCESS;
    ViSession rmSession = 0;
    ViChar    szBuf[BUFFER_SIZE];

    /* Param Validations */
    CheckParam(IsInvalidPtr(ResourceName), VI_ERROR_PARAMETER1);
    CheckParam(IsInvalidBooleanVal(IdQuery), VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(ResetDevice), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidPtr(vi), VI_ERROR_PARAMETER4);

    /* Open instrument session */
    CheckErr(viOpenDefaultRM(&rmSession));     

    if((status = viOpen(rmSession, ResourceName, VI_NULL, VI_NULL, vi)) < 0)
    {
        viClose(rmSession);
        return status;
    }

    /* Configure VISA formatted I/O */
    CheckErr(viSetAttribute(*vi, VI_ATTR_TMO_VALUE, 10000));
    CheckErr(viSetBuf(*vi, VI_READ_BUF|VI_WRITE_BUF, 4000));
    CheckErr(viSetAttribute(*vi, VI_ATTR_WR_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS));
    CheckErr(viSetAttribute(*vi, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS));

    /* Identification Query */
    if(IdQuery)
    {
        #define B2901A_VALID_ID_RESPONSE_STRING_START   "Agilent Technologies,B2901A" 
        #define B2902A_VALID_ID_RESPONSE_STRING_START   "Agilent Technologies,B2902A"  
		#define B2911A_VALID_ID_RESPONSE_STRING_START   "Agilent Technologies,B2911A"  
		#define B2912A_VALID_ID_RESPONSE_STRING_START   "Agilent Technologies,B2912A" 
		#define B2961A_VALID_ID_RESPONSE_STRING_START   "Agilent Technologies,B2961A"   
		#define B2962A_VALID_ID_RESPONSE_STRING_START   "Agilent Technologies,B2962A"   
		#define B2912B_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2912B"  
		#define B2911B_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2911B"  	
		#define B2902B_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2902B"  
		#define B2901B_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2901B"  
		#define B2910BL_VALID_ID_RESPONSE_STRING_START  "Keysight Technologies,B2910BL" 
		#define B2901BL_VALID_ID_RESPONSE_STRING_START  "Keysight Technologies,B2901BL" 
		#define B2961B_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2961B"	
		#define B2962B_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2962B"  
        #define B2901CL_VALID_ID_RESPONSE_STRING_START  "Keysight Technologies,B2901CL"
        #define B2901C_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2901C"
        #define B2902C_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2902C"
        #define B2910CL_VALID_ID_RESPONSE_STRING_START  "Keysight Technologies,B2910CL"
        #define B2911C_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2911C"
        #define B2912C_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2912C"
        #define B2961C_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2961C"
        #define B2962C_VALID_ID_RESPONSE_STRING_START   "Keysight Technologies,B2962C"
																					
												    
        CheckErr(viQueryf(*vi, RECTIFY_CMD("*IDN?"), "%256[^\n]", szBuf));

        /* Verify whether the instrument is valid */
		if(strncmp(szBuf, B2901A_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2901A_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 0;
		}
		else if(strncmp(szBuf, B2902A_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2902A_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 1;
		}
		else if(strncmp(szBuf, B2911A_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2911A_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 2;
		}
		else if(strncmp(szBuf, B2912A_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2912A_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 3;
		}
		else if(strncmp(szBuf, B2912B_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2912B_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 4;
			
		}
		else if(strncmp(szBuf, B2911B_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2911B_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 5;
			
		}
		else if(strncmp(szBuf, B2902B_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2902B_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 6;
			
		}
		else if(strncmp(szBuf, B2901B_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2901B_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 7;
			
		}
		else if(strncmp(szBuf, B2910BL_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2910BL_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 8;
			
		}
		else if(strncmp(szBuf, B2901BL_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2901BL_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 9;
			
		}
		else if(strncmp(szBuf, B2961B_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2961B_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 10;
			
		}
		else if(strncmp(szBuf, B2962B_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2962B_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 11;
			
		}
        else if(strncmp(szBuf, B2901CL_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2901CL_VALID_ID_RESPONSE_STRING_START)) == 0)
        {
            userData = 9;
        }
        else if(strncmp(szBuf, B2901C_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2901C_VALID_ID_RESPONSE_STRING_START)) == 0)
        {
            userData = 7;
        }
        else if(strncmp(szBuf, B2902C_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2902C_VALID_ID_RESPONSE_STRING_START)) == 0)
        {
            userData = 6;
        }
        else if(strncmp(szBuf, B2910CL_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2910CL_VALID_ID_RESPONSE_STRING_START)) == 0)
        {
            userData = 8;
        }
        else if(strncmp(szBuf, B2911C_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2911C_VALID_ID_RESPONSE_STRING_START)) == 0)
        {
            userData = 5;
        }
        else if(strncmp(szBuf, B2912C_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2912C_VALID_ID_RESPONSE_STRING_START)) == 0)
        {
            userData = 4;
        }
        else if(strncmp(szBuf, B2961C_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2961C_VALID_ID_RESPONSE_STRING_START)) == 0)
        {
            userData = 10;
        }
        else if(strncmp(szBuf, B2962C_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2962C_VALID_ID_RESPONSE_STRING_START)) == 0)
        {
            userData = 11;
        }
			else if(strncmp(szBuf, B2961A_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2961A_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 12;
		}
		else if(strncmp(szBuf, B2962A_VALID_ID_RESPONSE_STRING_START,
                   strlen(B2962A_VALID_ID_RESPONSE_STRING_START)) == 0)
		{
			userData = 13;
		}
		else
		{
			userData = -1;
		}

        if(userData==0 || userData==2 || userData==5 || userData==7 || userData==8 || userData==9 || userData==10 || userData==12 )					 
        {
            giInstrIndex = 1;	   
        }
        else if(userData==1 || userData==3 || userData==4 || userData==6 || userData==11 || userData==13 ) 
        {
            giInstrIndex = 2;
        }
        else
        {
            CheckErr(VI_ERROR_FAIL_ID_QUERY);
        }
            
    }

    /* Reset device */
    if(ResetDevice)
        CheckErr(agb2900_Reset(*vi));
    else
        CheckErr(DefaultInstrSetup(*vi));

    return VI_SUCCESS;

Error:
    if(*vi != 0)
    {
        viClose(*vi);
        *vi = 0;
    }
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_Close
 * Description: Performs an instrument error query before terminating the
 *              software connection to the instrument.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_Close(ViSession vi)
{
    return viClose(vi);
}


/* -------------------------------------------------------------------------- */
/*                             Action-Status Functions                        */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_AcquireNullOffsetForLimitTest
 * Description: Queries null offset value. It is the present available value
 *              of the latest limit test data.
 * Note:        This function is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_AcquireNullOffsetForLimitTest (ViSession vi,
                                                         ViInt32 Channel,
                                                         ViReal64 * NullOffset)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidPtr(NullOffset), VI_ERROR_PARAMETER3);
	
    CheckErr(viQueryf(vi, RECTIFY_CMD(":CALC%d:OFFS:ACQ;:CALC%d:OFFS?"), "%Le",
                      (Channel), (Channel), NullOffset));

    CheckErr(CheckStatus(vi));
Error:
    return status;    
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_DeleteMathExpression
 * Description: Deletes user-defined math expression(s).              
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_DeleteMathExpression(ViSession vi,
                                               ViInt32 Channel,
                                               ViInt32 Operation,
                                               ViConstString ExpressionName)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(Operation, AGB2900_MATH_EXPRESSION_DELETE_ONE,
                                   AGB2900_MATH_EXPRESSION_DELETE_ALL),
               VI_ERROR_PARAMETER3);
			   
    switch(Operation)
	{
        case AGB2900_MATH_EXPRESSION_DELETE_ONE:
		    CheckParam(IsInvalidPtr(ExpressionName), VI_ERROR_PARAMETER4);
		    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:MATH:DEL \"%s\""), (Channel),
                              ExpressionName)); 
		    break;
	    case AGB2900_MATH_EXPRESSION_DELETE_ALL: 
		    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:MATH:DEL:ALL"), (Channel)));
		    break;
		default:
		    break;
	}

    CheckErr(CheckStatus(vi));
Error:
    return status;												   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_EnableOutputAutoOff
 * Description: Enables or disables the automatic output off function.              
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_EnableOutputAutoOff(ViSession vi,
                                              ViInt32 Channel,
                                              ViBoolean EnableOutputAutoOff)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(EnableOutputAutoOff), VI_ERROR_PARAMETER3);
	
	CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:OFF:AUTO %s"), (Channel),
                      gpstrSwitchTable[EnableOutputAutoOff]));		

    CheckErr(CheckStatus(vi));
Error:
    return status;													  
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_EnableOutput
 * Description: Enables or disables the source output.            
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_EnableOutput(ViSession vi,
                                       ViInt32 Channel,
                                       ViBoolean EnableOutput)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(EnableOutput), VI_ERROR_PARAMETER3);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d %s"), (Channel),
                      gpstrSwitchTable[EnableOutput]));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;													  
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_EnableRemoteSensing
 * Description: Enables or disables the remote sensing. The remote sensing must
 *              be enabled to use the 4-wire connection(Kelvin connection).            
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_EnableRemoteSensing(ViSession vi,
                                              ViInt32 Channel,
                                              ViBoolean EnableRemoteSensing)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(EnableRemoteSensing), VI_ERROR_PARAMETER3);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:REM %s"), (Channel),
                      gpstrSwitchTable[EnableRemoteSensing]));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;													  
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_OutputProtectionStatus
 * Description: Queries over voltage protection (OVP) and over current 
 *              protection (OCP) status for a desired channel.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_QueryOutputProtectionStatus (ViSession vi,
                                                       ViInt32 Channel,
                                                       ViInt32 *OVPStatus,
                                                       ViInt32 *OCPStatus)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidPtr(OVPStatus), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidPtr(OCPStatus), VI_ERROR_PARAMETER4);
    CheckErr(viQueryf(vi, RECTIFY_CMD(":SENS%d:VOLT:PROT:TRIP?"),"%d",
                      (Channel),OVPStatus));
    CheckErr(viQueryf(vi, RECTIFY_CMD(":SENS%d:CURR:PROT:TRIP?"),"%d",
                      (Channel),OCPStatus));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;										 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ResetRegisters
 * Description: Sets the all defined bits in the status system's PTR registers
 *              and clears the all bits in the NTR and Enable registers. The 
 *              registers are returned to the default condition.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ResetRegisters(ViSession vi)
{
    ViStatus status = VI_SUCCESS;

    CheckErr(viPrintf(vi, RECTIFY_CMD(":STAT:PRES")));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;										 
    
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_SendSoftwareTrigger
 * Description: Generates a trigger when the ConfigureTrigger has BUS selected 
 *              as Trigger Source.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_SendSoftwareTrigger (ViSession vi)
{
    ViStatus status = VI_SUCCESS;

    CheckErr(viPrintf(vi, RECTIFY_CMD("*TRG")));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;	    
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_SendARMTrigger
 * Description: Sends an immediate arm trigger for the specified device action
 *              to the specified channel. When the status of the specified 
 *              device action is initiated, the arm trigger causes the layer
 *              change from arm to trigger. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_SendARMTrigger(ViSession vi,
                                         ViInt32 ChannelList,
                                         ViInt32 DeviceAction)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DeviceAction,AGB2900_DEVICE_ACTION_ACQUIRE,
                                   AGB2900_DEVICE_ACTION_BOTH),
               VI_ERROR_PARAMETER3);
			   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":ARM:%s %s"),gpstrDeviceAction[DeviceAction],
	                  gpstrChanList[ChannelList]));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;									 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_SendLanEvent
 * Description: Send a LAN trigger Event. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_SendLanEvent (ViSession vi,
                                        ViInt32 LXIEvent, 
                                        ViInt32 LXIEventEdge)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(LXIEvent, AGB2900_LXI_EVENT_LAN0, AGB2900_LXI_EVENT_LAN7),
               VI_ERROR_PARAMETER2);
			   
    CheckParam(IsInvalidNumericVal(LXIEventEdge, AGB2900_LXI_EVENT_EDGE_RISE,
                                   AGB2900_LXI_EVENT_EDGE_FALL),
               VI_ERROR_PARAMETER3);
			   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":LXI:EVEN:OUTP:LAN:SEND \"%s\",%s"),gpstrLXIEvent[LXIEvent],
	                  gpstrLXIEventEdge[LXIEventEdge]));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;					 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_SendImmediateTrigger
 * Description: Sends an immediate trigger for the specified device action to
 *              the specified channel. When the status of the specified device
 *              action is initiated, the trigger causes the specified device 
 *              action.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_SendImmediateTrigger(ViSession vi,
                                               ViInt32 ChannelList,
                                               ViInt32 DeviceAction)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DeviceAction,AGB2900_DEVICE_ACTION_ACQUIRE,
                                   AGB2900_DEVICE_ACTION_BOTH),
               VI_ERROR_PARAMETER3);
			   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRIG:%s %s"),gpstrDeviceAction[DeviceAction],
                      gpstrChanList[ChannelList]));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;	
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_QueryStatus
 * Description: Queries the value of the measurement, operation, or questionable
 *              status condition register.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_QueryStatus(ViSession vi,
                                      ViInt32 RegisterType,
                                      ViInt32* StatusValue)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(RegisterType, 
                                   AGB2900_REGISTER_TYPE_QUESTIONABLE_STATUS_CONDITION,
                                   AGB2900_REGISTER_TYPE_QUESTIONABLE_VOLTAGE),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidPtr(StatusValue), VI_ERROR_PARAMETER3);
	
	switch(RegisterType)
	{
		case AGB2900_REGISTER_TYPE_QUESTIONABLE_STATUS_CONDITION:
		    CheckErr(viQueryf(vi, RECTIFY_CMD(":STAT:QUES:COND?"), "%d", 
                     StatusValue));
			break;
		case AGB2900_REGISTER_TYPE_MEASUREMENT_STATUS_CONDITION:
		    CheckErr(viQueryf(vi, RECTIFY_CMD(":STAT:MEAS:COND?"), "%d", 
                     StatusValue));
			break;
		case AGB2900_REGISTER_TYPE_OPERATION_STATUS_CONDITION:
		    CheckErr(viQueryf(vi, RECTIFY_CMD(":STAT:OPER:COND?"), "%d", 
                     StatusValue));
			break;
		case AGB2900_REGISTER_TYPE_QUESTIONABLE_CALIBRATION:
		    CheckErr(viQueryf(vi, RECTIFY_CMD(":STAT:QUES:CAL:COND?"), "%d", 
                     StatusValue));
			break;
		case AGB2900_REGISTER_TYPE_QUESTIONABLE_CURRENT:
		    CheckErr(viQueryf(vi, RECTIFY_CMD(":STAT:QUES:CURR:COND?"), "%d", 
                     StatusValue));
			break;
		case AGB2900_REGISTER_TYPE_QUESTIONABLE_TEMPERATURE:
		    CheckErr(viQueryf(vi, RECTIFY_CMD(":STAT:QUES:TEMP:COND?"), "%d", 
                     StatusValue));
			break;
		case AGB2900_REGISTER_TYPE_QUESTIONABLE_TEST:
		    CheckErr(viQueryf(vi, RECTIFY_CMD(":STAT:QUES:TEST:COND?"), "%d", 
                     StatusValue));
			break;
		case AGB2900_REGISTER_TYPE_QUESTIONABLE_VOLTAGE:
		    CheckErr(viQueryf(vi, RECTIFY_CMD(":STAT:QUES:VOLT:COND?"), "%d", 
                     StatusValue));
			break;
		default:
			break;			
	}
	
    CheckErr(CheckStatus(vi));
Error:
    return status;									  
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ResetCompositeLimitTest
 * Description: Clears the composite limit test results and the GPIO lines 
 *              immediately.
 * Note:        This function is not applicable for B296x family models
 *              B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ResetCompositeLimitTest(ViSession vi,
                                                  ViInt32 Channel)											  
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
			   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:CLIM:CLE"),(Channel)));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;
}

/* -------------------------------------------------------------------------- */
/*                             Configure Functions                            */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureLXIEventOutput
 * Description: Configures the LXI output triggers for status change between 
 *              idle-arm, arm-trigger or trigger-action layer. To use LXI output 
 *              triggers, please set output trigger signal to LAN first. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureLXIEventOutput (ViSession vi,
                                                   ViInt32 LXIEvent,
                                                   ViBoolean EnableEventOutput,
                                                   ViInt32 LXIEventSource,
                                                   ViInt32 LXIEventSlope,
                                                   ViInt32 LXIEventDrive,
                                                   ViConstString LXIEventDestination)
{
    ViStatus status = VI_SUCCESS;
		
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(LXIEvent, AGB2900_LXI_EVENT_LAN0, AGB2900_LXI_EVENT_LAN7),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(EnableEventOutput), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(LXIEventSource,AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_ACQUIRE_ARM1,
                                   AGB2900_LXI_EVENT_SOURCE_SETTLING2),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(LXIEventSlope, AGB2900_LXI_EVENT_SLOPE_NEGATIVE,
                                   AGB2900_LXI_EVENT_SLOPE_POSITIVE),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(LXIEventDrive,AGB2900_LXI_EVENT_DRIVE_OFF,
                                   AGB2900_LXI_EVENT_DRIVE_WIRED_OR),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidPtr(LXIEventDestination), VI_ERROR_PARAMETER7);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":LXI:EVEN:OUTP:LAN:CONF \"%s\",%s,\"%s\",%s,%s,\"%s\""),
                      gpstrLXIEvent[LXIEvent], gpstrSwitchTable[EnableEventOutput],
                      gpstrLXIEventSource[LXIEventSource], gpstrLXIEventSlope[LXIEventSlope], 
                      gpstrLXIEventDrive[LXIEventDrive], LXIEventDestination));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;													  
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureLXIForARMTrigger
 * Description: Configures the LXI Trigger Event for trigger layer or ARM layer. 
 *              To use LXI Triggers, set the trigger source to LAN first. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureLXIForARMTrigger (ViSession vi,
                                                     ViInt32 LXIEvent,
                                                     ViBoolean EnableLXIEvent,
                                                     ViInt32 LXIEventType,
                                                     ViInt32 LXIEventDetection,
                                                     ViInt32 LXIEventSource,
                                                     ViConstString LXIEventDestination)
{
    ViStatus status = VI_SUCCESS;
		
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(LXIEvent, AGB2900_LXI_EVENT_LAN0, AGB2900_LXI_EVENT_LAN7),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(EnableLXIEvent), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(LXIEventType, AGB2900_LXI_EVENT_TYPE_ARM,
                                   AGB2900_LXI_EVENT_TYPE_TRIGGER),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(LXIEventDetection,AGB2900_LXI_EVENT_DETECTION_RISE,
                                   AGB2900_LXI_EVENT_DETECTION_LOW),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(LXIEventSource,AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_ACQUIRE_ARM1,
                                   AGB2900_LXI_EVENT_SOURCE_SETTLING2),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidPtr(LXIEventDestination), VI_ERROR_PARAMETER7);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD("%s:LXI:LAN:CONF \"%s\",%s,%s,\"%s\",\"%s\""),
                      gpstrLXIEventType[LXIEventType], gpstrLXIEvent[LXIEvent], 
                      gpstrSwitchTable[EnableLXIEvent], gpstrLXIEventDetection[LXIEventDetection],  
                      gpstrLXIEventSource[LXIEventSource], LXIEventDestination));
	
    CheckErr(CheckStatus(vi));
Error:
    return status;													   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureInterlockThresholdLevel
 * Description: Configures the interlock threshold voltage level.
 * Note:        This function is only supported by C-series models:
 *              B2901CL, B2901C, B2902C, B2910CL, B2911C, B2912C, B2961C, B2962C
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureInterlockThresholdLevel(ViSession vi,
                                                           ViReal64 InterlockThresholdLevel)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */
    CheckParam(!(userData >= 4 && userData <= 11), AGB2900_ERROR_INSTRUMENT_ERROR);
    CheckParam(IsInvalidNumericVal(InterlockThresholdLevel,
                                   AGB2900_INTERLOCK_THRESHOLD_LEVEL_MIN,
                                   AGB2900_INTERLOCK_THRESHOLD_LEVEL_MAX),
               VI_ERROR_PARAMETER2);

    CheckErr(viPrintf(vi, RECTIFY_CMD(":SYST:INT:VOLT %Le"), InterlockThresholdLevel));

    CheckErr(CheckStatus(vi));
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureArmLayerTrigger
 * Description: Configures settings for ARM layer Trigger in ARM-Trigger model.
 *              B2900 supports the ARM-Trigger model described in 1999 SCPI 
 *              Command Reference. The ARM-Trigger model shows independent 
 *              event detection for the ARM and Trigger layers. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureARMLayerTrigger(ViSession vi,
                                                   ViInt32 Channel,
                                                   ViInt32 DeviceAction,
                                                   ViInt32 ArmTriggerSource, 
                                                   ViInt32 ArmCount,
                                                   ViInt32 ArmDelay,
                                                   ViReal64 ArmInterval, 
                                                   ViBoolean ArmBypassEnabled,
                                                   ViBoolean LXITriggers[],
                                                   ViInt32 ArraySize)
{
    ViStatus status = VI_SUCCESS;
    ViInt32  i = 0;
    ViInt32  len = 0;
    ViChar   cmdBuf[100];
		
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DeviceAction,AGB2900_DEVICE_ACTION_ACQUIRE,
                                   AGB2900_DEVICE_ACTION_BOTH),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(ArmTriggerSource,AGB2900_TRIGGER_SOURCE_AINT,
                                   AGB2900_TRIGGER_SOURCE_EXT14),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(ArmCount,AGB2900_TRIGGER_COUNT_MIN,
                                   AGB2900_TRIGGER_COUNT_MAX),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(ArmDelay,AGB2900_TRIGGER_DELAY_MIN,
                                   AGB2900_TRIGGER_DELAY_MAX),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(ArmInterval,AGB2900_TRIGGER_TIMER_INTERVAL_MIN,
                                   AGB2900_TRIGGER_TIMER_INTERVAL_MAX),
               VI_ERROR_PARAMETER7);		   
    CheckParam(IsInvalidBooleanVal(ArmBypassEnabled),VI_ERROR_PARAMETER8);

    CheckParam(IsInvalidPtr(LXITriggers), VI_ERROR_INSTR_PARAMETER9);
    CheckParam(IsInvalidNumericVal(ArraySize,AGB2900_LXI_TRIGGER_MIN,
                                   AGB2900_LXI_TRIGGER_MAX),
               VI_ERROR_INSTR_PARAMETER10);
		
    CheckErr(viPrintf(vi, RECTIFY_CMD(":ARM%d:%s:SOUR %s"), (Channel),
                      gpstrDeviceAction[DeviceAction], gpstrTriggerSource[ArmTriggerSource]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":ARM%d:%s:COUN %d"), (Channel),
                      gpstrDeviceAction[DeviceAction], ArmCount));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":ARM%d:%s:DEL %d"), (Channel),
                      gpstrDeviceAction[DeviceAction], ArmDelay));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":ARM%d:%s:TIM %Le"), (Channel),
                      gpstrDeviceAction[DeviceAction], ArmInterval));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":ARM%d:%s:BYP %s"), (Channel),
                      gpstrDeviceAction[DeviceAction], gpstrTriggerBypass[ArmBypassEnabled]));
	
    snprintf(cmdBuf, sizeof(cmdBuf), ":ARM%d:%s:SOUR:LAN ", (Channel), gpstrDeviceAction[DeviceAction]);
    len = (ViInt32)strlen(cmdBuf);
    for(i = 0; i < ArraySize; i++)
    {
        if(LXITriggers[i])
        {
            snprintf(cmdBuf+strlen(cmdBuf), sizeof(cmdBuf)-strlen(cmdBuf), "LAN%d,", i);
        }
    }	
    if((ViInt32)strlen(cmdBuf) != len)
    {
        cmdBuf[strlen(cmdBuf) - 1] = '\n';    /* Change the last character ',' to '\n' */	
        CheckErr(viPrintf(vi, cmdBuf));
    }
					  
    CheckErr(CheckStatus(vi));
Error:
    return status;													   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureTrigger
 * Description: Configures settings for Trigger layer Trigger in ARM-Trigger model.
 *              B2900 supports the ARM-Trigger model described in 1999 SCPI 
 *              Command Reference. The ARM-Trigger model shows independent 
 *              event detection for the ARM and Trigger layers. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureTrigger(ViSession vi,
                                           ViInt32 Channel,
                                           ViInt32 DeviceAction,
                                           ViInt32 TriggerSource, 
                                           ViInt32 TriggerCount,
                                           ViInt32 TriggerDelay,
                                           ViReal64 TimerInterval, 
                                           ViBoolean TriggerBypassEnabled,
                                           ViBoolean LXITriggers[],
                                           ViInt32 ArraySize)
{
    ViStatus status = VI_SUCCESS;
    ViInt32  i = 0;
    ViInt32  len = 0;
    ViChar   cmdBuf[100];
		
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DeviceAction,AGB2900_DEVICE_ACTION_ACQUIRE,
                                   AGB2900_DEVICE_ACTION_BOTH),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(TriggerSource,AGB2900_TRIGGER_SOURCE_AINT,
                                   AGB2900_TRIGGER_SOURCE_EXT14),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(TriggerCount,AGB2900_TRIGGER_COUNT_MIN,
                                   AGB2900_TRIGGER_COUNT_MAX),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(TriggerDelay,AGB2900_TRIGGER_DELAY_MIN,
                                   AGB2900_TRIGGER_DELAY_MAX),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(TimerInterval,AGB2900_TRIGGER_TIMER_INTERVAL_MIN,
                                   AGB2900_TRIGGER_TIMER_INTERVAL_MAX),
               VI_ERROR_PARAMETER7);		   
    CheckParam(IsInvalidBooleanVal(TriggerBypassEnabled),VI_ERROR_PARAMETER8);
    CheckParam(IsInvalidPtr(LXITriggers), VI_ERROR_INSTR_PARAMETER9);
    CheckParam(IsInvalidNumericVal(ArraySize,AGB2900_LXI_TRIGGER_MIN,
                                   AGB2900_LXI_TRIGGER_MAX),
               VI_ERROR_INSTR_PARAMETER10);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRIG%d:%s:SOUR %s"), (Channel),
                      gpstrDeviceAction[DeviceAction], gpstrTriggerSource[TriggerSource]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRIG%d:%s:COUN %d"), (Channel),
                      gpstrDeviceAction[DeviceAction], TriggerCount));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRIG%d:%s:DEL %d"), (Channel),
                      gpstrDeviceAction[DeviceAction], TriggerDelay));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRIG%d:%s:TIM %Le"), (Channel),
                      gpstrDeviceAction[DeviceAction], TimerInterval));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRIG%d:%s:BYP %s"), (Channel),
                      gpstrDeviceAction[DeviceAction], gpstrTriggerBypass[TriggerBypassEnabled]));
	
    snprintf(cmdBuf, sizeof(cmdBuf), ":TRIG%d:%s:SOUR:LAN ", (Channel), gpstrDeviceAction[DeviceAction]);
    len = (ViInt32)strlen(cmdBuf);
    for(i = 0; i < ArraySize; i++)
    {
        if(LXITriggers[i])
        {
            snprintf(cmdBuf+strlen(cmdBuf), sizeof(cmdBuf)-strlen(cmdBuf), "LAN%d,", i);
        }
    }	
    if((ViInt32)strlen(cmdBuf) != len)
    {
        cmdBuf[strlen(cmdBuf) - 1] = '\n';    /* Change the last character ',' to '\n' */	
        CheckErr(viPrintf(vi, cmdBuf));
    }
					  
    CheckErr(CheckStatus(vi));
Error:
    return status;													   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureOutputTrigger
 * Description: Configures output triggers for status change between idle-arm, 
 *              arm-trigger or trigger-action layer. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureOutputTrigger(ViSession vi,
                                                 ViInt32 Channel,
                                                 ViInt32 DeviceAction,
                                                 ViInt32 TriggerSignal,
                                                 ViBoolean TriggerEnabled,
                                                 ViInt32 TriggerLocation,
                                                 ViInt32 TriggerTiming)
{
    ViStatus status = VI_SUCCESS;
		
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DeviceAction,AGB2900_DEVICE_ACTION_ACQUIRE,
                                   AGB2900_DEVICE_ACTION_BOTH),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(TriggerSignal,AGB2900_TRIGGER_SIGNAL_EXT1,
                                   AGB2900_TRIGGER_SIGNAL_INT2),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidBooleanVal(TriggerEnabled), VI_ERROR_PARAMETER5);
		
    CheckParam(IsInvalidNumericVal(TriggerLocation,AGB2900_TRIGGER_LOCATION_IDLE_ARM,
                                   AGB2900_TRIGGER_LOCATION_TRIGGER_TRANSIENT),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(TriggerTiming,AGB2900_TRIGGER_TIMING_BEFORE,
                                   AGB2900_TRIGGER_TIMING_BOTH),
               VI_ERROR_PARAMETER7);
	
    if(TriggerLocation == AGB2900_TRIGGER_LOCATION_IDLE_ARM ||
	     TriggerLocation == AGB2900_TRIGGER_LOCATION_ARM_TRIGGER)
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":%s%d:%s:TOUT:SIGN %s"), gpstrTriggerLocation[TriggerLocation],
                          (Channel), gpstrDeviceAction[DeviceAction], gpstrTriggerSignal[TriggerSignal]));
        CheckErr(viPrintf(vi, RECTIFY_CMD(":%s%d:%s:TOUT %s"), gpstrTriggerLocation[TriggerLocation],
                          (Channel), gpstrDeviceAction[DeviceAction], gpstrSwitchTable[TriggerEnabled]));			
    }
    else
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":%s%d:TOUT:SIGN %s"), gpstrTriggerLocation[TriggerLocation],
                          (Channel), gpstrTriggerSignal[TriggerSignal]));
        CheckErr(viPrintf(vi, RECTIFY_CMD(":%s%d:TOUT %s"), gpstrTriggerLocation[TriggerLocation],
                          (Channel), gpstrSwitchTable[TriggerEnabled]));			
    }
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR:DIG:INT:TOUT:POS %s"), gpstrTriggerTiming[TriggerTiming]));	
					  
    CheckErr(CheckStatus(vi));
Error:
    return status;														 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureCompositeLimitTest
 * Description: Configures the composite limit test.
 * Note:        Commands in the outer conditional statement are not applicable
 *              for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureCompositeLimitTest(ViSession vi,
                                                      ViInt32 Channel,
                                                      ViBoolean EnableCompositeLimitTest,
                                                      ViInt32 OperationMode,
                                                      ViInt32 LimitResultUpdateMode, 
                                                      ViInt32 PassPatternBits,
                                                      ViInt32 FailPatternBits)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(EnableCompositeLimitTest), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(OperationMode,AGB2900_LIMIT_TEST_OPERATION_MODE_GRADING,
                                   AGB2900_LIMIT_TEST_OPERATION_MODE_SORTING),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(LimitResultUpdateMode,AGB2900_LIMIT_RESULT_UPDATE_MODE_IMMEDIATE,
                                   AGB2900_LIMIT_RESULT_UPDATE_MODE_END),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(PassPatternBits,AGB2900_PATTERN_BITS_MIN,
                                   AGB2900_PATTERN_BITS_MAX),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(FailPatternBits,AGB2900_PATTERN_BITS_MIN,
                                   AGB2900_PATTERN_BITS_MAX),
               VI_ERROR_PARAMETER7);
	
	CheckErr(viPrintf(vi, RECTIFY_CMD(":FORM:DIG ASC")));
	/* Commands in the outer conditional statement are not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C */
	if((userData>=0&&userData<=9))
	{
		CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:CLIM:CLE"),(Channel)));
	    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:CLIM:STAT %s"), (Channel),
	                      gpstrSwitchTable[EnableCompositeLimitTest]));			
	    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:CLIM:MODE %s"), (Channel),
	                      gpstrOperationMode[OperationMode]));
	    if(OperationMode == AGB2900_LIMIT_TEST_OPERATION_MODE_GRADING)
	    {
	        CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:CLIM:UPD %s"), (Channel),
	                          gpstrResultUpdateMode[LimitResultUpdateMode]));			
	    }	
	    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:CLIM:PASS:DIG %d"), (Channel),
	                      PassPatternBits));	
	    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:CLIM:FAIL:DIG %d"), (Channel),
	                      FailPatternBits));
	}
	
	
    CheckErr(CheckStatus(vi));
Error:
    return status;														
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureIndividualLimitTest
 * Description: Configures the properties of specified limit test. The limit test 
 *              means a pass/fail judgement performed while a composite limit test. 
 *              The limit test result is returned by ReadLimitTestResult.
 * Note:        This function is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C    
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureIndividualLimitTest(ViSession vi,
                                                       ViInt32 Channel,
                                                       ViInt32 TestNumber,
                                                       ViBoolean EnableIndividualLimitTest,
                                                       ViReal64 LowerLimit,
                                                       ViReal64 UpperLimit,
                                                       ViInt32 TestType,													   
                                                       ViInt32 FeedMode,
                                                       ViInt32 JudgementCriteria)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(TestNumber,AGB2900_LIMIT_TEST_NUMBER_MIN,
                                   AGB2900_LIMIT_TEST_NUMBER_MAX),
               VI_ERROR_PARAMETER3);			   
    CheckParam(IsInvalidBooleanVal(EnableIndividualLimitTest), VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(LowerLimit,AGB2900_LIMIT_TEST_LIMIT_MIN,
                                   AGB2900_LIMIT_TEST_LIMIT_MAX),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(UpperLimit,AGB2900_LIMIT_TEST_LIMIT_MIN,
                                   AGB2900_LIMIT_TEST_LIMIT_MAX),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(TestType,AGB2900_LIMIT_TEST_TYPE_COMPLIANCE,
                                   AGB2900_LIMIT_TEST_TYPE_LIMIT),
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidNumericVal(FeedMode,AGB2900_LIMIT_TEST_FEED_MODE_VOLTAGE,
                                   AGB2900_LIMIT_TEST_FEED_MODE_MATH),
               VI_ERROR_PARAMETER8);
    CheckParam(IsInvalidNumericVal(JudgementCriteria,AGB2900_LIMIT_TEST_JUDGEMENT_CRITERIA_IN,
                                   AGB2900_LIMIT_TEST_JUDGEMENT_CRITERIA_OUT),
               VI_ERROR_INSTR_PARAMETER9);
			   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:STAT %s"),(Channel), 
                       TestNumber, gpstrSwitchTable[EnableIndividualLimitTest]));	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:LOW %Le"),(Channel), 
                       TestNumber, LowerLimit));		
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:UPP %Le"),(Channel), 
                       TestNumber, UpperLimit));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:FUNC %s"),(Channel), 
                       TestNumber, gpstrTestType[TestType]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:FEED %s"),(Channel), 
                      gpstrFeedMode[FeedMode]));					   
    if(TestType == AGB2900_LIMIT_TEST_TYPE_COMPLIANCE)
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:COMP:FAIL %s"), (Channel),
                          TestNumber, gpstrJudgementCriteria[JudgementCriteria]));			
    }		
	
    CheckErr(CheckStatus(vi));
Error:
    return status;														   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureLimitTestNullOffset
 * Description: Configures the null offset function for cancelling the offset 
 *              value from the measurement data automatically.
 * Note:        This function is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureLimitTestNullOffset(ViSession vi,
                                                       ViInt32 Channel,
                                                       ViBoolean NullOffsetAutoSetEnabled,
                                                       ViBoolean NullOffsetFuncEnabled,
                                                       ViReal64 NullOffset)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);			   
    CheckParam(IsInvalidBooleanVal(NullOffsetFuncEnabled), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(NullOffsetAutoSetEnabled), VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(NullOffset,AGB2900_LIMIT_TEST_LIMIT_MIN,
                                   AGB2900_LIMIT_TEST_LIMIT_MAX),
               VI_ERROR_PARAMETER5);
			   
					   
    if(NullOffsetAutoSetEnabled)
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:OFFS:ACQ"),(Channel)));		
    }
    else
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:OFFS:STAT %s"),(Channel), 
                           gpstrSwitchTable[NullOffsetFuncEnabled]));
        CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:OFFS %Le"),(Channel), 
                           NullOffset));		
    }		
	
    CheckErr(CheckStatus(vi));
Error:
    return status;														   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureLimitTestResultBitPattern
 * Description: Configures the limit test result pattern.
 * Note:        This function is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.    
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureLimitTestResultBitPattern(ViSession vi,
                                                             ViInt32 Channel,
                                                             ViInt32 TestNumber,
                                                             ViInt32 PassPattern,
                                                             ViInt32 LowerExceedingFailurePattern,
                                                             ViInt32 UpperExceedingFailurePattern,
                                                             ViInt32 ComplianceFailurePattern)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);			   
    CheckParam(IsInvalidNumericVal(TestNumber, AGB2900_LIMIT_TEST_NUMBER_MIN,
                                   AGB2900_LIMIT_TEST_NUMBER_MAX),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(PassPattern, AGB2900_PATTERN_BITS_MIN,
                                   AGB2900_PATTERN_BITS_MAX),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(LowerExceedingFailurePattern, AGB2900_PATTERN_BITS_MIN,
                                   AGB2900_PATTERN_BITS_MAX),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(UpperExceedingFailurePattern, AGB2900_PATTERN_BITS_MIN,
                                   AGB2900_PATTERN_BITS_MAX),
               VI_ERROR_PARAMETER6);			   
    CheckParam(IsInvalidNumericVal(ComplianceFailurePattern, AGB2900_PATTERN_BITS_MIN,
                                   AGB2900_PATTERN_BITS_MAX),
               VI_ERROR_PARAMETER7);

    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:PASS:DIG %d"), (Channel),
                      TestNumber, PassPattern));		
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:LOW:DIG %d"), (Channel), 
                      TestNumber, LowerExceedingFailurePattern));	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:UPP:DIG %d"), (Channel),
                      TestNumber, UpperExceedingFailurePattern));	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:LIM%d:COMP:DIG %d"), (Channel),
                      TestNumber, ComplianceFailurePattern));	
	
    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureMeasurement
 * Description: Configures the specified measurement.
 * Note:        Commands  in the conditional statement are not applicable 
 *				for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.      
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureMeasurement(ViSession vi,
                                               ViInt32 Channel,
                                               ViInt32 MeasurementMode,
                                               ViReal64 Range,
                                               ViBoolean AutomaticNPLCEnabled,
                                               ViReal64 PowerLifeCycle,
                                               ViInt32 ResistanceMode,
                                               ViBoolean ResistanceCompensatedEnabled)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);			   
    CheckParam(IsInvalidNumericVal(MeasurementMode, AGB2900_MEASUREMENT_MODE_VOLTAGE,
                                   AGB2900_MEASUREMENT_MODE_RESISTANCE),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(AutomaticNPLCEnabled), VI_ERROR_PARAMETER5);			   
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:FUNC \"%s\""), (Channel),
                      gpstrMeasurementMode[MeasurementMode]));	
					   
    switch(MeasurementMode)
    {
        case AGB2900_MEASUREMENT_MODE_VOLTAGE:
            CheckParam(IsInvalidNumericVal(Range, AGB2900_VOLTAGE_RANGE_MIN,
                                           AGB2900_VOLTAGE_RANGE_MAX),
                       VI_ERROR_PARAMETER4);
            break;
        case AGB2900_MEASUREMENT_MODE_CURRENT:
            CheckParam(IsInvalidNumericVal(Range, AGB2900_CURRENT_RANGE_MIN,
                                           AGB2900_CURRENT_RANGE_MAX),
                       VI_ERROR_PARAMETER4); 
            break;
        case AGB2900_MEASUREMENT_MODE_RESISTANCE:
            CheckParam(IsInvalidNumericVal(Range, AGB2900_MEASURMENT_RESISTANCE_RANGE_MIN,
                                           AGB2900_MEASURMENT_RESISTANCE_RANGE_MAX),
                       VI_ERROR_PARAMETER4);
            CheckParam(IsInvalidNumericVal(ResistanceMode, AGB2900_MEASUREMENT_RESISTANCE_MODE_MANUAL,
                                           AGB2900_MEASUREMENT_RESISTANCE_MODE_AUTO),
                       VI_ERROR_PARAMETER6);
            CheckParam(IsInvalidBooleanVal(ResistanceCompensatedEnabled), VI_ERROR_PARAMETER7);
			/* Commands in this conditional statement are not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C  */
			if(userData>=0&&userData<=9)
			{
	            CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:MODE %s"), (Channel), 
	                              gpstrMeasurementMode[MeasurementMode], 
	                              gpstrResistanceMode[ResistanceMode]));
			}
            CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:OCOM %s"), (Channel), 
                              gpstrMeasurementMode[MeasurementMode], 
                              gpstrSwitchTable[ResistanceCompensatedEnabled]));
            break;	
        default:
            break;
    }
	/* Commands in this conditional statement are not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C  */
	if(userData>=0&&userData<=9)
	{
		CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:RANG:UPP %Le"), (Channel),
	                      gpstrMeasurementMode[MeasurementMode], Range));
	
	    CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:NPLC:AUTO %s"), (Channel), 
	                      gpstrMeasurementMode[MeasurementMode],gpstrSwitchTable[AutomaticNPLCEnabled]));
	}
    if(!AutomaticNPLCEnabled)		
    {
        CheckParam(IsInvalidNumericVal(PowerLifeCycle, AGB2900_POWER_LIFE_CYCLE_VALUE_MIN,
                                       AGB2900_POWER_LIFE_CYCLE_VALUE_MAX),
				   VI_ERROR_PARAMETER6);
		/* When MeasurementMode is resistance, command in this conditional statement is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C  */
		if(!((userData>=10&&userData<=13)&&(MeasurementMode==AGB2900_MEASUREMENT_MODE_RESISTANCE)))
		{
	        CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:NPLC %Le"), (Channel), 
	                          gpstrMeasurementMode[MeasurementMode], PowerLifeCycle));
		}
    }
	
    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}											 

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureMeasurementAutoRange
 * Description: Configures the specified measurement auto range.
 * Note:        Commands in the conditional statement are not applicable for
 *              B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.			   
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureMeasurementAutoRange(ViSession vi,
                                                        ViInt32 Channel,
                                                        ViInt32 MeasurementMode,
                                                        ViBoolean AutoRangeEnabled,
                                                        ViReal64 LowerLimit,
                                                        ViReal64 UpperLimit,
                                                        ViReal64 ThresholdRate,
                                                        ViInt32 AutoRangeMode)
{
    ViStatus status = VI_SUCCESS;																

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(MeasurementMode, AGB2900_MEASUREMENT_MODE_VOLTAGE,
                                   AGB2900_MEASUREMENT_MODE_RESISTANCE),
               VI_ERROR_PARAMETER3);			   
    CheckParam(IsInvalidBooleanVal(AutoRangeEnabled), VI_ERROR_PARAMETER4);			   
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:FUNC \"%s\""), (Channel),
                      gpstrMeasurementMode[MeasurementMode]));
	/* Commands in this conditional statement are not applicable for B2961A/B2962A and B2961B/B2962B */
	if(userData>=0&&userData<=9)
	{
	    CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:RANG:AUTO %s"), (Channel),
	                      gpstrMeasurementMode[MeasurementMode],  gpstrSwitchTable[AutoRangeEnabled]));	
					   
	    switch(MeasurementMode)
	    {
	        case AGB2900_MEASUREMENT_MODE_VOLTAGE:
	            CheckParam(IsInvalidNumericVal(LowerLimit, AGB2900_VOLTAGE_RANGE_MIN,
	                                           AGB2900_VOLTAGE_RANGE_MAX),
	                       VI_ERROR_PARAMETER5);
	            CheckParam(IsInvalidNumericVal(UpperLimit, AGB2900_VOLTAGE_RANGE_MIN,
	                                           AGB2900_VOLTAGE_RANGE_MAX),
	                       VI_ERROR_PARAMETER6);
	            CheckParam(IsInvalidNumericVal(ThresholdRate, AGB2900_AUTO_RANGE_THRESHOLD_RATE_MIN,
	                                           AGB2900_AUTO_RANGE_THRESHOLD_RATE_MAX),
	                       VI_ERROR_PARAMETER7);
	            CheckParam(IsInvalidNumericVal(AutoRangeMode, AGB2900_AUTO_RANGE_MODE_NORMAL,
	                                           AGB2900_AUTO_RANGE_MODE_SPEED),
	                       VI_ERROR_PARAMETER8);
	            CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:RANG:AUTO:THR %Le"), (Channel), 
	                              gpstrMeasurementMode[MeasurementMode], ThresholdRate));
	            CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:RANG:AUTO:MODE %s"), (Channel), 
	                              gpstrMeasurementMode[MeasurementMode], 
	                              gpstrAutoRangeMode[AutoRangeMode]));	
	            break; 
	        case AGB2900_MEASUREMENT_MODE_CURRENT: 
	            CheckParam(IsInvalidNumericVal(LowerLimit, AGB2900_CURRENT_RANGE_MIN,
	                                           AGB2900_CURRENT_RANGE_MAX),
	                       VI_ERROR_PARAMETER5);
	            CheckParam(IsInvalidNumericVal(UpperLimit, AGB2900_CURRENT_RANGE_MIN,
	                                           AGB2900_CURRENT_RANGE_MAX),
	                       VI_ERROR_PARAMETER6);
	            CheckParam(IsInvalidNumericVal(ThresholdRate, AGB2900_AUTO_RANGE_THRESHOLD_RATE_MIN,
	                                           AGB2900_AUTO_RANGE_THRESHOLD_RATE_MAX),
	                       VI_ERROR_PARAMETER7);
	            CheckParam(IsInvalidNumericVal(AutoRangeMode, AGB2900_AUTO_RANGE_MODE_NORMAL,
	                                           AGB2900_AUTO_RANGE_MODE_SPEED),
					       VI_ERROR_PARAMETER8);						   
	            CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:RANG:AUTO:THR %Le"), (Channel), 
	                              gpstrMeasurementMode[MeasurementMode], ThresholdRate));
	            CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:RANG:AUTO:MODE %s"), (Channel), 
	                              gpstrMeasurementMode[MeasurementMode], 
	                              gpstrAutoRangeMode[AutoRangeMode]));		
	            break; 
	        case AGB2900_MEASUREMENT_MODE_RESISTANCE: 
	            CheckParam(IsInvalidNumericVal(LowerLimit, AGB2900_MEASURMENT_RESISTANCE_RANGE_MIN,
	                                           AGB2900_MEASURMENT_RESISTANCE_RANGE_MAX),
	                       VI_ERROR_PARAMETER5);
	            CheckParam(IsInvalidNumericVal(UpperLimit, AGB2900_MEASURMENT_RESISTANCE_RANGE_MIN,
	                                           AGB2900_MEASURMENT_RESISTANCE_RANGE_MAX),
	                       VI_ERROR_PARAMETER6);
	            CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:RANG:AUTO:ULIM %Le"), (Channel),
	                              gpstrMeasurementMode[MeasurementMode], UpperLimit));	
	            break; 
	        default: 
	            break;		   
	        }		
	    CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:%s:RANG:AUTO:LLIM %Le"), (Channel),
	                      gpstrMeasurementMode[MeasurementMode], LowerLimit));		
	} 
	
    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureArbitraryWaveform
 * Description: Configure the arbitrary waveform output.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureArbitraryWaveform(ViSession vi,
                                                      ViInt32 Channel, 
                                                      ViInt32 ShapeOfWaveform, 
                                                      ViReal64 NumberOfWaveforms)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(ShapeOfWaveform, AGB2900_WAVEFORM_SHAPE_EXPONENTIAL,
                                   AGB2900_WAVEFORM_SHAPE_USERDEFINED),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(NumberOfWaveforms, AGB2900_WAVE_NUMBER_MIN,
                                   AGB2900_WAVE_NUMBER_MAX),
               VI_ERROR_PARAMETER4);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:COUN %Le"), (Channel),
                      NumberOfWaveforms));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:FUNC %s"), (Channel),
                      gpstrWaveformShape[ShapeOfWaveform]));
	
    CheckErr(CheckStatus(vi)); 


Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureWaveformExponential
 * Description: Configure current or voltage exponential waveform output.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureWaveformExponential(ViSession vi,
                                                        ViInt32 Channel, 
                                                        ViInt32 OutputMode, 
                                                        ViReal64 StartTime, 
                                                        ViReal64 StartLevel, 
                                                        ViReal64 EndLevel, 
                                                        ViReal64 TimeConstant, 
                                                        ViReal64 TimeSetting)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(StartTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER4);
    if(OutputMode==AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE)
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(EndLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    }
    else
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(EndLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    }
    CheckParam(IsInvalidNumericVal(TimeConstant, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidNumericVal(TimeSetting, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER8);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:EXP:END %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],EndLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:EXP:STAR %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:EXP:TCON %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],TimeConstant));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:EXP:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],TimeSetting));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:EXP:STAR:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartTime));
	
	CheckErr(CheckStatus(vi));    
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureWaveformRamp
 * Description: Configure current or voltage ramp waveform output.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureWaveformRamp(ViSession vi,
                                                 ViInt32 Channel, 
                                                 ViInt32 OutputMode, 
                                                 ViReal64 RiseTime, 
                                                 ViReal64 StartLevel, 
                                                 ViReal64 EndLevel, 
                                                 ViReal64 StartTime, 
                                                 ViReal64 EndTime)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(RiseTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER4);
    if(OutputMode==AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE)
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(EndLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    }
    else
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(EndLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    }
    CheckParam(IsInvalidNumericVal(StartTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidNumericVal(EndTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER8);

    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:RAMP:STAR %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:RAMP:END %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],EndLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:RAMP:RTIM %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],RiseTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:RAMP:STAR:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:RAMP:END:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],EndTime));
	
	CheckErr(CheckStatus(vi));    
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureWaveformSinusoid
 * Description: Configure current or voltage sinusoid waveform output.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.   
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureWaveformSinusoid(ViSession vi,
                                                     ViInt32 Channel, 
                                                     ViInt32 OutputMode, 
                                                     ViInt32 TriggerOutputPort, 
                                                     ViReal64 Amplitude, 
                                                     ViReal64 Frequency, 
                                                     ViReal64 OffsetLevel, 
                                                     ViReal64 PhaseMarker, 
                                                     ViBoolean EnableTriggerOutput)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(TriggerOutputPort, AGB2900_TRIGGER_OUTPUTPORT_EXT1,
                                   AGB2900_TRIGGER_OUTPUTPORT_INT2),
               VI_ERROR_PARAMETER4);
    if(OutputMode==AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE)
    {
        CheckParam(IsInvalidNumericVal(Amplitude, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(OffsetLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER7);
    }
    else
    {
        CheckParam(IsInvalidNumericVal(Amplitude, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(OffsetLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER7);
    }
    CheckParam(IsInvalidNumericVal(Frequency, AGB2900_FREQUENCY_MIN,
                                   AGB2900_FREQUENCY_MAX),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(PhaseMarker, AGB2900_PHASE_MARKER_MIN,
                                   AGB2900_PHASE_MARKER_MAX),
               VI_ERROR_PARAMETER8);
    CheckParam(IsInvalidBooleanVal(EnableTriggerOutput), VI_ERROR_INSTR_PARAMETER9);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SIN:AMPL %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],Amplitude));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SIN:FREQ %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],Frequency));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SIN:OFFS %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],OffsetLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SIN:PMAR %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],PhaseMarker));
    if(EnableTriggerOutput)
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SIN:PMAR:STAT 1"), (Channel),
                          gpstrSourceOutputMode[OutputMode]));	
    }
    else
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SIN:PMAR:STAT 0"), (Channel),
                          gpstrSourceOutputMode[OutputMode]));		
    }
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SIN:PMAR:SIGN %s"), (Channel),
                      gpstrSourceOutputMode[OutputMode],gpstrTriggerOutputPort[TriggerOutputPort]));

	CheckErr(CheckStatus(vi));    
Error:
    return status; 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureWaveformSquare
 * Description: Configure current or voltage square waveform output.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureWaveformSquare(ViSession vi,
                                                   ViInt32 Channel, 
                                                   ViInt32 OutputMode, 
                                                   ViReal64 TopTime, 
                                                   ViReal64 StartLevel, 
                                                   ViReal64 TopLevel, 
                                                   ViReal64 StartTime, 
                                                   ViReal64 EndTime)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(TopTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER4);
    if(OutputMode==AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE)
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(TopLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    }
    else
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(TopLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    }
    CheckParam(IsInvalidNumericVal(StartTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidNumericVal(EndTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER8);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SQU:END:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],EndTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SQU:STAR:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartTime));	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SQU:STAR %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SQU:TOP:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],TopTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:SQU:TOP %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],TopLevel));
	
	CheckErr(CheckStatus(vi));    
Error:
    return status; 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureWaveformTrapezoid
 * Description: Configure current or voltage trapezoid waveform output.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureWaveformTrapezoid(ViSession vi,
                                                      ViInt32 Channel, 
                                                      ViInt32 OutputMode, 
                                                      ViReal64 TopTime, 
                                                      ViReal64 StartLevel, 
                                                      ViReal64 TopLevel, 
                                                      ViReal64 StartTime, 
                                                      ViReal64 EndTime, 
                                                      ViReal64 RiseTime, 
                                                      ViReal64 FallTime)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(TopTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER4);
    if(OutputMode==AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE)
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(TopLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    }
    else
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER5);
        CheckParam(IsInvalidNumericVal(TopLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    }
    CheckParam(IsInvalidNumericVal(StartTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidNumericVal(EndTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER8);
    CheckParam(IsInvalidNumericVal(RiseTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_INSTR_PARAMETER9);
    CheckParam(IsInvalidNumericVal(FallTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_INSTR_PARAMETER10);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRAP:FTIM %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],FallTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRAP:RTIM %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],RiseTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRAP:STAR %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRAP:TOP %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],TopLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRAP:STAR:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRAP:TOP:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],TopTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRAP:END:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],EndTime));
	
	CheckErr(CheckStatus(vi));    
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureWaveformTriangle
 * Description: Configure current or voltage triangle waveform output.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureWaveformTriangle(ViSession vi,
                                                     ViInt32 Channel, 
                                                     ViInt32 OutputMode, 
                                                     ViReal64 StartLevel, 
                                                     ViReal64 TopLevel, 
                                                     ViReal64 StartTime, 
                                                     ViReal64 EndTime, 
                                                     ViReal64 RiseTime, 
                                                     ViReal64 FallTime)
{
    ViStatus status = VI_SUCCESS;	

    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    if(OutputMode==AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE)
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER4);
        CheckParam(IsInvalidNumericVal(TopLevel, AGB2900_VOLTAGE_RANGE_MIN,
                                       AGB2900_VOLTAGE_RANGE_MAX),
               VI_ERROR_PARAMETER5);
	}
    else
    {
        CheckParam(IsInvalidNumericVal(StartLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER4);
        CheckParam(IsInvalidNumericVal(TopLevel, AGB2900_CURRENT_RANGE_MIN,
                                       AGB2900_CURRENT_RANGE_MAX),
               VI_ERROR_PARAMETER5);
    }
    CheckParam(IsInvalidNumericVal(StartTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(EndTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidNumericVal(RiseTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidNumericVal(FallTime, AGB2900_TIME_RANGE_MIN,
                                   AGB2900_TIME_RANGE_MAX),
               VI_ERROR_INSTR_PARAMETER9);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRI:RTIM %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],RiseTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRI:FTIM %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],FallTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRI:STAR %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRI:TOP %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],TopLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRI:STAR:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StartTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:TRI:END:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],EndTime));
	
	CheckErr(CheckStatus(vi));    
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureWaveformUserDefined
 * Description: Configure current or voltage user defined waveform output.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureWaveformUserDefined(ViSession vi,
                                                        ViInt32 Channel, 
                                                        ViInt32 OutputMode, 
                                                        ViReal64 StepTime, 
                                                        ViInt32 ListOutputDataMode, 
                                                        ViReal64 ListOutputData[],
                                                        ViInt32 ListOutputDataSize,
                                                        ViBoolean EnableTriggerOutput, 
                                                        ViInt32 TriggerOutputPort, 
                                                        ViInt32 ListTriggerOutputMode, 
                                                        ViBoolean ListTriggerOutput[],
                                                        ViInt32 ListTriggerOutputSize)
{
    ViStatus status = VI_SUCCESS;	
    ViChar strOutputData[BUFFER_SIZE_LARGE];
    ViChar strTriggerData[BUFFER_SIZE_LARGE];
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(StepTime, AGB2900_STEP_TIME_MIN,
                                   AGB2900_STEP_TIME_MAX),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(ListOutputDataMode, AGB2900_LIST_OUTPUT_DATA_MODE_APPEND,
                                   AGB2900_LIST_OUTPUT_DATA_MODE_NEW),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidPtr(ListOutputData), VI_ERROR_PARAMETER6);	
    CheckParam(IsInvalidNumericVal(ListOutputDataSize, AGB2900_DATA_SIZE_MIN,      
                                   AGB2900_DATA_SIZE_MAX),
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidBooleanVal(EnableTriggerOutput), VI_ERROR_PARAMETER8);	
    CheckParam(IsInvalidNumericVal(TriggerOutputPort, AGB2900_TRIGGER_OUTPUTPORT_EXT1,
                                   AGB2900_TRIGGER_OUTPUTPORT_INT2),
               VI_ERROR_INSTR_PARAMETER9);
    CheckParam(IsInvalidNumericVal(ListTriggerOutputMode, AGB2900_LIST_TRIGGER_OUTPUT_MODE_APPEND,
                                   AGB2900_LIST_TRIGGER_OUTPUT_MODE_NEW),
               VI_ERROR_INSTR_PARAMETER10);
    CheckParam(IsInvalidPtr(ListTriggerOutput), VI_ERROR_INSTR_PARAMETER11);
    CheckParam(IsInvalidNumericVal(ListTriggerOutputSize, AGB2900_DATA_SIZE_MIN,      
                                   AGB2900_DATA_SIZE_MAX),
               VI_ERROR_INSTR_PARAMETER12);
	
    BuildFlattenStr(ListOutputData, ListOutputDataSize, "%Le", ",", strOutputData, BUFFER_SIZE_LARGE);
    switch(ListOutputDataMode)
    {
        case AGB2900_LIST_OUTPUT_DATA_MODE_APPEND:
            CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:UDEF:APP %s"), (Channel), 
                              gpstrSourceOutputMode[OutputMode], strOutputData)); 
            break;
        case AGB2900_LIST_OUTPUT_DATA_MODE_NEW:
            CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:UDEF %s"), (Channel), 
                              gpstrSourceOutputMode[OutputMode], strOutputData)); 
            break;
        default: 
            break;
    } 	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:UDEF:TIME %Le"), (Channel),
                      gpstrSourceOutputMode[OutputMode],StepTime));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:UDEF:BOST:SIGN %s"), (Channel),
                      gpstrSourceOutputMode[OutputMode],gpstrTriggerOutputPort[TriggerOutputPort]));
    if(EnableTriggerOutput)
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:UDEF:BOST:STAT 1"), (Channel),
                          gpstrSourceOutputMode[OutputMode]));	
    }
    else
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:UDEF:BOST:STAT 0"), (Channel),
                          gpstrSourceOutputMode[OutputMode]));		
    }
	

    ViInt32 iArrayIdx;
    ViChar * pcStr = strTriggerData;
    ViChar * pcEnd = strTriggerData + sizeof(strTriggerData);
    int n1;
    for(iArrayIdx = 0; iArrayIdx < ListTriggerOutputSize - 1; ++iArrayIdx)
    {
        n1 = snprintf(pcStr, pcEnd - pcStr, "%d", ((ListTriggerOutput[iArrayIdx]==VI_FALSE)?0:1));
        if (n1 > 0) pcStr += (n1 < (int)(pcEnd - pcStr)) ? n1 : (int)(pcEnd - pcStr);
        n1 = snprintf(pcStr, pcEnd - pcStr, ",");
        if (n1 > 0) pcStr += (n1 < (int)(pcEnd - pcStr)) ? n1 : (int)(pcEnd - pcStr);
    }
    snprintf(pcStr, pcEnd - pcStr, "%d", ((ListTriggerOutput[iArrayIdx]==VI_FALSE)?0:1));
    switch(ListTriggerOutputMode)
    {
        case AGB2900_LIST_TRIGGER_OUTPUT_MODE_APPEND:
            CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:UDEF:BOST:APP %s"), (Channel), 
                              gpstrSourceOutputMode[OutputMode], strTriggerData)); 
            break;
        case AGB2900_LIST_TRIGGER_OUTPUT_MODE_NEW:
            CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:ARB:%s:UDEF:BOST %s"), (Channel), 
                              gpstrSourceOutputMode[OutputMode], strTriggerData)); 
            break;
        default: 
            break;
    } 
	
	CheckErr(CheckStatus(vi));    
Error:
    return status;
}


/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureSourceRangeMode
 * Description: Configures list sweep for measurement and source
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureSourceRangeMode (ViSession vi,
                                                    ViInt32 Channel,
                                                    ViInt32 SweepRangingMode)
{
    ViStatus status = VI_SUCCESS;     
    
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(SweepRangingMode, AGB2900_SWEEP_RANGING_MODE_BEST,      
                                   AGB2900_SWEEP_RANGING_MODE_AUTO),                       
               VI_ERROR_PARAMETER3); 			   

    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:SWE:RANG %s"), (Channel), 
                      gpstrSweepRangingMode[SweepRangingMode])); 

    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureListSweep
 * Description: Configures list sweep for measurement and source
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureListSweep(ViSession vi,
                                             ViInt32 Channel,
                                             ViInt32 OutputMode,
                                             ViBoolean ListSweepEnabled,
                                             ViReal64 ListData[],
                                             ViInt32 DataLen,
                                             ViInt32 ListDataMode,
                                             ViInt32 StartIndex)
{
    ViStatus status = VI_SUCCESS;
    ViChar strData[BUFFER_SIZE_LARGE];
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                         
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(ListSweepEnabled), VI_ERROR_PARAMETER4);         
    CheckParam(IsInvalidPtr(ListData), VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(DataLen, AGB2900_DATA_SIZE_MIN,AGB2900_DATA_SIZE_MAX), 
               VI_ERROR_PARAMETER6);   
    CheckParam(IsInvalidNumericVal(ListDataMode, AGB2900_SWEEP_LIST_DATA_MODE_APPEND,      
                                   AGB2900_SWEEP_LIST_DATA_MODE_NEW),                       
               VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidNumericVal(StartIndex, 0, DataLen), VI_ERROR_PARAMETER8);

    BuildFlattenStr(ListData, DataLen, "%Le", ",", strData, BUFFER_SIZE_LARGE);

    switch(ListDataMode)
    {
        case AGB2900_SWEEP_LIST_DATA_MODE_APPEND:
            CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:LIST:%s:APP %s"), (Channel), 
                              gpstrSourceOutputMode[OutputMode], strData)); 
            break;
        case AGB2900_SWEEP_LIST_DATA_MODE_NEW:
            CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:LIST:%s %s"), (Channel), 
                              gpstrSourceOutputMode[OutputMode], strData)); 
            break;
        default: 
            break;
    }    
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:LIST:%s:STAR %d"), (Channel),  
                      gpstrSourceOutputMode[OutputMode], StartIndex)); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:MODE %s"), (Channel), 
                      gpstrSourceOutputMode[OutputMode], gpstrListSweep[ListSweepEnabled])); 
									   
    CheckErr(CheckStatus(vi));    
Error:
    return status;													 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureSweep
 * Description: Configures sweep for measurement and source.
 * Note:        Model-specific limits:
 *              B2901BL/B2901CL: SweepStepCount 1 - 10000
 *              All other models: SweepStepCount 1 - 100000
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureSweep(ViSession vi,
                                         ViInt32 Channel,
                                         ViInt32 OutputMode,										 
                                         ViBoolean SweepEnabled,
                                         ViReal64 SweepStartValue,
                                         ViReal64 SweepStopValue,
                                         ViInt32 SweepStepCount,
                                         ViInt32 SweepMode,
                                         ViInt32 SweepDirection)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(SweepEnabled), VI_ERROR_PARAMETER4); 

    switch(OutputMode)
    {
        case AGB2900_SOURCE_OUTPUT_MODE_CURRENT: 
            CheckParam(IsInvalidNumericVal(SweepStartValue, AGB2900_CURRENT_RANGE_MIN,      
                                           AGB2900_CURRENT_RANGE_MAX),                       
                       VI_ERROR_PARAMETER5);			   
            CheckParam(IsInvalidNumericVal(SweepStopValue, AGB2900_CURRENT_RANGE_MIN,      
                                           AGB2900_CURRENT_RANGE_MAX),                       
                       VI_ERROR_PARAMETER6); 
            break;
        case AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE:
            CheckParam(IsInvalidNumericVal(SweepStartValue, AGB2900_VOLTAGE_RANGE_MIN,      
                                           AGB2900_VOLTAGE_RANGE_MAX),                       
                       VI_ERROR_PARAMETER5);			   
            CheckParam(IsInvalidNumericVal(SweepStopValue, AGB2900_VOLTAGE_RANGE_MIN,      
                                           AGB2900_VOLTAGE_RANGE_MAX),                       
                       VI_ERROR_PARAMETER6); 
            break;
        default:
            break;
    }       
    CheckParam(IsInvalidNumericVal(SweepStepCount, AGB2900_SWEEP_STEP_MIN,      
                                   AGB2900_SWEEP_STEP_MAX),                       
               VI_ERROR_PARAMETER7);  
    CheckParam(IsInvalidNumericVal(SweepMode, AGB2900_SWEEP_MODE_LINEAR_SINGLE,      
                                   AGB2900_SWEEP_MODE_LOG_DOUBLE),                       
               VI_ERROR_PARAMETER8);			   
    CheckParam(IsInvalidNumericVal(SweepDirection, AGB2900_SWEEP_DIRECTION_UP ,      
                                   AGB2900_SWEEP_DIRECTION_DOWN),                       
               VI_ERROR_INSTR_PARAMETER9);
			   
    if(SweepEnabled)
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:MODE SWE"), (Channel), 
    	                  gpstrSourceOutputMode[OutputMode])); 		
    }
    else
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:MODE FIX"), (Channel), 
    	                  gpstrSourceOutputMode[OutputMode])); 		
    }
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:STAR %Le"), (Channel),  
                      gpstrSourceOutputMode[OutputMode], SweepStartValue)); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:STOP %Le"), (Channel),  
                      gpstrSourceOutputMode[OutputMode], SweepStopValue));					   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:POIN %d"), (Channel), 
                      gpstrSourceOutputMode[OutputMode], SweepStepCount)); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:SWE:SPAC %s"), (Channel), 
                      gpstrSweepScale[SweepMode%2])); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:SWE:STA %s"), (Channel), 
                      gpstrSweepStair[SweepMode/2])); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:SWE:DIR %s"), (Channel), 
                      gpstrSweepDirection[SweepDirection]));  
									   
    CheckErr(CheckStatus(vi));    
Error:
    return status;												 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureSourceAutoRange
 * Description: Configures Auto-range related values for source function. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureSourceAutoRange(ViSession vi,
                                                   ViInt32 Channel,
                                                   ViInt32 OutputMode,
                                                   ViBoolean AutoRangeEnabled,
                                                   ViReal64 LowerLimitForAutoRange)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(AutoRangeEnabled), VI_ERROR_PARAMETER4); 

    switch(OutputMode)
    {
        case AGB2900_SOURCE_OUTPUT_MODE_CURRENT:
            CheckParam(IsInvalidNumericVal(LowerLimitForAutoRange, AGB2900_CURRENT_RANGE_MIN,      
                                           AGB2900_CURRENT_RANGE_MAX),                       
                       VI_ERROR_PARAMETER5); 
            break;
        case AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE:
            CheckParam(IsInvalidNumericVal(LowerLimitForAutoRange, AGB2900_VOLTAGE_RANGE_MIN,      
                                           AGB2900_VOLTAGE_RANGE_MAX),                       
                       VI_ERROR_PARAMETER5); 
            break;
        default:
            break;
    }       
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:RANG:AUTO %s"), (Channel),  
                      gpstrSourceOutputMode[OutputMode], 
                      gpstrSwitchTable[AutoRangeEnabled])); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:RANG:AUTO:LLIM %Le"), (Channel),  
                      gpstrSourceOutputMode[OutputMode], LowerLimitForAutoRange));					   
									   
    CheckErr(CheckStatus(vi));    
Error:
    return status;												 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureSource
 * Description: Configures output level, output mode, and enables or disables 
 *              the output for specified channel of the power sourcing.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureSource(ViSession vi,
                                          ViInt32 Channel,
                                          ViInt32 OutputMode,
                                          ViInt32 OutputFuction,
                                          ViReal64 OutputLevel,
                                          ViReal64 TriggerOutputLevel,
                                          ViReal64 OutputRange,
                                          ViBoolean ContinuousTriggerEnabled)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(OutputFuction, AGB2900_SOURCE_OUTPUT_FUNCTION_PLUS,      
                                   AGB2900_SOURCE_OUTPUT_FUNCTION_DC),                       
               VI_ERROR_PARAMETER4);

    switch(OutputMode)
    {
        case AGB2900_SOURCE_OUTPUT_MODE_CURRENT:
            CheckParam(IsInvalidNumericVal(OutputLevel, AGB2900_CURRENT_RANGE_MIN,      
                                           AGB2900_CURRENT_RANGE_MAX),                       
                       VI_ERROR_PARAMETER5); 
            CheckParam(IsInvalidNumericVal(TriggerOutputLevel, AGB2900_CURRENT_RANGE_MIN,      
                                           AGB2900_CURRENT_RANGE_MAX),                       
                       VI_ERROR_PARAMETER6);
            CheckParam(IsInvalidNumericVal(OutputRange, AGB2900_CURRENT_RANGE_MIN,      
                                           AGB2900_CURRENT_RANGE_MAX),                       
                       VI_ERROR_PARAMETER7); 
            break;
        case AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE:
            CheckParam(IsInvalidNumericVal(OutputLevel, AGB2900_VOLTAGE_RANGE_MIN,      
                                           AGB2900_VOLTAGE_RANGE_MAX),                       
                       VI_ERROR_PARAMETER5);
            CheckParam(IsInvalidNumericVal(TriggerOutputLevel, AGB2900_VOLTAGE_RANGE_MIN,      
                                           AGB2900_VOLTAGE_RANGE_MAX),                       
                       VI_ERROR_PARAMETER6); 
            CheckParam(IsInvalidNumericVal(OutputRange, AGB2900_VOLTAGE_RANGE_MIN,      
                                           AGB2900_VOLTAGE_RANGE_MAX),                       
                       VI_ERROR_PARAMETER7);  
            break;
        default:
            break;
    } 		   
    CheckParam(IsInvalidBooleanVal(ContinuousTriggerEnabled), VI_ERROR_PARAMETER8);        			   

    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:FUNC:MODE %s"), (Channel),  
                      gpstrSourceOutputMode[OutputMode]));
	CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:FUNC:SHAP %s"), (Channel),  
                      gpstrOutputFunction[OutputFuction]));
	CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:FUNC:TRIG:CONT %s"), (Channel), 
                      gpstrSwitchTable[ContinuousTriggerEnabled]));					   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:LEV:IMM %Le"), (Channel),  
                      gpstrSourceOutputMode[OutputMode], OutputLevel)); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:LEV:TRIG %Le"), (Channel),  
                      gpstrSourceOutputMode[OutputMode], TriggerOutputLevel));					   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:%s:RANG %Le"), (Channel), 
                      gpstrSourceOutputMode[OutputMode], OutputRange)); 
									   
    CheckErr(CheckStatus(vi));    
Error:
    return status;												  
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureOVP
 * Description: Configures over current protection (OCP) and over voltage 
 *              protection (OVP).  Specify the output current limit value and 
 *              the behavior of the power supply when the output current is 
 *              greater than or equal to that value. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureOutputProtection(ViSession vi,
                                                    ViInt32 Channel, 
                                                    ViBoolean OutputProtectionEnabled, 
                                                    ViReal64 ComplianceVoltage, 
                                                    ViReal64 ComplianceCurrent)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(OutputProtectionEnabled), VI_ERROR_PARAMETER3);       
    CheckParam(IsInvalidNumericVal(ComplianceVoltage, AGB2900_VOLTAGE_RANGE_MIN,      
                                   AGB2900_VOLTAGE_RANGE_MAX),                       
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(ComplianceCurrent, AGB2900_CURRENT_RANGE_MIN,      
                                   AGB2900_CURRENT_RANGE_MAX),                       
               VI_ERROR_PARAMETER5);
    

    CheckErr(viPrintf(vi, RECTIFY_CMD(":SENS%d:VOLT:PROT %Le"), (Channel),  
	                  ComplianceVoltage));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:PROT %s"), (Channel), 
    	              gpstrSwitchTable[OutputProtectionEnabled]));	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:PROT %s"), (Channel), 
    	              gpstrSwitchTable[OutputProtectionEnabled]));					   


    CheckErr(CheckStatus(vi));    
Error:
    return status;										   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigurePulse
 * Description: Configures the pulse mode.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigurePulse(ViSession vi,
                                         ViInt32 Channel,
                                         ViReal64 PulseDelay,
                                         ViReal64 PulseWidth)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(PulseDelay, AGB2900_SOURCE_PLUS_DELAY_MIN,      
                                   AGB2900_SOURCE_PLUS_DELAY_MAX),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(PulseWidth, AGB2900_SOURCE_PLUS_WIDTH_MIN,      
                                   AGB2900_SOURCE_PLUS_WIDTH_MAX),                       
               VI_ERROR_PARAMETER4);       			   

    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:PULS:DEL %Le"), (Channel),  
	                  PulseDelay));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR%d:PULS:WIDT %Le"), (Channel), 
    	              PulseWidth));					   

    CheckErr(CheckStatus(vi));    
Error:
    return status;											 
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureOutputFilter
 * Description: Configures the output filter.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureOutputFilter (ViSession vi,
                                                 ViInt32 Channel,
                                                 ViBoolean AutomaticFilterEnabled,
                                                 ViBoolean OutputFilterEnabled,
                                                 ViReal64 CutoffFrequency)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(AutomaticFilterEnabled), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(OutputFilterEnabled), VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(CutoffFrequency, AGB2900_CUTOFF_FREQUENCY_MIN,      
                                   AGB2900_CUTOFF_FREQUENCY_MAX),                       
               VI_ERROR_PARAMETER5); 
    
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:FILT:AUTO %s"), (Channel),  
                      gpstrSwitchTable[AutomaticFilterEnabled]));			   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:FILT:STAT %s"), (Channel),  
                      gpstrSwitchTable[OutputFilterEnabled]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:FILT:FREQ %Le"), (Channel),  
                      CutoffFrequency)); 
    
    CheckErr(CheckStatus(vi));    
Error:
    return status;												  
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureOutput
 * Description: Configures the output for specified channel.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureOutput(ViSession vi,
                                          ViInt32 Channel,
                                          ViBoolean AutomaticOutputOnEnabled, 
                                          ViBoolean AutomaticOutputOffEnabled,
                                          ViBoolean HighCapacitanceEnabled,
                                          ViInt32 LowTerminalState,
                                          ViInt32 OutputOffMode)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidBooleanVal(AutomaticOutputOnEnabled), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(AutomaticOutputOffEnabled), VI_ERROR_PARAMETER4);    
    CheckParam(IsInvalidBooleanVal(HighCapacitanceEnabled), VI_ERROR_PARAMETER5); 
    CheckParam(IsInvalidNumericVal(LowTerminalState, AGB2900_OUTPUT_LOW_STATE_FLOAT,      
                                   AGB2900_OUTPUT_LOW_STATE_GROUND),                       
               VI_ERROR_PARAMETER6);       			   
    CheckParam(IsInvalidNumericVal(OutputOffMode, AGB2900_OUTPUT_OFF_MODE_ZERO,      
                                   AGB2900_OUTPUT_OFF_MODE_NORM),                       
               VI_ERROR_PARAMETER7); 

    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:ON:AUTO %s"), (Channel),  
                      gpstrSwitchTable[AutomaticOutputOnEnabled]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:OFF:AUTO %s"), (Channel),  
                      gpstrSwitchTable[AutomaticOutputOffEnabled]));	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:HCAP:STAT %s"), (Channel),  
                      gpstrSwitchTable[HighCapacitanceEnabled]));	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:LOW %s"), (Channel),  
                      gpstrLowTerminalState[LowTerminalState]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:OFF:MODE %s"), (Channel),  
                      gpstrOutputOffMode[OutputOffMode]));		
			   
    CheckErr(CheckStatus(vi));    
Error:
    return status;												  
} 

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureOutputFilterExternal
 * Description: Configures the output external filter.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureOutputFilterExternal(ViSession vi,
                                                         ViInt32 Channel, 
                                                         ViInt32 ExternalFilterType, 
                                                         ViBoolean EnableExternalFilter)
{
    ViStatus status = VI_SUCCESS;
	
	/* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(ExternalFilterType, AGB2900_EXTERNAL_FILTER_TYPE_LNF,      
                                   AGB2900_EXTERNAL_FILTER_TYPE_HCULNF),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(EnableExternalFilter), VI_ERROR_PARAMETER4);
	
    if(EnableExternalFilter)
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:FILT:EXT:STAT 0"), (Channel))); 
    }
    else
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:FILT:EXT:STAT 1"), (Channel)));
    }
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:FILT:EXT:TYPE %s"), (Channel),  
                      gpstrExternalFilterType[ExternalFilterType]));
	
	CheckErr(CheckStatus(vi));    
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureOutputResistance
 * Description: Configures the output resistance.
 * Note:        This function is only supported by B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureOutputResistance(ViSession vi,
                                                     ViInt32 Channel, 
                                                     ViInt32 OutputMode, 
                                                     ViInt32 OperationMode, 
                                                     ViConstString ListOfSourceType[],
                                                     ViInt32 ListOfSourceTypeSize,
                                                     ViInt32 ListOfOutputDataSize,
                                                     ViReal64 ListOfOutputData[], 
                                                     ViReal64 ShuntResistanceLevel, 
                                                     ViReal64 SeriesResistanceLevel, 
                                                     ViBoolean EnableProgrammableOutput)
{
    ViStatus status = VI_SUCCESS;
    ViChar strOutputData[BUFFER_SIZE];
    ViChar strSourceType[BUFFER_SIZE]; 
	
	/* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(OutputMode, AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE,      
                                   AGB2900_SOURCE_OUTPUT_MODE_CURRENT),
	           VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(OperationMode, AGB2900_RESISTANCE_OPERATION_MODE_FIXED,      
                                   AGB2900_RESISTANCE_OPERATION_MODE_EMULATE),
	           VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidPtr(ListOfSourceType), VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(ListOfSourceTypeSize, AGB2900_DATA_SIZE_MIN,      
                                   AGB2900_DATA_SIZE_MAX),
			   VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(ListOfOutputDataSize, AGB2900_DATA_SIZE_MIN,      
                                   AGB2900_DATA_SIZE_MAX),
			   VI_ERROR_PARAMETER7);
    CheckParam(IsInvalidPtr(ListOfOutputData), VI_ERROR_PARAMETER8);
    CheckParam(IsInvalidNumericVal(ShuntResistanceLevel, AGB2900_SHUNT_RESISTANCE_LEVEL_MIN,      
                                   AGB2900_SHUNT_RESISTANCE_LEVEL_MAX),
	           VI_ERROR_INSTR_PARAMETER9);
    CheckParam(IsInvalidNumericVal(SeriesResistanceLevel, AGB2900_SERIES_RESISTANCE_LEVEL_MIN,      
                                   AGB2900_SERIES_RESISTANCE_LEVEL_MAX),
	           VI_ERROR_INSTR_PARAMETER10);
    CheckParam(IsInvalidBooleanVal(EnableProgrammableOutput), VI_ERROR_INSTR_PARAMETER11);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:RES:SER %Le"), (Channel),  
                      SeriesResistanceLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:RES:SHUN %Le"), (Channel),  
                      ShuntResistanceLevel));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:RES:MODE %s"), (Channel),  
                      gpstrResistanceOperationMode[OperationMode]));
	
    BuildFlattenStr(ListOfOutputData, ListOfOutputDataSize, "%Le", ",", strOutputData, BUFFER_SIZE); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:RES:EMUL:%s %s"), (Channel),  
                      gpstrSourceOutputMode[OperationMode],strOutputData));
    if(EnableProgrammableOutput)
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:RES:STAT 1"), (Channel)));	
    }
    else
    {
        CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:RES:STAT 0"), (Channel)));
    }
	
    ViInt32 iArrayIdx;
    ViChar * pcStr = strSourceType;
    ViChar * pcEnd = strSourceType + sizeof(strSourceType);
    int n1;
    for(iArrayIdx = 0; iArrayIdx < ListOfSourceTypeSize - 1; ++iArrayIdx)
    {
        n1 = snprintf(pcStr, pcEnd - pcStr, "%s", ListOfSourceType[iArrayIdx]);
        if (n1 > 0) pcStr += (n1 < (int)(pcEnd - pcStr)) ? n1 : (int)(pcEnd - pcStr);
        n1 = snprintf(pcStr, pcEnd - pcStr, ",");
        if (n1 > 0) pcStr += (n1 < (int)(pcEnd - pcStr)) ? n1 : (int)(pcEnd - pcStr);
    }
    snprintf(pcStr, pcEnd - pcStr, "%s", ListOfSourceType[iArrayIdx]);
    CheckErr(viPrintf(vi, RECTIFY_CMD(":OUTP%d:RES:EMUL:MODE %s"), (Channel),strSourceType)); 
	
	CheckErr(CheckStatus(vi));    
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureCLTGPIOPins
 * Description: Configures the CLT GPIO Pins
 * Note:        This function is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureCLTGPIOPins(ViSession vi,
                                               ViInt32 Channel,
                                               ViBoolean PinsForResultOutput[],
                                               ViInt32 ArraySize,
                                               ViInt32 PinForBUSY,
                                               ViInt32 PinForSOT,
                                               ViInt32 PinForEOT)
{
    ViStatus status = VI_SUCCESS;
    ViInt32  i = 0;
    ViInt32  len = 0;
    ViChar   cmdBuf[100];
	
    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidPtr(PinsForResultOutput), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(PinForBUSY, AGB2900_PIN_EXT1,      
                                   AGB2900_PIN_NONE),                       
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(ArraySize, AGB2900_PIN_NUMBER_MIN,      
                                   AGB2900_PIN_NUMBER_MAX),                       
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(PinForSOT, AGB2900_PIN_EXT1,      
                                   AGB2900_PIN_NONE),                       
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(PinForEOT, AGB2900_PIN_EXT1,      
                                   AGB2900_PIN_NONE),                       
               VI_ERROR_PARAMETER7);

    snprintf(cmdBuf, sizeof(cmdBuf), ":CALC%d:DIG:BIT ", (Channel));
    len = (ViInt32)strlen(cmdBuf);
    for(i = 0; i < ArraySize; i++)
    {
        if(PinsForResultOutput[i])
        {
            snprintf(cmdBuf+strlen(cmdBuf), sizeof(cmdBuf)-strlen(cmdBuf), "EXT%d,", i);
        }
    }	
    if((ViInt32)strlen(cmdBuf) != len)
    {
        cmdBuf[strlen(cmdBuf) - 1] = '\n';    /* Change the last character ',' to '\n' */	
    }
    else
    {
        snprintf(cmdBuf+strlen(cmdBuf), sizeof(cmdBuf)-strlen(cmdBuf), "NONE");
    }
    CheckErr(viPrintf(vi, cmdBuf));	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:DIG:BUSY %s"), (Channel), 
                                      gpstrPinNumber[PinForBUSY]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:DIG:SOT %s"), (Channel),
                                      gpstrPinNumber[PinForSOT]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:DIG:EOT %s"), (Channel),
                                      gpstrPinNumber[PinForEOT]));
    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureGPIOPins
 * Description: Configures the GPIO Pins
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureGPIOPins(ViSession vi,
                                            ViInt32 IOFuction,
                                            ViInt32 PinNumber,
                                            ViInt32 FunctionPolarity,
                                            ViInt32 TriggerTiming,
                                            ViInt32 TriggerType,
                                            ViReal64 TriggerPlusWidth)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */ 
    CheckParam(IsInvalidNumericVal(IOFuction, AGB2900_PIN_IO_FUCTION_DINP,      
                                   AGB2900_PIN_IO_FUCTION_TOUT),                       
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(PinNumber, AGB2900_PIN_NUMBER_MIN,      
                                   AGB2900_PIN_NUMBER_MAX),                       
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(FunctionPolarity, AGB2900_PIN_FUNCTION_POLARITY_POSITIVE,      
                                   AGB2900_PIN_FUNCTION_POLARITY_NEGATIVE),                       
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(TriggerTiming,AGB2900_TRIGGER_TIMING_BEFORE,
								   AGB2900_TRIGGER_TIMING_BOTH),                      
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(TriggerType, AGB2900_TRIGGER_TYPE_EDGE,      
                                   AGB2900_TRIGGER_TYPE_LEVEL),                       
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(TriggerPlusWidth, AGB2900_TRIGGER_PLUS_WIDTH_MIN,      
                                   AGB2900_TRIGGER_PLUS_WIDTH_MAX),                       
               VI_ERROR_PARAMETER7);

    CheckErr(viPrintf(vi, RECTIFY_CMD(":FORM:DIG ASC")));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR:DIG:EXT%d:FUNC %s"), PinNumber, 
                                       gpstrIOFuction[IOFuction]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR:DIG:EXT%d:POL %s"), PinNumber, 
                                       gpstrFunctionPolarity[FunctionPolarity]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR:DIG:EXT%d:TOUT:POS %s"), PinNumber, 
                                       gpstrTriggerTiming[TriggerTiming]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR:DIG:EXT%d:TOUT:TYPE %s"), PinNumber, 
                                       gpstrTriggerType[TriggerType]));									   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":SOUR:DIG:EXT%d:TOUT:WIDT %Le"), PinNumber, 
                                       TriggerPlusWidth));	
			   
    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureMathExpression
 * Description: Configures the math expression to perform the Limit Test Caculation.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureMathExpression(ViSession vi,
                                                  ViInt32 Channel,
                                                  ViBoolean ExpressionEnabled,
                                                  ViConstString ExpressionName,
                                                  ViConstString ExpressionDefinition,
                                                  ViConstString ExpressionUnitName)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,      
                                   giInstrIndex),                       
               VI_ERROR_PARAMETER2);     
    CheckParam(IsInvalidBooleanVal(ExpressionEnabled), VI_ERROR_PARAMETER3);	
    CheckParam(IsInvalidPtr(ExpressionName), VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidPtr(ExpressionDefinition), VI_ERROR_PARAMETER5);   
    CheckParam(IsInvalidPtr(ExpressionUnitName), VI_ERROR_PARAMETER6);   

    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:MATH:STAT %s"), (Channel), 
                                       gpstrSwitchTable[ExpressionEnabled])); 	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:MATH:NAME \"%s\""), (Channel), 
                                       ExpressionName)); 	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:MATH:DEF %s"), (Channel),  
                                       ExpressionDefinition)); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:MATH:UNIT \"%s\""), (Channel), 
                                       ExpressionUnitName)); 
									   
    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureTraceBuffer
 * Description: Configures the trace buffer 
 * Note:        One command is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.   
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureTraceBuffer(ViSession vi,
                                               ViInt32 Channel,
                                               ViInt32 BufferSize,
                                               ViInt32 BufferDataType,
                                               ViInt32 BufferControl,
                                               ViInt32 TimeRule)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(BufferSize, AGB2900_TRACE_BUFFER_SIZE_MIN,
                                   AGB2900_TRACE_BUFFER_SIZE_MAX),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(BufferDataType, AGB2900_BUFFER_DATA_TYPE_MATH,
                                   AGB2900_BUFFER_DATA_TYPE_SENSE),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(BufferControl, AGB2900_BUFFER_CONTROL_NEXT,
                                   AGB2900_BUFFER_CONTROL_NEVER),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(TimeRule, AGB2900_TIME_RULE_DELTA,
                                   AGB2900_TIME_RULE_ABSOLUTE),
               VI_ERROR_PARAMETER6);

    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRAC%d:CLE"), (Channel)));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRAC%d:POIN %d"), (Channel), 
                                       BufferSize));
	/* Command ":TRAC[c]:FEED LIM" is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C. */
	if(!((userData>=10&&userData<=13) && (BufferDataType==AGB2900_BUFFER_DATA_TYPE_LIMIT)))
	{
	    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRAC%d:FEED %s"), (Channel), 
	                                       gpstrBufferDataType[BufferDataType]));		
	}
 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRAC%d:FEED:CONT %s"), (Channel), 
                                       gpstrBufferControl[BufferControl])); 
    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRAC%d:TST:FORM %s"), (Channel), 
                                       gpstrTimeRule[TimeRule])); 	
		   
    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ConfigureWaitTime
 * Description: Configures the source/measurement wait time.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureWaitTime(ViSession vi,
                                            ViInt32 Channel,
                                            ViBoolean WaitTimeEnabled,
                                            ViBoolean AutoSetEnabled,
                                            ViReal64 Gain,
                                            ViReal64 Offset,
                                            ViInt32 WaitTimeMode)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);	
    CheckParam(IsInvalidBooleanVal(WaitTimeEnabled), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidBooleanVal(AutoSetEnabled), VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(Gain, AGB2900_WAIT_TIME_GAIN_MIN,
                                   AGB2900_WAIT_TIME_GAIN_MAX),
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidNumericVal(Offset, AGB2900_WAIT_TIME_OFFSET_MIN,
                                   AGB2900_WAIT_TIME_OFFSET_MIN),
               VI_ERROR_PARAMETER6);
    CheckParam(IsInvalidNumericVal(WaitTimeMode, AGB2900_WAIT_TIME_MODE_SOURCE,
                                   AGB2900_WAIT_TIME_MODE_MEASUREMENT),
               VI_ERROR_PARAMETER7);
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":%s%d:WAIT:STAT %s"),gpstrWaitTimeMode[WaitTimeMode], 
                       (Channel), gpstrSwitchTable[WaitTimeEnabled]));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":%s%d:WAIT:AUTO %s"),gpstrWaitTimeMode[WaitTimeMode], 
                       (Channel), gpstrSwitchTable[AutoSetEnabled]));		   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":%s%d:WAIT:GAIN %Le"),gpstrWaitTimeMode[WaitTimeMode], 
                       (Channel), Gain));
    CheckErr(viPrintf(vi, RECTIFY_CMD(":%s%d:WAIT:OFFS %Le"),gpstrWaitTimeMode[WaitTimeMode], 
                       (Channel), Offset)); 
	
    CheckErr(CheckStatus(vi));    
Error:
    return status;	
}

/* -------------------------------------------------------------------------- */
/*                               Data Functions                               */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_Initiate
 * Description: Initiates the specified device action for the specified channel. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_Initiate(ViSession vi,
                                   ViInt32 ChannelList,
                                   ViInt32 DeviceAction)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DeviceAction,AGB2900_DEVICE_ACTION_ACQUIRE,
                                   AGB2900_DEVICE_ACTION_BOTH),
               VI_ERROR_PARAMETER3);
			   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":INIT:%s %s"), gpstrDeviceAction[DeviceAction],
                                      gpstrChanList[ChannelList]));
Error:
    return status;									
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_Abort
 * Description: Aborts the specified device action for the specified channel. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_Abort(ViSession vi,
                                ViInt32 ChannelList,
                                ViInt32 DeviceAction)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DeviceAction,AGB2900_DEVICE_ACTION_ACQUIRE,
                                   AGB2900_DEVICE_ACTION_BOTH),
               VI_ERROR_PARAMETER3);
			   
    CheckErr(viPrintf(vi, RECTIFY_CMD(":ABOR:%s %s"), gpstrDeviceAction[DeviceAction],
                                      gpstrChanList[ChannelList]));
Error:
    return status;									
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_FetchMeasurement
 * Description: Fetches the result of the current measurement.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_FetchMeasurement(ViSession vi,
                                           ViInt32 ChannelList,
                                           ViInt32 DataMode,
                                           ViReal64* MeasuredValue)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DataMode,AGB2900_DATA_MODE_VOLTAGE,
                                   AGB2900_DATA_MODE_TIME),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidPtr(MeasuredValue), VI_ERROR_PARAMETER4);
			   
    CheckErr(viQueryf(vi, RECTIFY_CMD(":FETC:%s? %s"), "%Le", gpstrDataMode[DataMode],
                                      gpstrChanList[ChannelList],MeasuredValue));
Error:
    return status;											   
}


/* -------------------------------------------------------------------------- *
 * Function:    agb2900_WaitForOperationComplete
 * Description: Wait for operation complete.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_WaitForOperationComplete (ViSession vi, 
                                                    ViInt32 ChannelList,
                                                    ViInt32 MaxTime)
{

    ViStatus   status = VI_SUCCESS;
    ViInt32    flag = 0;
    double    timeStart = Timer();
	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(MaxTime,AGB2900_TIMEOUT_MIN,AGB2900_TIMEOUT_MAX),
               VI_ERROR_PARAMETER4);
	
    while(Timer() - timeStart < MaxTime/1000)
    {
        agb2900_QueryStatus(vi, AGB2900_REGISTER_TYPE_MEASUREMENT_STATUS_CONDITION, &flag); 
   	    if(ChannelList == AGB2900_INSTRUMENT_CHANNEL_1 && 
           ((flag >> 1) & 7) == 1 && ((flag >> 4) & 7) == 1)
        {
            break;
        }
        else if(ChannelList == AGB2900_INSTRUMENT_CHANNEL_2 &&
                ((flag >> 7) & 7) == 1 && ((flag >> 10) & 7) == 1)
        {
            break;
        }
        else if(ChannelList == AGB2900_INSTRUMENT_CHANNEL_BOTH &&
                ((flag >> 1) & 7) == 1 && ((flag >> 4) & 7) == 1 &&
                ((flag >> 7) & 7) == 1 && ((flag >> 10) & 7) == 1)
        {
            break;
        }
        else
        {
            Delay(1);
        }
    }
	
Error:	
	return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_WaitForAcquisitionComplete
 * Description: Wait for Acquisition complete.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_WaitForAcquisitionComplete(ViSession vi, 
                                                     ViInt32 ChannelList,
                                                     ViInt32 MaxTime)
{

    ViStatus   status = VI_SUCCESS;
    ViInt32    flag = 0;
    double    timeStart = Timer();
	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(MaxTime,AGB2900_TIMEOUT_MIN,AGB2900_TIMEOUT_MAX),
               VI_ERROR_PARAMETER4);
	
    while(Timer() - timeStart < MaxTime/1000)
    {
        agb2900_QueryStatus(vi, AGB2900_REGISTER_TYPE_MEASUREMENT_STATUS_CONDITION, &flag); 
        if(ChannelList == AGB2900_INSTRUMENT_CHANNEL_1 && ((flag >> 4) & 7) == 1)
        {
            break;
        }
        else if(ChannelList == AGB2900_INSTRUMENT_CHANNEL_2 && ((flag >> 10) & 7) == 1)
        {
            break;
        }
        else if(ChannelList == AGB2900_INSTRUMENT_CHANNEL_BOTH && 
                ((flag >> 4) & 7) == 1 && ((flag >> 10) & 7) == 1)
        {
            break;
        }
        
        else
        {
            Delay(1);
        }	
    }
	
Error:	
	return status;
}



/* -------------------------------------------------------------------------- *
 * Function:    agb2900_FetchMeasurementMultiPoints
 * Description: Fetches the array data which contains all data of the current 
 *              measurement. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_FetchMeasurementMultiPoints(ViSession vi,
                                                      ViInt32 ChannelList,
                                                      ViInt32 DataMode,
                                                      ViInt32 DataSize,
                                                      ViReal64 MeasuredValues[])
{
    ViStatus status = VI_SUCCESS;
    ViChar szData[BUFFER_SIZE_LARGE];
	
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DataMode,AGB2900_DATA_MODE_VOLTAGE,
                                   AGB2900_DATA_MODE_TIME),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(DataMode,AGB2900_DATA_MODE_VOLTAGE,
                                   AGB2900_DATA_MODE_TIME),
               VI_ERROR_PARAMETER4);			   
    CheckParam(IsInvalidPtr(MeasuredValues), VI_ERROR_PARAMETER5);
			   
    CheckErr(viQueryf(vi, RECTIFY_CMD(":FETC:ARR:%s? %s"), "%511[^\n]", gpstrDataMode[DataMode],
                                      gpstrChanList[ChannelList], szData));
    StringToReal64Array(szData, DataSize, MeasuredValues, ',');
Error:
    return status;											   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ReadIndividualLimitTestResult
 * Description: Retrieves the Individual Limit Test Result.
 * Note:        This function is not applicable for B296x family models B2961A/B2962A, B2961B/B2962B, and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ReadIndividualLimitTestResult(ViSession vi,
                                                        ViInt32 Channel,
                                                        ViInt32 TestNumber,
                                                        ViBoolean * TestFailed)
{
    ViStatus status = VI_SUCCESS;
    ViInt32 result = 0;
	
    /* Param Validations */		
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(TestNumber,AGB2900_LIMIT_TEST_NUMBER_MIN,
                                   AGB2900_LIMIT_TEST_NUMBER_MAX),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidPtr(TestFailed), VI_ERROR_PARAMETER4);

    CheckErr(viQueryf(vi, RECTIFY_CMD(":CALC%d:LIM%d:FAIL?"), "%d", (Channel),
                                      TestNumber, &result));
    
    *TestFailed = result?VI_TRUE:VI_FALSE;
									   
    CheckErr(CheckStatus(vi));									   
Error:
    return status;																
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ReadLimitTestResult
 * Description: Retrieves the Limit Test Result.
 * Note:        Commands in the outer conditional statement are not applicable
 *              for B296x family models B2961A/B2962A, B2961B/B2962B,
 *              and B2961C/B2962C.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ReadLimitTestData(ViSession vi,
                                            ViInt32 Channel,
                                            ViInt32 Offset,
                                            ViInt32 Size,
                                            ViBoolean NullOffsetAutoSetEnabled,
                                            ViReal64 * CalculationData,
                                            ViReal64 * TimeStamp, 
                                            ViReal64 * Status, 
                                            ViBoolean TestFailed[],
                                            ViInt32 TestFailedSize
											)
{
    ViStatus status = VI_SUCCESS;
    ViInt32 i = 0;
    ViInt32 result[12];
	
    /* Param Validations */
    CheckErr(viPrintf(vi, RECTIFY_CMD(":FORM:ELEM:CALC CALC,TIME,STAT")));
	/* Commands in the outer conditional statement are not applicable for B2961A/B2962A */
	if (userData>=0&&userData<=9)
	{
	    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
	                                   giInstrIndex),
	               VI_ERROR_PARAMETER2);
	    CheckParam((Offset < AGB2900_LIMIT_TEST_DATA_OFFSET_MIN), VI_ERROR_PARAMETER3);
	    CheckParam(Size < AGB2900_LIMIT_TEST_DATA_SIZE_MIN, VI_ERROR_PARAMETER4);			   
	    CheckParam(IsInvalidBooleanVal(NullOffsetAutoSetEnabled), VI_ERROR_PARAMETER5);
	    CheckParam(IsInvalidPtr(CalculationData), VI_ERROR_PARAMETER6);
	    CheckParam(IsInvalidPtr(TimeStamp), VI_ERROR_PARAMETER7);
	    CheckParam(IsInvalidPtr(Status), VI_ERROR_PARAMETER8);  
	    CheckParam(IsInvalidPtr(TestFailed), VI_ERROR_INSTR_PARAMETER9);
	    CheckParam(IsInvalidNumericVal(TestFailedSize, AGB2900_LIMIT_TEST_NUMBER_MIN,
	                                   AGB2900_LIMIT_TEST_NUMBER_MAX),
	               VI_ERROR_INSTR_PARAMETER10);
	    if(NullOffsetAutoSetEnabled)
	    {
	        CheckErr(viPrintf(vi, RECTIFY_CMD(":CALC%d:OFFS:ACQ"), (Channel)));
	    }
	    CheckErr(viQueryf(vi, RECTIFY_CMD(":CALC%d:DATA? %d,%d"), "%Le,%Le,%Le", (Channel),
	                                       Offset, Size, CalculationData, TimeStamp, Status));
		
	    for(i = 0;i < TestFailedSize;i++)
	    {
	        CheckErr(viQueryf(vi, RECTIFY_CMD(":CALC%d:LIM%d:FAIL?"), "%d", (Channel),
	                                           (i+1), &result[i]));
	        TestFailed[i] = result[i]?VI_TRUE:VI_FALSE;
	    }		
	}

    CheckErr(CheckStatus(vi));									   
Error:
    return status;																
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ReadMathResult
 * Description: Calculate specified math expression and return the result.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ReadMathResult(ViSession vi,
                                         ViInt32 Channel,										 
                                         ViReal64 * CalculationData,
                                         ViReal64 * TimeStamp, 
                                         ViReal64 * Status)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */		
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidPtr(CalculationData), VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidPtr(TimeStamp), VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidPtr(Status), VI_ERROR_PARAMETER5);  
	
    CheckErr(viPrintf(vi, RECTIFY_CMD(":FORM:ELEM:CALC CALC,TIME,STAT")));
    CheckErr(viQueryf(vi, RECTIFY_CMD(":CALC%d:MATH:DATA?"), "%Le,%Le,%Le", (Channel),
                                       CalculationData, TimeStamp, Status));
		
    CheckErr(CheckStatus(vi));									   
Error:
    return status;																
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ReadSpotMeasurement
 * Description: Executes a spot measurement (one-shot measurement) and returns 
 *              the measurement result data.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ReadSpotMeasurement(ViSession vi,
                                              ViInt32 ChannelList,
                                              ViInt32 MeasurementMode,
                                              ViReal64* MeasuredValue)
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */	
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(MeasurementMode,AGB2900_DATA_MODE_VOLTAGE,
                                   AGB2900_DATA_MODE_RESISTANCE),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidPtr(MeasuredValue), VI_ERROR_PARAMETER4);
			   
    CheckErr(viQueryf(vi, RECTIFY_CMD(":MEAS:%s? %s"), "%Le", gpstrMeasurementMode[MeasurementMode],
                                       gpstrChanList[ChannelList], MeasuredValue));

    CheckErr(CheckStatus(vi));	    
Error:
    return status;											   

    
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ReadMeasurement
 * Description: Retrieves the measured value of the current measurement.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ReadMeasurement(ViSession vi,
                                          ViInt32 ChannelList,
                                          ViInt32 DataMode,
                                          ViInt32 MaxTime,
                                          ViReal64* MeasuredValue)
{
    ViStatus status = VI_SUCCESS;
 
    /* Param Validations */		
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DataMode,AGB2900_DATA_MODE_VOLTAGE,
                                   AGB2900_DATA_MODE_TIME),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(MaxTime,AGB2900_TIMEOUT_MIN,AGB2900_TIMEOUT_MAX),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidPtr(MeasuredValue), VI_ERROR_PARAMETER5);
	
    CheckErr(agb2900_Initiate(vi, ChannelList, AGB2900_DEVICE_ACTION_ACQUIRE));	
    CheckErr(agb2900_WaitForAcquisitionComplete(vi, ChannelList, MaxTime));  
    CheckErr(agb2900_FetchMeasurement(vi, ChannelList, DataMode, MeasuredValue));
 
    CheckErr(CheckStatus(vi));	
Error:
	return status;  											   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ReadMeasurementMultiPoints
 * Description: Retrieves the array data which contains all data of the current 
 *              measurement. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ReadMeasurementMultiPoints(ViSession vi,
                                                     ViInt32 ChannelList,
                                                     ViInt32 DataMode,
                                                     ViInt32 MaxTime, 
                                                     ViInt32 DataSize,										   
                                                     ViReal64 MeasuredValues[])
{
    ViStatus status = VI_SUCCESS;
	
    /* Param Validations */		
    CheckParam(IsInvalidNumericVal(ChannelList, AGB2900_INSTRUMENT_CHANNEL_1,
                                   ((giInstrIndex == 2)?(AGB2900_INSTRUMENT_CHANNEL_BOTH)
                                               :(AGB2900_INSTRUMENT_CHANNEL_1))),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DataMode,AGB2900_DATA_MODE_VOLTAGE,
                                   AGB2900_DATA_MODE_TIME),
               VI_ERROR_PARAMETER3);
    CheckParam(IsInvalidNumericVal(MaxTime,AGB2900_TIMEOUT_MIN,AGB2900_TIMEOUT_MAX),
               VI_ERROR_PARAMETER4);
    CheckParam(IsInvalidNumericVal(DataSize, AGB2900_DATA_SIZE_MIN,AGB2900_DATA_SIZE_MAX), 
               VI_ERROR_PARAMETER5);
    CheckParam(IsInvalidPtr(MeasuredValues), VI_ERROR_PARAMETER6);

	
    CheckErr(agb2900_Initiate(vi, ChannelList, AGB2900_DEVICE_ACTION_ACQUIRE));	
    CheckErr(agb2900_WaitForAcquisitionComplete(vi, ChannelList, MaxTime));
    CheckErr(agb2900_FetchMeasurementMultiPoints(vi, ChannelList, DataMode, DataSize, MeasuredValues)); 
    
    CheckErr(CheckStatus(vi));	
Error:
	return status;  										   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ReadTraceBufferStatistic
 * Description: Reads the result of the statistical operation for the data 
 *              stored in the trace buffer.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ReadTraceBufferStatistic(ViSession vi,
                                                   ViInt32 Channel,
                                                   ViInt32 StatisticalOperation,
                                                   ViReal64 * StatisticalData)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */		
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(StatisticalOperation, AGB2900_STATISTICAL_OPERATION_SDEV,
                                   AGB2900_STATISTICAL_OPERATION_MAX),
               VI_ERROR_PARAMETER3); 
    CheckParam(IsInvalidPtr(StatisticalData), VI_ERROR_PARAMETER4);

    CheckErr(viPrintf(vi, RECTIFY_CMD(":TRAC%d:STAT:FORM %s"), (Channel),
                                       gpstrStatisticalOperation[StatisticalOperation]));
    CheckErr(viQueryf(vi, RECTIFY_CMD(":TRAC%d:STAT:DATA?"), "%Le", (Channel),
                                       StatisticalData));
    
    CheckErr(CheckStatus(vi));									   
Error:
    return status;																
   
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ReadMeasurementMultiPoints
 * Description: Reads the data in the trace buffer. 
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ReadTraceBuffer(ViSession vi,
                                          ViInt32 Channel,
                                          ViInt32 DataSize,
                                          ViReal64  ResultData[])    
{
    ViStatus status = VI_SUCCESS;
    ViChar szData[BUFFER_SIZE_LARGE];   
    
    /* Param Validations */		
    CheckParam(IsInvalidNumericVal(Channel, AGB2900_INSTRUMENT_CHANNEL_1,
                                   giInstrIndex),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(DataSize, AGB2900_DATA_SIZE_MIN,AGB2900_DATA_SIZE_MAX), 
               VI_ERROR_PARAMETER3);			   
    CheckParam(IsInvalidPtr(ResultData), VI_ERROR_PARAMETER4);
	
    CheckErr(viQueryf(vi, RECTIFY_CMD(":TRAC%d:DATA?"), "%511[^\n]", (Channel),
                                        szData));
    StringToReal64Array(szData, DataSize, ResultData, ',');   
    
    CheckErr(CheckStatus(vi));									   
Error:
    return status;																
}

/* -------------------------------------------------------------------------- */
/*                             Utility Functions                              */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ErrorQuery
 * Description: Queries the instrument for any errors in the instrument's error
 *              queue.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ErrorQuery(ViSession vi,
                                     ViInt32 * ErrorCode,
                                     ViChar    ErrorMsg[])
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */
    CheckParam(IsInvalidPtr(ErrorCode), VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidPtr(ErrorMsg), VI_ERROR_PARAMETER3);

    CheckErr(viQueryf(vi, RECTIFY_CMD("SYST:ERR?"), "%ld,\"%[^\"]", ErrorCode, ErrorMsg));

Error:
    VerifyStatus(status);
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_Reset
 * Description: Resets the instrument and then sends a set of default setup
 *              commands to the instrument.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_Reset(ViSession vi)
{
    ViStatus status = VI_SUCCESS;

    /* Initialize the instrument to a known state */
    CheckErr(viPrintf(vi, RECTIFY_CMD("*RST")));
    CheckErr(DefaultInstrSetup(vi));

    /* Clean the low-res measurement flag */
    gbDoingLowResMeasure = VI_FALSE;

Error:
    VerifyStatus(status);
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_RevisionQuery
 * Description: Queries the current instrument firmware revision and instrument
 *              driver revision.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_RevisionQuery(ViSession vi,
                                        ViChar    DriverRevision[],
                                        ViChar    FirmwareRevision[])
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */
    CheckParam(IsInvalidPtr(DriverRevision), VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidPtr(FirmwareRevision), VI_ERROR_PARAMETER3);

    CheckErr(viQueryf(vi, RECTIFY_CMD("*IDN?"), "%*[^,],%*[^,],%*[^,],%[^\n]", FirmwareRevision));

    snprintf(DriverRevision, MSG_BUF_SIZE, "%s", AGB2900_REVISION);

Error:
    VerifyStatus(status);
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_SelfTest
 * Description: Runs the instrument's self-test routine and returns the test
 *              results.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_SelfTest(ViSession vi,
                                   ViInt32 * ResultCode,
                                   ViChar    ResultMsg[])
{
    ViStatus status = VI_SUCCESS;
    ViInt32  iOldTimeout = 0;

    /* Param Validations */
    CheckParam(IsInvalidPtr(ResultCode), VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidPtr(ResultMsg), VI_ERROR_PARAMETER3);

    SetNewTimeout(30000, &iOldTimeout);

    CheckErr(viQueryf(vi, RECTIFY_CMD("*TST?"), "%d", ResultCode));
    snprintf(ResultMsg, MSG_BUF_SIZE, "%s", *ResultCode ? "Failed self-test." : "Passed self-test.");

Error:
    SetOldTimeout(iOldTimeout);
    VerifyStatus(status);
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_SaveRecallState
 * Description: Save the current state or recall a previously saved state
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_SaveRecallState(ViSession vi,
                                          ViInt32   Action,
                                          ViInt32   StateNumber)
{
    ViStatus status = VI_SUCCESS;

    /* Param Validations */
    CheckParam(IsInvalidNumericVal(Action, AGB2900_STATE_ACTION_SAVE,
                                   AGB2900_STATE_ACTION_LOAD),
               VI_ERROR_PARAMETER2);
    CheckParam(IsInvalidNumericVal(StateNumber, AGB2900_SAVE_RECALL_STATE_NUM_MIN,
                                   AGB2900_SAVE_RECALL_STATE_NUM_MAX),
               VI_ERROR_PARAMETER3);

    CheckErr(viPrintf(vi, RECTIFY_CMD("%s %d"), gpstrSaveRecallActions[Action],
                      StateNumber));

    /* If we're loading a previously saved state, make sure to update the *
     * low-res measurement status flag.                                   */
    if(Action == AGB2900_STATE_ACTION_LOAD)
        CheckErr(UpdateLowResMeasurementStatus(vi));

    CheckErr(CheckStatus(vi));
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    agb2900_ErrorMessage
 * Description: This function takes the Status Code returned by the instrument
 *              driver functions, interprets it and returns it as a user
 *              readable string.
 * -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ErrorMessage(ViSession vi,
                                       ViStatus  ErrorCode,
                                       ViChar    ErrorMessage[])
{
    ViStatus status = VI_SUCCESS;
    ViInt32  i;

    static agb2900_TStringValPair statusDescArray[] = {
        {VI_WARN_NSUP_ID_QUERY,                "WARNING: ID Query not supported"},
        {VI_WARN_NSUP_RESET,                   "WARNING: Reset not supported"},
        {VI_WARN_NSUP_SELF_TEST,               "WARNING: Self-test not supported"},
        {VI_WARN_NSUP_ERROR_QUERY,             "WARNING: Error Query not supported"},
        {VI_WARN_NSUP_REV_QUERY,               "WARNING: Revision Query not supported"},
        {VI_ERROR_PARAMETER1,                  "ERROR: Parameter 1 out of range"},
        {VI_ERROR_PARAMETER2,                  "ERROR: Parameter 2 out of range"},
        {VI_ERROR_PARAMETER3,                  "ERROR: Parameter 3 out of range"},
        {VI_ERROR_PARAMETER4,                  "ERROR: Parameter 4 out of range"},
        {VI_ERROR_PARAMETER5,                  "ERROR: Parameter 5 out of range"},
        {VI_ERROR_PARAMETER6,                  "ERROR: Parameter 6 out of range"},
        {VI_ERROR_PARAMETER7,                  "ERROR: Parameter 7 out of range"},
        {VI_ERROR_PARAMETER8,                  "ERROR: Parameter 8 out of range"},
        {VI_ERROR_FAIL_ID_QUERY,               "ERROR: Identification query failed"},
        {VI_ERROR_INV_RESPONSE,                "ERROR: Interpreting instrument response"},
        {VI_ERROR_INSTR_FILE_OPEN,             "ERROR: Opening the specified file"},
        {VI_ERROR_INSTR_FILE_WRITE,            "ERROR: Writing to the specified file"},
        {VI_ERROR_INSTR_INTERPRETING_RESPONSE, "ERROR: Interpreting the instrument's response"},
        {VI_ERROR_INSTR_PARAMETER9,            "ERROR: Parameter 9 out of range"},
        {VI_ERROR_INSTR_PARAMETER10,           "ERROR: Parameter 10 out of range"},
        {VI_ERROR_INSTR_PARAMETER11,           "ERROR: Parameter 11 out of range"},
        {VI_ERROR_INSTR_PARAMETER12,           "ERROR: Parameter 12 out of range"},
        {VI_ERROR_INSTR_PARAMETER13,           "ERROR: Parameter 13 out of range"},
        {VI_ERROR_INSTR_PARAMETER14,           "ERROR: Parameter 14 out of range"},
        {VI_ERROR_INSTR_PARAMETER15,           "ERROR: Parameter 15 out of range"}, 
        {AGB2900_ERROR_INSTRUMENT_ERROR,       "ERROR: Instrument Error"},
        {0, VI_NULL}
    };

    /* Check input parameter ranges */
    CheckParam(IsInvalidPtr(ErrorMessage), VI_ERROR_PARAMETER3);

    status = viStatusDesc(vi, ErrorCode, ErrorMessage);
    if (status == VI_WARN_UNKNOWN_STATUS)
    {
        for(i = 0; statusDescArray[i].stringName; i++)
        {
            if(statusDescArray[i].stringVal == ErrorCode)
            {
                snprintf(ErrorMessage, MSG_BUF_SIZE, "%s", statusDescArray[i].stringName);
                return VI_SUCCESS;
            }
        }
        snprintf(ErrorMessage, MSG_BUF_SIZE, "Unknown Error 0x%x[uw8p0]", ErrorCode);
        return VI_WARN_UNKNOWN_STATUS;
    }

    status = VI_SUCCESS;

Error:
    return status;
}						
											
											
/* ========================================================================== */
/* --------------------------- Non-Exported APIs ---------------------------- */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*                     Utility Functions (Non-exported)                       */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 * Function:    CheckStatus
 * Description: Check whether there's any error occurred
 * -------------------------------------------------------------------------- */
static ViStatus CheckStatus(ViSession vi)
{
    ViStatus status = VI_SUCCESS;
    ViInt32  iESR;

    CheckErr(viQueryf(vi, RECTIFY_CMD("*ESR?"), "%ld", &iESR));

    /* Check if any error bit was asserted. */
    if ((iESR & 0x3C) != 0)
        status = AGB2900_ERROR_INSTRUMENT_ERROR;

Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    DefaultInstrSetup
 * Description: Routine to do instrument initialization when setup
 * -------------------------------------------------------------------------- */
static ViStatus DefaultInstrSetup(ViSession vi)
{
    ViStatus status = VI_SUCCESS;

    /*-------------------------------------------------------------------
     * *ESE 60:              Enables command, execution, query, and device
     *                       errors in event status register
     * *SRE 48:              Enables message available, standard event bits
     *                       in the status byte
     * STAT:QUES:ENAB 65535: Enable all bits of the questionable register
     * *CLS:                 Clears status
     *------------------------------------------------------------------- */
    CheckErr(viPrintf(vi, RECTIFY_CMD("*ESE 60;*SRE 48;:STAT:QUES:ENAB 65535;*CLS")));
Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    UpdateLowResMeasurementStatus
 * Description: Check whether the instrument is currently doing low-res
 *              measurement and update the global flag accordingly
 * -------------------------------------------------------------------------- */
static ViStatus UpdateLowResMeasurementStatus(ViSession vi)
{
    ViStatus status = VI_SUCCESS;
    ViChar   szFunction[BUFFER_SIZE];

    static const char * pszLowResFunc = "LRES";
	

    CheckErr(viQueryf(vi, RECTIFY_CMD("FUNC?"), "%s", szFunction));

    gbDoingLowResMeasure = (strcmp(szFunction, pszLowResFunc) == 0) ?
                           VI_TRUE : VI_FALSE;

Error:
    return status;
}

/* -------------------------------------------------------------------------- *
 * Function:    StringToReal64Array
 * Description: Convert the delimiter seperated data string into ViReal64 array.
 *              The return value will be the count of data element converted.
 * -------------------------------------------------------------------------- */
static ViInt32 StringToReal64Array(ViChar  DataStr[],
                                   ViInt32 ResultDataSize,
                                   ViReal64 ResultData[],
                                   ViChar  Delimiter)
{
    ViInt32 i, iArrayIdx, iNumLen, iLoopCnt = (ViInt32)strlen(DataStr);

    for(i = 0, iArrayIdx = 0, iNumLen = 0; iArrayIdx < ResultDataSize && i < iLoopCnt; ++i)
    {
        if(DataStr[i] == Delimiter || i == iLoopCnt - 1)
        {
            sscanf(DataStr + i - iNumLen, "%Le", &ResultData[iArrayIdx++]);
            iNumLen = 0;
        }
        else if(DataStr[i] != Delimiter)
        {
            ++iNumLen;
        }
    }

    return iArrayIdx;
}				

/* -------------------------------------------------------------------------- *
 * Function:    BuildFlattenStr
 * Description: Returns the flatten string of a ViReal64 array with specified
 *              format and delimiter.
 * Note:        As this fucntion is only for internal use (non-exported), we
 *              will not do validation on each parameter. It is the caller's
 *              responsibility to ensure that each parameter passed in is valid.
 * -------------------------------------------------------------------------- */
static ViChar * BuildFlattenStr(ViReal64  Array[],
                                ViInt32  ArraySize,
                                ViChar * Format,
                                ViChar * Delimiter,
                                ViChar   FlattenStr[],
                                ViInt32  BufSize)
{
    ViInt32 iArrayIdx;
    ViChar * pcStr = FlattenStr;
    ViChar * pcEnd = FlattenStr + BufSize;
    int n1;

    for(iArrayIdx = 0; iArrayIdx < ArraySize - 1; ++iArrayIdx)
    {
        n1 = snprintf(pcStr, pcEnd - pcStr, Format, Array[iArrayIdx]);
        if (n1 > 0) pcStr += (n1 < (int)(pcEnd - pcStr)) ? n1 : (int)(pcEnd - pcStr);
        n1 = snprintf(pcStr, pcEnd - pcStr, Delimiter);
        if (n1 > 0) pcStr += (n1 < (int)(pcEnd - pcStr)) ? n1 : (int)(pcEnd - pcStr);
    }
    snprintf(pcStr, pcEnd - pcStr, Format, Array[iArrayIdx]);

    return FlattenStr;
}
