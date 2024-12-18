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

#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)

extern  INT8U  OSIntNesting;             /* 中断嵌套 */
#define NOT_IN_ISR  (0==OSIntNesting)

#else/*#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)*/

#define NOT_IN_ISR  1

#endif/*#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)*/

#if defined(OS_EVENT_EN)|| (!defined(OS_CPU_MPC555))/*在扩展状态或非MPC555平台上，可以栈分开*/
#define OS_STACK_SEPARATE      /*栈分开*/
#endif/*#if defined(OS_EVENT_EN)&&defined(OS_CPU_MPC555)*/

#if defined(OS_CPU_HCS12)
#define OS_STACK_INIT /*需要初始化栈*/
#endif/*#if defined(OS_CPU_HCS12)*/ 

#ifdef WatchSystemInfo
extern  volatile  INT32U  OSTime;                   /* */
#endif /*WatchSystemInfo*/

#endif
