/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_evt.c
* 摘    要：实现事件Event模块的标准API
*
* 当前版本：1.2
* 作    者：厉蒋
* 完成日期：2005年5月22日
*
* 当前版本：1.1
* 作    者：高爽、厉蒋
* 完成日期：2005年3月1日
*
* 取代版本：1.0
* 原作者  ：古杨
* 完成日期：2004年12月4日
*********************************************************************************
*/


#ifndef  OS_EVT_C
#define  OS_EVT_C

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

/*
***************************************************************************
*StatusType SetEvent( TaskType TaskID, EventMaskType Mask)
* 描述：根据Mask来设置TaskID所表示任务的事件，
*       一旦改任务所等待的事件中有一个被置位，
*       该任务就进入就绪状态，有可能被调度
* 调用的函数：无
* 允许被调用的位置：任务级、二类中断服务程序
* 参数：TaskID：唯一标识任务的id
*		Mask：用于设置的事件掩码
* 返回：E_OK：调用成功
*		E_OS_ID：错误操作，TaskID无效或者tcb未经初始化
*       E_OS_ACCESS：错误操作，标识的任务不是扩展任务
*       E_OS_STATE：错误操作，标识的任务处在挂起状态
* 注意事项：
* 创建人：古杨
* 时间：2004.12.4
* 修改人：高爽
* 时间：2005.1.19
* 修改人：潘杭平
* 时间：2005.3.9
* 修改人：厉蒋
* 时间：2005.5.22
* 修改人：郁利吉
* 时间：2005.10.17
* 修改：郁利吉
* 时间：2005.12.13
* 备注：提高性能
***************************************************************************
*/
StatusType SetEvent ( TaskType ptcb,EventMaskType Mask )
{
    OSTCBRef curTcb=NULL;
    DEFINECRITICAL(cpu_sr);
#ifdef OS_EXTENDED_STATUS
    if( (ptcb >= (OSTCBTbl+OS_NUM_TASK)) || (ptcb<OSTCBTbl) 
        || ( ( (AddrType)ptcb-(AddrType)OSTCBTbl )%sizeof(OSTCB)!=0) )
    {
        OSRETERROR( E_OS_ID, OSServiceId_SetEvent, ptcb );		    
                /*错误操作，ptcb越界无效*/
    }
    if (EXTENDED != GET_TASK_TYPE( ptcb ))  
    {/*错误操作，标识的任务不是扩展任务*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_SetEvent, ptcb ); 
                      
    }
    
    if (SUSPENDED==GET_TASK_STATUS(ptcb))
    {/*错误操作，标识的任务处在挂起状态*/
        OSRETERROR( E_OS_STATE, OSServiceId_SetEvent, ptcb );  
    }
#endif/*#ifdef  OS_EXTENDED_STATUS*/

	OS_ENTER_CRITICAL();
	curTcb=OSTCBCur;
	SET_TASK_SET_EVENT(ptcb,Mask);/*设置该任务的事件掩码*/
	
	if ( GET_TASK_SET_EVENT(ptcb) & GET_TASK_WAIT_EVENT(ptcb))    
    {
        
        SET_TASK_STATUS(ptcb,RUN_READY);/*如果该任务在等待的事件被置位，则进入已运行的就绪状态*/
        OS_TASK_TO_READY(GET_TASK_ID(ptcb),ptcb);
	/******************************************************************
	以下调度可能还可以优化，因为如果发生调度，应该是被设置的事件的任务运行
	**********************************************************************/
#ifndef OS_NON_SCHEDULE
        if( NOT_IN_ISR    
#ifdef   OS_MIX_SCHEDULE /*混合调度有不可抢占任务*/
        && FULL_SCHED==GET_TASK_SCHED(OSTCBCur)  /*可抢占任务*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
        )  
        {    
        /*可以抢占,而且不在中断中而且调度器没有上锁*/
            if( GET_TASK_ID(ptcb) > GET_TASK_RUN_PRIO(curTcb)) /*如果设置事件的任务优先级比当前任务运行的优先级高*/
            {  /*被高优先级抢占*/
                OS_TASK_SWITCH(curTcb);
                OSPostTaskHook();/*在当前任务被切换前，调用PostTaskHook，在这里还能得到当前运行任务的ID*/
                OSTCBCur=ptcb;
                GET_INT_RES(ptcb);
                OSPreTaskHook();/*调用PreTaskHook*/
                OSCheckStack(curTcb);   
            
                SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
                OSTaskSaveLoadRun(curTcb,ptcb); /*保存原来的任务，运行旧任务*/   
            }	
	    }
#endif/*#ifndef OS_NON_SCHEDULE*/
    }
    OS_EXIT_CRITICAL();	
    return  E_OK ;
	
}



