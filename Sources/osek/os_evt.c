/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_evt.c
* ժ    Ҫ��ʵ���¼�Eventģ��ı�׼API
*
* ��ǰ�汾��1.2
* ��    �ߣ�����
* ������ڣ�2005��5��22��
*
* ��ǰ�汾��1.1
* ��    �ߣ���ˬ������
* ������ڣ�2005��3��1��
*
* ȡ���汾��1.0
* ԭ����  ������
* ������ڣ�2004��12��4��
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
* ����������Mask������TaskID����ʾ������¼���
*       һ�����������ȴ����¼�����һ������λ��
*       ������ͽ������״̬���п��ܱ�����
* ���õĺ�������
* �������õ�λ�ã����񼶡������жϷ������
* ������TaskID��Ψһ��ʶ�����id
*		Mask���������õ��¼�����
* ���أ�E_OK�����óɹ�
*		E_OS_ID�����������TaskID��Ч����tcbδ����ʼ��
*       E_OS_ACCESS�������������ʶ����������չ����
*       E_OS_STATE�������������ʶ�������ڹ���״̬
* ע�����
* �����ˣ�����
* ʱ�䣺2004.12.4
* �޸��ˣ���ˬ
* ʱ�䣺2005.1.19
* �޸��ˣ��˺�ƽ
* ʱ�䣺2005.3.9
* �޸��ˣ�����
* ʱ�䣺2005.5.22
* �޸��ˣ�������
* ʱ�䣺2005.10.17
* �޸ģ�������
* ʱ�䣺2005.12.13
* ��ע���������
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
                /*���������ptcbԽ����Ч*/
    }
    if (EXTENDED != GET_TASK_TYPE( ptcb ))  
    {/*�����������ʶ����������չ����*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_SetEvent, ptcb ); 
                      
    }
    
    if (SUSPENDED==GET_TASK_STATUS(ptcb))
    {/*�����������ʶ�������ڹ���״̬*/
        OSRETERROR( E_OS_STATE, OSServiceId_SetEvent, ptcb );  
    }
