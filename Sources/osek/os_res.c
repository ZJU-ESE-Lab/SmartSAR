/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_res.c
* ժ    Ҫ����Դģ���׼API��ʵ��
*
* ��ǰ�汾��1.1
* ��    �ߣ�����
* ������ڣ�2005��1��20��
*
* ȡ���汾��1.0
* ԭ����  ������
* ������ڣ�2003��12��4��
*********************************************************************************
*/
#ifndef OS_RES_C /*Ϊ�˲��ظ��������ļ�*/
#define OS_RES_C

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

#ifdef OS_OUT_RES /*ʹ���ⲿ��Դ*/

#ifndef OS_RES_SCHEDULER/*��ʹ�õ�������Դ*/
OSRCB  OSResources[OS_NUM_OUT_RES]; /*�ⲿ��Դ���ƽṹ������*/
#else /*ifndef OS_SCHEDULER*/

ResourceType RES_SCHEDULER=&OSResources[OS_NUM_OUT_RES];
OSRCB  OSResources[OS_NUM_OUT_RES+1];/*�ⲿ��Դ���ƽṹ�����飬
                                              OSResources[OS_NUM_OUT_RES]Ϊ��������Դ*/

#endif/*#ifndef OS_RES_SCHEDULER*/

#elif defined(OS_RES_SCHEDULER)/*ֻ�е�������Դ*/
static OSRCB  OSResources[1]; 
ResourceType RES_SCHEDULER=OSResources;                                              
#endif/*#ifdef OS_OUT_RES*/


#ifdef OS_OUT_RES /*ʹ���ⲿ��Դ*/

extern ResPrioType OSResPrioTbl[OS_NUM_OUT_RES];/*�ⲿ����OSResPrioTbl�����������û�����*/

#ifdef OS_ISR_RES

OSRCBRef OSISRLastResource=NO_RESOURCE;/*���ڼ�¼�ж��е���Դ������ͷ*/

#define ISR_LOCK_SCHEDULER  0x10 /*������������*/

#endif/*ifdef OS_ISR_RES*/    

#endif/*#ifdef OS_OUT_RES */          
/*
***************************************************************************
*void ResourceInit(void) 
* ��������ʼ��ÿ����Դ
* ���õĺ�������
* �������õ�λ�ã�StartOS
* ������	��
* ���أ�	��
* ע�������
* �����ˣ�����
* ʱ�䣺2004.11.23
* �޸��ˣ�����
* ʱ�䣺2005.1.13
***************************************************************************
*/
void ResourceInit(void)
{
#ifdef 	OS_OUT_RES 
	INT8U i=0;
	for(i=0;i<OS_NUM_OUT_RES;i++)
	{   /*��ʼ��ÿ����Դ*/

		OSResources[i].priority=OSResPrioTbl[i];
		OSResources[i].SavedTCB=(OSTCBRef)0;
        OSResources[i].NextRes=(OSRCBRef)0;
        SET_RES_UNUSED(&OSResources[i]);
#ifdef OS_ISR_RES /*�жϼ�����Դ����*/
        OSResources[i].SavedMask=0x00;/*�����MASK*/
#endif/*ifdef OS_ISR_RES*/      			
    }
    
#endif/*#ifdef 	OS_OUT_RES */    
#ifdef OS_RES_SCHEDULER/*ʹ�õ�������Դ*/
    RES_SCHEDULER->priority=(OS_NUM_SYS_TASK)-1;
    RES_SCHEDULER->SavedTCB=(OSTCBRef)0;
    RES_SCHEDULER->NextRes=(OSRCBRef)0;
    SET_RES_UNUSED(RES_SCHEDULER);
#ifdef OS_ISR_RES /*�жϼ�����Դ����*/
    RES_SCHEDULER->SavedMask=0x00;/*�����MASK*/
#endif/*ifdef OS_ISR_RES*/ 
#endif /*ifdef OS_RES_SCHEDULER*/ 
}

