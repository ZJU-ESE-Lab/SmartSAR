/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_res.c
* 摘    要：资源模块标准API的实现
*
* 当前版本：1.1
* 作    者：厉蒋
* 完成日期：2005年1月20日
*
* 取代版本：1.0
* 原作者  ：陈威
* 完成日期：2003年12月4日
*********************************************************************************
*/
#ifndef OS_RES_C /*为了不重复包含该文件*/
#define OS_RES_C

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

#ifdef OS_OUT_RES /*使用外部资源*/

#ifndef OS_RES_SCHEDULER/*不使用调度器资源*/
OSRCB  OSResources[OS_NUM_OUT_RES]; /*外部资源控制结构体数组*/
#else /*ifndef OS_SCHEDULER*/

ResourceType RES_SCHEDULER=&OSResources[OS_NUM_OUT_RES];
OSRCB  OSResources[OS_NUM_OUT_RES+1];/*外部资源控制结构体数组，
                                              OSResources[OS_NUM_OUT_RES]为调度器资源*/

#endif/*#ifndef OS_RES_SCHEDULER*/

#elif defined(OS_RES_SCHEDULER)/*只有调度器资源*/
static OSRCB  OSResources[1]; 
ResourceType RES_SCHEDULER=OSResources;                                              
#endif/*#ifdef OS_OUT_RES*/


#ifdef OS_OUT_RES /*使用外部资源*/

extern ResPrioType OSResPrioTbl[OS_NUM_OUT_RES];/*外部引用OSResPrioTbl，该数组由用户定义*/

#ifdef OS_ISR_RES

OSRCBRef OSISRLastResource=NO_RESOURCE;/*用于记录中断中的资源的链表头*/

#define ISR_LOCK_SCHEDULER  0x10 /*用于锁调度器*/

#endif/*ifdef OS_ISR_RES*/    

#endif/*#ifdef OS_OUT_RES */          
/*
***************************************************************************
*void ResourceInit(void) 
* 描述：初始化每个资源
* 调用的函数：无
* 允许被调用的位置：StartOS
* 参数：	无
* 返回：	无
* 注意事项：无
* 创建人：厉蒋
* 时间：2004.11.23
* 修改人：厉蒋
* 时间：2005.1.13
***************************************************************************
*/
void ResourceInit(void)
{
#ifdef 	OS_OUT_RES 
	INT8U i=0;
	for(i=0;i<OS_NUM_OUT_RES;i++)
	{   /*初始化每个资源*/

		OSResources[i].priority=OSResPrioTbl[i];
		OSResources[i].SavedTCB=(OSTCBRef)0;
        OSResources[i].NextRes=(OSRCBRef)0;
        SET_RES_UNUSED(&OSResources[i]);
#ifdef OS_ISR_RES /*中断级的资源管理*/
        OSResources[i].SavedMask=0x00;/*保存的MASK*/
#endif/*ifdef OS_ISR_RES*/      			
    }
    
#endif/*#ifdef 	OS_OUT_RES */    
#ifdef OS_RES_SCHEDULER/*使用调度器资源*/
    RES_SCHEDULER->priority=(OS_NUM_SYS_TASK)-1;
    RES_SCHEDULER->SavedTCB=(OSTCBRef)0;
    RES_SCHEDULER->NextRes=(OSRCBRef)0;
    SET_RES_UNUSED(RES_SCHEDULER);
#ifdef OS_ISR_RES /*中断级的资源管理*/
    RES_SCHEDULER->SavedMask=0x00;/*保存的MASK*/
#endif/*ifdef OS_ISR_RES*/ 
#endif /*ifdef OS_RES_SCHEDULER*/ 
}

#ifdef OS_OUT_RES/*使用外部资源*/
/*
***************************************************************************
*StatusType GetOutResource( ResourceType res ) 
* 描述：当前任务得到res的外部资源
* 调用的函数：无
* 允许被调用的位置：TASK、二类ISR
* 参数：	res：要得到的外部资源
* 返回：	E_OK：调用成功
*		    E_OS_ID：参数res非法
            E_OS_ACCESS：res已经被使用或
                           当前任务的优先级比该资源的优先级高
* 注意事项：该函数只适用于外部资源
* 创建人：厉蒋
* 时间：2005.1.13
* 修改人：厉蒋
* 时间：2005.5.23
* 备注：实现调度器资源与其他外部资源的分开，提高效率
***************************************************************************
*/

