/******************************************************************************
 *
 * 2011-2021, National Instruments, Corporation.  All Rights Reserved.
 *
 * Title:    agb2900.h
 *
 * Purpose: Agilent Technologies B2900 Series Precision Source/Measure Unit
 *          instrument driver declarations.
 *
 *****************************************************************************/

#ifndef __AGB2900_HEADER
#define __AGB2900_HEADER

#include <cvidef.h>
#include <vpptype.h>	

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* ========================================================================== */
/* ----------- Instrument Driver Specific Error/Warning Codes --------------- */
/* ========================================================================== */
#define VI_ERROR_INSTR_FILE_OPEN                    (_VI_ERROR+0x3FFC0800L)
#define VI_ERROR_INSTR_FILE_WRITE                   (_VI_ERROR+0x3FFC0801L)
#define VI_ERROR_INSTR_INTERPRETING_RESPONSE        (_VI_ERROR+0x3FFC0803L)
#define VI_ERROR_INSTR_PARAMETER9                   (_VI_ERROR+0x3FFC0809L)
#define VI_ERROR_INSTR_PARAMETER10                  (_VI_ERROR+0x3FFC080AL)
#define VI_ERROR_INSTR_PARAMETER11                  (_VI_ERROR+0x3FFC080BL)
#define VI_ERROR_INSTR_PARAMETER12                  (_VI_ERROR+0x3FFC080CL)
#define VI_ERROR_INSTR_PARAMETER13                  (_VI_ERROR+0x3FFC080DL)
#define VI_ERROR_INSTR_PARAMETER14                  (_VI_ERROR+0x3FFC080EL)
#define VI_ERROR_INSTR_PARAMETER15                  (_VI_ERROR+0x3FFC080FL)
    
    


/* ========================================================================== */
/* ----------- Instrument Specific Error/Warning Codes ---------------------- */
/* ========================================================================== */
#define VI_WARNING_INSTR_OFFSET                     (0x3FFC0900L)
#define VI_ERROR_INSTR_OFFSET                       (_VI_ERROR+0x3FFC0900L)

#define AGB2900_ERROR_INSTRUMENT_ERROR              (VI_ERROR_INSTR_OFFSET + 0xF0L)    
    
    
/* ========================================================================== */
/* -------------------- Constants for instrument driver --------------------- */
/* ========================================================================== */

/* --------------------- Instrument Channel --------------------------------- */
#define AGB2900_INSTRUMENT_CHANNEL_1                               (1)
#define AGB2900_INSTRUMENT_CHANNEL_2                               (2)
#define AGB2900_INSTRUMENT_CHANNEL_BOTH                            (3)    

/* --------------------- Instrument Channel --------------------------------- */
#define AGB2900_DEVICE_ACTION_ACQUIRE                              (0)
#define AGB2900_DEVICE_ACTION_TRANSIENT                            (1)
#define AGB2900_DEVICE_ACTION_BOTH                                 (2) 

/* --------------------- Math Expression ----------------------------------- */    
#define AGB2900_MATH_EXPRESSION_DELETE_ONE                         (0)
#define AGB2900_MATH_EXPRESSION_DELETE_ALL                         (1)
    
/* --------------------- Trigger Source ------------------------------------- */
#define AGB2900_TRIGGER_SOURCE_AINT                                (0)
#define AGB2900_TRIGGER_SOURCE_BUS                                 (1)
#define AGB2900_TRIGGER_SOURCE_TIMER                               (2)
#define AGB2900_TRIGGER_SOURCE_INT1                                (3)
#define AGB2900_TRIGGER_SOURCE_INT2                                (4)
#define AGB2900_TRIGGER_SOURCE_LAN                                 (5)
#define AGB2900_TRIGGER_SOURCE_EXT1                                (6)
#define AGB2900_TRIGGER_SOURCE_EXT2                                (7)
#define AGB2900_TRIGGER_SOURCE_EXT3                                (8)
#define AGB2900_TRIGGER_SOURCE_EXT4                                (9)
#define AGB2900_TRIGGER_SOURCE_EXT5                                (10)
#define AGB2900_TRIGGER_SOURCE_EXT6                                (11)
#define AGB2900_TRIGGER_SOURCE_EXT7                                (12)
#define AGB2900_TRIGGER_SOURCE_EXT8                                (13)
#define AGB2900_TRIGGER_SOURCE_EXT9                                (14)
#define AGB2900_TRIGGER_SOURCE_EXT10                               (15)
#define AGB2900_TRIGGER_SOURCE_EXT11                               (16)
#define AGB2900_TRIGGER_SOURCE_EXT12                               (17)
#define AGB2900_TRIGGER_SOURCE_EXT13                               (18)
#define AGB2900_TRIGGER_SOURCE_EXT14                               (19)

