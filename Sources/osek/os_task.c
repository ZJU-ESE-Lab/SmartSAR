/*
***************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_task.c
* 摘    要：任务模块API的实现
*
* 当前版本：1.2
* 作    者：厉将
* 完成日期：2005年5月17日
*
* 当前版本：1.1
* 作    者：潘杭平,厉将
* 完成日期：2005年1月17日
*
* 取代版本：1.0
* 原作者  ：陈威
* 完成日期：2003年12月4日
***************************************************************************
*/
/*郁利吉2005年12月13日修改*/



#ifndef OS_TASK_C /*为了不重复包含该c文件*/
#define OS_TASK_C

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

/***************************全局变量的定义********************************/
extern OSTaskCfg OSTaskCfgTbl[];
OSTCBRef           OSTCBCur=(OSTCBRef)0;				/*指向当前任务的TCB指针*/
OSTCBRef           OSTCBPrioTbl[(OS_NUM_SYS_TASK)];/* 优先级索引的TCB表，调度时候用到     */
OSTCB              OSTCBTbl[(OS_NUM_SYS_TASK) ];   /* 所有的TCB，用ID来索引*/
const INT8U extraStackSize = ExtraStackSize; 

OSSTK ExtraStack[ExtraStackSize];
OSTCBRef           OSTCBOld=(OSTCBRef)0;/*指向刚刚切换的任务*/
OSSTKRef           FreeStkTop;
OSSTKRef           FreeStkBottom;


/***************************定义就绪表*******************************/
INT8U  OSRdyGrp=0;/* 就绪数组索引 */
INT8U  OSRdyTbl[OS_RDY_TBL_SIZE];/* 就绪队列 */   


