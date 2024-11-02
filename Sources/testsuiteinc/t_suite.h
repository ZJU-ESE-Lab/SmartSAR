/*
-------------------------------------------------------------------------------
    Company     : IMH GmbH
    Author      : Bahman Amidzadeh
    Copyright (c) by IMH GMBH
                  All rights reserved.
    Project     : OSEK Testsuite
-------------------------------------------------------------------------------
    Filename    : t_suite.h
-------------------------------------------------------------------------------
*/

#ifndef t_suite_h
#define t_suite_h

#include "os.h"

                           /* needed for Messages see test.oil files */
//#include <string.h>

#include "test_api.h"
// #include "com.h"

DeclareTask(com_task);

typedef unsigned int My_int8 ;
/* Convert milliseconds to ticks, must work for values a=10..250 */
#define MS2TICK(a)	(a)

/*-----------------24.06.2003 14:28-----------------
 * SetRelAlarm(AlarmError,MS2TICK(10000),MS2TICK(0));
 * --------------------------------------------------*/
#define  TESTSETRELALARM_ALARMERROR(a,b) SetRelAlarm(AlarmError,MS2TICK(a),MS2TICK(b))
/* e.g.	TESTSETRELALARM_ALARMERROR(10000,0); */


#define INVALID_COMMOD 0x0ff               /* mus be define  for some Tests*/
#define INVALID_MESSAGE 0x0ff           /* mus be define  for some Tests*/


#endif  /* t_suite_h */