/*
***************************************************************************
*StatusType ClearEvent( EventMaskType Mask)
* 描述：根据Mask来清除当前任务的事件
* 调用的函数：无
* 允许被调用的位置：任务级
* 参数：Mask：用于清除的事件掩码
* 返回：E_OK：调用成功
*       E_OS_ACCESS：错误操作，当前任务不是扩展任务
*       E_OS_CALLEVEL：错误操作，在中断服务程序中被调用
* 注意事项：
* 创建人：古杨
* 时间：2004.12.4
* 修改人：高爽
* 时间：2005.1.19
* 修改人：厉蒋
* 时间：2005.3.1
* 修改人：厉蒋
* 时间：2005.5.22
* 备注：提高性能
***************************************************************************
*/
StatusType ClearEvent ( EventMaskType Mask )
{
    
    DEFINECRITICAL(cpu_sr);

#ifdef OS_EXTENDED_STATUS
     
    if (EXTENDED != GET_TASK_TYPE( OSTCBCur ))  
    {
        OSRETERROR( E_OS_ACCESS, OSServiceId_ClearEvent, 0 );  
            /*错误操作，当前任务不是扩展任务*/
    }	

    if (!NOT_IN_ISR) 
    { 
        OSRETERROR( E_OS_CALLEVEL, OSServiceId_ClearEvent, 0 );            
            /*错误操作，在中断服务程序中被调用*/
    }
	
#endif/*#ifdef OS_EXTENDED_STATUS*/
	OS_ENTER_CRITICAL();
	CLERA_TASK_MASK_EVENT(OSTCBCur,Mask);/*被清除的事件位置零*/ 
	OS_EXIT_CRITICAL();

	return E_OK;
}



/*
***************************************************************************
*StatusType GetEvent( TaskType TaskID, EventMaskRefType Event)
* 描述：返回TaskID所表示任务的事件状态
* 调用的函数：无
* 允许被调用的位置：任务级，第二类中断服务程序，
*                   ErrorHook, PreTaskHook, PostTaskHook
* 参数：TaskID：唯一标识任务的id
*       Event：指向返回信息的指针
* 返回：E_OK：调用成功
*       E_OS_ID：错误操作，TaskID无效或者tcb未经初始化
*       E_OS_ACCESS：错误操作，标识的任务不是扩展任务
*       E_OS_STATE：错误操作，标识的任务处在挂起状态
* 注意事项：
* 创建人：古杨
* 时间：2004.12.4
* 修改人：高爽
* 时间：2005.1.19
* 修改人：厉蒋
* 时间：2005.3.1
* 修改人：厉蒋
* 时间：2005.5.22
* 修改人：郁利吉
* 时间：2005.10.17
* 备注：提高性能
***************************************************************************
*/
StatusType GetEvent ( TaskType ptcb,EventMaskRefType Event )
{
	
#ifdef OS_EXTENDED_STATUS    
        
    if( (ptcb >= (OSTCBTbl+OS_NUM_TASK)) || (ptcb<OSTCBTbl) 
        || ( ( (AddrType)ptcb-(AddrType)OSTCBTbl )%sizeof(OSTCB)!=0) )
    {/*错误操作，ptcb越界无效*/
      OSRETERROR( E_OS_ID, OSServiceId_GetEvent, ptcb );	    
            
    }
    if (EXTENDED != GET_TASK_TYPE( ptcb))
    {/*错误操作，标识的任务不是扩展任务*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetEvent, ptcb );  
    }
    
    if(SUSPENDED== GET_TASK_STATUS(ptcb))
    {/*错误操作，标识的任务处在挂起状态*/
        OSRETERROR( E_OS_STATE, OSServiceId_GetEvent, ptcb );  
    }

#endif/*#ifdef OS_EXTENDED_STATUS*/
    
	*Event = GET_TASK_SET_EVENT(ptcb);   /*将任务的事件位返回*/
	
	return  E_OK ;    
}