/* --------------------- Trigger Signal ------------------------------------- */
#define AGB2900_TRIGGER_SIGNAL_EXT1                                (0)
#define AGB2900_TRIGGER_SIGNAL_EXT2                                (1)
#define AGB2900_TRIGGER_SIGNAL_EXT3                                (2)
#define AGB2900_TRIGGER_SIGNAL_EXT4                                (3)
#define AGB2900_TRIGGER_SIGNAL_EXT5                                (4)
#define AGB2900_TRIGGER_SIGNAL_EXT6                                (5)
#define AGB2900_TRIGGER_SIGNAL_EXT7                                (6)
#define AGB2900_TRIGGER_SIGNAL_EXT8                                (7)
#define AGB2900_TRIGGER_SIGNAL_EXT9                                (8)
#define AGB2900_TRIGGER_SIGNAL_EXT10                               (9)
#define AGB2900_TRIGGER_SIGNAL_EXT11                               (10)
#define AGB2900_TRIGGER_SIGNAL_EXT12                               (11)
#define AGB2900_TRIGGER_SIGNAL_EXT13                               (12)
#define AGB2900_TRIGGER_SIGNAL_EXT14                               (13)
#define AGB2900_TRIGGER_SIGNAL_LAN                                 (14)
#define AGB2900_TRIGGER_SIGNAL_INT1                                (15)
#define AGB2900_TRIGGER_SIGNAL_INT2                                (16)
	

/* --------------------- Trigger Output ------------------------------------- */
#define AGB2900_TRIGGER_OUTPUTPORT_EXT1                            (0)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT2                            (1)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT3                            (2)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT4                            (3)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT5                            (4)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT6                            (5)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT7                            (6)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT8                            (7)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT9                            (8)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT10                           (9)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT11                           (10)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT12                           (11)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT13                           (12)
#define AGB2900_TRIGGER_OUTPUTPORT_EXT14                           (13)
#define AGB2900_TRIGGER_OUTPUTPORT_INT1                            (14)
#define AGB2900_TRIGGER_OUTPUTPORT_INT2                            (15)

/* --------------------- Trigger Timing ------------------------------------- */
#define AGB2900_TRIGGER_TIMING_BEFORE                              (0)
#define AGB2900_TRIGGER_TIMING_AFTER                               (1)
#define AGB2900_TRIGGER_TIMING_BOTH                                (2)

/* --------------------- Trigger Location ----------------------------------- */
#define AGB2900_TRIGGER_LOCATION_IDLE_ARM                          (0)
#define AGB2900_TRIGGER_LOCATION_ARM_TRIGGER                       (1)
#define AGB2900_TRIGGER_LOCATION_TRIGGER_ACQUIRE                   (2)
#define AGB2900_TRIGGER_LOCATION_TRIGGER_TRANSIENT                 (3)

/* ------------------- Limit test operation mode ---------------------------- */
#define AGB2900_LIMIT_TEST_OPERATION_MODE_GRADING                  (0)
#define AGB2900_LIMIT_TEST_OPERATION_MODE_SORTING                  (1)

/* ------------------------ Limit test type --------------------------------- */
#define AGB2900_LIMIT_TEST_TYPE_COMPLIANCE                         (0)
#define AGB2900_LIMIT_TEST_TYPE_LIMIT                              (1)

/* ---------------------- Limit test feed mode ------------------------------ */
#define AGB2900_LIMIT_TEST_FEED_MODE_VOLTAGE                       (0)
#define AGB2900_LIMIT_TEST_FEED_MODE_CURRENT                       (1)
#define AGB2900_LIMIT_TEST_FEED_MODE_RESISTANCE                    (2)
#define AGB2900_LIMIT_TEST_FEED_MODE_MATH                          (3)

/* ------------------- Limit test judgement criteria ------------------------ */
#define AGB2900_LIMIT_TEST_JUDGEMENT_CRITERIA_IN                   (0)
#define AGB2900_LIMIT_TEST_JUDGEMENT_CRITERIA_OUT                  (1)

/* ---------------------- Measurement mode ---------------------------------- */
#define AGB2900_MEASUREMENT_MODE_VOLTAGE                           (0)
#define AGB2900_MEASUREMENT_MODE_CURRENT                           (1)
#define AGB2900_MEASUREMENT_MODE_RESISTANCE                        (2)

/* ---------------- Measurement resistance mode ----------------------------- */
#define AGB2900_MEASUREMENT_RESISTANCE_MODE_MANUAL                 (0)
#define AGB2900_MEASUREMENT_RESISTANCE_MODE_AUTO                   (1)

/* ------------------------- Auto range mode -------------------------------- */
#define AGB2900_AUTO_RANGE_MODE_NORMAL                             (0)
#define AGB2900_AUTO_RANGE_MODE_RESOLUTION                         (1)
#define AGB2900_AUTO_RANGE_MODE_SPEED                              (2)

/* ------------------------- Wait time mode --------------------------------- */
#define AGB2900_WAIT_TIME_MODE_SOURCE                              (0)
#define AGB2900_WAIT_TIME_MODE_MEASUREMENT                         (1)

/* ------------------------- Buffer data type ------------------------------- */
#define AGB2900_BUFFER_DATA_TYPE_MATH                              (0)
#define AGB2900_BUFFER_DATA_TYPE_LIMIT                             (1)
#define AGB2900_BUFFER_DATA_TYPE_SENSE                             (2)

/* -------------------------- Buffer control -------------------------------- */
#define AGB2900_BUFFER_CONTROL_NEXT                                (0)
#define AGB2900_BUFFER_CONTROL_NEVER                               (1)

/* --------------------------- Time rule ------------------------------------ */
#define AGB2900_TIME_RULE_DELTA                                    (0)
#define AGB2900_TIME_RULE_ABSOLUTE                                 (1)

