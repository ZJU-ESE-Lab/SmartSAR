/******************************************************************************
*	Test program for the Task Management
*	OS Status: Standard
*	Conformance Class: BCC1,
*	Scheduling: non pre-emptive
*
*	Developed for OSEK-VDX Version 2.2 and OIL Version 2.3
*   Copyright by IMH GmbH
*   Sub routunes for some Tests e.g. std\bcc1\non\task\Test1
*
*   	Author B. Amidzadeh (BA)
*   	Date 04.06.2003,       Start of development (BA)
*       Date 24.06.2003,      redefine the TestTaskBuffer
*
*
*
*******************************************************************************/

 #include <t_suite.h>

 TaskType TestActivateSub1(unsigned int Testparameter);
 void TestActivateSub2(TaskType TestTaskNr);
 void TestTerminateSub(void);
/******************************************************************************
 * SubRoutine Testsub for evaluation of OS-API ActivateTask() calls
 * Input = Task Nr (e.g. 2 for Task2) to start
 * output = Task ID (e.g. Task2 if the was 2)
 * The sub Routine write "102" to the current ("testCurrentResults") Buffer

 ******************************************************************************/

TaskType TestActivateSub1(unsigned int Testparameter)
{
     static TaskType TestTaskBuffer[8];
	 static TaskType Aktuell_TestTask;
	 static TaskRefType Ptr_Aktuell_TestTask;
	 static unsigned int Task_Nr;
	 static uint8 Var_Nr = 0;

	if (0 == Var_Nr )
	 {
	   TestTaskBuffer[0] = Task1;
	   TestTaskBuffer[1] = Task1;
	   TestTaskBuffer[2] = Task2;
	   TestTaskBuffer[3] = Task3;
	   TestTaskBuffer[4] = Task4;
	   TestTaskBuffer[5] = Task5;
	   TestTaskBuffer[6] = Task6;
	   TestTaskBuffer[7] = Task7;
	   Var_Nr =1;
	 }

	Task_Nr = Testparameter;
	Ptr_Aktuell_TestTask =  &TestTaskBuffer[Task_Nr];

	Aktuell_TestTask = *Ptr_Aktuell_TestTask;
	testWrite(Task_Nr);                  /* write Task Number to the current buffer */
	TestActivateSub2(Aktuell_TestTask);  /* call TestActivateSub Routine to the
										  * Activate Task with ID =Task Number */
	testWrite(Task_Nr);                  /* write Task Number to the current buffer */
    return Aktuell_TestTask;
}


/******************************************************************************
 * SubRoutine TestActivateSub2 for Activate a Task()
 * Input: TestTaskNr = Task ID
 ******************************************************************************/


void TestActivateSub2(TaskType TestTaskNr)
{
 TaskType ActivateTaskdummy = TestTaskNr ;
static uint8 TestTaskCounter = 62;
 ActivateTask(ActivateTaskdummy);              /* activate Task with ID = TestTaskNr */
 testWrite(TestTaskCounter);
 TestTaskCounter++;
}
/******************************************************************************
 * SubRoutine Testsub for evaluation of OS-API  TerminateTask() calls
 ******************************************************************************/


void TestTerminateSub(void)
 {
	static uint8 TestTaskCounter = 77;
	 testWrite(TestTaskCounter);
	 TestTaskCounter--;
	 TerminateTask();
}


