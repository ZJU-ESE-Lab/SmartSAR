/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_task.h
* 摘    要：实现任务模块的数据结构以及特定变量的声明和宏操作的定义
*
* 当前版本：1.2
* 作    者：厉蒋
* 完成日期：2005年5月22日
*
* 当前版本：1.1
* 作    者：潘杭平、厉蒋
* 完成日期：2005年3月1日
*
* 取代版本：1.0
* 原作者  ：陈威
* 完成日期：2004年12月4日
*********************************************************************************
*/


#ifndef  OS_TASK_H
#define	 OS_TASK_H

/*
*********************************************************************************************************
*                              宏定义
*********************************************************************************************************
*/
#define OS_NUM_SYS_TASK ((OS_NUM_TASK)+1)
#define  OS_RDY_TBL_SIZE   (((OS_NUM_SYS_TASK)) / 8 + 1)   /*ready table 的大小可控制 */
/*最多64个任务*/
#define OS_MAX_TASKS 64


/*任务的状态标记宏*/
#define  READY            0x00        
#define  WAITING          0x02        
#define  SUSPENDED        0x04        
#define  RUNNING          0x01        

#define  RUN_READY        (INT8U)(0x10)   /*表明运行的Ready状态，用此状态判断是否是第一次运行*/

#define  INVALID_TASK     (NULL)          /* 无效任务*/

/*是否可抢占的宏定义*/
#define	Preemptive	1
#define	NoPreemptive 0


/*调度模式的宏定义*/
#define FULL_SCHED 0x10
/*表示自启动宏定义*/
#define AUTO_START 0x20
/*表示占有内部资源宏定义*/
#define HOLD_INT_RES 0x40
/*表示扩展任务宏定义*/
#define EXTENDED 0x80

/***********************************************************************
                             OSEK标准类型声明
***********************************************************************/
typedef struct OSTCB   *    TaskType;
typedef TaskType    *TaskRefType;
typedef INT8U       TaskStateType;
typedef TaskStateType       *TaskStateRefType;
/***********************************************************************
                             SmartOSEK内部类型声明
***********************************************************************/
/*优先级类型*/
typedef INT8U PriorityType;
/*优先级类型*/
typedef INT8S ResPrioType;
/*tcb指针*/
typedef struct OSTCB* OSTCBRef;

/*任务体入口*/
typedef void (*OSTaskEntry)(void) ;


/********************************************
该结构体用于用户配置任务属性
用户配置的任务属性数组应该按优先级从小到大进行
********************************************/
typedef struct TaskCfg
{
    OSTaskEntry PTask;/*任务入口指针*/
#ifdef OS_STACK_SEPARATE	/*在BCC1的情况下，任务不需要单独的堆栈*/ 
    OSSTKRef    PStkHigh;/*堆栈高指针*/ 
    OSSTKRef    PStkLow;   /* 堆栈底指针*/
#endif/*#ifdef OS_EVENT_EN*/     
    PriorityType  ID; /*内部资源优先级*/
	  PriorityType  RunPrio; /*内部资源优先级*/
	  INT8U	InfoBit;	  /*记录任务属性以及运行信息，第一位表示留待扩展、
                                                                   第二位表示留待扩展、
                                                                   第三位表示留待扩展、
                                                                   第四位表示是否是全新运行任务、
                                                                   第五位表示是否可抢占即调度方式、
                                                                   第六位表示是否自启动、                                                                   
                                                                   第七位表示是否占有内部资源、
                                                                   第八位表示是否扩展任务*/
}OSTaskCfg;

/*******************************************
* 		资源模块的部分结构体定义
*
*由于数据结构设计的原因，tcb和资源控制结构的
*成员变量互相引用。因此在这里typedef
*
********************************************/
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*外部或调度器资源*/
typedef struct OSResCB OSRCB;
typedef struct OSResCB* OSRCBRef;


#endif /*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/