StatusType GetOutResource( ResourceType res )  
{
    DEFINECRITICAL(cpu_sr);

#ifdef OS_EXTENDED_STATUS
    if( ((OSResources+OS_NUM_OUT_RES)<=res) || (res<OSResources) 
        ||(((AddrType)res-(AddrType)OSResources)%sizeof(OSRCB)!=0))
    {        /*参数res大于设定的最大值，则出错。RES_SCHEDULER*/
        OSRETERROR( E_OS_ID, OSServiceId_GetResource, res); 
    }
    if( OUT_RES_USED == GET_RES_USED(res))
    {  /*资源已经被使用*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res);    
    }
#ifndef OS_ISR_RES  /*任务级资源*/  
    if( (GET_TASK_ID(OSTCBCur) > GET_RES_PRIO(res)) || (!NOT_IN_ISR)   )
    {               /*如果资源已经被占有或当前任务的优先级比该资源的优先级高*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res);    
    }
#else  /*既有中断级，又有任务级资源*/
    if((GET_RES_PRIO(res) >0  ) /*如果是任务级资源*/
        && ( (GET_TASK_ID(OSTCBCur) > GET_RES_PRIO(res)) || ( !NOT_IN_ISR ) ))
    /*如果资源已经被占有或当前任务的优先级比该资源的优先级高*/
    {
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res);    
    }
    else if( (GET_RES_PRIO(res) <=0)  /*如果是中断级资源*/
       &&( !NOT_IN_ISR )&&( GET_ISR_PRIO() > -(GET_RES_PRIO(res) ) ))
    {  /*如果在中断调用中断级资源，并且该中断优先级比资源优先级高，则出错*/
        
    	OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res); 
    }
#endif /*ifndef OS_ISR_RES*/    

#endif  /*ifdef OS_EXTENDED_STATUS*/
    OS_ENTER_CRITICAL();
    
    SET_RES_USED(res);
#ifdef  OS_ISR_RES 
    if (GET_RES_PRIO(res)<0) /*中断级资源*/
    {
#ifdef OS_EXCEPTION_EN /*防止编译错误*/    
        DISABLE_LOW_PRIO_ISR(-(GET_RES_PRIO(res)));/*把小于该资源的优先级的中断屏蔽*/
        SET_RES_ISR_MASK(res,ISR_MASK);/*设置资源的掩码*/
#endif/*ifdef OS_EXCEPTION_EN*/
        ADD_RES_ISR_LIST(res); 
        OSIntNesting|=ISR_LOCK_SCHEDULER;/*设置OSIntNesting，防止在任务中引发调度，从而被其他任务抢占，即锁住调度器*/
    } 
    else
#endif /*ifdef  OS_ISR_RES*/    
    {/*任务级资源*/
    	SET_OUT_PRIORITY(res,OSTCBCur);/*设置得到外部资源后的优先级变换*/
    	ADD_RES_LIST(OSTCBCur,res);/*把当前得到的资源放在列表的首地址*/
    }
    OS_EXIT_CRITICAL();
    return( E_OK );    
    
}

