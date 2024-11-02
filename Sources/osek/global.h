/*
***************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：global.h
* 摘    要：涉及全局变量的声明和宏的定义
*
* 当前版本：1.1
* 作    者：厉蒋
* 完成日期：2005年1月17日
*
* 取代版本：1.0
* 原作者  ：陈威
* 完成日期：2003年12月4日
***************************************************************************
*/
#ifndef	 GLOBAL_H
#define  GLOBAL_H

#include "os_cfg.h"
#include "os_cpu.h"



#ifndef  FALSE
#define  FALSE  0
#endif

#ifndef  TRUE
#define  TRUE  1
#endif

#ifndef  NULL
#define  NULL 0
#endif


/***************OSKE API标准出错信息**********************/
#define E_OK            0
#define E_OS_ACCESS     1
#define E_OS_CALLEVEL   2
#define E_OS_ID         3
#define E_OS_LIMIT      4
#define E_OS_NOFUNC     5
#define E_OS_RESOURCE   6
#define E_OS_STATE      7
#define E_OS_VALUE      8
#define E_OS_TICKS     9  




#ifdef   OS_SHOW_OTHER_ERROR
#define    E_OS_SYS_BASE    20/*实现使用的非标准的错误信息的基本值*/
#define    E_OS_SYS_PARA    ((E_OS_SYS_BASE)+1)/*参数错误*/
#define    E_OS_SYS_RESULT    ((E_OS_SYS_BASE)+2) /*结果错误*/
#define    E_OS_SYS_PRIO_EXIST    ((E_OS_SYS_BASE)+3)/*优先级已经存在*/
#define    E_OS_INT_ERR        (E_OS_SYS_BASE)+4)/*中断模块错误码定义*/
#define    E_OS_STACK          ((E_OS_SYS_BASE)+5) /*堆栈溢出*/


#else /*#ifdef   OS_SHOW_OTHER_ERROR*/
#define    E_OS_OTHER    20
#endif 

typedef INT8U       StatusType;  
typedef INT32U       EventMaskType;
typedef EventMaskType    *EventMaskRefType;

extern StatusType OsLastError;   /*用于返回错误*/


extern  INT8U  OSIntNesting;             /* 中断嵌套 */
#define NOT_IN_ISR  (0==OSIntNesting)




#define OS_STACK_SEPARATE      /*栈分开*/

#define OS_STACK_INIT /*需要初始化栈*/

#ifdef OS_STACK_SEPARATE
#if  !defined(OS_TASK_IDLE_STK_SIZE) || (OS_TASK_IDLE_STK_SIZE != 512)
#define  OS_TASK_IDLE_STK_SIZE  512
#endif
#endif/*#ifdef OS_STACK_SEPARATE*/

#ifdef OS_STACK_INIT
void  OSStartHighRdy();
#define OSStartFirstTask(curTcb) OSTaskHighRun()
#endif/* #ifdef OS_STACK_INIT*/

#define OS_NUM_SYS_TASK ((OS_NUM_TASK)+1)
#define  OS_RDY_TBL_SIZE   (((OS_NUM_SYS_TASK)) / 8 + 1)   /*ready table 的大小可控制 */
/*最多64个任务*/
#define OS_MAX_TASKS 64

#ifndef OS_STK_SYSTEM_SIZE
#define OS_STK_SYSTEM_SIZE 300
#endif

#ifndef OS_NUM_ISR
#define OS_NUM_ISR 0
#endif

#ifndef OS_NUM_CTR
#define OS_NUM_CTR 0
#endif

#ifndef OS_NUM_ALARM
#define OS_NUM_ALARM 0
#endif

#ifndef OS_NUM_OUT_RES
#define OS_NUM_OUT_RES 0
#endif

#endif
