/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：driver.c
* 摘    要：包含各模块的寄存器定义文件
*
* 当前版本：1.0
* 作    者：姜旭锋
* 完成日期：2005年2月20日
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