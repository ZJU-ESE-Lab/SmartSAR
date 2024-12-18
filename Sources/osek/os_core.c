/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_core.c
* 摘    要：操作系统核心模块实现，包括系统的启动、关闭、
               核心调度函数、以及开关调度器
*
* 当前版本：1.1
* 作    者：厉蒋、潘杭平
* 完成日期：2005年1月20日
*
* 取代版本：1.0
* 原作者  ：陈威
* 完成日期：2003年12月4日
*********************************************************************************
*/
#ifndef OS_CORE_C /*为了不重复包含该文件*/
#define OS_CORE_C


#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

AppModeType	ActiveAppMode=OSDEFAULTAPPMODE;/*当前激活的APPMODE*/

/*****************************************************************************
*        系统堆栈
*******************************************************************************/
#if (defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)) && defined(OS_EVENT_EN)
OSSTK   SystemSTK[OS_STK_SYSTEM_SIZE];

#if OS_STK_GROWTH == 1 
OSSTKRef   OSSTKSystem=&SystemSTK[OS_STK_SYSTEM_SIZE-2];
#else/*#if OS_STK_GROWTH == 1 */
OSSTKRef   OSSTKSystem=SystemSTK;
#endif/*#if OS_STK_GROWTH == 1 */

#endif/*#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)*/


#ifdef OS_HOOK_ERROR
INT8U  OSNestedErrorHook=0;
INT8U  OSNestedHook=0;
#ifdef OS_HOOK_GET_SERVICEID
OSSERVICEIDTYPE  ServiceId=OSServiceId_NoService;
#endif /*ifdef OS_HOOK_GET_SERVICEID*/

#ifdef OS_HOOK_PARAM_ACCESS
ObjectIDType  OSObjId= 0; 
#endif /*ifdef OS_HOOK_PARAM_ACCESS*/

#endif /*#ifdef OS_HOOK_ERROR*/ 

#ifdef OS_STACK_SEPARATE
#ifndef  OS_TASK_IDLE_STK_SIZE
#define  OS_TASK_IDLE_STK_SIZE  200
#endif/*#ifndef  OS_TASK_IDLE_STK_SIZE*/
OSSTK OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE];
#endif/*#ifdef OS_EVENT_EN*/

#ifdef OS_STACK_INIT
void  OSStartHighRdy();
#define OSStartFirstTask(curTcb) OSTaskHighRun()
#else /* #ifdef OS_STACK_INIT*/
#define OSStartFirstTask(curTcb) OSTaskRun(curTcb) /* 启动最高优先级的任务 */
#endif/* #ifdef OS_STACK_INIT*/
/*如果处理器不支持找到最左边的1在哪里,我们用查表法
osmaptbl实际上是一个掩码表，分别代表了8个位上某一位为1的值*/

#ifndef OS_CPU_MPC555
INT8U  const  OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/*一个0－255之间的数，找到其最左边的1在哪一个位上
比如，17二进制表示为0b10001,最左边的1在第5位上，查表可以迅速得到*/
INT8U  const  OSUnMapTbl[] = {
    0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};
#endif/*#ifndef OS_CPU_MPC555*/
/*
***************************************************************************
*void  OSTaskIdle (void)
* 描述：idle任务体
* 调用的函数：无
* 允许被调用的位置：OSInit
* 参数：	无
* 返回：	无
* 注意事项：
* 创建人：陈威
* 时间：2003.12.23
* 修改人：厉蒋
* 时间：2005.1.17    
***************************************************************************
*/
void  OSTaskIdle (void)
{
    for(;;) 
    {
        ;  
    }    
}
/*
***************************************************************************
*void  OSInit (void)
* 描述：操作系统的初始化
* 调用的函数：各模块的初始化函数(countInit/ResourceInit/interrupt_init)
* 允许被调用的位置：main函数
* 参数：	无
* 返回：	无
* 注意事项：
* 创建人：陈威
* 时间：2003.12.23
* 修改人：厉蒋
* 时间：2005.1.23    
***************************************************************************
*/
#ifndef OS_TASK_AUTO_DEFINE /*用户自己利用DefineTask等API来定义实体的时候需要用到*/
void  OSInit(void)
{
    INT16U     i;
#ifndef OS_CPU_MPC555   
    INT8U * prdytbl;
#endif

#ifndef OS_CPU_MPC555
    prdytbl = &OSRdyTbl[0];
    for (i = 0; i < OS_RDY_TBL_SIZE; i++) 
    {
        *prdytbl++ = 0x00;
    }  
#endif/* #ifdef OS_CPU_MPC555 */
    
    /*创建空闲任务*/
    SET_TASK_PROPERTY(&OSTCBTbl[0],FULL_SCHED|AUTO_START);
    SET_TASK_STATUS(&OSTCBTbl[0],READY);
    SET_TASK_ENTRY(&OSTCBTbl[0],OSTaskIdle);
#if OS_STK_GROWTH == 1
    SET_TASK_STK(&OSTCBTbl[0],&OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1],OSTaskIdleStk);
