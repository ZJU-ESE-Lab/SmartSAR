/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：osek.h
* 摘    要：包含各模块的*.h文件
*
* 当前版本：1.1
* 作    者：厉蒋
* 完成日期：2005年2月20日
*
* 取代版本：1.0
* 原作者  ：潘杭平
* 完成日期：2004年9月14日
*********************************************************************************
*/
#ifndef _OSEK_H
#define _OSEK_H


#include  "os_core.h"
#include  "os_core_cpu.h" 
#include  "os_task.h"
#include "os_task_cpu.h"
#include  "os_res.h"  /*这个头文件不需要宏开关的控制，因为内部和外部资源是分开*/


#include  "os_evt.h"

#include  "os_alarm.h"

#include "os_irq.h"


//#ifdef	COM_EN
//#include  "com.h"
//#endif

#endif