#ifdef OS_OUT_RES/*ʹ���ⲿ��Դ*/
/*
***************************************************************************
*StatusType GetOutResource( ResourceType res ) 
* ��������ǰ����õ�res���ⲿ��Դ
* ���õĺ�������
* �������õ�λ�ã�TASK������ISR
* ������	res��Ҫ�õ����ⲿ��Դ
* ���أ�	E_OK�����óɹ�
*		    E_OS_ID������res�Ƿ�
            E_OS_ACCESS��res�Ѿ���ʹ�û�
                           ��ǰ��������ȼ��ȸ���Դ�����ȼ���
* ע������ú���ֻ�������ⲿ��Դ
* �����ˣ�����
* ʱ�䣺2005.1.13
* �޸��ˣ�����
* ʱ�䣺2005.5.23
* ��ע��ʵ�ֵ�������Դ�������ⲿ��Դ�ķֿ������Ч��
***************************************************************************
*/

StatusType GetOutResource( ResourceType res )  
{
    DEFINECRITICAL(cpu_sr);

#ifdef OS_EXTENDED_STATUS
    if( ((OSResources+OS_NUM_OUT_RES)<=res) || (res<OSResources) 
        ||(((AddrType)res-(AddrType)OSResources)%sizeof(OSRCB)!=0))
    {        /*����res�����趨�����ֵ�������RES_SCHEDULER*/
        OSRETERROR( E_OS_ID, OSServiceId_GetResource, res); 
    }
    if( OUT_RES_USED == GET_RES_USED(res))
    {  /*��Դ�Ѿ���ʹ��*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res);    
    }
#ifndef OS_ISR_RES  /*������Դ*/  
    if( (GET_TASK_ID(OSTCBCur) > GET_RES_PRIO(res)) || (!NOT_IN_ISR)   )
    {               /*�����Դ�Ѿ���ռ�л�ǰ��������ȼ��ȸ���Դ�����ȼ���*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res);    
    }
#else  /*�����жϼ�������������Դ*/
    if((GET_RES_PRIO(res) >0  ) /*�����������Դ*/
        && ( (GET_TASK_ID(OSTCBCur) > GET_RES_PRIO(res)) || ( !NOT_IN_ISR ) ))
    /*�����Դ�Ѿ���ռ�л�ǰ��������ȼ��ȸ���Դ�����ȼ���*/
    {
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res);    
    }
    else if( (GET_RES_PRIO(res) <=0)  /*������жϼ���Դ*/
       &&( !NOT_IN_ISR )&&( GET_ISR_PRIO() > -(GET_RES_PRIO(res) ) ))
    {  /*������жϵ����жϼ���Դ�����Ҹ��ж����ȼ�����Դ���ȼ��ߣ������*/
        
    	OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res); 
    }
#endif /*ifndef OS_ISR_RES*/    

#endif  /*ifdef OS_EXTENDED_STATUS*/
    OS_ENTER_CRITICAL();
    
    SET_RES_USED(res);
#ifdef  OS_ISR_RES 
    if (GET_RES_PRIO(res)<0) /*�жϼ���Դ*/
    {
#ifdef OS_EXCEPTION_EN /*��ֹ�������*/    
        DISABLE_LOW_PRIO_ISR(-(GET_RES_PRIO(res)));/*��С�ڸ���Դ�����ȼ����ж�����*/
        SET_RES_ISR_MASK(res,ISR_MASK);/*������Դ������*/
#endif/*ifdef OS_EXCEPTION_EN*/
        ADD_RES_ISR_LIST(res); 
        OSIntNesting|=ISR_LOCK_SCHEDULER;/*����OSIntNesting����ֹ���������������ȣ��Ӷ�������������ռ������ס������*/
    } 
    else
#endif /*ifdef  OS_ISR_RES*/    
    {/*������Դ*/
    	SET_OUT_PRIORITY(res,OSTCBCur);/*���õõ��ⲿ��Դ������ȼ��任*/
    	ADD_RES_LIST(OSTCBCur,res);/*�ѵ�ǰ�õ�����Դ�����б���׵�ַ*/
    }
    OS_EXIT_CRITICAL();
    return( E_OK );    
    
}