#endif/*#ifdef  OS_EXTENDED_STATUS*/

	OS_ENTER_CRITICAL();
	curTcb=OSTCBCur;
	SET_TASK_SET_EVENT(ptcb,Mask);/*���ø�������¼�����*/
	
	if ( GET_TASK_SET_EVENT(ptcb) & GET_TASK_WAIT_EVENT(ptcb))    
    {
        
        SET_TASK_STATUS(ptcb,RUN_READY);/*����������ڵȴ����¼�����λ������������еľ���״̬*/
        OS_TASK_TO_READY(GET_TASK_ID(ptcb),ptcb);
	/******************************************************************
	���µ��ȿ��ܻ������Ż�����Ϊ����������ȣ�Ӧ���Ǳ����õ��¼�����������
	**********************************************************************/
#ifndef OS_NON_SCHEDULE
        if( NOT_IN_ISR    
#ifdef   OS_MIX_SCHEDULE /*��ϵ����в�����ռ����*/
        && FULL_SCHED==GET_TASK_SCHED(OSTCBCur)  /*����ռ����*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
        )  
        {    
        /*������ռ,���Ҳ����ж��ж��ҵ�����û������*/
            if( GET_TASK_ID(ptcb) > GET_TASK_RUN_PRIO(curTcb)) /*��������¼����������ȼ��ȵ�ǰ�������е����ȼ���*/
            {  /*�������ȼ���ռ*/
                OS_TASK_SWITCH(curTcb);
                OSPostTaskHook();/*�ڵ�ǰ�����л�ǰ������PostTaskHook�������ﻹ�ܵõ���ǰ���������ID*/
                OSTCBCur=ptcb;
                GET_INT_RES(ptcb);
                OSPreTaskHook();/*����PreTaskHook*/
                OSCheckStack(curTcb);   
            
                SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
                OSTaskSaveLoadRun(curTcb,ptcb); /*����ԭ�����������о�����*/   
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
* ����������Mask�������ǰ������¼�
* ���õĺ�������
* �������õ�λ�ã�����
* ������Mask������������¼�����
* ���أ�E_OK�����óɹ�
*       E_OS_ACCESS�������������ǰ��������չ����
*       E_OS_CALLEVEL��������������жϷ�������б�����
* ע�����
* �����ˣ�����
* ʱ�䣺2004.12.4
* �޸��ˣ���ˬ
* ʱ�䣺2005.1.19
* �޸��ˣ�����
* ʱ�䣺2005.3.1
* �޸��ˣ�����
* ʱ�䣺2005.5.22
* ��ע���������
***************************************************************************
*/
StatusType ClearEvent ( EventMaskType Mask )
{
    
    DEFINECRITICAL(cpu_sr);

#ifdef OS_EXTENDED_STATUS
     
    if (EXTENDED != GET_TASK_TYPE( OSTCBCur ))  
    {
        OSRETERROR( E_OS_ACCESS, OSServiceId_ClearEvent, 0 );  
            /*�����������ǰ��������չ����*/
    }	

    if (!NOT_IN_ISR) 
    { 
        OSRETERROR( E_OS_CALLEVEL, OSServiceId_ClearEvent, 0 );            
            /*������������жϷ�������б�����*/
    }
	
#endif/*#ifdef OS_EXTENDED_STATUS*/
	OS_ENTER_CRITICAL();
	CLERA_TASK_MASK_EVENT(OSTCBCur,Mask);/*��������¼�λ����*/ 
	OS_EXIT_CRITICAL();

	return E_OK;
}



/*
***************************************************************************
*StatusType GetEvent( TaskType TaskID, EventMaskRefType Event)
* ����������TaskID����ʾ������¼�״̬
* ���õĺ�������
* �������õ�λ�ã����񼶣��ڶ����жϷ������
*                   ErrorHook, PreTaskHook, PostTaskHook
* ������TaskID��Ψһ��ʶ�����id
*       Event��ָ�򷵻���Ϣ��ָ��
* ���أ�E_OK�����óɹ�
*       E_OS_ID�����������TaskID��Ч����tcbδ����ʼ��
*       E_OS_ACCESS�������������ʶ����������չ����
*       E_OS_STATE�������������ʶ�������ڹ���״̬
* ע�����
* �����ˣ�����
* ʱ�䣺2004.12.4
* �޸��ˣ���ˬ
* ʱ�䣺2005.1.19
* �޸��ˣ�����
* ʱ�䣺2005.3.1
* �޸��ˣ�����
* ʱ�䣺2005.5.22
* �޸��ˣ�������
* ʱ�䣺2005.10.17
* ��ע���������
***************************************************************************
*/
StatusType GetEvent ( TaskType ptcb,EventMaskRefType Event )
{
	
#ifdef OS_EXTENDED_STATUS    
        
    if( (ptcb >= (OSTCBTbl+OS_NUM_TASK)) || (ptcb<OSTCBTbl) 
        || ( ( (AddrType)ptcb-(AddrType)OSTCBTbl )%sizeof(OSTCB)!=0) )
    {/*���������ptcbԽ����Ч*/
      OSRETERROR( E_OS_ID, OSServiceId_GetEvent, ptcb );	    
            
    }
    if (EXTENDED != GET_TASK_TYPE( ptcb))
    {/*�����������ʶ����������չ����*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetEvent, ptcb );  
    }
    
    if(SUSPENDED== GET_TASK_STATUS(ptcb))
    {/*�����������ʶ�������ڹ���״̬*/
        OSRETERROR( E_OS_STATE, OSServiceId_GetEvent, ptcb );  
    }

#endif/*#ifdef OS_EXTENDED_STATUS*/
    
	*Event = GET_TASK_SET_EVENT(ptcb);   /*��������¼�λ����*/
	
	return  E_OK ;    
}



/*
***************************************************************************
*StatusType WaitEvent( EventMaskType Mask)
* ������ʹ��ǰ�������ȴ�״ֱ̬����Mask��ʾ���¼�����
* ���õĺ�������
* �������õ�λ�ã�����
* ������Mask���ȴ��¼�������
* ���أ�E_OK�����óɹ�
*       E_OS_ACCESS�������������ǰ��������չ����
*       E_OS_RESOURCE�������������ǰ����ռ����Դ
*       E_OS_CALLEVEL��������������жϷ�������б�����
* ע�����
* �����ˣ�����
* ʱ�䣺2004.12.4
* �޸��ˣ���ˬ
* ʱ�䣺2005.1.19
* �޸��ˣ�����
* ʱ�䣺2005.3.1
* �޸��ˣ�����
* ʱ�䣺2005.5.22
* ��ע���������
***************************************************************************
*/
StatusType WaitEvent ( EventMaskType Mask )
{
    OSTCBRef curTCB=NULL;
    OSTCBRef ptcb=NULL;
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)  
    INT8U	i=0;/*�ⲿ��Դ��ʱ��Ż��õ�*/
#endif 
    DEFINECRITICAL(cpu_sr);       
#ifdef OS_EXTENDED_STATUS    
        
    if (EXTENDED != GET_TASK_TYPE( OSTCBCur))
    {/*�����������ǰ��������չ����*/  
        OSRETERROR( E_OS_ACCESS, OSServiceId_WaitEvent, 0 );
    	    
    }
    if (!NOT_IN_ISR)
    { /*������������жϷ�������б�����*/
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
	SET_TASK_WAIT_EVENT(curTCB,Mask);/*���ø�����ĵȴ��¼�λ*/
	if ( 0== ( GET_TASK_SET_EVENT(curTCB) & Mask ) )
    {  /*����ȴ����¼���û�б��������ͽ��е���*/
        RELEASE_INT_RES(curTCB); /*�ͷ���Դ*/       
        OSPostTaskHook();
	      SET_TASK_UNREADY(GET_TASK_ID(curTCB),curTCB,WAITING);/*�������ȴ�״̬*/
	      SET_TASK_OLD(curTCB);	      
	    /*�ҵ�������ȼ������񣬿�ʼ����*/
        OSTCBCur=ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()];/*�ҵ�������ȼ�������*/
        GET_INT_RES(ptcb);   /*��ȡ�ڲ���Դ*/
        OSPreTaskHook();/*����PreTaskHook*/
        OSCheckStack(curTCB);
        if(RUN_READY!=GET_TASK_STATUS(ptcb))
        {  /*���������û���й�*/
            SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
            OSTaskSaveRun(curTCB,ptcb);        	
        }
        else
        {
        	OSTaskSaveLoadRun(curTCB,ptcb);        	
        }
        OS_EXIT_CRITICAL();
    }
    else
    {/*����ȴ����¼��Ѿ����������Ͳ���Ҫ����ȴ�״̬*/
        OS_EXIT_CRITICAL();
    }
	
	return E_OK;
}

#ifdef OS_ALARM_EN
/*
***************************************************************************
*void OSSetEvent( TaskType TaskID, EventMaskRefType Mask)
* ����������Mask������TaskID����ʾ������¼���
*       һ�����������ȴ����¼�����һ������λ��
*       ������ͽ������״̬���п��ܱ�����
* ���õĺ�������
* �������õ�λ�ã�Alarm�е�NotifyAlarm
* ������TaskID��Ψһ��ʶ�����id
*		Mask���������õ��¼������ָ��
* ���أ�E_OK�����óɹ�
*		E_OS_ID�����������TaskID��Ч����tcbδ����ʼ��
*       E_OS_ACCESS�������������ʶ����������չ����
*       E_OS_STATE�������������ʶ�������ڹ���״̬
* ע�����
* �����ˣ�����
* ʱ�䣺2004.12.4
* �޸��ˣ���ˬ
* ʱ�䣺2005.1.19
* �޸��ˣ��˺�ƽ
* ʱ�䣺2005.3.9
* �޸��ˣ�����
* ʱ�䣺2005.5.22
* �޸ģ�������
* ʱ�䣺2005.12.13
* ��ע���������
***************************************************************************
*/
void OSSetEvent ( TaskType ptcb,EventMaskRefType Mask )
{
    OSTCBRef curTcb=NULL;
	curTcb=OSTCBCur;
	SET_TASK_SET_EVENT(ptcb,*Mask);/*���ø�������¼�����*/
	
	if ( GET_TASK_SET_EVENT(ptcb) & GET_TASK_WAIT_EVENT(ptcb))    
    {
        
        SET_TASK_STATUS(ptcb,RUN_READY);/*����������ڵȴ����¼�����λ������������еľ���״̬*/
        OS_TASK_TO_READY(GET_TASK_ID(ptcb),ptcb);
	/******************************************************************
	���µ��ȿ��ܻ������Ż�����Ϊ����������ȣ�Ӧ���Ǳ����õ��¼�����������
	**********************************************************************/
#ifndef OS_NON_SCHEDULE
        if( NOT_IN_ISR    
#ifdef   OS_MIX_SCHEDULE /*��ϵ����в�����ռ����*/
        && FULL_SCHED==GET_TASK_SCHED(OSTCBCur)  /*����ռ����*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
        )  
        {    
        /*������ռ,���Ҳ����ж��ж��ҵ�����û������*/
		    if( GET_TASK_ID(ptcb) > GET_TASK_RUN_PRIO(curTcb)) /*��������¼����������ȼ��ȵ�ǰ�������е����ȼ���*/
            {  /*�������ȼ���ռ*/
                OS_TASK_SWITCH(curTcb);
                OSPostTaskHook();/*�ڵ�ǰ�����л�ǰ������PostTaskHook�������ﻹ�ܵõ���ǰ���������ID*/
                OSTCBCur=ptcb;
                GET_INT_RES(ptcb);
                OSPreTaskHook();/*����PreTaskHook*/
                OSCheckStack(curTcb);   
                
                SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
                OSTaskSaveLoadRun(curTcb,ptcb); /*����ԭ�����������о�����*/   
            }		    
	    }
#endif/*#ifndef OS_NON_SCHEDULE*/
    }

}
#endif /*#ifdef OS_ALARM_EN*/

#endif/*#ifdef OS_EVENT_C*/