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
INT8U OSIntNesting=0;             /* �ж�Ƕ�� */


/*
***************************************************************
os_core.c
***************************************************************
*/


/*************************************
*        ϵͳ��ջ
*************************************/
OSSTK   SystemSTK[OS_STK_SYSTEM_SIZE];

OSSTKRef   OSSTKSystem=&SystemSTK[OS_STK_SYSTEM_SIZE-2];


//#ifdef OS_HOOK_ERROR
INT8U  OSNestedErrorHook=0;
INT8U  OSNestedHook=0;
#ifdef OS_HOOK_GET_SERVICEID
OSSERVICEIDTYPE  ServiceId=OSServiceId_NoService;
#endif /*ifdef OS_HOOK_GET_SERVICEID*/

#ifdef OS_HOOK_PARAM_ACCESS
ObjectIDType  OSObjId= 0; 
#endif /*ifdef OS_HOOK_PARAM_ACCESS*/

//#endif /*#ifdef OS_HOOK_ERROR*/																	
/*
***********************************************************
os_task.c
***********************************************************
*/
/***************************ȫ�ֱ����Ķ���********************************/
OSTCBRef           OSTCBCur=(OSTCBRef)0;				/*ָ��ǰ�����TCBָ��*/
OSTCBRef           OSTCBPrioTbl[(OS_NUM_SYS_TASK)];/* ���ȼ�������TCB������ʱ���õ�     */
OSTCB              OSTCBTbl[(OS_NUM_SYS_TASK) ];   /* ���е�TCB����ID������*/

/***************************���������*******************************/
INT8U  OSRdyGrp=0;/* ������������ */
INT8U  OSRdyTbl[OS_RDY_TBL_SIZE];/* �������� */

const INT8U extraStackSize = ExtraStackSize;

void  OSTaskIdle (void);

const INT8U os_num_task = OS_NUM_TASK;
const INT8U os_num_sys_task  = OS_NUM_TASK+1;
const INT8U os_rdy_tbl_size  = OS_NUM_SYS_TASK/8 +1;
const INT8U os_max_task = 64;
const INT8U os_task_idle_stk_size = OS_TASK_IDLE_STK_SIZE ;


/*
***********************************************************
os_res.c
***********************************************************
*/
OSRCB  OSResources[OS_NUM_OUT_RES+1];/*�ⲿ��Դ���ƽṹ�����飬
                                              OSResources[OS_NUM_OUT_RES]Ϊ��������Դ*/
ResourceType RES_SCHEDULER=&OSResources[OS_NUM_OUT_RES];

const INT8U os_num_out_res = OS_NUM_OUT_RES;

#if !defined(OS_OUT_RES) || OS_NUM_OUT_RES == 0
ResPrioType OSResPrioTbl[1];/*��ʹ���ⲿ��Դʱ���ã���ֹ���Ӵ���*/
#endif   



/*
**********************************************************
os_irq.c
**********************************************************
*/

#if OS_NUM_ISR==0
OSISRCfg OSISRCfgTbl[1] = { {NULL, UNIMPISR, OSISRCategory1,OSIRQ_EDGE} };
#else
extern OSISRCfg OSISRCfgTbl[];
#endif

extern ISR_FP   ISRVector[];
extern INT32U OSISRCategory2Mask[];

const INT8U os_num_isr = OS_NUM_ISR; 

/*
***********************************************************
os_alarm.c
***********************************************************
*/

#if OS_NUM_ALARM == 0
AlarmCB OSAlarms[1]; /*Alarm����*/
CtrCB OSCounters[1];   /*Counter����,�����������ֹ�������*/
#else
AlarmCB OSAlarms[OS_NUM_ALARM]; /*Alarm����*/
CtrCB OSCounters[OS_NUM_CTR];   /*Counter����*/
#endif


#if OS_NUM_CTR==0
AlarmBaseType *OSCounterCfgTbl;
#else
extern AlarmBaseType OSCounterCfgTbl[];
#endif

#if OS_NUM_ALARM==0
OSAlarmCfg *OSAlarmCfgTbl;
#else
extern OSAlarmCfg OSAlarmCfgTbl[];
#endif

const INT8U os_num_ctr = OS_NUM_CTR; 
const INT8U os_num_alarm = OS_NUM_ALARM;

/*�û�û�ж�������ӵ�ʱ��*/
#ifndef OS_HOOK_ERROR
void ErrorHook(StatusType error) {
}
#endif


#ifdef	COM_EN
/*
***********************************************************
com
***********************************************************
*/
#ifdef COM_ERROR_HOOK
    INT8U  COMNestedErrorHook=0;
    
#ifdef COM_HOOK_GET_SERVICEID    
    COMServiceIdType  ServiceId=COMServiceId_NoService;
#endif /*ifdef OS_HOOK_GET_SERVICEID*/ 

#ifdef COM_HOOK_PARA_ACCESS
    INT32U    COMObjId; 
#endif/*#ifdef COM_USE_PARAMETERACCESS*/ 

#endif/*#ifdef COM_ERROR_HOOK*/ 


#endif/*COM*/





#endif /*#ifndef _GLOBAL_C*/