/*
***************************************************************************
*StatusType WaitEvent( EventMaskType Mask)
* 描述：使当前任务进入等待状态直到有Mask表示的事件发生
* 调用的函数：无
* 允许被调用的位置：任务级
* 参数：Mask：等待事件的掩码
* 返回：E_OK：调用成功
*       E_OS_ACCESS：错误操作，当前任务不是扩展任务
*       E_OS_RESOURCE：错误操作，当前任务还占有资源
*       E_OS_CALLEVEL：错误操作，在中断服务程序中被调用
* 注意事项：
* 创建人：古杨
* 时间：2004.12.4
* 修改人：高爽
* 时间：2005.1.19
* 修改人：厉蒋
* 时间：2005.3.1
* 修改人：厉蒋
* 时间：2005.5.22
* 备注：提高性能
***************************************************************************
*/
StatusType WaitEvent ( EventMaskType Mask )
{
    OSTCBRef curTCB=NULL;
    OSTCBRef ptcb=NULL;
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)  
    INT8U	i=0;/*外部资源的时候才会用到*/
#endif 
    DEFINECRITICAL(cpu_sr);       
#ifdef OS_EXTENDED_STATUS    
        
    if (EXTENDED != GET_TASK_TYPE( OSTCBCur))
    {/*错误操作，当前任务不是扩展任务*/  
        OSRETERROR( E_OS_ACCESS, OSServiceId_WaitEvent, 0 );
    	    
    }
    if (!NOT_IN_ISR)
    { /*错误操作，在中断服务程序中被调用*/
        OSRETERROR( E_OS_CALLEVEL, OSServiceId_WaitEvent, 0 );
    }
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
    if(NO_RESOURCE != GET_TASK_OUT_RES(OSTCBCur)
#ifdef   OS_ISR_RES
     || NO_RESOURCE !=OSISRLastResource
#endif/*ifdef   OS_ISR_RES*/
    ) 
   	{
        OSRETERROR( E_OS_RESOURCE, OSServiceId_WaitEvent, 0 );
 	}
#endif /*#ifdef OS_OUT_RES||defined(OS_RES_SCHEDULER)*/
   	   
#endif/* #ifdef OS_EXTENDED_STATUS */
	
	OS_ENTER_CRITICAL();
	curTCB=OSTCBCur;
	SET_TASK_WAIT_EVENT(curTCB,Mask);/*设置该任务的等待事件位*/
	if ( 0== ( GET_TASK_SET_EVENT(curTCB) & Mask ) )
    {  /*如果等待的事件还没有被触发，就进行调度*/
        RELEASE_INT_RES(curTCB); /*释放资源*/       
        OSPostTaskHook();
	      SET_TASK_UNREADY(GET_TASK_ID(curTCB),curTCB,WAITING);/*任务进入等待状态*/
	      SET_TASK_OLD(curTCB);	      
	    /*找到最高优先级的任务，开始调度*/
        OSTCBCur=ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()];/*找到最高优先级的任务*/
        GET_INT_RES(ptcb);   /*获取内部资源*/
        OSPreTaskHook();/*调用PreTaskHook*/
        OSCheckStack(curTCB);
        if(RUN_READY!=GET_TASK_STATUS(ptcb))
        {  /*如果该任务还没运行过*/
            SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
            OSTaskSaveRun(curTCB,ptcb);        	
        }
        else
        {
        	OSTaskSaveLoadRun(curTCB,ptcb);        	
        }
        OS_EXIT_CRITICAL();
    }
    else
    {/*如果等待的事件已经被触发，就不需要进入等待状态*/
        OS_EXIT_CRITICAL();
    }
	
	return E_OK;
}