/* ------------------------- Statistical operation -------------------------- */
#define AGB2900_STATISTICAL_OPERATION_SDEV                         (0)
#define AGB2900_STATISTICAL_OPERATION_PKPK                         (1)
#define AGB2900_STATISTICAL_OPERATION_MEAN                         (2)
#define AGB2900_STATISTICAL_OPERATION_MIN                          (3)
#define AGB2900_STATISTICAL_OPERATION_MAX                          (4)

/* ----------------------------- IO fuction --------------------------------- */
#define AGB2900_PIN_IO_FUCTION_DINP                                (0)
#define AGB2900_PIN_IO_FUCTION_DIO                                 (1)
#define AGB2900_PIN_IO_FUCTION_HVOL                                (2)
#define AGB2900_PIN_IO_FUCTION_TINP                                (3)
#define AGB2900_PIN_IO_FUCTION_TOUT                                (4)

/* ------------------------- Pin fuction Polarity --------------------------- */
#define AGB2900_PIN_FUNCTION_POLARITY_POSITIVE                     (0)
#define AGB2900_PIN_FUNCTION_POLARITY_NEGATIVE                     (1)

/* ------------------------- Output trigger type ---------------------------- */
#define AGB2900_TRIGGER_TYPE_EDGE                                  (0)
#define AGB2900_TRIGGER_TYPE_LEVEL                                 (1)

/* --------------------- State Action --------------------------------------- */
#define AGB2900_STATE_ACTION_SAVE                                  (0)
#define AGB2900_STATE_ACTION_LOAD                                  (1)  

/* ------------------------------- Pin NUMBER ------------------------------- */
#define AGB2900_PIN_EXT1                                           (0)
#define AGB2900_PIN_EXT2                                           (1)
#define AGB2900_PIN_EXT3                                           (2)
#define AGB2900_PIN_EXT4                                           (3)
#define AGB2900_PIN_EXT5                                           (4)
#define AGB2900_PIN_EXT6                                           (5)
#define AGB2900_PIN_EXT7                                           (6)
#define AGB2900_PIN_EXT8                                           (7)
#define AGB2900_PIN_EXT9                                           (8)
#define AGB2900_PIN_EXT10                                          (9)
#define AGB2900_PIN_EXT11                                          (10)
#define AGB2900_PIN_EXT12                                          (11)
#define AGB2900_PIN_EXT13                                          (12)
#define AGB2900_PIN_EXT14                                          (13)
#define AGB2900_PIN_NONE                                           (14)
	
/* ----------------------------- Waveform Shape ------------------------------ */
#define AGB2900_WAVEFORM_SHAPE_EXPONENTIAL                         (0)
#define AGB2900_WAVEFORM_SHAPE_RAMP                                (1)
#define AGB2900_WAVEFORM_SHAPE_SINUSOID                            (2)
#define AGB2900_WAVEFORM_SHAPE_SQUARE                              (3)
#define AGB2900_WAVEFORM_SHAPE_TRAPEZOID                           (4)
#define AGB2900_WAVEFORM_SHAPE_TRIANGLE                            (5)
#define AGB2900_WAVEFORM_SHAPE_USERDEFINED                         (6)

/* ------------------------- Fetch Data Mode -------------------------------- */
#define AGB2900_DATA_MODE_VOLTAGE                                  (0)
#define AGB2900_DATA_MODE_CURRENT                                  (1)
#define AGB2900_DATA_MODE_RESISTANCE                               (2)
#define AGB2900_DATA_MODE_SOURCE                                   (3)
#define AGB2900_DATA_MODE_STATUS                                   (4)
#define AGB2900_DATA_MODE_TIME                                     (5)

/* ------------------------ List Output Mode -------------------------------- */
#define AGB2900_SOURCE_OUTPUT_MODE_VOLTAGE                         (0)
#define AGB2900_SOURCE_OUTPUT_MODE_CURRENT                         (1)
    
/* ------------------------ List Data Mode ---------------------------------- */
#define AGB2900_SWEEP_LIST_DATA_MODE_APPEND                        (0)
#define AGB2900_SWEEP_LIST_DATA_MODE_NEW                           (1)

/* ------------------------ List Data Mode ---------------------------------- */
#define AGB2900_SWEEP_MODE_LINEAR_SINGLE                           (0)
#define AGB2900_SWEEP_MODE_LINEAR_DOUBLE                           (1)
#define AGB2900_SWEEP_MODE_LOG_SINGLE                              (2)
#define AGB2900_SWEEP_MODE_LOG_DOUBLE                              (3)

/* ------------------------ List Data Mode ---------------------------------- */
#define AGB2900_SWEEP_DIRECTION_UP                                 (0)
#define AGB2900_SWEEP_DIRECTION_DOWN                               (1)
														   
/* ------------------------ Sweep Ranging Mode ------------------------------ */
#define AGB2900_SWEEP_RANGING_MODE_BEST                            (0)
#define AGB2900_SWEEP_RANGING_MODE_FIX                             (1)
#define AGB2900_SWEEP_RANGING_MODE_AUTO                            (2)

/* ---------------------- Source Output Function ---------------------------- */
#define AGB2900_SOURCE_OUTPUT_FUNCTION_PLUS                        (0)
#define AGB2900_SOURCE_OUTPUT_FUNCTION_DC                          (1)