/*
***************************************************************************
*StatusType ReleaseOutResource( ResourceType res ) 
* 描述：释放当前任务拥有的外部资源
* 调用的函数：无
* 允许被调用的位置：TASK、二类ISR
* 参数：	res：要释放的外部资源
* 返回：	E_OK：调用成功
*		    E_OS_ID：参数res非法
            E_OS_NOFUNC：res表示的资源没被使用或是必须要先释放其他资源
            E_OS_ACCESS：释放的资源优先级比当前任务的优先级低
* 注意事项：该函数只适用于外部资源，
                  而且用户按照LIFO顺序调用该API来释放资源
* 创建人：厉蒋
* 时间：2005.1.13
* 修改人：厉蒋
* 时间：2005.5.23
* 修改：郁利吉
* 时间：2005.12.13
* 备注：实现调度器资源与其他外部资源的分开，提高效率
***************************************************************************
*/
StatusType ReleaseOutResource( ResourceType res )
{
    DEFINECRITICAL(cpu_sr);
    OSTCBRef curTcb=NULL;
    OSTCBRef ptcb=NULL;
    
#ifdef OS_EXTENDED_STATUS
    if( ((OSResources+OS_NUM_OUT_RES)<=res) || (res<OSResources) 
        ||(((AddrType)res-(AddrType)OSResources)%sizeof(OSRCB)!=0))
    {        /*参数res大于设定的最大值，则出错*/
        OSRETERROR( E_OS_ID, OSServiceId_ReleaseResource, res);
    }
    if ( (OUT_RES_NOT_USED==GET_RES_USED(res)) )
    {            /*resID表示的资源没被使用*/
        OSRETERROR( E_OS_NOFUNC, OSServiceId_ReleaseResource, res);
    }
#ifndef OS_ISR_RES /*任务级资源*/ 
    if(  (!NOT_IN_ISR)  ||(GET_RES_PRIO(res) < GET_TASK_ID(OSTCBCur) ))
    {           /*释放的资源优先级比当前任务的优先级低*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_ReleaseResource, res);
    }
    if ( GET_TASK_OUT_RES(OSTCBCur)!=res )  /*释放的资源不是当前最新占有的资源*/
    {
    	OSRETERROR( E_OS_NOFUNC, OSServiceId_ReleaseResource, res);
    }
#else /*既有中断级，又有任务级资源*/
    if( GET_RES_PRIO(res) >0 )  /*如果是任务级资源*/        
    {
        if(  ( !NOT_IN_ISR)  ||(GET_RES_PRIO(res) < GET_TASK_ID(OSTCBCur) ))
        {           /*释放的资源优先级比当前任务的优先级低*/
            OSRETERROR( E_OS_ACCESS, OSServiceId_ReleaseResource, res);
        }
        else if( GET_TASK_OUT_RES(OSTCBCur)!=res)/*释放的资源不是当前最新占有的资源*/
        {
        	OSRETERROR( E_OS_ACCESS, OSServiceId_ReleaseResource, res);	
        }
        
    }
    else /*如果是中断级资源*/     
    {
        if(OSISRLastResource != res)
        {   /*当前中断资源不是要释放的资源*/
            OSRETERROR( E_OS_NOFUNC, OSServiceId_ReleaseResource, res);	
        }
    }
#endif /*ifndef OS_ISR_RES*/    

#endif  /*ifdef OS_EXTENDED_STATUS*/
    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;
    SET_RES_UNUSED(res);
#ifdef  OS_ISR_RES 
    if(  GET_RES_PRIO(res)<=0  )/*中断级资源*/
    {
        RUSUME_ISR(GET_RES_ISR_MASK(res));/*恢复中断掩码*/
        OSISRLastResource = GET_RES_NEXT(res);/*从链表堆栈中POP出该资源*/
        OSIntNesting &=~ISR_LOCK_SCHEDULER;/*释放调度器,可以引发调度*/
    }
    else/*非中断级资源*/
    {
#endif /*ifdef  OS_ISR_RES*/ 
        if( !IS_TASK_READY(&OSTCBTbl[GET_RES_PRIO(res)]) /*资源优先级的任务没有就绪*/
        && (GET_TASK_RUN_PRIO(curTcb)!=GET_RES_OLD_RUN_PRIO(res))/*并且其当前运行优先级不与原来的获取资源前的运行优先级不一样*/
             /*如果一样，表明获取了相同优先级的（内、外）资源一次以上*/
        ) 
        {   /*如果资源优先级在获取资源时就已经就绪，就将其从就绪队列中清除*/
            OS_TASK_TO_UNREADY(GET_RES_PRIO(res),GET_RES_SAVED_TCB(res));
        }
        RESET_OUT_PRIORITY(curTcb,res);/*重置优先级*/
        DELETE_RES_LIST(curTcb,res); 
#ifdef  OS_ISR_RES     
    }
#endif /*ifdef  OS_ISR_RES*/

#ifndef OS_NON_SCHEDULE
    if( NOT_IN_ISR 
#ifdef   OS_MIX_SCHEDULE 
	 && FULL_SCHED==GET_TASK_SCHED(curTcb)  /*混合调度有可抢占任务*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
    )  
    {    
        /*可以抢占,而且不在中断中而且调度器没有上锁*/
		if( (ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()] )!=curTcb)
        {  /*被高优先级抢占*/
            OS_TASK_SWITCH(curTcb);
            OSPostTaskHook();/*在当前任务被切换前，调用PostTaskHook，在这里还能得到当前运行任务的ID*/
            OSTCBCur=ptcb;
            GET_INT_RES(ptcb);
            OSPreTaskHook();/*调用PreTaskHook*/
            OSCheckStack(curTcb);   
            
            
#ifdef OS_EVENT_EN /*BCC1下不需要判断，因为肯定是新任务运行*/       
            if(RUN_READY!=GET_TASK_STATUS(ptcb))
#endif/*#ifdef OS_EVENT_EN*/            
            {  /*如果该任务还没运行过*/
                SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
                OSTaskSaveRun(curTcb,ptcb);        	
            }
#ifdef OS_EVENT_EN /*BCC1下不需要判断，因为肯定是新任务运行*/       
            else
            {
                OSTaskSaveLoadRun(curTcb,ptcb);        	
            }
#endif/*#ifdef OS_EVENT_EN*/        
        }
	}
#endif/*OS_NULL_SCHEDULE 不进入调度*/
    OS_EXIT_CRITICAL();
    return( E_OK );    
}