/******************************************************************************
 	                       任务控制块的结构体定义
*******************************************************************************/
typedef struct  OSTCB
{
    OSSTKRef     OSTCBStkCur;         /* 目前的栈指针，即栈顶指针  */
    OSTaskEntry  PTask;               /* 任务入口指针*/
    OSSTKRef     OSTCBStkTop;         /* 栈顶指针，记录任务栈的顶指针，用于初始化栈用*/

#ifdef OS_EVENT_EN
    EventMaskType   WEvent;             /* waited events的掩码	    */
    EventMaskType   SEvent;             /* setted events的掩码 		*/
#endif /*#ifdef OS_EVENT_EN*/ 

#if defined(OS_CHECK_STACK) && defined(OS_STACK_SEPARATE) /*在栈分开的情况下下才有栈的检查*/  
    OSSTKRef     OSTCBStkBottom;        /* 堆栈底指针				    */
#endif /*#if defined(OS_CHECK_STACK) && defined(OS_EVENT_EN)*/    
 	
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*外部或调度器资源*/
    OSRCBRef		resources;          /* 外部资源链表头指针            */
#endif /*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/
 	
#ifdef OS_INT_RES
    OSTCBRef        SavedTCB;           /* 指向原来在该位置的tcb指针     */
#endif
#if defined(OS_INT_RES)	||defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*当得到内部资源或外部资源的时候都设置一下运行优先级*/
	PriorityType    RunPrio;         /* 运行优先级				*/
#endif/*#if defined(OS_INT_RES)	*//*||defined(OS_OUT_RES)*/	

    PriorityType       OSTCBId;            /* Task ID                       */
    INT8U          OSTCBStat;          /* 任务状态                      */

#ifndef OS_CPU_MPC555
    INT8U          OSTCBBitX;          /* 用于访问就绪表的掩码*/
    INT8U          OSTCBBitY;          /* 用于访问就绪表数组的掩码 */
#endif/*#ifndef OS_CPU_MPC555*/

	INT8U			InfoBit;           /*记录任务属性以及运行信息，第一位表示留待扩展、
                                                                   第二位表示留待扩展、
                                                                   第三位表示留待扩展、
                                                                   第四位表示是否是全新运行任务、
                                                                   第五位表示是否可抢占即调度方式、
                                                                   第六位表示是否自启动、                                                                   
                                                                   第七位表示是否占有内部资源、
                                                                   第八位表示是否扩展任务*/
		
} OSTCB;





 
/*得到任务的类型，扩展或标准*/
#define GET_TASK_TYPE(PTCB) ((PTCB)->InfoBit & EXTENDED)

/*得到任务调度方式*/
#define GET_TASK_SCHED(PTCB)  ((PTCB)->InfoBit & FULL_SCHED)

/*得到任务是否占有内部资源*/
#define GET_TASK_HOLD_INT_RES(PTCB) ((PTCB)->InfoBit & HOLD_INT_RES)
/*设置任务属性*/
#define SET_TASK_PROPERTY(PTCB,PROPERTY) (PTCB)->InfoBit=(PROPERTY)
/*设置任务体入口*/
#define SET_TASK_ENTRY(PTCB,ENTRY) (PTCB)->PTask=(ENTRY)  
/*得到任务状态*/
#define GET_TASK_STATUS(PTCB)  ((PTCB)->OSTCBStat)
/*设置任务ID*/
#define SET_TASK_ID(PTCB,ID) (PTCB)->OSTCBId=(ID)
/*设置任务状态*/
#define SET_TASK_STATUS(PTCB,STATUS) (PTCB)->OSTCBStat = (STATUS) 

/*得到任务本身优先级（任务ID）*/
#define GET_TASK_ID(PTCB)  ((PTCB)->OSTCBId)


/*判断任务是否就绪状态，1表示就绪，0表示非就绪*/
#define IS_TASK_READY(PTCB) ( READY==(GET_TASK_STATUS(PTCB)&~RUN_READY) )

/******************************************************************
             得到任务运行优先级
*********************************************************************/

/******************************************************************
             TCB中与内部资源相关的操作
*********************************************************************/
#ifdef OS_INT_RES

/*得到任务的内部资源优先级*/
#define GET_TASK_INT_RES(PTCB)  ((PTCB)->RunPrio)

/*得到任务的保存的TCB指针*/
#define GET_TASK_SAVED_TCB(PTCB)  ((PTCB)->SavedTCB)