#ifdef OS_TASK_AUTO_DEFINE
/*
***************************************************************************
* void  OSTaskInit()
* 描述：初始化TCB数组
* 允许被调用的位置：StartOS
* 参数：无
* 返回：无
* 注意事项：非OSEK标准函数
* 创建人：厉蒋
* 时间：2005.07.05
* 修改：郁利吉
* 时间：2005.10.31
* 修改：郁利吉
* 时间：2005.12.13
***************************************************************************
*/
void OSTaskInit()
{
    OSTCBRef Tcb;
    OSTaskCfg * TcbCfg=OSTaskCfgTbl;
    INT8U index=1;
    
    INITRDYTBL();/*初始化就绪表*/
     
    /*创建空闲任务*/
    Tcb= &OSTCBTbl[0];
    SET_TASK_PROPERTY(Tcb,FULL_SCHED|AUTO_START);
    SET_TASK_ENTRY(Tcb,OSTaskIdle);
    
    IDLEREADY(Tcb);/*就绪idle任务*/
    
    SET_TASK_STK(Tcb,&OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 2],OSTaskIdleStk);
    FreeStkTop = &ExtraStack[extraStackSize - 2];
    FreeStkBottom = ExtraStack;

    SET_TASK_STK_INIT(Tcb);
    OSTCBPrioTbl[0] = Tcb;
    
    for(;index<OS_NUM_SYS_TASK;index++)/*循环设置每个任务属性*/
    {
        Tcb=OSTCBTbl+TcbCfg->ID; /*取与TaskID相对应的TCB*/
        OSTCBPrioTbl[TcbCfg->ID]=Tcb; 
        SET_TASK_ID(Tcb,(TcbCfg->ID));
        SET_TASK_PROPERTY(Tcb,TcbCfg->InfoBit);
        SET_TASK_ENTRY(Tcb,TcbCfg->PTask);
        CLEAR_TASK_EVENT(Tcb);
        SET_TASK_MASK(Tcb, TcbCfg->ID);

        SET_TASK_STK(Tcb,TcbCfg->PStkHigh,TcbCfg->PStkLow);
        
        if(0!=((TcbCfg->InfoBit) & AUTO_START))/*如果为自启动*/
        {
    	      SET_TASK_READY(TcbCfg->ID,Tcb);
        }
        else
        {
    	      SET_TASK_STATUS(Tcb,SUSPENDED);
        }
        
        SET_TASK_RUN_PRIO(Tcb,TcbCfg->RunPrio);
        
        TcbCfg++;
    }
}
#else/*#ifdef OS_TASK_AUTO_DEFINE*/
/*
***************************************************************************
INT8U  FuncDefineTask (void (*PTask)(void ),OSSTKRef PStkHigh,OSSTKRef PStkLow,INT8U Priority,INT8U Property)
* 描述：初始化prio（也等于任务的ID）索引的TCB域
* 调用的函数：OSTaskStkInit，OSTCBInit
* 允许被调用的位置：无
* 参数：(void (*PTask)(void *pd) task入口函数指针
	    PStkHigh 栈的高指针
	    PStkLow 栈的低指针
		Priority    优先级（即id） 
		Property  记录是否自启动、是否占有内部资源、是否可抢占、是否扩展任务
                                                                   第五位表示是否可抢占即调度方式、
                                                                   第六位表示是否自启动、                                                                   
                                                                   第七位表示是否占有内部资源、
                                                                   第八位表示是否扩展任务
        ResPrio 内部资源优先级，如果没有内部资源，则将该项设为任务的优先级

* 返回：E_OS_SYS_PARA 参数出错	
		E_OS_SYS_PRIO_EXIST 该任务已经定义了
		E_OK 代表ok

* 注意事项：非OSEK标准函数
* 创建人：陈威
* 时间：2003.11.23
* 修改人：潘杭平
* 时间：2005.1.13
* 修改人：厉蒋
* 时间：2005.5.31
* 备注：提高效率
***************************************************************************
*/
INT8U  FuncDefineTask (void (*PTask)(void ),OSSTKRef PStkHigh,OSSTKRef PStkLow,
							INT8U Priority,INT8U Property,PriorityType ResPrio)
{
    OSTCBRef  Tcb=NULL;
#ifdef OS_SHOW_OTHER_ERROR     
   	 /*优先级超出最多个数*/
    if (Priority >= (OS_NUM_SYS_TASK) )              
    {
        RETURN_OTHER_ERROR( E_OS_SYS_PARA );	
    }
    if (NULL==PStk || NULL == PTask)/*任务入口指针不为空,而且堆栈指针不为空*/
    {
    	RETURN_OTHER_ERROR( E_OS_SYS_PARA );
    }    
	 /*三个bool参数检查正确性*/
	if ( NULL!=OSTCBPrioTbl[Priority]) 
    { 
        RETURN_OTHER_ERROR (E_OS_SYS_PRIO_EXIST);
    }
#endif
    Tcb=&OSTCBTbl[Priority];       
    OSTCBPrioTbl[Priority]=Tcb;
    SET_TASK_ID(Tcb,Priority);
    SET_TASK_PROPERTY(Tcb,Property);
    SET_TASK_ENTRY(Tcb,PTask);
    CLEAR_TASK_EVENT(Tcb);
    if(0!=(Property & AUTO_START))/*如果为自启动*/
    {
    	SET_TASK_READY(Priority,Tcb);
    }
    else
    {
    	SET_TASK_STATUS(Tcb,SUSPENDED);
    }
#ifdef OS_EVENT_EN     
    SET_TASK_STK(Tcb,PStkHigh,PStkLow);

#else/*#ifdef OS_EVENT_EN*/
    SET_TASK_STK(Tcb,&OSSTKTbl[Priority],&OSSTKTbl[Priority]);
#endif/*#ifdef OS_EVENT_EN*/
    SET_TASK_INT_RES(Tcb,ResPrio);
	
    PStkLow=PStkLow;/*防止编译警告*/
    ResPrio=ResPrio;
    return E_OK;
}

