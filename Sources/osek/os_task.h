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

/*任务的状态标记宏*/
#define  READY            0x00        
#define  WAITING          0x02        
#define  SUSPENDED        0x04        
#define  RUNNING          0x01        

#define  RUN_READY        (INT8U)(0x10)   /*表明运行的Ready状态，用此状态判断是否是第一次运行*/

#define  INVALID_TASK     (NULL)          /* 无效任务*/

#define  INVALID_RESOURCE     (NULL)          /* 无效任务*/
#define  INVALID_ALARM     (NULL)          /* 无效任务*/

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
    OSSTKRef    PStkHigh;/*堆栈高指针*/ 
    OSSTKRef    PStkLow;   /* 堆栈底指针*/
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
//#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*外部或调度器资源*/
typedef struct OSResCB OSRCB;
typedef struct OSResCB* OSRCBRef;


//#endif /*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/


/******************************************************************************
 	                       任务控制块的结构体定义
*******************************************************************************/
typedef struct  OSTCB
{
    OSSTKRef     OSTCBStkCur;         /* 目前的栈指针，即栈顶指针  */
    OSTaskEntry  PTask;               /* 任务入口指针*/
    OSSTKRef     OSTCBStkTop;         /* 栈顶指针，记录任务栈的顶指针，用于初始化栈用*/

    EventMaskType   WEvent;             /* waited events的掩码	    */
    EventMaskType   SEvent;             /* setted events的掩码 		*/

    OSSTKRef     OSTCBStkBottom;        /* 堆栈底指针				    */
 	
    OSRCBRef		resources;          /* 外部资源链表头指针            */
 	
    OSTCBRef        SavedTCB;           /* 指向原来在该位置的tcb指针     */
	PriorityType    RunPrio;         /* 运行优先级				*/

    PriorityType       OSTCBId;            /* Task ID                       */
    INT8U          OSTCBStat;          /* 任务状态                      */

    INT8U          OSTCBBitX;          /* 用于访问就绪表的掩码*/
    INT8U          OSTCBBitY;          /* 用于访问就绪表数组的掩码 */

	INT8U			InfoBit;           /*记录任务属性以及运行信息，第一位表示留待扩展、
                                                                   第二位表示留待扩展、
                                                                   第三位表示留待扩展、
                                                                   第四位表示是否是全新运行任务、
                                                                   第五位表示是否可抢占即调度方式、
                                                                   第六位表示是否自启动、                                                                   
                                                                   第七位表示是否占有内部资源、
                                                                   第八位表示是否扩展任务*/
		
} OSTCB;

/*
*********************************************************************************************************
* 全局变量的声明
*********************************************************************************************************
*/
extern  OSTCBRef           OSTCBCur;                  /* 当前任务tcb指针          */
extern  OSTCBRef           OSTCBPrioTbl[];/* 优先级索引的tcb指针数组  */
extern  OSTCB              OSTCBTbl[];   /* TCB数组                  */

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

//#ifdef OS_ALARM_EN
void OSActivateTask(TaskType TaskId);
//#endif

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
