/*
***************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_task.c
* ժ    Ҫ������ģ��API��ʵ��
*
* ��ǰ�汾��1.2
* ��    �ߣ�����
* ������ڣ�2005��5��17��
*
* ��ǰ�汾��1.1
* ��    �ߣ��˺�ƽ,����
* ������ڣ�2005��1��17��
*
* ȡ���汾��1.0
* ԭ����  ������
* ������ڣ�2003��12��4��
***************************************************************************
*/
/*������2005��12��13���޸�*/



#ifndef OS_TASK_C /*Ϊ�˲��ظ�������c�ļ�*/
#define OS_TASK_C

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

/***************************ȫ�ֱ����Ķ���********************************/
extern OSTaskCfg OSTaskCfgTbl[];
OSTCBRef           OSTCBCur=(OSTCBRef)0;				/*ָ��ǰ�����TCBָ��*/
OSTCBRef           OSTCBPrioTbl[(OS_NUM_SYS_TASK)];/* ���ȼ�������TCB������ʱ���õ�     */
OSTCB              OSTCBTbl[(OS_NUM_SYS_TASK) ];   /* ���е�TCB����ID������*/
const INT8U extraStackSize = ExtraStackSize; 

OSSTK ExtraStack[ExtraStackSize];
OSTCBRef           OSTCBOld=(OSTCBRef)0;/*ָ��ո��л�������*/
OSSTKRef           FreeStkTop;
OSSTKRef           FreeStkBottom;


/***************************���������*******************************/
INT8U  OSRdyGrp=0;/* ������������ */
INT8U  OSRdyTbl[OS_RDY_TBL_SIZE];/* �������� */   