#else /*#if OS_STK_GROWTH == 1*/
    SET_TASK_STK(&OSTCBTbl[0],OSTaskIdleStk,&OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1]);
#endif/*if OS_STK_GROWTH == 1*/
    OSTCBPrioTbl[0] = (OSTCBRef)&OSTCBTbl[0];
    /*初始化任务相关的全局数据结构*/
    for (i = 1; i < (OS_NUM_SYS_TASK) ; i++) 
    {     /* 初始化TCB优先级表*/
        OSTCBPrioTbl[i] = (OSTCBRef)0;
        SET_TASK_STK(&OSTCBTbl[i],NULL,NULL);
    }

#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
    ResourceInit();
#endif

}
#endif/*#ifndef OS_TASK_AUTO_DEFINE*/


/*
***************************************************************************
void  StartOS (AppModeType mode)
* 描述：启动操作系统
* 调用的函数：OSStartupHook()
* 允许被调用的位置：main()
* 参数：	mode：操作系统运行的模式
* 返回：	无
* 注意事项：
* 创建人：陈威
* 时间：2003.12.23
* 修改人：厉蒋
* 时间：2005.1.23    
***************************************************************************
*/

void  StartOS(AppModeType mode)
{
    OSTCBRef curTcb=NULL;
    if(OS_NUM_SYS_TASK > OS_MAX_TASKS)/*判断任务个数是否超过要求*/
        return ;
#ifdef OS_TASK_AUTO_DEFINE    
    OSTaskInit();
#endif
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
    ResourceInit();
#endif/*#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/    
#ifdef OS_ALARM_EN
    OSAlarmInit();
#endif/*#ifdef OS_ALARM_EN*/
#ifdef OS_EXCEPTION_EN
    OSISRInit();
#endif/*#ifdef OS_EXCEPTION_EN*/    
    OSStartupHook();
	  ActiveAppMode=mode;
    OSTCBCur=curTcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()];/* 找到最高优先级的任务*/
    GET_INT_RES(curTcb);   /*获取内部资源*/
    OSPreTaskHook();
    SET_TASK_STATUS(curTcb,RUN_READY);
    OSStartFirstTask(curTcb);
}



/*
***************************************************************************
*void  OSIntExit (void)
* 描述：二类中断退出时的处理工作
* 调用的函数：OSIntCtxSw：
* 允许被调用的位置：DECHdlr，ExceptionRoutine
* 参数：	无
* 返回：	无
* 注意事项：中断里面与任务调度有关的api为ActivateTask,SetEvent、GetResource、ReleaseResource
* 创建人：陈威
* 时间：2003.12.23
* 修改人：潘杭平
* 时间：2005.2.23   
* 修改人：厉蒋
* 时间：2005.6.9
* 修改：郁利吉
* 时间：2005.12.13 
* 备注：注意该函数操作时已经屏蔽了所有外部中断
***************************************************************************
*/

