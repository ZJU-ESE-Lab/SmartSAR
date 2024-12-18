/***********************************************************************
*	Test program for the Alarm Control
*	OS Status: Extended
*	Conformance Class: ECC1
*	Scheduling: mixed pre-emptive
*	Developed for OSEK\VDX Version 2.2.x and OIL Version 2.4.x
*   Copyright by IMH GmbH
*
*	Author B. Amidzadeh (BA)
*   	Date 05.10.2001,       Start of development (BA)
*
*
*
*		File: ..\std\ecc1\mixed\alarm\test3\test.h
*************************************************************************/


/* Definition of functions for test */
#include <test_api.h>

const uint8 Test_Number = TestSubNr; /* 		TestSubNr is per define to set,
 for the value see below  test.bat 4. input parameter (%4)  */



/* Buffer with the expected test outputs */
const uint8 testExpectedResults[] = {
	TEST_START,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,
	TEST_END,255,255 };

/* Length of buffer for the current test outputs */
/*  test cases + TEST_START + TEST_END + 2 reserves */
#define BUFFERSIZE sizeof (testExpectedResults)

const uint8 testSize = BUFFERSIZE;
const char TestName[]  = {"std-ecc1-mixed-alarm-Test3"};

DeclareTask(Task1);
DeclareTask(Task2);
DeclareTask(Task3);

DeclareEvent(Event1);
DeclareEvent(Event2);

DeclareAlarm(Alarm1);
DeclareAlarm(Alarm2);
DeclareAlarm(Alarm3);
DeclareAlarm(Alarm4);
DeclareAlarm(Alarm5);
DeclareAlarm(Alarm6);
DeclareAlarm(Alarm7);

