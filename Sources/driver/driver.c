/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�driver.c
* ժ    Ҫ��������ģ��ļĴ��������ļ�
*
* ��ǰ�汾��1.0
* ��    �ߣ������
* ������ڣ�2005��2��20��
*********************************************************************************
*/
#ifndef  _HCS12_DRIVER_C_
#define  _HCS12_DRIVER_C_


#include "includes.h"


#define CRG_WATCHDOG_EN  0

void BoardInit(void){



#if CRG_WATCHDOG_EN
  COPCTL=0x03;
#endif


  SCI0Init();
  OSTickStart();  
//  vInterface_ECTInit();
 // vInterface_ADInit();
//  vInterface_PIMInit();
}



#endif