/*
***************************************************************************
*StatusType ReleaseOutResource( ResourceType res ) 
* �������ͷŵ�ǰ����ӵ�е��ⲿ��Դ
* ���õĺ�������
* �������õ�λ�ã�TASK������ISR
* ������	res��Ҫ�ͷŵ��ⲿ��Դ
* ���أ�	E_OK�����óɹ�
*		    E_OS_ID������res�Ƿ�
            E_OS_NOFUNC��res��ʾ����Դû��ʹ�û��Ǳ���Ҫ���ͷ�������Դ
            E_OS_ACCESS���ͷŵ���Դ���ȼ��ȵ�ǰ��������ȼ���
* ע������ú���ֻ�������ⲿ��Դ��
                  �����û�����LIFO˳����ø�API���ͷ���Դ
* �����ˣ�����
* ʱ�䣺2005.1.13
* �޸��ˣ�����
* ʱ�䣺2005.5.23
* �޸ģ�������
* ʱ�䣺2005.12.13
* ��ע��ʵ�ֵ�������Դ�������ⲿ��Դ�ķֿ������Ч��
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
    {        /*����res�����趨�����ֵ�������*/
        OSRETERROR( E_OS_ID, OSServiceId_ReleaseResource, res);
    }
    if ( (OUT_RES_NOT_USED==GET_RES_USED(res)) )
    {            /*resID��ʾ����Դû��ʹ��*/
        OSRETERROR( E_OS_NOFUNC, OSServiceId_ReleaseResource, res);
    }
#ifndef OS_ISR_RES /*������Դ*/ 
    if(  (!NOT_IN_ISR)  ||(GET_RES_PRIO(res) < GET_TASK_ID(OSTCBCur) ))
    {           /*�ͷŵ���Դ���ȼ��ȵ�ǰ��������ȼ���*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_ReleaseResource, res);
    }
    if ( GET_TASK_OUT_RES(OSTCBCur)!=res )  /*�ͷŵ���Դ���ǵ�ǰ����ռ�е���Դ*/
    {
    	OSRETERROR( E_OS_NOFUNC, OSServiceId_ReleaseResource, res);
    }
#else /*�����жϼ�������������Դ*/
    if( GET_RES_PRIO(res) >0 )  /*�����������Դ*/        
    {
        if(  ( !NOT_IN_ISR)  ||(GET_RES_PRIO(res) < GET_TASK_ID(OSTCBCur) ))
        {           /*�ͷŵ���Դ���ȼ��ȵ�ǰ��������ȼ���*/
            OSRETERROR( E_OS_ACCESS, OSServiceId_ReleaseResource, res);
        }
        else if( GET_TASK_OUT_RES(OSTCBCur)!=res)/*�ͷŵ���Դ���ǵ�ǰ����ռ�е���Դ*/
        {
        	OSRETERROR( E_OS_ACCESS, OSServiceId_ReleaseResource, res);	
        }
        
    }
    else /*������жϼ���Դ*/     
    {
        if(OSISRLastResource != res)
        {   /*��ǰ�ж���Դ����Ҫ�ͷŵ���Դ*/
            OSRETERROR( E_OS_NOFUNC, OSServiceId_ReleaseResource, res);	
        }
    }
#endif /*ifndef OS_ISR_RES*/    

#endif  /*ifdef OS_EXTENDED_STATUS*/
    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;
    SET_RES_UNUSED(res);
#ifdef  OS_ISR_RES 
    if(  GET_RES_PRIO(res)<=0  )/*�жϼ���Դ*/
    {
        RUSUME_ISR(GET_RES_ISR_MASK(res));/*�ָ��ж�����*/
        OSISRLastResource = GET_RES_NEXT(res);/*�������ջ��POP������Դ*/
        OSIntNesting &=~ISR_LOCK_SCHEDULER;/*�ͷŵ�����,������������*/
    }
    else/*���жϼ���Դ*/
    {
#endif /*ifdef  OS_ISR_RES*/ 
        if( !IS_TASK_READY(&OSTCBTbl[GET_RES_PRIO(res)]) /*��Դ���ȼ�������û�о���*/
        && (GET_TASK_RUN_PRIO(curTcb)!=GET_RES_OLD_RUN_PRIO(res))/*�����䵱ǰ�������ȼ�����ԭ���Ļ�ȡ��Դǰ���������ȼ���һ��*/
             /*���һ����������ȡ����ͬ���ȼ��ģ��ڡ��⣩��Դһ������*/
        ) 
        {   /*�����Դ���ȼ��ڻ�ȡ��Դʱ���Ѿ��������ͽ���Ӿ������������*/
            OS_TASK_TO_UNREADY(GET_RES_PRIO(res),GET_RES_SAVED_TCB(res));
        }
        RESET_OUT_PRIORITY(curTcb,res);/*�������ȼ�*/
        DELETE_RES_LIST(curTcb,res); 
#ifdef  OS_ISR_RES     
    }
#endif /*ifdef  OS_ISR_RES*/

#ifndef OS_NON_SCHEDULE
    if( NOT_IN_ISR 
#ifdef   OS_MIX_SCHEDULE 
	 && FULL_SCHED==GET_TASK_SCHED(curTcb)  /*��ϵ����п���ռ����*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
    )  
    {    
        /*������ռ,���Ҳ����ж��ж��ҵ�����û������*/
		if( (ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()] )!=curTcb)
        {  /*�������ȼ���ռ*/
            OS_TASK_SWITCH(curTcb);
            OSPostTaskHook();/*�ڵ�ǰ�����л�ǰ������PostTaskHook�������ﻹ�ܵõ���ǰ���������ID*/
            OSTCBCur=ptcb;
            GET_INT_RES(ptcb);
            OSPreTaskHook();/*����PreTaskHook*/
            OSCheckStack(curTcb);   
            
            
#ifdef OS_EVENT_EN /*BCC1�²���Ҫ�жϣ���Ϊ�϶�������������*/       
            if(RUN_READY!=GET_TASK_STATUS(ptcb))
#endif/*#ifdef OS_EVENT_EN*/            
            {  /*���������û���й�*/
                SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
                OSTaskSaveRun(curTcb,ptcb);        	
            }
#ifdef OS_EVENT_EN /*BCC1�²���Ҫ�жϣ���Ϊ�϶�������������*/       
            else
            {
                OSTaskSaveLoadRun(curTcb,ptcb);        	
            }
#endif/*#ifdef OS_EVENT_EN*/        
        }
	}