/* ---------------------- Source Output Function ---------------------------- */
#define AGB2900_OUTPUT_OFF_MODE_ZERO                               (0)
#define AGB2900_OUTPUT_OFF_MODE_HIZ                                (1)
#define AGB2900_OUTPUT_OFF_MODE_NORM                               (2)
	
/* ---------------------- External Filter Type ------------------------------ */ 
#define AGB2900_EXTERNAL_FILTER_TYPE_LNF                           (0)
#define AGB2900_EXTERNAL_FILTER_TYPE_ULNF                          (1)
#define AGB2900_EXTERNAL_FILTER_TYPE_HCULNF						   (2)
	
/* ---------------------- List Output Data Mode ----------------------------- */
#define AGB2900_LIST_OUTPUT_DATA_MODE_APPEND                       (0)
#define AGB2900_LIST_OUTPUT_DATA_MODE_NEW                          (1)
	
/* ---------------------- List Trigger Output Mode -------------------------- */
#define AGB2900_LIST_TRIGGER_OUTPUT_MODE_APPEND                    (0)
#define AGB2900_LIST_TRIGGER_OUTPUT_MODE_NEW                       (1)
	
/* ---------------------- Resistance Operation Mode ------------------------- */ 
#define AGB2900_RESISTANCE_OPERATION_MODE_FIXED                    (0)
#define AGB2900_RESISTANCE_OPERATION_MODE_EMULATE                  (1)

/* ------------------------ Output Low State -------------------------------- */
#define AGB2900_OUTPUT_LOW_STATE_FLOAT                             (0)
#define AGB2900_OUTPUT_LOW_STATE_GROUND                            (1)

/* -------------------- Limit Result Update Mode ---------------------------- */
#define AGB2900_LIMIT_RESULT_UPDATE_MODE_IMMEDIATE                 (0)
#define AGB2900_LIMIT_RESULT_UPDATE_MODE_END                       (1)

/* ----------------------- Register Type ------------------------------------ */
#define AGB2900_REGISTER_TYPE_QUESTIONABLE_STATUS_CONDITION        (0)
#define AGB2900_REGISTER_TYPE_MEASUREMENT_STATUS_CONDITION         (1)
#define AGB2900_REGISTER_TYPE_OPERATION_STATUS_CONDITION           (2)
#define AGB2900_REGISTER_TYPE_QUESTIONABLE_CALIBRATION             (3)
#define AGB2900_REGISTER_TYPE_QUESTIONABLE_CURRENT                 (4)
#define AGB2900_REGISTER_TYPE_QUESTIONABLE_TEMPERATURE             (5)
#define AGB2900_REGISTER_TYPE_QUESTIONABLE_TEST                    (6)
#define AGB2900_REGISTER_TYPE_QUESTIONABLE_VOLTAGE                 (7)
    																					
/* ------------------------- LXI Event  ------------------------------------- */
#define AGB2900_LXI_EVENT_LAN0                                     (0)
#define AGB2900_LXI_EVENT_LAN1                                     (1)  
#define AGB2900_LXI_EVENT_LAN2                                     (2)
#define AGB2900_LXI_EVENT_LAN3                                     (3)  
#define AGB2900_LXI_EVENT_LAN4                                     (4)      
#define AGB2900_LXI_EVENT_LAN5                                     (5)  
#define AGB2900_LXI_EVENT_LAN6                                     (6)  
#define AGB2900_LXI_EVENT_LAN7                                     (7)      

/* ---------------------- LXI Event Edge ------------------------------------ */
#define AGB2900_LXI_EVENT_EDGE_RISE                                (0)
#define AGB2900_LXI_EVENT_EDGE_FALL                                (1)  

/* ---------------------- LXI Event Type ------------------------------------ */
#define AGB2900_LXI_EVENT_TYPE_ARM                                 (0)
#define AGB2900_LXI_EVENT_TYPE_TRIGGER                             (1)  

/* ---------------------- LXI Event Source ---------------------------------- */ 
#define AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_ACQUIRE_ARM1             (0)
#define AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_TRANSITION_ARM1          (1)      
#define AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_ACQUIRE_ARM2             (2)   
#define AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_TRANSITION_ARM2          (3)       
#define AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_ACQUIRE_TRIGGER1         (4)
#define AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_TRANSITION_TRIGGER1      (5)      
#define AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_ACQUIRE_TRIGGER2         (6)   
#define AGB2900_LXI_EVENT_SOURCE_WAIT_FOR_TRANSITION_TRIGGER2      (7)  
#define AGB2900_LXI_EVENT_SOURCE_MEASURING1                        (8)
#define AGB2900_LXI_EVENT_SOURCE_SETTLING1                         (9)  
#define AGB2900_LXI_EVENT_SOURCE_MEASURING2                        (10)  
#define AGB2900_LXI_EVENT_SOURCE_SETTLING2                         (11)  
    
/* ---------------------- LXI Event Detection ------------------------------- */
#define AGB2900_LXI_EVENT_DETECTION_RISE                           (0)
#define AGB2900_LXI_EVENT_DETECTION_FALL                           (1)  
#define AGB2900_LXI_EVENT_DETECTION_HIGH                           (0)
#define AGB2900_LXI_EVENT_DETECTION_LOW                            (1)  

