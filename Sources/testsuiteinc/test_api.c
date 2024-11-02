/*
-------------------------------------------------------------------------------

    Company     : IMH GmbH
    Author      : Bahman Amidzadeh
    Copyright (c) by IMH GmbH
                  All rights reserved
-------------------------------------------------------------------------------

    Filename    : test_api.c
    Description : ECU specific file for Motorola 68HC12 on Evaluation Board EVAHC08A with 68HC912D60.
				  OSCAN (Vector OS) for HC12 Specific File
-------------------------------------------------------------------------------
    Date		   Description
    ---------   ---------------------------------------------------------------
		20.10.2001  	First version
-------------------------------------------------------------------------------
*/


#include "test_api.h"
#include "t_suite.h"
#define EXTEVAL
// #define DEBUGING




static int buffpos = 0;

/*--------------------------------------------------------------------------
 *
 *                       void testInit(void)
 *--------------------------------------------------------------------------*/

void testInit(void)
{
	int t_buffpos = 0;
	char ini_specification = {(char)"TestSpecification"};
	unsigned char ini_Test_Number = TestSubNr;
 /*	InitCOM(); */

    OSEKPrintString("/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */ \n");
    OSEKPrintString("/*  --- Test Start -- Test Start -- Test Start -- Test Start --    */ \n");
    OSEKPrintString("/* --- Test running on Motorola 68HC12 CPU and Vector OSCAN OS --- */ \n");
    OSEKPrintString("/*     Testprogram */");
    OSEKPrintString((char*)TestName);
    OSEKPrintString(" Test sub Number <%d> being executed */ \n\r"),
    OSEKPrintString("/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */ \n");
    OSEKPrintString("\n\r");

    for (t_buffpos=0; t_buffpos < testSize; t_buffpos++)
  	{
  	;
	//	testCurrentResults[t_buffpos] = TEST_INIT;
//		printf("/* testCurrentResults =  %d                                        */ \n",(int)testCurrentResults[t_buffpos]);
	}
}

/*--------------------------------------------------------------------------
 *
 *                     void testWrite(uint8 number)
 *
 *--------------------------------------------------------------------------*/

 void testWrite(unsigned char number)
{
  #ifdef DEBUGING
   printf("/*\taktuelle Test Position =\t%d\t*/ \n", (unsigned char) number );
  #endif

    if ( buffpos < (testSize - 1))
      {
       testCurrentResults[buffpos] = number;
       buffpos++;
      }
}

/*--------------------------------------------------------------------------
 *
 *                       uint8 testCompare(void)
 *--------------------------------------------------------------------------*/


uint8 testCompare(void)
{

	int t_buffpos;
    uint8 errorcode = TEST_OK;
	char copm_specification = {(char)"TestSpecification"};
	unsigned char comp_Test_Number = TestSubNr;
    int i,j;

#if defined (EXTEVAL)
	for (t_buffpos=0; t_buffpos < testSize; t_buffpos++)
	{
     OSEKPrintString("Buffer Position =  ");
     OSEKPrintInt(t_buffpos);
     OSEKPrintString(" \t");
         
     if(TEST_END==testExpectedResults[t_buffpos])
     {
     		 OSEKPrintString("End of the Test.\n");    
             break;
     }
     if (testCurrentResults[t_buffpos] != testExpectedResults[t_buffpos])
       {
        errorcode = testCurrentResults[t_buffpos];
        OSEKPrintString("\t\tError\t");
        OSEKPrintString("ExpectedResults = \t");
        OSEKPrintInt((INT32U)testExpectedResults[t_buffpos]);
     	OSEKPrintString(" \t");
        OSEKPrintString("CurrentResults =\t");
        OSEKPrintInt((INT32U)testCurrentResults[t_buffpos]);
        OSEKPrintString(" \n");
       }
     else
       {
        OSEKPrintString("\t\tOK\t");
        OSEKPrintString("ExpectedResults = CurrentResults=\t ");
        OSEKPrintInt((INT32U)testExpectedResults[t_buffpos]);
     	OSEKPrintString(" \n");
       }
    }
#else /* defined(EXTEVAL) */
	for (t_buffpos=0; t_buffpos < testSize; t_buffpos++)
	{
		if(TEST_END==testExpectedResults[t_buffpos])
             break;
		
        if (testCurrentResults[t_buffpos] != testExpectedResults[t_buffpos])
		{
           errorcode = testCurrentResults[t_buffpos];  /* error */
           printf("/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */ \n");
           printf("/*    incorrect Test ... incorrect Test ... incorrect Test         */ \n");
           printf("/*  --- Test running on Intel 80X86 (32 bit) CPU and 3Soft ProOSEK OS for Win32x86 --- */ \n");
           printf("/* First Error on Bufferposition =\t%d\t                       */ \n",t_buffpos);
           printf("CurrentResults =\t%d\t",(int)testCurrentResults[t_buffpos] );
           printf("and\t ExpectedResults =\t%d\t \n",(int)testExpectedResults[t_buffpos]);
           printf("/*     Testprogram <%s> Test sub Number <%d> \t faild */ \n",TestName,comp_Test_Number );
           printf("/*       ---- end of Test ---- end of Test ---- end of Test   ---- */ \n");
           printf("/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */ \n");
           return errorcode;
        }
    }
#endif /* defined(EXTEVAL) */

   

    OSEKPrintString("\n\r");
    OSEKPrintString("/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */ \n");
    OSEKPrintString("/*  --- Test End -- Test End -- Test End -- Test End --    */ \n");
    OSEKPrintString("/* --- Test running on Motorola 68HC12 CPU and Vector OSCAN OS --- */ \n");
    OSEKPrintString("/*     Test program ");
    OSEKPrintString((char*)TestName);
    OSEKPrintString("  being executed */ \n\r"),
    OSEKPrintString("/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */ \n");
    OSEKPrintString("\n\r");

 if (errorcode==TEST_OK)
      {
      	OSEKPrintString("\n\r\n\r***********This case passed.***************\n\r");
      }
    else
    {	OSEKPrintString("\n\r\n\r***********This case failed.***************\n\r");

    }
    return errorcode;
}


 /*-----------------------------------------------------------------------------
  *              void IsrFirstAction(void)
  * Function for clearing the Interrupt Flag and/or
  * any Harware and implemantion specific Action
  * --------------------------------------------------------------------------*/

void IsrFirstAction(void)
 {
 }

/*-----------------------------------------------------------------------------
  *                void IsrLastAction(void)
  * Function for any implemantion or Harware
  * specific Action befor executing the ISR
  * --------------------------------------------------------------------------*/
 void IsrLastAction(void)
 {

 }
/*-----------------------------------------------------------------------------
   *            void ShutdownLastAction(void)
   * First Action in the ShutdownHook Routine.
  * --------------------------------------------------------------------------*/
 void ShutdownFirstAction(void)
  {
  }

/*-----------------------------------------------------------------------------
   *            void ShutdownLastAction(void)
   * Last Action befor executing the ShutdownHook Routine.
  * --------------------------------------------------------------------------*/
 void ShutdownLastAction(void)
  {

  }

/*-----------------------------------------------------------------------------
 *   initialise SystemTimer
    used in StartupHook
* --------------------------------------------------*/
void HardwareInit (void)
 {

   }


/*****************************************************************************/
/* end of file                                                               */
/*****************************************************************************/
