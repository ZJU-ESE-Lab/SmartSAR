/***********************************************************************
*	Test program for the Alarm Control
*	OS Status: Extended
*	Conformance Class: ECC1
*	Scheduling: non pre-emptive
*	Developed for OSEK\VDX Version 2.2.x and OIL Version 2.4.x
*   Copyright by IMH GmbH
*
*	Author B. Amidzadeh (BA)
*   	Date 05.10.2001,       Start of development (BA)
*          19.11.2002        put declare Tasks, Events ...
*
*

************************************************************************
*/


/* Definition of functions for test */
#include <test_api.h>

const uint8 Test_Number = TestSubNr;     /* 		TestSubNr is per define to set,
 for the value see below  test.bat 4. input parameter (%4)  */

const char TestName[]  = {"ext-ecc1-non-alarm-Test1"};
/* Buffer with the expected test outputs */
const uint8 testExpectedResults[] = {
	TEST_START,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,
	30,31,32,33,34,35,36,37,38,39,
	40,41,42,43,44,45,46,97,47,48,
	49,50,51,52,53,54,55,56,57,58,
	59,60,61,62,63,64,65,66,67,68,
	69,96,70,71,72,73,74,98,75,76,
    77,78,79,80,81,83,TEST_END,255,255 };


/* Length of buffer for the current test outputs */
/* test cases + TEST_START + TEST_END + 2 reserves */


DeclareTask(Task1);
DeclareTask(Task2);
DeclareTask(Task3);

DeclareAlarm(Alarm1);
DeclareAlarm(Alarm2);
DeclareAlarm(Alarm3);
DeclareAlarm(Alarm4);
DeclareAlarm(Alarm5);
DeclareAlarm(Alarm6);
DeclareAlarm(Alarm7);

DeclareEvent(Event1);
DeclareEvent(Event2);