/* ------------------------ LXI Event Slope --------------------------------- */
#define AGB2900_LXI_EVENT_SLOPE_NEGATIVE                           (0)
#define AGB2900_LXI_EVENT_SLOPE_POSITIVE                           (1)  

/* ------------------------ LXI Event Drive --------------------------------- */
#define AGB2900_LXI_EVENT_DRIVE_OFF                                (0)
#define AGB2900_LXI_EVENT_DRIVE_NORMAL                             (1)  
#define AGB2900_LXI_EVENT_DRIVE_WIRED_OR                           (2)  
    

/****************************************************************************
*---------------- Instrument Driver Function Declarations -----------------*
****************************************************************************/

/* ========================================================================== */
/* ------------------------ Initialization / Close Function ----------------- */
/* ========================================================================== */
ViStatus _VI_FUNC  agb2900_Initialize (ViRsrc resourceName,
                                       ViBoolean IDQuery,
                                       ViBoolean resetDevice,
                                       ViSession *vi);
ViStatus _VI_FUNC  agb2900_Close (ViSession vi);   


/* ========================================================================== */
/* ------------------------ Action-Status Functions ------------------------- */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*                           Low Level                                        */
/* -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_AcquireNullOffsetForLimitTest (ViSession vi,
                                                         ViInt32 Channel,
                                                         ViReal64 * NullOffset);
/* -------------------------------------------------------------------------- */

ViStatus _VI_FUNC agb2900_DeleteMathExpression(ViSession vi,
                                               ViInt32 Channel,
                                               ViInt32 Operation,
                                               ViConstString ExpressionName);
ViStatus _VI_FUNC agb2900_EnableOutputAutoOff(ViSession vi,
                                              ViInt32 Channel,
                                              ViBoolean EnableOutputAutoOff);
ViStatus _VI_FUNC agb2900_EnableOutput(ViSession vi,
                                       ViInt32 Channel,
                                       ViBoolean EnableOutput);
ViStatus _VI_FUNC agb2900_EnableRemoteSensing(ViSession vi,
                                              ViInt32 Channel,
                                              ViBoolean EnableRemoteSensing);
ViStatus _VI_FUNC agb2900_QueryOutputProtectionStatus (ViSession vi,
                                                       ViInt32 Channel,
                                                       ViInt32 *OVPStatus,
                                                       ViInt32 *OCPStatus);
ViStatus _VI_FUNC agb2900_ResetRegisters(ViSession vi);
ViStatus _VI_FUNC agb2900_SendSoftwareTrigger(ViSession vi);
ViStatus _VI_FUNC agb2900_SendARMTrigger(ViSession vi,
                                         ViInt32 ChannelList,
                                         ViInt32 DeviceAction);

ViStatus _VI_FUNC agb2900_SendLanEvent (ViSession vi,
                                        ViInt32 LXIEvent, 
                                        ViInt32 LXIEventEdge);

ViStatus _VI_FUNC agb2900_SendImmediateTrigger(ViSession vi,
                                               ViInt32 ChannelList,
                                               ViInt32 DeviceAction);
ViStatus _VI_FUNC agb2900_QueryStatus(ViSession vi,
                                      ViInt32 RegisterType,
                                      ViInt32* StatusValue);
ViStatus _VI_FUNC agb2900_ResetCompositeLimitTest(ViSession vi,
                                                  ViInt32 Channel);												 
ViStatus _VI_FUNC agb2900_ConfigureLXIEventOutput (ViSession vi,
                                                   ViInt32 LXIEvent,
                                                   ViBoolean EnableEventOutput,
                                                   ViInt32 LXIEventSource,
                                                   ViInt32 LXIEventSlope,
                                                   ViInt32 LXIEventDrive,
                                                   ViConstString LXIEventDestination);

/* ========================================================================== */
/* ------------------------ Configure Functions ----------------------------- */
/* ========================================================================== */


ViStatus _VI_FUNC agb2900_ConfigureLXIForARMTrigger (ViSession vi,
                                                     ViInt32 LXIEvent,
                                                     ViBoolean EnableLXIEvent,
                                                     ViInt32 LXIEventType,
                                                     ViInt32 LXIEventDetection,
                                                     ViInt32 LXIEventSource,
                                                     ViConstString LXIEventDestination);
ViStatus _VI_FUNC agb2900_ConfigureInterlockThresholdLevel(ViSession vi,
                                                           ViReal64 InterlockThresholdLevel);