#endif/*OS_NULL_SCHEDULE ���������*/
    OS_EXIT_CRITICAL();
    return( E_OK );    
}

#endif /*ifdef OS_OUT_RES*/

#ifdef  OS_RES_SCHEDULER /*ֻʹ�õ�������Դ����ʹ���������κ��ⲿ��Դ�����*/
/*
***************************************************************************
*StatusType GetSchedulerResource( ResourceType res ) 
* �������õ���������Դ
* ���õĺ�������
* �������õ�λ�ã�TASK������ISR
* ������	res����������ԴID
* ���أ�	E_OK�����óɹ�
*		    E_OS_ID������res�Ƿ�
            E_OS_ACCESS����������Դ�Ѿ���ʹ�û����ж���ռ�и���Դ
* ע������ú���ֻ�����ڵ�������Դ
* �����ˣ�����
* ʱ�䣺2005.3.7
* �޸��ˣ�����
* ʱ�䣺2005.5.23
* ��ע��ʵ�ֵ�������Դ�������ⲿ��Դ�ķֿ������Ч��
***************************************************************************
*/

StatusType GetSchedulerResource( ResourceType res )  
{
    DEFINECRITICAL(cpu_sr);
    
    OSTCBRef curTcb=NULL;
#ifdef OS_EXTENDED_STATUS
    if(RES_SCHEDULER!=res)	 /*RES_SCHEDULERΪOS_NUM_OUT_RES*/
    {        /*����res��ΪRES_SCHEDULER�����RES_SCHEDULER*/
        OSRETERROR( E_OS_ID, OSServiceId_GetResource, res); 
    }
    if( (OUT_RES_USED == GET_RES_USED(res) )
       || ( !NOT_IN_ISR)
    )
    {               /*�����Դ�Ѿ���ռ�л����ж���ռ�и�*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_GetResource, res);    
    }
#endif  /*ifdef OS_EXTENDED_STATUS*/
    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;
    SET_OUT_PRIORITY(res,curTcb);/*���õõ��ⲿ��Դ������ȼ��任*/
#ifdef OS_EXTENDED_STATUS
    SET_RES_USED(res);
#endif /*ifdef OS_EXTENDED_STATUS*/
    ADD_RES_LIST(curTcb,res);/*�ѵ�ǰ�õ�����Դ�����б���׵�ַ*/
    OS_EXIT_CRITICAL();
    return( E_OK );    
    
}

/*
***************************************************************************
*StatusType ReleaseSchedulerResource( ResourceType res ) 
* �������ͷŵ�������Դ
* ���õĺ�������
* �������õ�λ�ã�TASK������ISR
* ������	res����������ԴID
* ���أ�	E_OK�����óɹ�
*		    E_OS_ID������res�Ƿ�
            E_OS_NOFUNC��res��ʾ����Դû��ʹ�û��Ǳ���Ҫ���ͷ�������Դ
            E_OS_ACCESS���ͷŵ���Դ���ȼ��ȵ�ǰ��������ȼ���
* ע������ú���ֻ�����ڵ�������Դ
* �����ˣ�����
* ʱ�䣺2005.3.7
* �޸ģ�������
* ʱ�䣺2005.12.13
***************************************************************************
*/
StatusType ReleaseSchedulerResource( ResourceType res )
{
    DEFINECRITICAL(cpu_sr);
    OSTCBRef ptcb=NULL;
    OSTCBRef curTcb=NULL;
#ifdef OS_EXTENDED_STATUS
    if ( RES_SCHEDULER!=res )	 /*RES_SCHEDULERΪOS_NUM_OUT_RES*/
    {        /*����res��ΪRES_SCHEDULER�����RES_SCHEDULER*/
        OSRETERROR( E_OS_ID, OSServiceId_ReleaseResource, res);
    }
    if ( (OUT_RES_NOT_USED==GET_RES_USED(res) )  )
    {            /*resID��ʾ����Դû��ʹ�û��Ǳ���Ҫ���ͷ�������Դ*/
        OSRETERROR( E_OS_NOFUNC, OSServiceId_ReleaseResource, res);
    }
    if( (!NOT_IN_ISR ) )
    {           /*�ͷŵ���Դ���ȼ��ȵ�ǰ��������ȼ���*/
        OSRETERROR( E_OS_ACCESS, OSServiceId_ReleaseResource, res);
    }
#endif  /*ifdef OS_EXTENDED_STATUS*/
    OS_ENTER_CRITICAL();
    curTcb=OSTCBCur;
    if( !IS_TASK_READY(&OSTCBTbl[GET_RES_PRIO(res)]) /*��Դ���ȼ�������û�о���*/
        && (GET_TASK_RUN_PRIO(curTcb)!=GET_RES_OLD_RUN_PRIO(res))/*�����䵱ǰ�������ȼ�����ԭ���Ļ�ȡ��Դǰ���������ȼ���һ��*/
             /*���һ����������ȡ����ͬ���ȼ��ģ��ڡ��⣩��Դһ������*/
    ) 
    {   /*�����Դ���ȼ��ڻ�ȡ��Դʱ���Ѿ��������ͽ���Ӿ������������*/
        OS_TASK_TO_UNREADY(GET_RES_PRIO(res),GET_RES_SAVED_TCB(res));
    }
    RESET_OUT_PRIORITY(curTcb,res);/*�������ȼ�*/
    SET_RES_UNUSED(res);
    DELETE_RES_LIST(curTcb,res);  

#ifndef OS_NON_SCHEDULE
    if(NOT_IN_ISR 
#ifdef   OS_MIX_SCHEDULE 
	 && FULL_SCHED==GET_TASK_SCHED(curTcb)  /*��ϵ����п���ռ����*/
#endif /*#ifdef   OS_MIX_SCHEDULE*/
    )  
    {    
        /*������ռ,���Ҳ����ж��ж��ҵ�����û������*/
		if( (ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()] )!=curTcb)
        {  /*�������ȼ���ռ*/
            OS_TASK_SWITCH(curTcb);
            OSPostTaskHook();/*�ڵ�ǰ�����л�ǰ������PostTaskHook�������ﻹ�ܵõ���ǰ���������ID*/
            OSTCBCur=ptcb;
            GET_INT_RES(ptcb);
            OSPreTaskHook();/*����PreTaskHook*/
            OSCheckStack(curTcb);   
            
#ifdef OS_EVENT_EN /*BCC1�²���Ҫ�жϣ���Ϊ�϶�������������*/       
            if(RUN_READY!=GET_TASK_STATUS(ptcb))
#endif/*#ifdef OS_EVENT_EN*/
            {  /*���������û���й�*/
                SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
                OSTaskSaveRun(curTcb,ptcb);        	
            }
#ifdef OS_EVENT_EN /*BCC1�²���Ҫ�жϣ���Ϊ�϶�������������*/       
            else
            {
                OSTaskSaveLoadRun(curTcb,ptcb);        	
            }
#endif/*#ifdef OS_EVENT_EN*/            
        }
	}
#endif/*OS_NULL_SCHEDULE ���������*/
    OS_EXIT_CRITICAL();	
    return( E_OK );    
}

#endif /*ifdef  OS_RES_SCHEDULER*/


#endif /*#ifndef OS_RES_C*/