#endif/*#ifdef OS_TASK_AUTO_DEFINE*/
/*
***************************************************************************
StatusType ActivateTask (TaskType ptcb)
* 描述：激活一个处在suspend状态的任务，重新开始执行
* 调用的函数：
* 允许被调用的位置：任务级和中断级（只允许二类中断调用）
* 参数：ptcb 任务
* 返回：E_OS_ID 参数非法
        E_OS_LIMIT 任务不在suspend状态
* 注意事项：OSEK标准函数
* 创建人：陈威
* 时间：2003.11.23
* 修改人：潘杭平
* 时间：2005.1.13
* 修改人：厉蒋
* 时间：2005.5.15
* 修改：郁利吉
* 时间：2005.12.13
* 备注：提高性能
***************************************************************************
*/
StatusType ActivateTask (TaskType ptcb)
{
    OSTCBRef    oldRunning=NULL;
    DEFINECRITICAL(cpu_sr);    
    
#ifdef OS_EXTENDED_STATUS
    if( (ptcb >= (OSTCBTbl+OS_NUM_SYS_TASK)) || (ptcb<OSTCBTbl) 
        || ( ( (AddrType)ptcb-(AddrType)OSTCBTbl )%sizeof(OSTCB)!=0) )
    {    /*TCB越界*/
        OSRETERROR( E_OS_ID, OSServiceId_ActivateTask, ptcb );	/*注意这是个宏，所以要加中括号*/
    }
#endif /*#ifdef OS_EXTENDED_STATUS*/
    /*为了保证只有一个任务进入下面对同一个任务的操作*/
    OS_ENTER_CRITICAL();     	            
    if (SUSPENDED != GET_TASK_STATUS(ptcb))
    {
        OS_EXIT_CRITICAL();  	
	      OSRETERROR( E_OS_LIMIT, OSServiceId_ActivateTask, ptcb );  
    }
    oldRunning=OSTCBCur;    
    CLEAR_TASK_EVENT(ptcb);/*清除设置过的事件*/   
	  SET_TASK_READY(GET_TASK_ID(ptcb),ptcb);/*设置任务状态以及将其放入就绪状态*/
	      
        
#ifndef OS_NON_SCHEDULE /*不进入调度*/
    if( NOT_IN_ISR   
#ifdef   OS_MIX_SCHEDULE 
        && FULL_SCHED==GET_TASK_SCHED(oldRunning)  /*可抢占任务*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
    )  
    { 
        /*可以抢占,而且不在中断中而且调度器没有上锁*/
        if( GET_TASK_ID(ptcb) > GET_TASK_RUN_PRIO(oldRunning)) /*如果激活的任务优先级比当前任务运行的优先级高*/
        {  /*被高优先级抢占*/
            OSPostTaskHook();
            OS_TASK_SWITCH(oldRunning);            
            OSTCBCur = ptcb;
            GET_INT_RES(ptcb);   /*获取内部资源*/            
            SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
            OSPreTaskHook();
            OSCheckStack(oldRunning);
            OSTaskSaveRun(oldRunning,ptcb); /*保存原来的任务，运行新任务*/   
        }
    }	
#endif/*OS_NON_SCHEDULE */
    OS_EXIT_CRITICAL();	
	return E_OK;
}
/*
***************************************************************************
*StatusType TerminateTask( void ) 
* 描述：将当前任务挂起
* 调用的函数：OSSchedule()
* 允许被调用的位置：TASK
* 参数：	无
* 返回：	
*		    E_OS_CALLEVEL：在ISR中调用
            E_OS_RESOURCE：调用此API的函数占有外部资源
* 注意事项：
* 创建人：陈威
* 时间：2003.12.23
* 修改人：厉蒋，潘杭平
* 时间：2005.1.17
* 修改人：厉蒋
* 时间：2005.5.15
* 修改：郁利吉
* 时间：2005.12.13
* 备注：提高性能    
***************************************************************************
*/
StatusType TerminateTask( void )
{
    OSTCBRef    curTcb=NULL;    
    DEFINECRITICAL(cpu_sr);
  
#ifdef OS_EXTENDED_STATUS
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
   	if(NO_RESOURCE != GET_TASK_OUT_RES(OSTCBCur)
#if defined(OS_ISR_RES)&&defined(OS_OUT_RES)
     || NO_RESOURCE !=OSISRLastResource
#endif/*#if defined(OS_ISR_RES)&&defined(OS_OUT_RES)*/	
   	)
   	{    /*当前任务占有外部资源*/
        OSRETERROR( E_OS_RESOURCE, OSServiceId_TerminateTask, 0);
 	}
#endif/*defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/

    if (!NOT_IN_ISR)  
    {    /*在中断中调用*/
        OSRETERROR( E_OS_CALLEVEL, OSServiceId_TerminateTask, 0 );
    } 
#endif/*ifdef OS_EXTENDED_STATUS*/
   
    OS_ENTER_CRITICAL(); 
    curTcb=OSTCBCur;
    RELEASE_INT_RES(curTcb);
    OS_TASK_SWITCH(curTcb);
    OSPostTaskHook();
    SET_TASK_UNREADY(GET_TASK_ID(curTcb),curTcb,SUSPENDED);
        
    
    if(NOT_IN_ISR){     
      
      OSTCBCur=curTcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()];/*找到最高优先级的任务*/      
      GET_INT_RES (curTcb);   /*获取内部资源*/
      OSPreTaskHook();/*调用PreTaskHook*/
      if(RUN_READY!=GET_TASK_STATUS(curTcb))
      {  /*如果该任务还没运行过*/
          SET_TASK_STATUS(curTcb,RUN_READY);/*设置状态*/
          OSTaskRun(curTcb);        	
      }
      else
      {
          OSTaskLoadRun(curTcb);        	
      }
      OS_EXIT_CRITICAL();
    }
    else
    {
        OS_EXIT_CRITICAL();	
    }

    return E_OK;/*防止编译警告，实际执行不到*/
}
/*
***************************************************************************
StatusType ChainTask (TaskType ptcb)
* 描述：符合判断条件的情况下，挂起自己并且激活一个处在suspend状态的任务（ptcb），重新开始执行
* 调用的函数：ReDefineTask，OSSchedule
* 允许被调用的位置：任务级
* 参数：ptcb 任务
* 返回：E_OS_ID 参数非法
        E_OS_LIMIT 任务不在suspend状态
        E_OS_CALLEVEL 在中断级调用
        E_OS_RESOURCE：调用此API的函数占有外部资源
        E_OK 成功
* 注意事项：OSEK标准函数
* 创建人：陈威
* 时间：2003.11.23
* 修改人：潘杭平
* 时间：2005.1.19
* 修改人：厉蒋
* 时间：2005.5.17
* 修改人：郁利吉
* 时间：2005.10.31
* 备注：提高性能
***************************************************************************
*/
StatusType ChainTask( TaskType ptcb )  
{
    OSTCBRef    curTcb=NULL;
    DEFINECRITICAL(cpu_sr);
    
    OSSTKRef StkTopTemp;
    OSSTKRef StkBottomTemp;
	

    
#ifdef OS_EXTENDED_STATUS

    if( (ptcb >= (OSTCBTbl+OS_NUM_SYS_TASK)) || (ptcb<OSTCBTbl) 
        || ( ( (AddrType)ptcb-(AddrType)OSTCBTbl )%sizeof(OSTCB)!=0) )
    {
    /*tcb未经初始化或者id越界*/
        OSRETERROR( E_OS_ID, OSServiceId_ChainTask, ptcb );		    
    }
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
    if(NO_RESOURCE != GET_TASK_OUT_RES(OSTCBCur)
#ifdef   OS_ISR_RES
     || NO_RESOURCE !=OSISRLastResource
#endif/*ifdef   OS_ISR_RES*/
    )
   	{
        OSRETERROR( E_OS_RESOURCE, OSServiceId_ChainTask, ptcb );
 	 }
#endif  /*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/
    if (!NOT_IN_ISR)  /*判断是否在中断中调用*/
    {
        OSRETERROR( E_OS_CALLEVEL, OSServiceId_ChainTask, ptcb );
    } 

#endif /*#ifdef OS_EXTENDED_STATUS*/
    if ( ( SUSPENDED != GET_TASK_STATUS(ptcb))&& ptcb!=OSTCBCur)
    {		/*被激活的任务不在挂起状态，并且要激活的任务不是自己时*/
	      OSRETERROR( E_OS_LIMIT, OSServiceId_ChainTask, ptcb );  
    }
    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;
    RELEASE_INT_RES(curTcb);
    /*结束任务*/
    OSPostTaskHook();
    SET_TASK_UNREADY(GET_TASK_ID(curTcb),curTcb,SUSPENDED);	
    SET_TASK_OLD(curTcb);/*保存旧任务*/
    /*激活任务*/
	  CLEAR_TASK_EVENT(ptcb);/*清除设置过的事件*/ 	  
	  
	  if(curTcb == ptcb)
    { 
      SET_TASK_STATUS(ptcb,READY);
      OS_TASK_TO_READY(GET_TASK_ID(ptcb),ptcb);
      ptcb->OSTCBStkCur=OSTaskStkInit(ptcb->PTask,FreeStkTop);
      StkTopTemp = FreeStkTop;
      StkBottomTemp = FreeStkBottom;
      FreeStkTop = ptcb->OSTCBStkTop;
      FreeStkBottom = ptcb->OSTCBStkBottom;
      ptcb->OSTCBStkTop = StkTopTemp;
      ptcb->OSTCBStkBottom = StkBottomTemp;   
    } 
    else
    {		 
    SET_TASK_READY(GET_TASK_ID(ptcb),ptcb);/*设置任务状态以及将其放入就绪状态*/
    }
    
    /*找到最高优先级的任务，调度*/
    OSTCBCur=ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()];/*找到最高优先级的任务*/
    GET_INT_RES(ptcb);   /*获取内部资源*/
    OSPreTaskHook();/*调用PreTaskHook*/
    if(RUN_READY!=GET_TASK_STATUS(ptcb))
    {  /*如果该任务还没运行过*/
        SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
        OSTaskRun(ptcb);        	
    }
    else
    {
        OSTaskLoadRun(ptcb);        	
    }
    OS_EXIT_CRITICAL();	 
    return E_OK;/*防止编译警告，实际执行不到*/   
}

