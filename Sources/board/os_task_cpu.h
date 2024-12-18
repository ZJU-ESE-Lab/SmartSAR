/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_task_cpu.h
* 摘    要：task模块中与平台相关的宏定义以及相关操作,这里的平台是MPC555
*
* 当前版本：2.0
* 作    者：郁利吉
* 完成日期：2005年12月12日
*
*********************************************************************************
*/

#ifndef OS_TASK_CPU_H
#define OS_TASK_CPU_H

/*多一个堆栈用于解决chaitask问题*/
#define ExtraStackSize 100
extern OSSTK ExtraStack[];

/********************其他平台************/
extern  INT8U   OSRdyGrp;                        /*就绪队列数组索引*/
extern  INT8U   OSRdyTbl[OS_RDY_TBL_SIZE];       /*就绪队列子数组*/


/*设置用于就绪队列操作的掩码*/
#define SET_TASK_MASK(ptcb,prio) (ptcb)->OSTCBBitY= OSMapTbl[(prio) >> 3]; (ptcb)->OSTCBBitX=OSMapTbl[(prio) & 0x07]      
#define OS_TASK_TO_READY(TaskId,PTCB)  OSRdyGrp|= PTCB->OSTCBBitY; OSRdyTbl[TaskId >>3] |= PTCB->OSTCBBitX
    
#define OS_TASK_TO_UNREADY(TaskId,PTCB) if (( OSRdyTbl[TaskId >>3]&=~PTCB->OSTCBBitX) == 0x00) OSRdyGrp &= ~PTCB->OSTCBBitY

#define	OS_FIND_HIGHEST_TASK() (INT8U)((OSUnMapTbl[OSRdyGrp] << 3)+OSUnMapTbl[OSRdyTbl[OSUnMapTbl[OSRdyGrp]]])



#ifdef OS_STACK_INIT
OSSTKRef OSTaskStkInit(OSTaskEntry task, OSSTKRef ptos);   /*栈初始化*/
        
#define SET_TASK_STK_INIT(PTCB)	 (PTCB)->OSTCBStkCur=OSTaskStkInit((PTCB)->PTask,(PTCB)->OSTCBStkTop)
#else /*#ifdef OS_STACK_INIT*/
#define SET_TASK_STK_INIT(PTCB)
#endif/*#ifdef OS_STACK_INIT*/


/*设置任务堆栈*/
#if defined(OS_CHECK_STACK) && defined(OS_STACK_SEPARATE)

#ifdef OS_STACK_INIT
#define SET_TASK_STK(PTCB,SP,SB)  (PTCB)->OSTCBStkTop=SP;(PTCB)->OSTCBStkBottom=(SB)
#else /*#ifdef OS_STACK_INIT*/
#define SET_TASK_STK(PTCB,SP,SB)  (PTCB)->OSTCBStkCur=SP;(PTCB)->OSTCBStkBottom=(SB)
#endif/*#ifdef OS_STACK_INIT*/

#define GET_TASK_STK_BOTTOM(PTCB) ((PTCB)->OSTCBStkBottom)

#else /*#if defined(OS_CHECK_STACK) && defined(OS_STACK_SEPARATE) */

#ifdef OS_STACK_INIT
#define SET_TASK_STK(PTCB,SP,SB)  (PTCB)->OSTCBStkTop=SP
#else/*#ifdef OS_STACK_INIT*/
#define SET_TASK_STK(PTCB,SP,SB)  (PTCB)->OSTCBStkCur=SP
#endif/*#ifdef OS_STACK_INIT*/

#endif /*#if defined(OS_CHECK_STACK) && defined(OS_STACK_SEPARATE)*/


#ifdef OS_STACK_INIT
/*获取任务栈顶*/
#define GET_TASK_STK_TOP(PTCB)	 (PTCB)->OSTCBStkTop
/*保存旧任务*/
#define SET_TASK_OLD(PTCB)   OSTCBOld=(PTCB)
#else
#define SET_TASK_OLD(PTCB)   
#endif/*#ifdef OS_STACK_INIT*/

/********************************************************
* 任务切换时的平台相关的代码
************************************************************/

#define OS_TASK_SWITCH(OldTcb) SET_TASK_OLD(OldTcb)
        
       

/*
*********************************************************************************************************
* 全局变量的声明
*********************************************************************************************************
*/
#ifdef OS_STACK_INIT /*如果上下文切换需要栈初始化等工作*/
extern  OSTCBRef           OSTCBOld;				/*指向刚刚切换的任务*/
#endif/*#ifdef OS_STACK_INIT*/

#ifdef OS_STACK_INIT /*如果任务切换是通过入栈和出栈实现的，那就通过软中断来实现*/
#define OSTaskRun(PRunTcb) OSTaskHighRun()
#define OSTaskSaveRun(PSavedTcb,PRunTcb) OS_TASK_SW() 
#define OSTaskLoadRun(PRunTcb) OSIntCtxSw() 
#define OSTaskSaveLoadRun(PSavedTcb,PRunTcb) OS_TASK_SW() 
#endif/*#ifdef OS_STACK_INIT*/




/***************************************************************************
* void  OSTaskInit() 中的平台相关操作
***************************************************************************/

/*****************************就绪表初始化*******************************/
#define INITRDYTBL()  \
INT8U * prdytbl; \
INT8U     i; \
prdytbl = &OSRdyTbl[0];\
for (i = 0; i < OS_RDY_TBL_SIZE; i++) *prdytbl++ = 0x00

/*就绪idle任务*/
#define IDLEREADY(PTCB) \
    SET_TASK_MASK(PTCB, 0); /*设置用于就绪队列操作的掩码*/\
    SET_TASK_READY(0,PTCB)
    



    
#endif /*#ifndef OS_TASK_CPU_H*/