/*内部资源优先级的设置*/
#define SET_TASK_INT_RES(PTcb,ResPrio) ((PTcb)->RunPrio)=ResPrio

/*设置保存的TCB指针*/	
#define RESET_TASK_SAVED_TCB(PTCB) \
	OSTCBPrioTbl[GET_TASK_INT_RES(PTCB)]=GET_TASK_SAVED_TCB(PTCB)

#else /*#ifdef OS_INT_RES*/
#define RESET_TASK_SAVED_TCB(PTCB)	
#define SET_TASK_INT_RES(PTcb,ResPrio) 
#endif /*#define OS_INT_RES*/	
/******************************************************************
             TCB中与外部资源相关的操作
*********************************************************************/
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
/*得到任务占有的外部资源链表的头指针*/
#define GET_TASK_OUT_RES(PTCB) ((PTCB)->resources)
/*设置任务占有的外部资源链表的头指针*/
#define SET_TASK_OUT_RES(PTCB,Res) (PTCB)->resources=Res

#endif /*#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/

/**********************************************************************
             TCB中与内部或外部资源相关的操作
*************************************************************************/
#if defined(OS_INT_RES) ||defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
#define GET_TASK_RUN_PRIO(PTCB) ((PTCB)->RunPrio)
#define SET_TASK_RUN_PRIO(PTCB,PRIO) ((PTCB)->RunPrio)=((PriorityType)(PRIO) )
#else/*#if defined(OS_INT_RES) ||defined(OS_OUT_RES)*/
#define GET_TASK_RUN_PRIO(PTCB) GET_TASK_ID(PTCB)
#define SET_TASK_RUN_PRIO(PTCB,PRIO) 
#endif/*#if defined(OS_INT_RES) ||defined(OS_OUT_RES)*/
/******************************************************************
             TCB中与EVENT相关的操作
*********************************************************************/

#ifdef OS_EVENT_EN
/*清除TCB中的设置过的所有事件*/
#define CLEAR_TASK_EVENT(PTCB)  (PTCB)->SEvent=(NOSETEVENT)

/*设置TCB中的触发的事件*/
#define SET_TASK_SET_EVENT(PTCB,mask)  (PTCB)->SEvent|=(EventMaskType)(mask)

/*设置TCB中的等待的事件*/
#define SET_TASK_WAIT_EVENT(PTCB,mask)  (PTCB)->WEvent|=(EventMaskType)(mask)


/*得到TCB中的触发的事件*/
#define GET_TASK_SET_EVENT(PTCB)  ((PTCB)->SEvent)

/*得到TCB中的等待的事件*/
#define GET_TASK_WAIT_EVENT(PTCB)  ((PTCB)->WEvent)

/*清除TCB中的设置过的MASK值为mask的事件*/
#define CLERA_TASK_MASK_EVENT(PTCB,mask)  (PTCB)->SEvent&=(EventMaskType)(~Mask)


#else/*#ifdef OS_EVENT_EN*/

#define CLEAR_TASK_EVENT(PTCB)

#define SET_TASK_EVENT(PTCB,SEvent)

#endif /*#ifdef OS_EVENT_EN*/

/************************************************************
* 几个经常调用的相关的宏定义
* SET_TASK_READY(TaskId,PTCB) 
* SET_TASK_UNREADY(TaskId,PTCB,Status)
* 两个参数为了保证可移植性
****************************************************/
#ifdef OS_STACK_INIT
/*改变任务状态，并把任务放入就绪队列*//*这里还需要实现栈的初始化*/
#define SET_TASK_READY(TaskId,PTCB) SET_TASK_STATUS(PTCB,READY);OS_TASK_TO_READY(TaskId,PTCB);SET_TASK_STK_INIT(PTCB)
#else /*#ifdef OS_STACK_INIT*/
/*改变任务状态，并把任务放入就绪队列*/
#define SET_TASK_READY(TaskId,PTCB) SET_TASK_STATUS(PTCB,READY);OS_TASK_TO_READY(TaskId,PTCB)
#endif/*#ifdef OS_STACK_INIT*/
/*改变任务状态(Suspend/Waiting)，并把任务从就绪队列中去除*/
#define SET_TASK_UNREADY(TaskId,PTCB,Status) SET_TASK_STATUS(PTCB,Status);OS_TASK_TO_UNREADY(TaskId,PTCB)