#ifdef OS_TASK_AUTO_DEFINE
/*
***************************************************************************
* void  OSTaskInit()
* ��������ʼ��TCB����
* �������õ�λ�ã�StartOS
* ��������
* ���أ���
* ע�������OSEK��׼����
* �����ˣ�����
* ʱ�䣺2005.07.05
* �޸ģ�������
* ʱ�䣺2005.10.31
* �޸ģ�������
* ʱ�䣺2005.12.13
***************************************************************************
*/
void OSTaskInit()
{
    OSTCBRef Tcb;
    OSTaskCfg * TcbCfg=OSTaskCfgTbl;
    INT8U index=1;
    
    INITRDYTBL();/*��ʼ��������*/
     
    /*������������*/
    Tcb= &OSTCBTbl[0];
    SET_TASK_PROPERTY(Tcb,FULL_SCHED|AUTO_START);
    SET_TASK_ENTRY(Tcb,OSTaskIdle);
    
    IDLEREADY(Tcb);/*����idle����*/
    
    SET_TASK_STK(Tcb,&OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 2],OSTaskIdleStk);
    FreeStkTop = &ExtraStack[extraStackSize - 2];
    FreeStkBottom = ExtraStack;

    SET_TASK_STK_INIT(Tcb);
    OSTCBPrioTbl[0] = Tcb;
    
    for(;index<OS_NUM_SYS_TASK;index++)/*ѭ������ÿ����������*/
    {
        Tcb=OSTCBTbl+TcbCfg->ID; /*ȡ��TaskID���Ӧ��TCB*/
        OSTCBPrioTbl[TcbCfg->ID]=Tcb; 
        SET_TASK_ID(Tcb,(TcbCfg->ID));
        SET_TASK_PROPERTY(Tcb,TcbCfg->InfoBit);
        SET_TASK_ENTRY(Tcb,TcbCfg->PTask);
        CLEAR_TASK_EVENT(Tcb);
        SET_TASK_MASK(Tcb, TcbCfg->ID);

        SET_TASK_STK(Tcb,TcbCfg->PStkHigh,TcbCfg->PStkLow);
        
        if(0!=((TcbCfg->InfoBit) & AUTO_START))/*���Ϊ������*/
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
* ��������ʼ��prio��Ҳ���������ID��������TCB��
* ���õĺ�����OSTaskStkInit��OSTCBInit
* �������õ�λ�ã���
* ������(void (*PTask)(void *pd) task��ں���ָ��
	    PStkHigh ջ�ĸ�ָ��
	    PStkLow ջ�ĵ�ָ��
		Priority    ���ȼ�����id�� 
		Property  ��¼�Ƿ����������Ƿ�ռ���ڲ���Դ���Ƿ����ռ���Ƿ���չ����
                                                                   ����λ��ʾ�Ƿ����ռ�����ȷ�ʽ��
                                                                   ����λ��ʾ�Ƿ���������                                                                   
                                                                   ����λ��ʾ�Ƿ�ռ���ڲ���Դ��
                                                                   �ڰ�λ��ʾ�Ƿ���չ����
        ResPrio �ڲ���Դ���ȼ������û���ڲ���Դ���򽫸�����Ϊ��������ȼ�

* ���أ�E_OS_SYS_PARA ��������	
		E_OS_SYS_PRIO_EXIST �������Ѿ�������
		E_OK ����ok

* ע�������OSEK��׼����
* �����ˣ�����
* ʱ�䣺2003.11.23
* �޸��ˣ��˺�ƽ
* ʱ�䣺2005.1.13
* �޸��ˣ�����
* ʱ�䣺2005.5.31
* ��ע�����Ч��
***************************************************************************
*/
INT8U  FuncDefineTask (void (*PTask)(void ),OSSTKRef PStkHigh,OSSTKRef PStkLow,
							INT8U Priority,INT8U Property,PriorityType ResPrio)
{
    OSTCBRef  Tcb=NULL;
#ifdef OS_SHOW_OTHER_ERROR     
   	 /*���ȼ�����������*/
    if (Priority >= (OS_NUM_SYS_TASK) )              
    {
        RETURN_OTHER_ERROR( E_OS_SYS_PARA );	
    }
    if (NULL==PStk || NULL == PTask)/*�������ָ�벻Ϊ��,���Ҷ�ջָ�벻Ϊ��*/
    {
    	RETURN_OTHER_ERROR( E_OS_SYS_PARA );
    }    
	 /*����bool���������ȷ��*/
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
    if(0!=(Property & AUTO_START))/*���Ϊ������*/
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
	
    PStkLow=PStkLow;/*��ֹ���뾯��*/
    ResPrio=ResPrio;
    return E_OK;
}

#endif/*#ifdef OS_TASK_AUTO_DEFINE*/
/*
***************************************************************************
StatusType ActivateTask (TaskType ptcb)
* ����������һ������suspend״̬���������¿�ʼִ��
* ���õĺ�����
* �������õ�λ�ã����񼶺��жϼ���ֻ��������жϵ��ã�
* ������ptcb ����
* ���أ�E_OS_ID �����Ƿ�
        E_OS_LIMIT ������suspend״̬
* ע�����OSEK��׼����
* �����ˣ�����
* ʱ�䣺2003.11.23
* �޸��ˣ��˺�ƽ
* ʱ�䣺2005.1.13
* �޸��ˣ�����
* ʱ�䣺2005.5.15
* �޸ģ�������
* ʱ�䣺2005.12.13
* ��ע���������
***************************************************************************
*/
StatusType ActivateTask (TaskType ptcb)
{
    OSTCBRef    oldRunning=NULL;
    DEFINECRITICAL(cpu_sr);    
    
#ifdef OS_EXTENDED_STATUS
    if( (ptcb >= (OSTCBTbl+OS_NUM_SYS_TASK)) || (ptcb<OSTCBTbl) 
        || ( ( (AddrType)ptcb-(AddrType)OSTCBTbl )%sizeof(OSTCB)!=0) )
    {    /*TCBԽ��*/
        OSRETERROR( E_OS_ID, OSServiceId_ActivateTask, ptcb );	/*ע�����Ǹ��꣬����Ҫ��������*/
    }
#endif /*#ifdef OS_EXTENDED_STATUS*/
    /*Ϊ�˱�ֻ֤��һ��������������ͬһ������Ĳ���*/
    OS_ENTER_CRITICAL();     	            
    if (SUSPENDED != GET_TASK_STATUS(ptcb))
    {
        OS_EXIT_CRITICAL();  	
	      OSRETERROR( E_OS_LIMIT, OSServiceId_ActivateTask, ptcb );  
    }
    oldRunning=OSTCBCur;    
    CLEAR_TASK_EVENT(ptcb);/*������ù����¼�*/   
	  SET_TASK_READY(GET_TASK_ID(ptcb),ptcb);/*��������״̬�Լ�����������״̬*/
	      
        
#ifndef OS_NON_SCHEDULE /*���������*/
    if( NOT_IN_ISR   
#ifdef   OS_MIX_SCHEDULE 
        && FULL_SCHED==GET_TASK_SCHED(oldRunning)  /*����ռ����*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
    )  
    { 
        /*������ռ,���Ҳ����ж��ж��ҵ�����û������*/
        if( GET_TASK_ID(ptcb) > GET_TASK_RUN_PRIO(oldRunning)) /*���������������ȼ��ȵ�ǰ�������е����ȼ���*/
        {  /*�������ȼ���ռ*/
            OSPostTaskHook();
            OS_TASK_SWITCH(oldRunning);            
            OSTCBCur = ptcb;
            GET_INT_RES(ptcb);   /*��ȡ�ڲ���Դ*/            
            SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
            OSPreTaskHook();
            OSCheckStack(oldRunning);
            OSTaskSaveRun(oldRunning,ptcb); /*����ԭ������������������*/   
        }
    }	
#endif/*OS_NON_SCHEDULE */
    OS_EXIT_CRITICAL();	
	return E_OK;
}
/*
***************************************************************************
*StatusType TerminateTask( void ) 
* ����������ǰ�������
* ���õĺ�����OSSchedule()
* �������õ�λ�ã�TASK
* ������	��
* ���أ�	
*		    E_OS_CALLEVEL����ISR�е���
            E_OS_RESOURCE�����ô�API�ĺ���ռ���ⲿ��Դ
* ע�����
* �����ˣ�����
* ʱ�䣺2003.12.23
* �޸��ˣ��������˺�ƽ
* ʱ�䣺2005.1.17
* �޸��ˣ�����
* ʱ�䣺2005.5.15
* �޸ģ�������
* ʱ�䣺2005.12.13
* ��ע���������    
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
   	{    /*��ǰ����ռ���ⲿ��Դ*/
        OSRETERROR( E_OS_RESOURCE, OSServiceId_TerminateTask, 0);
 	}
#endif/*defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/

    if (!NOT_IN_ISR)  
    {    /*���ж��е���*/
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
      
      OSTCBCur=curTcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()];/*�ҵ�������ȼ�������*/      
      GET_INT_RES (curTcb);   /*��ȡ�ڲ���Դ*/
      OSPreTaskHook();/*����PreTaskHook*/
      if(RUN_READY!=GET_TASK_STATUS(curTcb))
      {  /*���������û���й�*/
          SET_TASK_STATUS(curTcb,RUN_READY);/*����״̬*/
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

    return E_OK;/*��ֹ���뾯�棬ʵ��ִ�в���*/
}
/*
***************************************************************************
StatusType ChainTask (TaskType ptcb)
* �����������ж�����������£������Լ����Ҽ���һ������suspend״̬������ptcb�������¿�ʼִ��
* ���õĺ�����ReDefineTask��OSSchedule
* �������õ�λ�ã�����
* ������ptcb ����
* ���أ�E_OS_ID �����Ƿ�
        E_OS_LIMIT ������suspend״̬
        E_OS_CALLEVEL ���жϼ�����
        E_OS_RESOURCE�����ô�API�ĺ���ռ���ⲿ��Դ
        E_OK �ɹ�
* ע�����OSEK��׼����
* �����ˣ�����
* ʱ�䣺2003.11.23
* �޸��ˣ��˺�ƽ
* ʱ�䣺2005.1.19
* �޸��ˣ�����
* ʱ�䣺2005.5.17
* �޸��ˣ�������
* ʱ�䣺2005.10.31
* ��ע���������
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
    /*tcbδ����ʼ������idԽ��*/
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
    if (!NOT_IN_ISR)  /*�ж��Ƿ����ж��е���*/
    {
        OSRETERROR( E_OS_CALLEVEL, OSServiceId_ChainTask, ptcb );
    } 

#endif /*#ifdef OS_EXTENDED_STATUS*/
    if ( ( SUSPENDED != GET_TASK_STATUS(ptcb))&& ptcb!=OSTCBCur)
    {		/*������������ڹ���״̬������Ҫ������������Լ�ʱ*/
	      OSRETERROR( E_OS_LIMIT, OSServiceId_ChainTask, ptcb );  
    }
    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;
    RELEASE_INT_RES(curTcb);
    /*��������*/
    OSPostTaskHook();
    SET_TASK_UNREADY(GET_TASK_ID(curTcb),curTcb,SUSPENDED);	
    SET_TASK_OLD(curTcb);/*���������*/
    /*��������*/
	  CLEAR_TASK_EVENT(ptcb);/*������ù����¼�*/ 	  
	  
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
    SET_TASK_READY(GET_TASK_ID(ptcb),ptcb);/*��������״̬�Լ�����������״̬*/
    }
    
    /*�ҵ�������ȼ������񣬵���*/
    OSTCBCur=ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()];/*�ҵ�������ȼ�������*/
    GET_INT_RES(ptcb);   /*��ȡ�ڲ���Դ*/
    OSPreTaskHook();/*����PreTaskHook*/
    if(RUN_READY!=GET_TASK_STATUS(ptcb))
    {  /*���������û���й�*/
        SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
        OSTaskRun(ptcb);        	
    }
    else
    {
        OSTaskLoadRun(ptcb);        	
    }
    OS_EXIT_CRITICAL();	 
    return E_OK;/*��ֹ���뾯�棬ʵ��ִ�в���*/   
}