/*
***************************************************************************
*StatusType GetTaskID( TaskRefType PTaskID )
* 描述：得到当前运行任务的id
* 调用的函数：无
* 允许被调用的位置：TASK和二类中断
* 参数：	TaskRefType PTaskID, TaskType类型的指针
* 返回：	E_OK：调用成功
* 注意事项：
* 创建人：潘杭平，厉将
* 时间：2005.1.19  
* 修改人：厉蒋
* 时间：2005.5.17
* 备注：提高性能  
***************************************************************************
*/
StatusType GetTaskID( TaskRefType PTask)
{
#ifdef OS_SHOW_OTHER_ERROR 
   if (NULL==PTask) 
   {
       RETURN_OTHER_ERROR( E_OS_SYS_PARA );
   }
#endif
       
    if(NULL==OSTCBCur||OSTCBCur==OSTCBTbl)/*如果当前TCB为空或为IDLE任务的指针，则返回INVALID_TASK*/
    {
        *PTask=(TaskType)INVALID_TASK;   	
    }
    else
    {
        *PTask=(TaskType)OSTCBCur;
    }   
    return E_OK;
}

/*
***************************************************************************
*StatusType GetTaskState ( TaskType ptcb, TaskStateRefType state )
* 描述：得到任务(ptcb)的状态
* 调用的函数：无
* 允许被调用的位置：TASK和二类中断
* 参数：	ptcb 指定哪个任务，state 一个状态变量的指针
* 返回：	E_OK：调用成功
*		    E_OS_ID：ptcb非法        
* 注意事项：
* 创建人：潘杭平，厉蒋
* 时间：2005.1.19   
* 修改人：厉蒋
* 时间：2005.5.17
* 备注：提高性能
***************************************************************************
*/
StatusType GetTaskState ( TaskType ptcb, TaskStateRefType state )
{
   

#ifdef OS_EXTENDED_STATUS
    if( (ptcb >= (OSTCBTbl+OS_NUM_SYS_TASK)) || (ptcb<OSTCBTbl) 
        || ( ( (AddrType)ptcb-(AddrType)OSTCBTbl )%sizeof(OSTCB)!=0) )
    {
        OSRETERROR( E_OS_ID, OSServiceId_GetTaskState, ptcb );
    }
#endif/*#ifdef OS_EXTENDED_STATUS*/

    if(OSTCBCur==ptcb)
    {
        *state=RUNNING;	
    }
    else
    {
    	*state=(TaskStateType)(GET_TASK_STATUS(ptcb)&~(RUN_READY));
    	/*因为有RUN_READY状态，所以得到的状态码要&~~RUN_READY。
    	如果GET_TASK_STATUS得到的值为~RUN_READY，则*state为0，即READY
    	如果为其他值，则不受影响。*/
    }	  

    return E_OK;
}

