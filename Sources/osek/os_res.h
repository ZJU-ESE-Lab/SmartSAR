/*
****************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_res.h
* 摘    要：定义资源模块的数据结构以及声明标准API
*
* 当前版本：1.2
* 作    者：厉蒋
* 完成日期：2005年7月20日
*
* 当前版本：1.1
* 作    者：厉蒋
* 完成日期：2005年1月20日
*
* 取代版本：1.0
* 原作者  ：陈威
* 完成日期：2003年12月10日
****************************************************************
*/
#ifndef	 OS_RES_H
#define	 OS_RES_H

typedef struct OSResCB * ResourceType; /*资源类型*/

/*************************************************************
     内部资源相关的一些操作
***************************************************************/
#ifdef OS_INT_RES /*使用内部资源*/


#ifdef OS_COMPILE_INLINE	/*支持inline函数*/

/*设置内部资源自动获取时的优先级变化*/
inline void GET_INT_RES(OSTCBRef PTCB)  
{
    if((HOLD_INT_RES==GET_TASK_HOLD_INT_RES(PTCB))&& 
        (GET_TASK_ID(PTCB)<GET_TASK_RUN_PRIO(PTCB)))
        /*占有内部资源并内部资源优先级比本身ID高*/
    {
    	PTCB->SavedTCB=&OSTCBTbl[PTCB->RunPrio];
	    OSTCBPrioTbl[PTCB->RunPrio]=PTCB;
	    OS_TASK_TO_READY(PTCB->RunPrio,PTCB->SavedTCB);
    }
}
/*释放内部资源的优先级变化以及TCB数组变化*/
inline void RELEASE_INT_RES(OSTCBRef curTcb)
{
    if( (HOLD_INT_RES==GET_TASK_HOLD_INT_RES(curTcb))&& 
        (GET_TASK_ID(curTcb)<GET_TASK_RUN_PRIO(curTcb)))
       /*占有内部资源并内部资源优先级比本身ID高*/
    { /*占有内部资源*/
        if(!IS_TASK_READY(&OSTCBTbl[GET_TASK_INT_RES(curTcb)])) 
        {  /*如果内部资源优先级对应的任务不在就绪状态，就可以将该优先级从就绪队列中拿出*/
        	OS_TASK_TO_UNREADY(GET_TASK_INT_RES(curTcb),GET_TASK_SAVED_TCB(curTcb));
        }
        RESET_TASK_SAVED_TCB(curTcb);/*释放内部资源*/
    }    	
}

#else/*#ifdef OS_COMPILE_INLINE*/

/*设置内部资源自动获取时的优先级变化*/
#define GET_INT_RES(PTCB)  \
    do{if((HOLD_INT_RES==GET_TASK_HOLD_INT_RES(PTCB))&&\
        (GET_TASK_ID(PTCB)<GET_TASK_RUN_PRIO(PTCB))){\
    	PTCB->SavedTCB=&OSTCBTbl[PTCB->RunPrio];\
	    OSTCBPrioTbl[PTCB->RunPrio]=PTCB;\
	    OS_TASK_TO_READY(PTCB->RunPrio,PTCB->SavedTCB);}}while(0)

/*释放内部资源的优先级变化以及TCB数组变化*/
#define RELEASE_INT_RES(curTcb) \
    do{if((HOLD_INT_RES==GET_TASK_HOLD_INT_RES(curTcb))&&\
        (GET_TASK_ID(curTcb)<GET_TASK_RUN_PRIO(curTcb))){\
       /*占有内部资源并内部资源优先级比本身ID高*/\
        if(!IS_TASK_READY(&OSTCBTbl[GET_TASK_INT_RES(curTcb)])){\
        	OS_TASK_TO_UNREADY(GET_TASK_INT_RES(curTcb),GET_TASK_SAVED_TCB(curTcb));\
        }\
        RESET_TASK_SAVED_TCB(curTcb);/*释放内部资源*/\
    }}while(0)    	
    