/*
***************************************************************************
*StatusType GetTaskID( TaskRefType PTaskID )
* �������õ���ǰ���������id
* ���õĺ�������
* �������õ�λ�ã�TASK�Ͷ����ж�
* ������	TaskRefType PTaskID, TaskType���͵�ָ��
* ���أ�	E_OK�����óɹ�
* ע�����
* �����ˣ��˺�ƽ������
* ʱ�䣺2005.1.19  
* �޸��ˣ�����
* ʱ�䣺2005.5.17
* ��ע���������  
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
       
    if(NULL==OSTCBCur||OSTCBCur==OSTCBTbl)/*�����ǰTCBΪ�ջ�ΪIDLE�����ָ�룬�򷵻�INVALID_TASK*/
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
* �������õ�����(ptcb)��״̬
* ���õĺ�������
* �������õ�λ�ã�TASK�Ͷ����ж�
* ������	ptcb ָ���ĸ�����state һ��״̬������ָ��
* ���أ�	E_OK�����óɹ�
*		    E_OS_ID��ptcb�Ƿ�        
* ע�����
* �����ˣ��˺�ƽ������
* ʱ�䣺2005.1.19   
* �޸��ˣ�����
* ʱ�䣺2005.5.17
* ��ע���������
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
    	/*��Ϊ��RUN_READY״̬�����Եõ���״̬��Ҫ&~~RUN_READY��
    	���GET_TASK_STATUS�õ���ֵΪ~RUN_READY����*stateΪ0����READY
    	���Ϊ����ֵ������Ӱ�졣*/
    }	  

    return E_OK;
}