/* -------------------------------------------------------------------------- */
/*                           ARM-Trigger                                      */
/* -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureARMLayerTrigger(ViSession vi,
                                                   ViInt32 Channel,
                                                   ViInt32 DeviceAction,
                                                   ViInt32 ArmTriggerSource, 
                                                   ViInt32 ArmCount,
                                                   ViInt32 ArmDelay,
                                                   ViReal64 ArmInterval, 
                                                   ViBoolean ArmBypassEnabled,
                                                   ViBoolean LXITriggers[],
                                                   ViInt32 ArraySize);
ViStatus _VI_FUNC agb2900_ConfigureTrigger(ViSession vi,
                                           ViInt32 Channel,
                                           ViInt32 DeviceAction,
                                           ViInt32 TriggerSource, 
                                           ViInt32 TriggerCount,
                                           ViInt32 TriggerDelay,
                                           ViReal64 TimerInterval, 
                                           ViBoolean TriggerBypassEnabled,
                                           ViBoolean LXITriggers[],
                                           ViInt32 ArraySize);												   
ViStatus _VI_FUNC agb2900_ConfigureOutputTrigger(ViSession vi,
                                                 ViInt32 Channel,
                                                 ViInt32 DeviceAction,
                                                 ViInt32 TriggerSignal,												 
                                                 ViBoolean TriggerEnabled,
                                                 ViInt32 TriggerLocation,
                                                 ViInt32 TriggerTiming);

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                               Limit Test                                   */
/* -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureCompositeLimitTest(ViSession vi,
                                                      ViInt32 Channel,
                                                      ViBoolean EnableCompositeLimitTest,
                                                      ViInt32 OperationMode,
                                                      ViInt32 LimitResultUpdateMode,
                                                      ViInt32 PassPatternBits,
                                                      ViInt32 FailPatternBits);
ViStatus _VI_FUNC agb2900_ConfigureIndividualLimitTest(ViSession vi,
                                                       ViInt32 Channel,                   
                                                       ViInt32 TestNumber,
                                                       ViBoolean EnableIndividualLimitTest,
                                                       ViReal64 LowerLimit,
                                                       ViReal64 UpperLimit,
                                                       ViInt32 TestType,													   
                                                       ViInt32 FeedMode,
                                                       ViInt32 JudgementCriteria);
ViStatus _VI_FUNC agb2900_ConfigureLimitTestNullOffset(ViSession vi,
                                                       ViInt32 Channel,
                                                       ViBoolean NullOffsetAutoSetEnabled,
                                                       ViBoolean NullOffsetFuncEnabled,
                                                       ViReal64 NullOffset);
ViStatus _VI_FUNC agb2900_ConfigureLimitTestResultBitPattern(ViSession vi,
                                                             ViInt32 Channel,
                                                             ViInt32 TestNumber,
                                                             ViInt32 PassPattern,
                                                             ViInt32 LowerExceedingFailurePattern,
                                                             ViInt32 UpperExceedingFailurePattern,
                                                             ViInt32 ComplianceFailurePattern);

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                               Measurement                                  */
/* -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureMeasurement(ViSession vi,
                                               ViInt32 Channel,
                                               ViInt32 MeasurementMode,
                                               ViReal64 Range,
                                               ViBoolean AutomaticNPLCEnabled,
                                               ViReal64 PowerLifeCycle,
                                               ViInt32 ResistanceMode,
                                               ViBoolean ResistanceCompensatedEnabled);
ViStatus _VI_FUNC agb2900_ConfigureMeasurementAutoRange(ViSession vi,
                                                        ViInt32 Channel,
                                                        ViInt32 MeasurementMode,
                                                        ViBoolean AutoRangeEnabled,
                                                        ViReal64 LowerLimit,
                                                        ViReal64 UpperLimit,
                                                        ViReal64 ThresholdRate,
                                                        ViInt32 AutoRangeMode);
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                               Source Output                                */
/* -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureSourceAutoRange(ViSession vi,
                                                   ViInt32 Channel,
                                                   ViInt32 OutputMode,
                                                   ViBoolean AutoRangeEnabled,
                                                   ViReal64 LowerLimitForAutoRange);
ViStatus _VI_FUNC agb2900_ConfigureSource(ViSession vi,
                                          ViInt32 Channel,
                                          ViInt32 OutputMode,
                                          ViInt32 OutputFuction,
                                          ViReal64 OutputLevel,
                                          ViReal64 TriggerOutputLevel,
                                          ViReal64 OutputRange,
                                          ViBoolean ContinuousTriggerEnabled);
ViStatus _VI_FUNC agb2900_ConfigureOutputProtection(ViSession vi,
                                                    ViInt32 Channel, 
                                                    ViBoolean OutputProtectionEnabled, 
                                                    ViReal64 ComplianceVoltage, 
                                                    ViReal64 ComplianceCurrent);
ViStatus _VI_FUNC agb2900_ConfigurePulse(ViSession vi,
                                         ViInt32 Channel,
                                         ViReal64 PulseDelay,
                                         ViReal64 PulseWidth);

ViStatus _VI_FUNC agb2900_ConfigureOutputFilter(ViSession vi,
                                                ViInt32 Channel,
                                                ViBoolean AutomaticFilterEnabled,
                                                ViBoolean OutputFilterEnabled,
                                                ViReal64 CutoffFrequency); 

ViStatus _VI_FUNC agb2900_ConfigureOutput(ViSession vi,
                                          ViInt32 Channel,
                                          ViBoolean AutomaticOutputOnEnabled,
                                          ViBoolean AutomaticOutputOffEnabled,
                                          ViBoolean HighCapacitanceEnabled,
                                          ViInt32 LowTerminalState,
                                          ViInt32 OutputOffMode);
ViStatus _VI_FUNC  agb2900_ConfigureOutputFilterExternal(ViSession vi,
                                                         ViInt32 Channel, 
                                                         ViInt32 ExternalFilterType, 
                                                         ViBoolean EnableExternalFilter);
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
                                                     ViBoolean EnableProgrammableOutput);
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/*                             Arbitrary Waveform                             */
/* -------------------------------------------------------------------------- */
ViStatus _VI_FUNC  agb2900_ConfigureArbitraryWaveform(ViSession vi,
                                                      ViInt32 Channel, 
                                                      ViInt32 ShapeOfWaveform, 
                                                      ViReal64 NumberOfWaveforms);