/****************************************************************
检查堆栈
****************************************************************/
#if defined(OS_CHECK_STACK)&&defined(OS_STACK_SEPARATE)&&defined(OS_CPU_MPC555)

INT8U OSStackOverFlow(OSSTKRef StkBottom);
#define OSCheckStack(PTCB)   \
        do\
        {\
            if(OSStackOverFlow(GET_TASK_STK_BOTTOM(PTCB)))\
            {\
        	    OSEKPrintInt(GET_TASK_ID(PTCB));\
        	    RETURN_OTHER_ERROR(E_OS_STACK);\
            }\
        }while(0)
#else/*#if defined(OS_CHECK_STACK)&&defined(OS_EVENT_EN)*/
#define OSCheckStack(PTCB) 
#endif/*#ifdef  OS_CHECK_STACK*/     


/*
*********************************************************************************************************
* 全局变量的声明
*********************************************************************************************************
*/
extern  OSTCBRef           OSTCBCur;                  /* 当前任务tcb指针          */
extern  OSTCBRef           OSTCBPrioTbl[(OS_NUM_SYS_TASK)];/* 优先级索引的tcb指针数组  */
extern  OSTCB              OSTCBTbl[(OS_NUM_SYS_TASK) ];   /* TCB数组                  */

#ifndef OS_STACK_INIT 

/*********************************************************************************************************
上下文切换的函数声明，具体定义在OS_CPU_A.ASM中
按照OSEK标准，将上下文切换分为4种情况：
1.Save原来任务，Load新任务。 适合于Schedule、WaitEvent、SetEvent、ReleaseResource
2.Save原来任务，不Load新任务。适合于ActivateTask、Schedule、WaitEvent、ReleaseResource
3.不Save原来任务，Load新任务。适合于TerminateTask、ChainTask、二类中断中返回被抢占
4.直接运行，不需要Save、Load。适合于TerminateTask、ChainTask、二类中断中返回被抢占
**************************************************************************************************/

/*情况1.Save原来任务，Load新任务*/
void OSTaskSaveLoadRun(OSTCBRef PSavedTcb,OSTCBRef PLoadTcb);
/*情况2.Save原来任务，不Load新任务*/
void OSTaskSaveRun(OSTCBRef PSavedTcb,OSTCBRef PLoadTcb);
/*情况3.不Save原来任务，Load新任务*/
void OSTaskLoadRun(OSTCBRef PRunTcb);
/*情况4.直接运行，不需要Save、Load*/
void OSTaskRun(OSTCBRef PRunTcb);
#endif/*#ifndef OS_STACK_INIT*/

/*TASK宏定义，用来定义一个任务*/
#define TASK(name) void Func##name(void)
/*definetask的宏，第一个参数进行改变*/
#define DefineTask(p1,p2,p3,p4,p5,p6) FuncDefineTask(Func##p1,p2,p3,p4,p5,p6)

#ifdef OS_ALARM_EN
void OSActivateTask(TaskType TaskId);
#endif

/*
*********************************************************************************************************
* 给其他模块调用的函数
*********************************************************************************************************
*/
void OSTaskInit(void);

/*
*********************************************************************************************************
* 给外部调用的非标准api
*********************************************************************************************************
*/
INT8U  FuncDefineTask (void (*PTask)(void ),OSSTKRef PStkHigh,OSSTKRef PStkLow,INT8U Priority,INT8U Property,PriorityType ResPrio);

/*
*********************************************************************************************************
* 标准的osek api
*********************************************************************************************************
*/
#define DeclareTask(TaskID)  void Func##TaskID(void)
StatusType ActivateTask (TaskType ptcb);
StatusType TerminateTask (void);
StatusType ChainTask( TaskType ptcb );
StatusType GetTaskID( TaskRefType PTask );
StatusType GetTaskState ( TaskType ptcb, TaskStateRefType state );
StatusType Schedule( void );


#endif/*#ifndef  OS_TASK_H*/