/*
***************************************************************************
*StatusType Schedule( void ) 
* ������ǿ�Ƶ���,����и����ȼ��������������ǰ�������������
* ���õĺ�������
* �������õ�λ�ã�TASK
* ������	��
* ���أ�	E_OK�����óɹ�
*		    E_OS_CALLEVEL����ISR�е���
            E_OS_RESOURCE�����ô�API�ĺ���ռ���ⲿ��Դ
* ע�������APIֻ����ռ���ڲ���Դ�������ڲ���Դ���ȼ������䱾��IDʱ��
*           ���䱾�����ռʱ�Żᷢ������.
*           �����������ռ�������������ȼ��뱾��ID���ʱ����Ҫ���ȡ�
*           ������ ��BCC1�£�����Ҫ�ж������е������Ƿ��Լ����й���
* �����ˣ�����,�˺�ƽ
* ʱ�䣺2005.1.13
* �޸��ˣ�����
* ʱ�䣺2005.5.17
* ��ע���������
* �޸��ˣ�����
* ʱ�䣺2005.7.27
* �޸ģ�������
* ʱ�䣺2005.12.13
* ��ע���޸ĵ����жϣ��Լ��������л��龳
***************************************************************************
*/

StatusType Schedule( void )
{
    OSTCBRef    ptcb ;
    OSTCBRef    curTcb=NULL;
    DEFINECRITICAL(cpu_sr);
    
#ifdef OS_EXTENDED_STATUS
    if (!NOT_IN_ISR)  
    {           /*���ж��е���*/
        OSRETERROR( E_OS_CALLEVEL, OSServiceId_Schedule, 0 );
    } 
    
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
    if ( NO_RESOURCE != GET_TASK_OUT_RES(OSTCBCur))
    {         /*ռ���ⲿ��Դ*/
        OSRETERROR( E_OS_RESOURCE, OSServiceId_Schedule, 0);
    }
#endif /*ifdef OS_OUT_RESOURCE*/
#endif /*ifdef OS_EXTENDED_STATUS*/

    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;

#ifdef OS_FULL_SCHEDULE
#ifdef OS_INT_RES /*���ڲ���Դ*/
    if(GET_TASK_ID(curTcb)==GET_TASK_RUN_PRIO(curTcb))
#endif/*#ifdef OS_INT_RES */        
    {  /*����������ȼ���ID��==�������ȼ���Run Priority��
              (���ڲ���Դ���ȼ����������ȼ���Ȼ�ռ���ڲ���Դ����ʱ����Ҫ����*/
    	OS_EXIT_CRITICAL();
    	return (E_OK);
    }
#elif defined(OS_MIX_SCHEDULE)/*#ifdef OS_FULL_SCHEDULE*/  /*�����ռ*/
    if( ( FULL_SCHED==GET_TASK_SCHED(curTcb) ) /*����ռ*/
#ifdef OS_INT_RES /*���ڲ���Դ*/        
        &&(GET_TASK_ID(curTcb)==GET_TASK_RUN_PRIO(curTcb))/*����ռ����,���ҵ���������ȼ���ID��С���������ȼ���Run Priority��
              (��ռ���ڲ���Դ���ڲ���Դ���ȼ����������ȼ��ߣ�*/      
#endif/*#ifdef OS_INT_RES */
        )
    {
        OS_EXIT_CRITICAL();
        return (E_OK);	
    }
#endif/*#ifdef OS_FULL_SCHEDULE*/                          

    RELEASE_INT_RES(curTcb);/*���ͷ��ڲ���Դ*/
    if( (ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()] )!=curTcb)
    {  /*�������ȼ���ռ*/
        OS_TASK_SWITCH(curTcb);
        OSPostTaskHook();/*�ڵ�ǰ�����л�ǰ������PostTaskHook�������ﻹ�ܵõ���ǰ���������ID*/
        OSTCBCur=ptcb;        
        GET_INT_RES(ptcb);   /*��ȡ�ڲ���Դ*/
        OSPreTaskHook();/*����PreTaskHook*/
        OSCheckStack(curTcb); 
#ifdef OS_EVENT_EN /*BCC1�²���Ҫ�жϣ���Ϊ�϶�������������*/       
        if(RUN_READY!=GET_TASK_STATUS(ptcb))
#endif/*#ifdef OS_EVENT_EN*/          
        {  /*���������û���й�*/
            SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
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
        GET_INT_RES(curTcb);/*û�������ȣ������»�ȡ�ڲ���Դ*/
        OS_EXIT_CRITICAL();	
    }
    return (E_OK);
}