/*
***************************************************************************
*StatusType Schedule( void ) 
* 描述：强制调度,如果有高优先级的任务就绪，当前任务放弃处理器
* 调用的函数：无
* 允许被调用的位置：TASK
* 参数：	无
* 返回：	E_OK：调用成功
*		    E_OS_CALLEVEL：在ISR中调用
            E_OS_RESOURCE：调用此API的函数占有外部资源
* 注意事项：在API只有在占有内部资源（且其内部资源优先级大于其本生ID时）
*           或其本身非抢占时才会发生调度.
*           即任务本身可抢占并且其运行优先级与本身ID相等时不需要调度。
*           ！！！ 在BCC1下，不需要判断新运行的任务是否以及运行过。
* 创建人：厉蒋,潘杭平
* 时间：2005.1.13
* 修改人：厉蒋
* 时间：2005.5.17
* 备注：提高性能
* 修改人：厉蒋
* 时间：2005.7.27
* 修改：郁利吉
* 时间：2005.12.13
* 备注：修改调度判断，以及上下文切换情境
***************************************************************************
*/

StatusType Schedule( void )
{
    OSTCBRef    ptcb ;
    OSTCBRef    curTcb=NULL;
    DEFINECRITICAL(cpu_sr);
    
#ifdef OS_EXTENDED_STATUS
    if (!NOT_IN_ISR)  
    {           /*在中断中调用*/
        OSRETERROR( E_OS_CALLEVEL, OSServiceId_Schedule, 0 );
    } 
    
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
    if ( NO_RESOURCE != GET_TASK_OUT_RES(OSTCBCur))
    {         /*占有外部资源*/
        OSRETERROR( E_OS_RESOURCE, OSServiceId_Schedule, 0);
    }
#endif /*ifdef OS_OUT_RESOURCE*/
#endif /*ifdef OS_EXTENDED_STATUS*/

    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;

#ifdef OS_FULL_SCHEDULE
#ifdef OS_INT_RES /*有内部资源*/
    if(GET_TASK_ID(curTcb)==GET_TASK_RUN_PRIO(curTcb))
#endif/*#ifdef OS_INT_RES */        
    {  /*当任务的优先级（ID）==运行优先级（Run Priority）
              (即内部资源优先级与任务优先级相等或不占有内部资源）此时不需要调度*/
    	OS_EXIT_CRITICAL();
    	return (E_OK);
    }
#elif defined(OS_MIX_SCHEDULE)/*#ifdef OS_FULL_SCHEDULE*/  /*混合抢占*/
    if( ( FULL_SCHED==GET_TASK_SCHED(curTcb) ) /*可抢占*/
#ifdef OS_INT_RES /*有内部资源*/        
        &&(GET_TASK_ID(curTcb)==GET_TASK_RUN_PRIO(curTcb))/*可抢占任务,并且当任务的优先级（ID）小于运行优先级（Run Priority）
              (即占有内部资源且内部资源优先级比任务优先级高）*/      
#endif/*#ifdef OS_INT_RES */
        )
    {
        OS_EXIT_CRITICAL();
        return (E_OK);	
    }
#endif/*#ifdef OS_FULL_SCHEDULE*/                          

    RELEASE_INT_RES(curTcb);/*先释放内部资源*/
    if( (ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()] )!=curTcb)
    {  /*被高优先级抢占*/
        OS_TASK_SWITCH(curTcb);
        OSPostTaskHook();/*在当前任务被切换前，调用PostTaskHook，在这里还能得到当前运行任务的ID*/
        OSTCBCur=ptcb;        
        GET_INT_RES(ptcb);   /*获取内部资源*/
        OSPreTaskHook();/*调用PreTaskHook*/
        OSCheckStack(curTcb); 
#ifdef OS_EVENT_EN /*BCC1下不需要判断，因为肯定是新任务运行*/       
        if(RUN_READY!=GET_TASK_STATUS(ptcb))
#endif/*#ifdef OS_EVENT_EN*/          
        {  /*如果该任务还没运行过*/
            SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
            OSTaskSaveRun(curTcb,ptcb);        	
        }
#ifdef OS_EVENT_EN        
        else
        {
            OSTaskSaveLoadRun(curTcb,ptcb);        	
        }
 #endif/*#ifdef OS_EVENT_EN*/        
        OS_EXIT_CRITICAL();
    }
    else
    {
        GET_INT_RES(curTcb);/*没发生调度，则重新获取内部资源*/
        OS_EXIT_CRITICAL();	
    }
    return (E_OK);
}


