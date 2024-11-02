/*
-------------------------------------------------------------------------------

    Company     : IMH GmbH
    Author      : Peter Grosshans
    Copyright (c) 2000 by IMH GmbH
                  All rights reserved
    Project     : OSEK Serieneinfuerung

-------------------------------------------------------------------------------

    Filename    : procspec.h
    Description : ECU specific file for Motorola 68HC12 on Evaluation Board EVAHC08A with 68HC912D60.
				  This file contains additional information for test run under Motorola OSEK.
-------------------------------------------------------------------------------
    Date/Time   Description
    ---------   ---------------------------------------------------------------
    30.08.1998  First version
    09.02.2000  Declaration of external variables
	05.10.2000  added implementation specific defines
-------------------------------------------------------------------------------
*/

#ifndef _procspec_h
#define _procspec_h
#include	"includes.h"
#include  "app_cfg.h"
#include	"testsuite.h"
#define TestSubNr 1//ÏÈÄ¬ÈÏÎª1°É
#include"t_suite.h"
typedef unsigned char Uint8;


/*****************************************************
* Constants for test buffers, implementation specific
*****************************************************/
#define TEST_START 0
#define TEST_END 199
#define TEST_OK 200
#define TEST_ERROR 254
#define TEST_INIT  255
#define ERROR_HOOK  173


/*-----------------06.04.2000 15:31-----------------
 * Constants for Interrupt Masks, implementation specific
 * --------------------------------------------------*/
#define DISABLEINTERRUPTMASK 0x00
#define ENABLEINTERRUPTMASK 0x10

#define COMPARE_AT_SHUTDOWNHOOK

/****************************************************
* OS does not return E_OK at the standard status
****************************************************/
/* #define NO_E_OK_RETURN_AT_STANDARD */

#endif                                  /* defined _procspec_h */
