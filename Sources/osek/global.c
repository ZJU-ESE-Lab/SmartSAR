/*
***************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�global.c
* ժ    Ҫ���漰ȫ�ֱ����Ķ���
*
* ��ǰ�汾��1.0
* ��    �ߣ�����
* ������ڣ�2005��1��17��
***************************************************************************
*/
#ifndef _GLOBAL_C
#define _GLOBAL_C

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif
StatusType OsLastError=E_OK;   /*���ڷ��ش���*/
#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)
INT8U OSIntNesting=0;             /* �ж�Ƕ�� */
#endif/*#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)*/


#endif /*#ifndef _GLOBAL_C*/