#endif/*#ifdef OS_CPU_MPC555*/

    
    

#else /*#ifdef OS_INT_RES*/

#define GET_INT_RES(PTCB)

#define RELEASE_INT_RES(curTcb)	
#endif/*#ifdef OS_INT_RES*/

/*************************************************************
     外部资源相关的定义和一些操作的定义
***************************************************************/

#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*使用外部资源或调度器资源*/

#define OUT_RES_USED  1
#define OUT_RES_NOT_USED  0

#ifdef OS_ISR_RES /*中断级的资源管理*/
typedef INT32U OSISRMaskType;/*MASK类型*/
#endif/*ifdef OS_ISR_RES*/ 
/*外部资源控制结构体*/
struct OSResCB 
{
    ResPrioType   priority;     /*资源优先级*/
    PriorityType  OldRunPrio;   /*用于保存原来的运行优先级，方便ReleaseResource时恢复*/
#ifdef OS_EXTENDED_STATUS    
    INT8U         IsUsed;     /*标志该资源是否被使用,0表示没有在使用，非零表示在使用*/
#endif /*ifdef OS_EXTENDED_STATUS*/    
    OSTCBRef  SavedTCB;   /* 保存的TCB指针*/
    OSRCBRef     NextRes;    /*下一个资源的指针*/
#ifdef OS_ISR_RES /*中断级的资源管理*/
    OSISRMaskType SavedMask;/*保存的MASK*/
#endif/*ifdef OS_ISR_RES*/      
};
extern OSRCB  OSResources[];


/*得到资源优先级*/
#define GET_RES_PRIO(PRCB) ((PRCB)->priority)
/*得到原来保存的运行优先级*/
#define GET_RES_OLD_RUN_PRIO(PRCB) (PRCB)->OldRunPrio
/*得到保存的TCB指针*/
#define GET_RES_SAVED_TCB(PRCB) ((PRCB)->SavedTCB)

/*得到下一个资源指针*/
#define GET_RES_NEXT(PRCB)  ((PRCB)->NextRes)

#define SET_RES_NEXT(PRCB,next)  (PRCB)->NextRes=next

/********************************************************************
* 设置外部资源获取时的优先级变化：
* 1)保存资源优先级对应的TCB,注意：这里必须从OSTCBTbl中取值，不能从OSTCBPrioTbl取值，因为
*   OSTCBPrioTbl可能已经不正确了，内部资源获取也一样。
* 2)替换资源优先级对应的TCBPrioTbl中的值为当前任务
* 3)保存原来的运行优先级，用于ReleaseResource时恢复原来的运行优先级
* 4)设置运行优先级
* 5)将资源优先级放入就绪队列
*********************************************************************/
#define SET_OUT_PRIORITY(RCB,TCB) \
	do{if(((RCB)->OldRunPrio = GET_TASK_RUN_PRIO(TCB) ) < GET_RES_PRIO(RCB))\
	{\
		(RCB)->SavedTCB=&OSTCBTbl[(RCB)->priority];\
		OSTCBPrioTbl[(RCB)->priority]=(TCB);\
		OS_TASK_TO_READY((RCB)->priority,(RCB)->SavedTCB);\
		SET_TASK_RUN_PRIO(TCB,(RCB)->priority);\
	}}while(0)

/********************************************************************
* 设置外部资源释放时的优先级变化
* 1)恢复运行优先级，将其值设为原来保存的运行优先级
* 2)将OSTCBPrioTbl中的资源优先级所索引项的值设为原来的TCB
*********************************************************************/ 	
#define RESET_OUT_PRIORITY(PTCB,RCB) \
    do{if(GET_TASK_RUN_PRIO(PTCB) > GET_RES_OLD_RUN_PRIO(RCB))\
    {\
    	SET_TASK_RUN_PRIO(PTCB,GET_RES_OLD_RUN_PRIO(RCB));\
    	OSTCBPrioTbl[(RCB)->priority]=(RCB)->SavedTCB;\
    }}while(0)
	
