/*
***************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：global.c
* 摘    要：涉及全局变量的定义
*
* 当前版本：1.0
* 作    者：厉蒋
* 完成日期：2005年1月17日
***************************************************************************
*/
#ifndef _GLOBAL_C
#define _GLOBAL_C

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif
StatusType OsLastError=E_OK;   /*用于返回错误*/
#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)
INT8U OSIntNesting=0;             /* 中断嵌套 */
#endif/*#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)*/


#endif /*#ifndef _GLOBAL_C*/
