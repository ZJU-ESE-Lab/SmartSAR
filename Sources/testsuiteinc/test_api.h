/*
-------------------------------------------------------------------------------

    Company     : IMH GmbH
    Author      : Bahman Amidzadeh
    Copyright (c) 2003 by IMH GmbH
                  All rights reserved.
    Project     : OSEK/VDX Testsuite

-------------------------------------------------------------------------------

    Filename    : test_api.h

    Description : Definitions of macros and functions for the test suite
-------------------------------------------------------------------------------
*/

#ifndef test_api_h
#define test_api_h

#include "procspec.h"

/*****************************************************
* Constants for test buffers have to be provided
* by procspec.h
*****************************************************
* TEST_START, TEST_END, TEST_OK, TEST_ERROR, TEST_INIT
*****************************************************/

/*----------------------------------------------------
 * The following line is used for extended output information.
 * If you want just the short information output, put it between
 * comment symbols
 * --------------------------------------------------*/
#ifndef _TEST_API_H_
#define _TEST_API_H_

#ifndef uint8
#define uint8   unsigned char
#endif

#ifndef uint16
#define uint16   unsigned int
#endif


/* Provided by test.c */
extern const uint8 testSize;
extern const uint8 testExpectedResults[];
extern uint8 testCurrentResults[];
extern const char TestName[];
extern const uint8 Test_Number;

/* Provided by microcontoller-specific test_api.c */
void  testInit(void);
void  testWrite(uint8 number);
uint8 testCompare(void);

void IsrFirstAction(void);  		/* Function for clearing the Interrupt Flag  and/or implemantion specific Action
							 	 	 * (Hardware and Implemantion specific), must be defined in Test_api.c */
void IsrLastAction(void);   		/* Function for any implemantion specific Action befor executing the ISR
							 	 	 * (Hardware and Implemantion specific), must be defined in Test_api.c */
void ShutdownFirstAction(void);    /* prototype for First Action Function in ShutdownHook Routine.
									 * (Hardware and Implemantion specific) Must be defined in Test_api.C*/
void ShutdownLastAction(void);  	/* prototype for Last Action Function befor executing the ShutdownHook Routine.
									 * (Hardware and Implemantion specific) Must be defined in Test_api.C*/
void HardwareInit (void);		    /* prototype for Hardware Specific Function, must be defined in Test_api.c*/

#endif


DeclareTask(TaskError);			        /* prototype for Error Task, defined in Test.oil */
DeclareAlarm(AlarmError);               /* prototype for Error Alarm, defined in main.oil */

#endif                                  /* test_api_h */
