/***********************************************************************
*	Test program for the Alarm Control
*	OS Status: Extended
*	Conformance Class: ECC1
*	Scheduling: non pre-emptive
*	Developed for OSEK\VDX Version 2.2.x and OIL Version 2.4.x
*   Copyright by IMH GmbH
*
*	Remark: the API-calls from the interrupt level and hook routines
*		are not tested
*
*		If an API-call returns no status or a returned status
*		will be unknown, the expected status value will be
*       E_OS_RESOURCE, because no API-call for Alarm Control
*		returns this value
*
*   	Author B. Amidzadeh (BA)
*   Date 28.01.2003,       Start of development (BA)
*	Date 05.07.2003,       move the consts to test.h (BA)
*
*
*
*
************************************************************************/


#include <t_suite.h>
#include "test.h"

/* macros uses for busy waiting*/
#define SET_FLAG(a)	a=1
#define WAIT_FOR_FLAG(a)	while(a == 0) {Schedule();}
#define CLEAR_FLAG(a)	a=0


#define BUFFERSIZE sizeof (testExpectedResults)

const uint8 testSize = BUFFERSIZE;

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
StatusType status_task1;
static TaskStateType task_state = SUSPENDED;
static TaskStateRefType p_task_state;


	p_task_state = &task_state;

	if (task1_run == 1)
	{
		testWrite(35);
	}

	if (task1_run == 2)
	{
		testWrite(37);
	}

	if (task1_run == 3)
	{
		testWrite(39);
	}

	if (task1_run == 4)
	{
		testWrite(64);
	}

	if (task1_run == 5)
	{
		status_task1=SetRelAlarm(Alarm5,MS2TICK(10),MS2TICK(0));
		CompareAndtestWrite(72,E_OK,status_task1);
		status_task1= E_OS_RESOURCE;

		do
		{
			status_task1=GetTaskState(Task3, p_task_state);
		}
		while (task_state != READY);
		CompareAndtestWrite(73,E_OK,status_task1);
		status_task1= E_OS_RESOURCE;

		status_task1=Schedule();
		CompareAndtestWrite(98,E_OK,status_task1);
		status_task1= E_OS_RESOURCE;
	}

	if (task1_run == 6)
	{
		testWrite(TEST_ERROR);
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
StatusType status_task2;
TickType tick;
TickRefType p_tick;
static TaskStateType task_state = SUSPENDED;
static TaskStateRefType p_task_state;


	p_tick = &tick;
	p_task_state = &task_state;

	if (task2_run == 1)
	{
		testWrite(36);
	}

	if (task2_run == 2)
	{
		testWrite(38);
	}

	if (task2_run == 3)
	{
		testWrite(40);

		status_task2=CancelAlarm(Alarm1);
		CompareAndtestWrite(41,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;
	}

	if (task2_run == 4)
	{
		testWrite(42);
	}

	if (task2_run == 5)
	{
		testWrite(43);

		status_task2=CancelAlarm(Alarm2);
		CompareAndtestWrite(44,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;

		status_task2=SetRelAlarm(Alarm3,MS2TICK(20),MS2TICK(0));
		CompareAndtestWrite(45,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;

		status_task2=SetRelAlarm(Alarm4,MS2TICK(100),MS2TICK(50));
		CompareAndtestWrite(46,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;

		do
		{
			status_task2=GetTaskState(Task3, p_task_state);
			if (status_task2 != E_OK)
			{
				testWrite(TEST_ERROR);
			}
			else
			{
				;
			}
		}
		while (task_state != READY);
		CompareAndtestWrite(97,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;
	}

	if (task2_run == 6)
	{
		testWrite(58);

		status_task2=SetRelAlarm(Alarm1,MS2TICK(250),MS2TICK(0));
		CompareAndtestWrite(59,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;


		CLEAR_FLAG(task2_flag);

		WAIT_FOR_FLAG(task2_flag); /* busy waiting, Task2 has to be pushed by Task3 */

		testWrite(62);

		CLEAR_FLAG(task2_flag);

		status_task2=SetRelAlarm(Alarm2,MS2TICK(200),MS2TICK(0));
		CompareAndtestWrite(63,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;
	}

	if (task2_run == 7)
	{
		testWrite(65);

		status_task2=SetRelAlarm(Alarm3,MS2TICK(50),MS2TICK(0));
		CompareAndtestWrite(66,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;

		CLEAR_FLAG(task2_flag);

		WAIT_FOR_FLAG(task2_flag); /* busy waiting, Task2 has to be pushed by Task3 */

		CLEAR_FLAG(task2_flag);

		testWrite(96);
	}

	if (task2_run == 8)
	{
		testWrite(70);
	}

	if (task2_run == 9)
	{
		testWrite(71);
	}

	if (task2_run == 10)
	{
		testWrite(75);

		status_task2=CancelAlarm(Alarm1);
		CompareAndtestWrite(76,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;

		status_task2=GetAlarm(Alarm1, p_tick);
		CompareAndtestWrite(77,E_OS_NOFUNC,status_task2);
		status_task2= E_OS_RESOURCE;

		status_task2=CancelAlarm(Alarm2);
		CompareAndtestWrite(78,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;

		status_task2=GetAlarm(Alarm2, p_tick);
		CompareAndtestWrite(79,E_OS_NOFUNC,status_task2);
		status_task2= E_OS_RESOURCE;

		status_task2=SetRelAlarm(Alarm4,MS2TICK(50),MS2TICK(0));
		CompareAndtestWrite(80,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;

		status_task2=SetRelAlarm(Alarm3,MS2TICK(70),MS2TICK(0));
		CompareAndtestWrite(81,E_OK,status_task2);
		status_task2= E_OS_RESOURCE;

		WAIT_FOR_FLAG(task2_flag); /* busy waiting, Task2 has to be pushed by Task3 */
	}

	if (task2_run == 11)
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
TickRefType p_tick;
StatusType status_task3= E_OS_RESOURCE;
AlarmBaseType counter_info;
AlarmBaseRefType p_counter_info;
static uint8 task3_run= 1;


	p_counter_info = &counter_info;
	p_tick = &tick;


	if (task3_run == 1)
	{
        TESTSETRELALARM_ALARMERROR(10000,0);     /* must be define in t_suite.h */


        testWrite(TEST_START);


	/********************************************************************/
	/**** relative alarms, check only return values of API-calls ********/
	/**** no actions will be checked                             ********/
	/********************************************************************/

		status_task3=GetAlarmBase(Alarm6,p_counter_info);
		CompareAndtestWrite(1,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm6,p_tick);
		CompareAndtestWrite(2,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm6);
		CompareAndtestWrite(3,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

        status_task3=SetRelAlarm(Alarm6,counter_info.maxallowedvalue+1 ,MS2TICK(0));
		CompareAndtestWrite(4,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

        status_task3=SetRelAlarm(Alarm6,(-1),MS2TICK(0));
		CompareAndtestWrite(5,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

        status_task3=SetRelAlarm(Alarm6,MS2TICK(30),counter_info.maxallowedvalue+1 );
		CompareAndtestWrite(6,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetRelAlarm(Alarm6,MS2TICK(30),(-1));
		CompareAndtestWrite(7,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

        status_task3=SetRelAlarm(Alarm6,MS2TICK(30),counter_info.mincycle-1);
		CompareAndtestWrite(8,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetRelAlarm(Alarm6,MS2TICK(50),MS2TICK(2));
		CompareAndtestWrite(9,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetRelAlarm(Alarm6,MS2TICK(30),MS2TICK(0));
		CompareAndtestWrite(10,E_OS_STATE,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm6,p_tick);
		CompareAndtestWrite(11,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm6);
		CompareAndtestWrite(12,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm6, p_tick);
		CompareAndtestWrite(13,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm6);
		CompareAndtestWrite(14,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;


	/********************************************************************/
	/**** absolute alarms, check only return values of API-calls ********/
	/**** no actions will be checked                             ********/
	/********************************************************************/

		status_task3=GetAlarmBase(Alarm7,p_counter_info);
		CompareAndtestWrite(15,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm7,p_tick);
		CompareAndtestWrite(16,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm7);
		CompareAndtestWrite(17,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

        status_task3=SetAbsAlarm(Alarm7,counter_info.maxallowedvalue+1 ,MS2TICK(0));
		CompareAndtestWrite(18,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

        status_task3=SetAbsAlarm(Alarm7,(-1),MS2TICK(0));
		CompareAndtestWrite(19,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

        status_task3=SetAbsAlarm(Alarm7,MS2TICK(30),counter_info.maxallowedvalue+1 );
		CompareAndtestWrite(20,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetAbsAlarm(Alarm7,MS2TICK(30),(-1));
		CompareAndtestWrite(21,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

        status_task3=SetAbsAlarm(Alarm7,MS2TICK(30),counter_info.mincycle-1);
		CompareAndtestWrite(22,E_OS_VALUE,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetAbsAlarm(Alarm7,MS2TICK(50),MS2TICK(2));
		CompareAndtestWrite(23,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetAbsAlarm(Alarm7,MS2TICK(70),MS2TICK(0));
		CompareAndtestWrite(24,E_OS_STATE,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm7, p_tick);
		CompareAndtestWrite(25,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm7);
		CompareAndtestWrite(26,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm7, p_tick);
		CompareAndtestWrite(27,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm7);
		CompareAndtestWrite(28,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;


	/****************************************************************************/
	/********** Actions *********************************************************/
	/****************************************************************************/

		status_task3=SetAbsAlarm(Alarm3,MS2TICK(100),MS2TICK(0));
		CompareAndtestWrite(29,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;
    } /* End task3_run == 1 */

	if (task3_run == 2)
	{
		testWrite(30);

		status_task3=SetRelAlarm(Alarm3,MS2TICK(20),MS2TICK(0));
		CompareAndtestWrite(31,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;
	}

	if (task3_run == 3)
	{
		testWrite(32);

		status_task3=SetRelAlarm(Alarm2,MS2TICK(100),MS2TICK(40));
		CompareAndtestWrite(33,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetRelAlarm(Alarm1,MS2TICK(80),MS2TICK(40));
		CompareAndtestWrite(34,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;
	}

	if (task3_run == 4)
	{
		testWrite(47);

		status_task3=WaitEvent(Event1);
		CompareAndtestWrite(48,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=ClearEvent(Event1);
		CompareAndtestWrite(49,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=WaitEvent(Event1);
		CompareAndtestWrite(50,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=ClearEvent(Event1);
		CompareAndtestWrite(51,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=WaitEvent(Event1);
		CompareAndtestWrite(52,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=ClearEvent(Event1);
		CompareAndtestWrite(53,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=CancelAlarm(Alarm4);
		CompareAndtestWrite(54,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=GetAlarm(Alarm4, p_tick);
		CompareAndtestWrite(55,E_OS_NOFUNC,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetRelAlarm(Alarm2,MS2TICK(100),MS2TICK(0));
		CompareAndtestWrite(56,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetRelAlarm(Alarm5,MS2TICK(200),MS2TICK(0));
		CompareAndtestWrite(57,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=WaitEvent(Event2);
		CompareAndtestWrite(60,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=ClearEvent(Event2);
		CompareAndtestWrite(61,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		SET_FLAG(task2_flag);
	}

	if (task3_run == 5)
	{
		testWrite(67);

		status_task3=SetRelAlarm(Alarm2,MS2TICK(50),MS2TICK(50));
		CompareAndtestWrite(68,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=SetRelAlarm(Alarm1,MS2TICK(120),MS2TICK(60));
		CompareAndtestWrite(69,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		SET_FLAG(task2_flag);

		status_task3=WaitEvent(Event2);
		CompareAndtestWrite(74,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

		status_task3=WaitEvent(Event1);
		CompareAndtestWrite(83,E_OK,status_task3);
		status_task3= E_OS_RESOURCE;

	}

	if (task3_run == 6)
	{
		testWrite(TEST_END);


		ShutdownOS(E_OK);
	}

	if (task3_run == 7)
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
	testWrite(TEST_ERROR);
	ShutdownOS(0xff);
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

