/*
***************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�global.h
* ժ    Ҫ���漰ȫ�ֱ����������ͺ�Ķ���
*
* ��ǰ�汾��1.1
* ��    �ߣ�����
* ������ڣ�2005��1��17��
*
* ȡ���汾��1.0
* ԭ����  ������
* ������ڣ�2003��12��4��
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


/***************OSKE API��׼������Ϣ**********************/
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
#define    E_OS_SYS_BASE    20/*ʵ��ʹ�õķǱ�׼�Ĵ�����Ϣ�Ļ���ֵ*/
#define    E_OS_SYS_PARA    ((E_OS_SYS_BASE)+1)/*��������*/
#define    E_OS_SYS_RESULT    ((E_OS_SYS_BASE)+2) /*�������*/
#define    E_OS_SYS_PRIO_EXIST    ((E_OS_SYS_BASE)+3)/*���ȼ��Ѿ�����*/
#define    E_OS_INT_ERR        (E_OS_SYS_BASE)+4)/*�ж�ģ������붨��*/
#define    E_OS_STACK          ((E_OS_SYS_BASE)+5) /*��ջ���*/


#else /*#ifdef   OS_SHOW_OTHER_ERROR*/
#define    E_OS_OTHER    20
#endif 

typedef INT8U       StatusType;  
typedef INT32U       EventMaskType;
typedef EventMaskType    *EventMaskRefType;

extern StatusType OsLastError;   /*���ڷ��ش���*/


extern  INT8U  OSIntNesting;             /* �ж�Ƕ�� */
#define NOT_IN_ISR  (0==OSIntNesting)




#define OS_STACK_SEPARATE      /*ջ�ֿ�*/

#define OS_STACK_INIT /*��Ҫ��ʼ��ջ*/

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
#define  OS_RDY_TBL_SIZE   (((OS_NUM_SYS_TASK)) / 8 + 1)   /*ready table �Ĵ�С�ɿ��� */
/*���64������*/
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