#ifdef OS_ALARM_EN
/*
***************************************************************************
*void OSSetEvent( TaskType TaskID, EventMaskRefType Mask)
* 描述：根据Mask来设置TaskID所表示任务的事件，
*       一旦改任务所等待的事件中有一个被置位，
*       该任务就进入就绪状态，有可能被调度
* 调用的函数：无
* 允许被调用的位置：Alarm中的NotifyAlarm
* 参数：TaskID：唯一标识任务的id
*		Mask：用于设置的事件掩码的指针
* 返回：E_OK：调用成功
*		E_OS_ID：错误操作，TaskID无效或者tcb未经初始化
*       E_OS_ACCESS：错误操作，标识的任务不是扩展任务
*       E_OS_STATE：错误操作，标识的任务处在挂起状态
* 注意事项：
* 创建人：古杨
* 时间：2004.12.4
* 修改人：高爽
* 时间：2005.1.19
* 修改人：潘杭平
* 时间：2005.3.9
* 修改人：厉蒋
* 时间：2005.5.22
* 修改：郁利吉
* 时间：2005.12.13
* 备注：提高性能
***************************************************************************
*/
void OSSetEvent ( TaskType ptcb,EventMaskRefType Mask )
{
    OSTCBRef curTcb=NULL;
	curTcb=OSTCBCur;
	SET_TASK_SET_EVENT(ptcb,*Mask);/*设置该任务的事件掩码*/
	
	if ( GET_TASK_SET_EVENT(ptcb) & GET_TASK_WAIT_EVENT(ptcb))    
    {
        
        SET_TASK_STATUS(ptcb,RUN_READY);/*如果该任务在等待的事件被置位，则进入已运行的就绪状态*/
        OS_TASK_TO_READY(GET_TASK_ID(ptcb),ptcb);
	/******************************************************************
	以下调度可能还可以优化，因为如果发生调度，应该是被设置的事件的任务运行
	**********************************************************************/
#ifndef OS_NON_SCHEDULE
        if( NOT_IN_ISR    
#ifdef   OS_MIX_SCHEDULE /*混合调度有不可抢占任务*/
        && FULL_SCHED==GET_TASK_SCHED(OSTCBCur)  /*可抢占任务*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
        )  
        {    
        /*可以抢占,而且不在中断中而且调度器没有上锁*/
		    if( GET_TASK_ID(ptcb) > GET_TASK_RUN_PRIO(curTcb)) /*如果设置事件的任务优先级比当前任务运行的优先级高*/
            {  /*被高优先级抢占*/
                OS_TASK_SWITCH(curTcb);
                OSPostTaskHook();/*在当前任务被切换前，调用PostTaskHook，在这里还能得到当前运行任务的ID*/
                OSTCBCur=ptcb;
                GET_INT_RES(ptcb);
                OSPreTaskHook();/*调用PreTaskHook*/
                OSCheckStack(curTcb);   
                
                SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
                OSTaskSaveLoadRun(curTcb,ptcb); /*保存原来的任务，运行旧任务*/   
            }		    
	    }
#endif/*#ifndef OS_NON_SCHEDULE*/
    }

}
#endif /*#ifdef OS_ALARM_EN*/

#endif/*#ifdef OS_EVENT_C*/