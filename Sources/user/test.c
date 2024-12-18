/***********************************************************************
*	Test program for the Alarm Control
*	OS Status: Extended
*	Conformance Class: ECC1
*	Scheduling: mixed pre-emptive
*	Developed for OSEK\VDX Version 2.2.x and OIL Version 2.4.x
*   Copyright by IMH GmbH
*
*	Remark: the API-calls from the interrupt level and hook routines
*			are not tested
*
*	        If an API-call returns no status or a returned status
*			will be unknown, the expected status value will be
*           E_OS_RESOURCE, because no API-call for Alarm Control
*			returns this value
*
*   Author B. Amidzadeh (BA)
*   Date 28.01.2003,       Start of development (BA)
*	Date 05.07.2003,       move the consts to test.h (BA)
*
*
*
*    File: ..\std\ecc1\mixed\alarm\test3\test.c
*
************************************************************************/


#include <t_suite.h>
#include "test.h"


/* macros uses for busy waiting*/
#define SET_FLAG(a)	a=1
#define WAIT_FOR_FLAG(a)	while(a == 0) {;}
#define CLEAR_FLAG(a)	a=0

/* Buffer with the real test outputs */
uint8 testCurrentResults[BUFFERSIZE];


/* flag for Task2 for busy waiting */
volatile uint8 task2_flag= 0;



/* Local funktion (or macro) for easier use of Test-API  */
void CompareAndtestWrite(uint8 number, StatusType expected_status, StatusType is_status)
{
	if(is_status == expected_status)
	{
		testWrite(number);
	}
	else
	{
		testWrite(TEST_ERROR);
	}
}



/*************************************************
**************** Task1 ***************************
**************************************************
*/
TASK(Task1)
{
static uint8 task1_run = 1;

	if (1 == task1_run)
	{
		testWrite(TEST_END);
 		ShutdownOS(E_OK);
	}

	if (2 == task1_run)
	{
		testWrite(37);
	}


 task1_run++;
 TerminateTask();
}



/*************************************************
**************** Task2 ***************************
**************************************************
*/
TASK(Task2)
{
static uint8 task2_run = 1;

  if (1 == task2_run)
	{
		testWrite(29);
	}

  if (2 == task2_run)
	{
	 	testWrite(TEST_ERROR);
	}

	task2_run++;


TerminateTask();
}



/*************************************************
**************** Task3 ***************************
**************************************************
*/
TASK(Task3)
{
TickType tick;
TickRefType ptr_tick;
StatusType status_task3= E_OS_RESOURCE;
AlarmBaseType counter_info;
AlarmBaseRefType ptr_counter_info;
static uint8 task3_run= 1;

	ptr_counter_info = &counter_info;
	ptr_tick = &tick;


	if (1 == task3_run)
	{
		  CancelAlarm(AlarmError);
		   	 	 TESTSETRELALARM_ALARMERROR(10000,0);     /* must be define in t_suite.h */


        testWrite(TEST_START);

	/********************************************************************/
	/**** autostart alarms , check only return values of API-calls ********/
	/**** no actions will be checked                             ********/
	/********************************************************************/

		status_task3=GetAlarmBase(Alarm7,ptr_counter_info);
		CompareAndtestWrite(1,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm7,ptr_tick);
		CompareAndtestWrite(2,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm7);
		CompareAndtestWrite(3,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;


		status_task3=GetAlarmBase(Alarm6,ptr_counter_info);
		CompareAndtestWrite(4,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm6,ptr_tick);
		CompareAndtestWrite(5,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm6);
		CompareAndtestWrite(6,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;


		status_task3=GetAlarmBase(Alarm5,ptr_counter_info);
		CompareAndtestWrite(7,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm5,ptr_tick);
		CompareAndtestWrite(8,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm5);
		CompareAndtestWrite(9,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;


		status_task3=GetAlarmBase(Alarm4,ptr_counter_info);
		CompareAndtestWrite(10,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm4,ptr_tick);
		CompareAndtestWrite(11,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm4);
		CompareAndtestWrite(12,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarmBase(Alarm3,ptr_counter_info);
		CompareAndtestWrite(13,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm3,ptr_tick);
		CompareAndtestWrite(14,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm3);
		CompareAndtestWrite(15,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;


  		status_task3=GetAlarmBase(Alarm2,ptr_counter_info);
		CompareAndtestWrite(16,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm2,ptr_tick);
		CompareAndtestWrite(17,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm2);
		CompareAndtestWrite(18,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;


		status_task3=GetAlarmBase(Alarm1,ptr_counter_info);
		CompareAndtestWrite(19,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm1,ptr_tick);
		CompareAndtestWrite(20,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm1);
		CompareAndtestWrite(21,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm1,ptr_tick);
		CompareAndtestWrite(22,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm2,ptr_tick);
		CompareAndtestWrite(23,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm3,ptr_tick);
		CompareAndtestWrite(24,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm4,ptr_tick);
		CompareAndtestWrite(25,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;


		status_task3=GetAlarm(Alarm5,ptr_tick);
		CompareAndtestWrite(26,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm6,ptr_tick);
		CompareAndtestWrite(27,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm7,ptr_tick);
		CompareAndtestWrite(28,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

    } /* End task3_run == 1 */




	if (2 == task3_run)
	{
		testWrite(TEST_ERROR);
	}

	task3_run++;


TerminateTask();
}


/**************************************************
**************** Error Task ***********************
***************************************************
*/

TASK(TaskError)
{
	static uint8 taskError_run= 1;
	if ( 2 == taskError_run)
	  {
	    testWrite(TEST_ERROR);
       ShutdownOS(0xff);
	  }
	taskError_run++;
	TerminateTask();
}

/**************************************************
**************** ShutdownHook *********************
***************************************************
*/
void ShutdownHook(StatusType Error)
        {
		   testCompare();
        }


/**************************************************
*************** StartupHook *********************
***************************************************
*/


void StartupHook(void)
	{
	testInit();
	}