#ifdef OS_ALARM_EN
/*
***************************************************************************
void OSActivateTask (TaskType ptcb)
* ����������һ������suspend״̬������������Ե��ȣ���������
* ���õĺ�����
* �������õ�λ�ã�Alarm�е�NotifyAlarm����
* ������ptcb ����
* ���أ�E_OS_ID ����ptcb�Ƿ�
        E_OS_LIMIT ������suspend״̬
        
* ע�����OSEK��׼����
* �����ˣ�����
* ʱ�䣺2005.5.15
* �޸ģ�������
* ʱ�䣺2005.12.13
***************************************************************************
*/
void OSActivateTask (TaskType ptcb)
{
    OSTCBRef    oldRunning=NULL;
    
    oldRunning=OSTCBCur;
    CLEAR_TASK_EVENT(ptcb);/*������ù����¼�*/
    
	   
    SET_TASK_READY(GET_TASK_ID(ptcb),ptcb);/*��������״̬�Լ�����������״̬*/
#ifndef OS_NON_SCHEDULE /*���������*/
    if( NOT_IN_ISR   
#ifdef   OS_MIX_SCHEDULE 
	 && FULL_SCHED==GET_TASK_SCHED(oldRunning)  /*����ռ����*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
    )  
    { 
        /*������ռ,���Ҳ����ж��ж��ҵ�����û������*/
        if( GET_TASK_ID(ptcb) > GET_TASK_RUN_PRIO(oldRunning)) /*���������������ȼ��ȵ�ǰ�������е����ȼ���*/
        {  /*�������ȼ���ռ*/
            OSPostTaskHook();/*�ڵ�ǰ�����л�ǰ������PostTaskHook�������ﻹ�ܵõ���ǰ���������ID*/            
            OS_TASK_SWITCH(oldRunning);
            GET_INT_RES(ptcb);   /*��ȡ�ڲ���Դ*/
            OSTCBCur=ptcb;
            SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
            OSPreTaskHook();/*����PreTaskHook*/
            OSTaskSaveRun(oldRunning,ptcb); /*����ԭ������������������*/   
        }
	}	
#endif/*OS_NON_SCHEDULE */
	return ;
}

#endif/*#ifdef OS_ALARM_EN*/
#endif /*end of  #ifndef OS_TASK_C*/