ViStatus _VI_FUNC  agb2900_ConfigureWaveformExponential(ViSession vi,
                                                        ViInt32 Channel, 
                                                        ViInt32 OutputMode, 
                                                        ViReal64 StartTime, 
                                                        ViReal64 StartLevel, 
                                                        ViReal64 EndLevel, 
                                                        ViReal64 TimeConstant, 
                                                        ViReal64 TimeSetting);
ViStatus _VI_FUNC  agb2900_ConfigureWaveformRamp(ViSession vi,
                                                 ViInt32 Channel, 
                                                 ViInt32 OutputMode, 
                                                 ViReal64 RiseTime, 
                                                 ViReal64 StartLevel, 
                                                 ViReal64 EndLevel, 
                                                 ViReal64 StartTime, 
                                                 ViReal64 EndTime);
ViStatus _VI_FUNC  agb2900_ConfigureWaveformSinusoid(ViSession vi,
                                                     ViInt32 Channel, 
                                                     ViInt32 OutputMode, 
                                                     ViInt32 TriggerOutputPort, 
                                                     ViReal64 Amplitude, 
                                                     ViReal64 Frequency, 
                                                     ViReal64 OffsetLevel, 
                                                     ViReal64 PhaseMarker, 
                                                     ViBoolean EnableTriggerOutput);
ViStatus _VI_FUNC  agb2900_ConfigureWaveformSquare(ViSession vi,
                                                   ViInt32 Channel, 
                                                   ViInt32 OutputMode, 
                                                   ViReal64 TopTime, 
                                                   ViReal64 StartLevel, 
                                                   ViReal64 TopLevel, 
                                                   ViReal64 StartTime, 
                                                   ViReal64 EndTime);
ViStatus _VI_FUNC  agb2900_ConfigureWaveformTrapezoid(ViSession vi,
                                                      ViInt32 Channel, 
                                                      ViInt32 OutputMode, 
                                                      ViReal64 TopTime, 
                                                      ViReal64 StartLevel,
                                                      ViReal64 TopLevel, 
                                                      ViReal64 StartTime, 
                                                      ViReal64 EndTime, 
                                                      ViReal64 RiseTime, 
                                                      ViReal64 FallTime);
ViStatus _VI_FUNC  agb2900_ConfigureWaveformTriangle(ViSession vi,
                                                     ViInt32 Channel, 
                                                     ViInt32 OutputMode, 
                                                     ViReal64 StartLevel, 
                                                     ViReal64 TopLevel, 
                                                     ViReal64 StartTime, 
                                                     ViReal64 EndTime, 
                                                     ViReal64 RiseTime, 
                                                     ViReal64 FallTime);
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
                                                        ViInt32 ListTriggerOutputSize);  
/* -------------------------------------------------------------------------- */
/*                                     Sweep                                  */
/* -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_ConfigureSweep(ViSession vi,
                                         ViInt32 Channel,
                                         ViInt32 OutputMode,
                                         ViBoolean SweepEnabled,
                                         ViReal64 SweepStartValue,
                                         ViReal64 SweepStopValue,
                                         ViInt32 SweepStepCount,
                                         ViInt32 SweepMode,
                                         ViInt32 SweepDirection);  
ViStatus _VI_FUNC agb2900_ConfigureSourceRangeMode (ViSession instrumentHandle,
                                                    ViInt32 Channel,
                                                    ViInt32 SweepRangingMode);
ViStatus _VI_FUNC agb2900_ConfigureListSweep(ViSession vi,
                                             ViInt32 Channel,
                                             ViInt32 OutputMode,
                                             ViBoolean ListSweepEnabled,
                                             ViReal64 ListData[],
                                             ViInt32 DataLen,
                                             ViInt32 ListDataMode,
                                             ViInt32 StartIndex);
/* -------------------------------------------------------------------------- */

ViStatus _VI_FUNC agb2900_ConfigureCLTGPIOPins(ViSession vi,
                                               ViInt32 Channel,
                                               ViBoolean PinsForResultOutput[],
                                               ViInt32 ArraySize,
                                               ViInt32 PinForBUSY,
                                               ViInt32 PinForEOT,
                                               ViInt32 PinForSOT);
ViStatus _VI_FUNC agb2900_ConfigureGPIOPins(ViSession vi, 
                                            ViInt32 IOFuction,
                                            ViInt32 PinNumber,
                                            ViInt32 FunctionPolarity,
                                            ViInt32 TriggerTiming,
                                            ViInt32 TriggerType,
                                            ViReal64 TriggerPlusWidth);										
ViStatus _VI_FUNC agb2900_ConfigureMathExpression(ViSession vi,
                                                  ViInt32 Channel,
                                                  ViBoolean ExpressionEnabled,
                                                  ViConstString ExpressionName,
                                                  ViConstString ExpressionDefinition,
                                                  ViConstString ExpressionUnitName);							
