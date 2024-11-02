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
INT8U OSIntNesting=0;             /* 中断嵌套 */


/*
***************************************************************
os_core.c
***************************************************************
*/


/*************************************
*        系统堆栈
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
/***************************全局变量的定义********************************/
OSTCBRef           OSTCBCur=(OSTCBRef)0;				/*指向当前任务的TCB指针*/
OSTCBRef           OSTCBPrioTbl[(OS_NUM_SYS_TASK)];/* 优先级索引的TCB表，调度时候用到     */
OSTCB              OSTCBTbl[(OS_NUM_SYS_TASK) ];   /* 所有的TCB，用ID来索引*/

/***************************定义就绪表*******************************/
INT8U  OSRdyGrp=0;/* 就绪数组索引 */
INT8U  OSRdyTbl[OS_RDY_TBL_SIZE];/* 就绪队列 */

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
OSRCB  OSResources[OS_NUM_OUT_RES+1];/*外部资源控制结构体数组，
                                              OSResources[OS_NUM_OUT_RES]为调度器资源*/
ResourceType RES_SCHEDULER=&OSResources[OS_NUM_OUT_RES];

const INT8U os_num_out_res = OS_NUM_OUT_RES;

#if !defined(OS_OUT_RES) || OS_NUM_OUT_RES == 0
ResPrioType OSResPrioTbl[1];/*不使用外部资源时无用，防止链接错误*/
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
AlarmCB OSAlarms[1]; /*Alarm数组*/
CtrCB OSCounters[1];   /*Counter数组,定义这两项防止编译错误*/
#else
AlarmCB OSAlarms[OS_NUM_ALARM]; /*Alarm数组*/
CtrCB OSCounters[OS_NUM_CTR];   /*Counter数组*/
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

/*用户没有定义错误钩子的时候*/
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