/*在任务体的资源链表中增加资源*/
#define ADD_RES_LIST(TCB,RCB) \
    SET_RES_NEXT(res,(TCB)->resources);\
    SET_TASK_OUT_RES((TCB),res)

/*在任务体的资源链表中删除资源*/   
#define DELETE_RES_LIST(Tcb,RCB) (Tcb)->resources=RCB->NextRes

/***********************************************************
有中断级资源的一些操作
*************************************************************/
#ifdef OS_ISR_RES
/*保存资源的中断掩码*/
extern OSRCBRef OSISRLastResource;
#define SET_RES_ISR_MASK(RCB,mask) (RCB)->SavedMask=mask
/*得到资源的中断掩码*/
#define GET_RES_ISR_MASK(RCB) ((RCB)->SavedMask)
/*设置中断级资源的链表*/
#define ADD_RES_ISR_LIST(RCB) \
        (RCB)->NextRes= OSISRLastResource; /*资源链表*/\
        OSISRLastResource = (RCB)

#else/*#ifdef OS_ISR_RES*/
#define SET_RES_ISR_MASK(RCB,mask) 
#endif /*#ifdef OS_ISR_RES*/

/***********************************************************
扩展状态下的一些操作
*************************************************************/

#ifdef OS_EXTENDED_STATUS 
/*得到资源使用状态*/
#define GET_RES_USED(PRCB) ((PRCB)->IsUsed)
/*设置资源已使用*/
#define SET_RES_USED(PRCB) (PRCB)->IsUsed=OUT_RES_USED
/*设置资源未使用*/
#define SET_RES_UNUSED(PRCB) (PRCB)->IsUsed=OUT_RES_NOT_USED
#else
/*设置资源已使用*/
#define SET_RES_USED(PRCB) 
/*设置资源未使用*/
#define SET_RES_UNUSED(PRCB) 

#endif/*#ifdef OS_EXTENDED_STATUS */
 
 
    	
#define NO_RESOURCE ((OSRCBRef)0) /*无外部资源*/

#define DeclareResource(ResId) extern ResourceType ResId
void ResourceInit(void);

#endif/*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/ 



#if defined(OS_RES_SCHEDULER)&&defined(OS_OUT_RES) 

extern  ResourceType RES_SCHEDULER;    /*调度器资源*/
 
#define GetResource(ResId) \
       (ResId==RES_SCHEDULER)?GetSchedulerResource(RES_SCHEDULER):GetOutResource(ResId)
       
#define ReleaseResource(ResId) \
       (ResId==RES_SCHEDULER)?ReleaseSchedulerResource(RES_SCHEDULER):ReleaseOutResource(ResId)
       
#endif /*defined(OS_RES_SCHEDULER)&&defined(OS_OUT_RES) */

#if defined(OS_RES_SCHEDULER)&&!defined(OS_OUT_RES) /*只使用调度器资源，不使用其他任何资源**/

extern  ResourceType RES_SCHEDULER;    /*调度器资源*/ 

#define GetResource(ResId) GetSchedulerResource(ResId)

#define ReleaseResource(ResId) ReleaseSchedulerResource(ResId)

#endif /*#if !defined(OS_RES_SCHEDULER)&&defined(OS_RES_SCHEDULER) */

#if defined(OS_OUT_RES)&&!defined(OS_RES_SCHEDULER)/*不使用调度器资源，只使用其他任何资源**/

#define GetResource(ResId) GetOutResource(ResId)
#define ReleaseResource(ResId) ReleaseOutResource(ResId)

#endif /*#if defined(OS_RES_SCHEDULER)&&!defined(OS_RES_SCHEDULER)*/


#endif /*ifndef	 OS_RES_H*/