ViStatus _VI_FUNC agb2900_ConfigureTraceBuffer(ViSession vi,
                                               ViInt32 Channel,
                                               ViInt32 BufferSize,
                                               ViInt32 BufferDataType,
                                               ViInt32 BufferControl,
                                               ViInt32 TimeRule);
ViStatus _VI_FUNC agb2900_ConfigureWaitTime(ViSession vi,
                                            ViInt32 Channel,
                                            ViBoolean WaitTimeEnabled,
                                            ViBoolean AutoSetEnabled,
                                            ViReal64 Gain,
                                            ViReal64 Offset,
                                            ViInt32 WaitTimeMode);

/* ========================================================================== */
/* -------------------------------- Data ------------------------------------ */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*                           Low Level                                        */
/* -------------------------------------------------------------------------- */
ViStatus _VI_FUNC agb2900_Abort(ViSession vi,
                                ViInt32 ChannelList,
                                ViInt32 DeviceAction);
ViStatus _VI_FUNC agb2900_Initiate(ViSession vi,
                                   ViInt32 ChannelList,
                                   ViInt32 DeviceAction);
ViStatus _VI_FUNC agb2900_FetchMeasurement(ViSession vi,
                                           ViInt32 ChannelList,
                                           ViInt32 DataMode,
                                           ViReal64* MeasuredValue);
ViStatus _VI_FUNC agb2900_FetchMeasurementMultiPoints(ViSession vi,
                                                      ViInt32 ChannelList,
                                                      ViInt32 DataMode,
                                                      ViInt32 DataSize,
                                                      ViReal64 MeasuredValues[]);
ViStatus _VI_FUNC agb2900_ReadIndividualLimitTestResult(ViSession vi,
                                                        ViInt32 Channel,
                                                        ViInt32 TestNumber,
                                                        ViBoolean * TestFailed);
ViStatus _VI_FUNC agb2900_WaitForOperationComplete (ViSession vi,
                                                    ViInt32 ChannelList,
                                                    ViInt32 MaxTime);
ViStatus _VI_FUNC agb2900_WaitForAcquisitionComplete(ViSession vi, 
                                                     ViInt32 ChannelList,
                                                     ViInt32 MaxTime);
/* -------------------------------------------------------------------------- */

ViStatus _VI_FUNC agb2900_ReadLimitTestData(ViSession vi,
                                            ViInt32 Channel,
                                            ViInt32 Offset,
                                            ViInt32 Size,
                                            ViBoolean NullOffsetAutoSetEnabled,
                                            ViReal64 * CalculationData,
                                            ViReal64 * TimeStamp, 
                                            ViReal64 * Status, 
                                            ViBoolean TestFailed[],
                                            ViInt32 TestFailedSize);
ViStatus _VI_FUNC agb2900_ReadMathResult(ViSession vi,
                                         ViInt32 Channel,										 
                                         ViReal64 * CalculationData,
                                         ViReal64 * TimeStamp, 
                                         ViReal64 * Status);
ViStatus _VI_FUNC agb2900_ReadSpotMeasurement(ViSession vi,
                                              ViInt32 ChannelList,
                                              ViInt32 MeasurementMode,
                                              ViReal64* MeasuredValue);
ViStatus _VI_FUNC agb2900_ReadMeasurement(ViSession vi,
                                          ViInt32 ChannelList,
                                          ViInt32 DataMode,
                                          ViInt32 MaxTime,
                                          ViReal64 * MeasuredValue);
ViStatus _VI_FUNC agb2900_ReadMeasurementMultiPoints(ViSession vi,
                                                     ViInt32 ChannelList,
                                                     ViInt32 DataMode,
                                                     ViInt32 MaxTime, 
                                                     ViInt32 DataSize,										   
                                                     ViReal64 MeasuredValues[]);
ViStatus _VI_FUNC agb2900_ReadTraceBufferStatistic(ViSession vi,
                                                   ViInt32 Channel,
                                                   ViInt32 StatisticalOperation,
                                                   ViReal64 * StatisticalData);
ViStatus _VI_FUNC agb2900_ReadTraceBuffer(ViSession vi,
                                          ViInt32 Channel,
                                          ViInt32 DataSize,
                                          ViReal64 ResultData[]);    

/* ========================================================================== */
/* ------------------------------ Utility ----------------------------------- */
/* ========================================================================== */
ViStatus _VI_FUNC agb2900_ErrorQuery(ViSession vi,
                                     ViInt32 * ErrorCode,
                                     ViChar    ErrorMsg[]);
ViStatus _VI_FUNC agb2900_Reset(ViSession vi);
ViStatus _VI_FUNC agb2900_RevisionQuery(ViSession vi,
                                        ViChar    DriverRevision[],
                                        ViChar    FirmwareRevision[]);
ViStatus _VI_FUNC agb2900_SelfTest(ViSession vi,
                                   ViInt32 * ResultCode,
                                   ViChar    ResultMsg[]);
ViStatus _VI_FUNC agb2900_ErrorMessage(ViSession vi,
                                       ViStatus  ErrorCode,
                                       ViChar    ErrorMessage[]);
ViStatus _VI_FUNC agb2900_SaveRecallState(ViSession vi,
                                          ViInt32   Action,
                                          ViInt32   StateNumber);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __AGB2900_HEADER */	
	