#ifdef OS_ALARM_EN
/*
***************************************************************************
void OSActivateTask (TaskType ptcb)
* 描述：激活一个处在suspend状态的任务，如果可以调度，则发生调度
* 调用的函数：
* 允许被调用的位置：Alarm中的NotifyAlarm函数
* 参数：ptcb 任务
* 返回：E_OS_ID 参数ptcb非法
        E_OS_LIMIT 任务不在suspend状态
        
* 注意事项：OSEK标准函数
* 创建人：厉蒋
* 时间：2005.5.15
* 修改：郁利吉
* 时间：2005.12.13
***************************************************************************
*/
void OSActivateTask (TaskType ptcb)
{
    OSTCBRef    oldRunning=NULL;
    
    oldRunning=OSTCBCur;
    CLEAR_TASK_EVENT(ptcb);/*清除设置过的事件*/
    
	   
    SET_TASK_READY(GET_TASK_ID(ptcb),ptcb);/*设置任务状态以及将其放入就绪状态*/
#ifndef OS_NON_SCHEDULE /*不进入调度*/
    if( NOT_IN_ISR   
#ifdef   OS_MIX_SCHEDULE 
	 && FULL_SCHED==GET_TASK_SCHED(oldRunning)  /*可抢占任务*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
    )  
    { 
        /*可以抢占,而且不在中断中而且调度器没有上锁*/
        if( GET_TASK_ID(ptcb) > GET_TASK_RUN_PRIO(oldRunning)) /*如果激活的任务优先级比当前任务运行的优先级高*/
        {  /*被高优先级抢占*/
            OSPostTaskHook();/*在当前任务被切换前，调用PostTaskHook，在这里还能得到当前运行任务的ID*/            
            OS_TASK_SWITCH(oldRunning);
            GET_INT_RES(ptcb);   /*获取内部资源*/
            OSTCBCur=ptcb;
            SET_TASK_STATUS(ptcb,RUN_READY);/*设置状态*/
            OSPreTaskHook();/*调用PreTaskHook*/
            OSTaskSaveRun(oldRunning,ptcb); /*保存原来的任务，运行新任务*/   
        }
	}	
#endif/*OS_NON_SCHEDULE */
	return ;
}

#endif/*#ifdef OS_ALARM_EN*/
#endif /*end of  #ifndef OS_TASK_C*/