#endif /*ifdef OS_OUT_RES*/

#ifdef  OS_RES_SCHEDULER /*只使用调度器资源，不使用其他人任何外部资源的情况*/
/*
***************************************************************************
*StatusType GetSchedulerResource( ResourceType res ) 
* 描述：得到调度器资源
* 调用的函数：无
* 允许被调用的位置：TASK、二类ISR
* 参数：	res：调度器资源ID
* 返回：	E_OK：调用成功
*		    E_OS_ID：参数res非法
            E_OS_ACCESS：调度器资源已经被使用或在中断中占有该资源
* 注意事项：该函数只适用于调度器资源
* 创建人：厉蒋
* 时间：2005.3.7
* 修改人：厉蒋
* 时间：2005.5.23
* 备注：实现调度器资源与其他外部资源的分开，提高效率
***************************************************************************
*/

StatusType GetSchedulerResource( ResourceType res )  
{
    DEFINECRITICAL(cpu_sr);
    
    OSTCBRef curTcb=NULL;
#ifdef OS_EXTENDED_STATUS
    if(RES_SCHEDULER!=res)	 /*RES_SCHEDULER为OS_NUM_OUT_RES*/
    {        /*参数res不为RES_SCHEDULER则出错。RES_SCHEDULER*/
        OSRETERROR( E_OS_ID, OSServiceId_GetResource, res); 
    }
    if( (OUT_RES_USED == GET_RES_USED(res) )
       || ( !NOT_IN_ISR)
    )
    {               /*如果资源已经被占有或在中断中占有该*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res);    
    }
#endif  /*ifdef OS_EXTENDED_STATUS*/
    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;
    SET_OUT_PRIORITY(res,curTcb);/*设置得到外部资源后的优先级变换*/
#ifdef OS_EXTENDED_STATUS
    SET_RES_USED(res);
#endif /*ifdef OS_EXTENDED_STATUS*/
    ADD_RES_LIST(curTcb,res);/*把当前得到的资源放在列表的首地址*/
    OS_EXIT_CRITICAL();
    return( E_OK );    
    
}

/*
***************************************************************************
*StatusType ReleaseSchedulerResource( ResourceType res ) 
* 描述：释放调度器资源
* 调用的函数：无
* 允许被调用的位置：TASK、二类ISR
* 参数：	res：调度器资源ID
* 返回：	E_OK：调用成功
*		    E_OS_ID：参数res非法
            E_OS_NOFUNC：res表示的资源没被使用或是必须要先释放其他资源
            E_OS_ACCESS：释放的资源优先级比当前任务的优先级低
* 注意事项：该函数只适用于调度器资源
* 创建人：厉蒋
* 时间：2005.3.7
* 修改：郁利吉
* 时间：2005.12.13
***************************************************************************
*/
StatusType ReleaseSchedulerResource( ResourceType res )
{
    DEFINECRITICAL(cpu_sr);
    OSTCBRef ptcb=NULL;
    OSTCBRef curTcb=NULL;
#ifdef OS_EXTENDED_STATUS
    if ( RES_SCHEDULER!=res )	 /*RES_SCHEDULER为OS_NUM_OUT_RES*/
    {        /*参数res不为RES_SCHEDULER则出错。RES_SCHEDULER*/
        OSRETERROR( E_OS_ID, OSServiceId_ReleaseResource, res);
    }
    if ( (OUT_RES_NOT_USED==GET_RES_USED(res) )  )
    {            /*resID表示的资源没被使用或是必须要先释放其他资源*/
        OSRETERROR( E_OS_NOFUNC, OSServiceId_ReleaseResource, res);
    }
    if( (!NOT_IN_ISR ) )
    {           /*释放的资源优先级比当前任务的优先级低*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_ReleaseResource, res);
    }
#endif  /*ifdef OS_EXTENDED_STATUS*/
    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;
    if( !IS_TASK_READY(&OSTCBTbl[GET_RES_PRIO(res)]) /*资源优先级的任务没有就绪*/
        && (GET_TASK_RUN_PRIO(curTcb)!=GET_RES_OLD_RUN_PRIO(res))/*并且其当前运行优先级不与原来的获取资源前的运行优先级不一样*/
             /*如果一样，表明获取了相同优先级的（内、外）资源一次以上*/
    ) 
    {   /*如果资源优先级在获取资源时就已经就绪，就将其从就绪队列中清除*/
        OS_TASK_TO_UNREADY(GET_RES_PRIO(res),GET_RES_SAVED_TCB(res));
    }
    RESET_OUT_PRIORITY(curTcb,res);/*重置优先级*/
    SET_RES_UNUSED(res);
    DELETE_RES_LIST(curTcb,res);  

#ifndef OS_NON_SCHEDULE
    if(NOT_IN_ISR 
#ifdef   OS_MIX_SCHEDULE 
	 && FULL_SCHED==GET_TASK_SCHED(curTcb)  /*混合调度有可抢占任务*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
    )  
    {    
        /*可以抢占,而且不在中断中而且调度器没有上锁*/
		if( (ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()] )!=curTcb)
        {  /*被高优先级抢占*/
            OS_TASK_SWITCH(curTcb);
            OSPostTaskHook();/*在当前任务被切换前，调用PostTaskHook，在这里还能得到当前运行任务的ID*/
            OSTCBCur=ptcb;
            GET_INT_RES(ptcb);
            OSPreTaskHook();/*调用PreTaskHook*/
            OSCheckStack(curTcb);   
            
#ifdef OS_EVENT_EN /*BCC1下不需要判断，因为肯定是新任务运行*/       
            if(RUN_READY!=GET_TASK_STATUS(ptcb))
#endif/*#ifdef OS_EVENT_EN*/
            {  /*如果该任务还没运行过*/
                SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
                OSTaskSaveRun(curTcb,ptcb);        	
            }
#ifdef OS_EVENT_EN /*BCC1下不需要判断，因为肯定是新任务运行*/       
            else
            {
                OSTaskSaveLoadRun(curTcb,ptcb);        	
            }
#endif/*#ifdef OS_EVENT_EN*/            
        }
	}
#endif/*OS_NULL_SCHEDULE 不进入调度*/
    OS_EXIT_CRITICAL();	
    return( E_OK );    
}

#endif /*ifdef  OS_RES_SCHEDULER*/


#endif /*#ifndef OS_RES_C*/