#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)
void  OSIntExit (void)
{
    OSTCBRef ptcb=NULL;
    OSTCBRef oldRunning=NULL;
    if (--OSIntNesting==0){
#ifndef OS_FULL_SCHEDULE  /*不是全抢占，即要么是混合抢占，要么非抢占*/  
#ifdef OS_MIX_SCHEDULE  /*混合抢占*/
   	    if(FULL_SCHED!=GET_TASK_SCHED(OSTCBCur))/*不可抢占任务*/
   	    {
   		    return;/*退出中断*/
   	    }
#else /*不可抢占调度的情况*/
   	    if(0!=GET_TASK_ID(OSTCBCur))/*非IDLE任务，就不进行调度，如果为IDLE任务运行就要调度*/
   	    {
   	        return; /*退出中断*/    	
   	    }   	    
#endif /*#ifdef OS_MIX_SCHEDULE*/
#endif /*#ifndef OS_FULL_SCHEDULE*/   		 	
    	/*可以抢占*/
        oldRunning=OSTCBCur;
        if((ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()] )!=oldRunning)
        {  /*被高优先级抢占*/
            OS_TASK_SWITCH(oldRunning);
            OSPostTaskHook();/*在当前任务被切换前，调用PostTaskHook，在这里还能得到当前运行任务的ID*/
            OSTCBCur=ptcb;
            GET_INT_RES(ptcb);
            OSPreTaskHook();/*调用PreTaskHook*/
            OSCheckStack(oldRunning);   
#ifdef  OS_EVENT_EN  /*在ECC1的情况下*/                           
            if(RUN_READY!=GET_TASK_STATUS(ptcb))
#endif/*#ifdef  OS_EVENT_EN*/            
            {  /*如果该任务还没运行过*/
                SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
                OSTaskRun(ptcb);        	
            }
#ifdef  OS_EVENT_EN  /*在ECC1的情况下*/            
            else
            {
                OSTaskLoadRun(ptcb);        	
            }   
#endif/*#ifdef  OS_EVENT_EN*/             
        }
    }
}
#endif /* if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN) */


/*
***************************************************************************
*AppModeType GetActiveApplicationMode ( void )
* 描述：得到当前运行的APPMODE
* 调用的函数：无
* 允许被调用的位置：除了一类中断和ALARM中的CALLBACK外的其他所有地方都可以调用
* 参数：	无
* 返回：	当前运行的APPMODE
* 注意事项：
* 创建人：陈威
* 时间：2003.12.23
* 修改人：厉蒋
* 时间：2005.1.17    
***************************************************************************
*/
AppModeType GetActiveApplicationMode ( void )
{
	return ActiveAppMode;
}

/*
***************************************************************************
*void ShutdownOS ( StatusType Error )
* 描述：关闭OS
* 调用的函数：无
* 允许被调用的位置：TASK、二类ISR、ErrorHook、StartupHook、内部函数
* 参数：Error：出错信息
* 返回：无
* 注意事项：参数Error不能为E_OK
* 创建人：林树新
* 时间：2004.1.23
* 修改人：厉蒋
* 时间：2005.1.17    
***************************************************************************
*/
void ShutdownOS ( StatusType Error )
{
    DEFINECRITICAL(cpu_sr); 
    Error=Error;/*不需要判断Error是否为E_OK，这是用户需要注意的，不是实现的要求*/
#ifdef OS_HOOK_SHUTDOWN
    OSShutdownHook(Error); 
#endif /*ifdef OS_HOOK_SHUTDOWN*/
    OS_ENTER_CRITICAL(); 
	while(1)
	  ;
	OS_EXIT_CRITICAL();
}

#ifndef OS_EXCEPTION_EN
/*
***************************************************************************
* void UserException() 
* 描述：用户中断总入口。该函数只有在没用中断模块的时候才起左右，防止编译错误
* 调用的函数：无
* 允许被调用的位置：无
* 参数：无
* 返回：无
* 注意事项：
* 创建人：高爽
* 时间：2004.1.23
* 修改人：厉蒋
* 时间：2005.1.17    
***************************************************************************
*/

void UserException()
{

}

/*
***************************************************************************
*void SystemException()
* 描述：系统中断总入口。该函数只有在没用中断模块的时候才起左右，防止编译错误
* 调用的函数：无
* 允许被调用的位置：无
* 参数：无
* 返回：无
* 注意事项：
* 创建人：高爽
* 时间：2004.1.23
* 修改人：厉蒋
* 时间：2005.1.17    
***************************************************************************
*/

void SystemException()
{

}


#endif /*#ifndef OS_EXCEPTION_EN*/


#endif /*#ifndef OS_CORE_C*/
