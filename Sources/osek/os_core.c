/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_core.c
* ժ    Ҫ������ϵͳ����ģ��ʵ�֣�����ϵͳ���������رա�
               ���ĵ��Ⱥ������Լ����ص�����
*
* ��ǰ�汾��1.1
* ��    �ߣ��������˺�ƽ
* ������ڣ�2005��1��20��
*
* ȡ���汾��1.0
* ԭ����  ������
* ������ڣ�2003��12��4��
*********************************************************************************
*/
#ifndef OS_CORE_C /*Ϊ�˲��ظ��������ļ�*/
#define OS_CORE_C


#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

AppModeType	ActiveAppMode=OSDEFAULTAPPMODE;/*��ǰ�����APPMODE*/

/*****************************************************************************
*        ϵͳ��ջ
*******************************************************************************/
#if (defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)) && defined(OS_EVENT_EN)
OSSTK   SystemSTK[OS_STK_SYSTEM_SIZE];

#if OS_STK_GROWTH == 1 
OSSTKRef   OSSTKSystem=&SystemSTK[OS_STK_SYSTEM_SIZE-2];
#else/*#if OS_STK_GROWTH == 1 */
OSSTKRef   OSSTKSystem=SystemSTK;
#endif/*#if OS_STK_GROWTH == 1 */

#endif/*#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)*/


#ifdef OS_HOOK_ERROR
INT8U  OSNestedErrorHook=0;
INT8U  OSNestedHook=0;
#ifdef OS_HOOK_GET_SERVICEID
OSSERVICEIDTYPE  ServiceId=OSServiceId_NoService;
#endif /*ifdef OS_HOOK_GET_SERVICEID*/

#ifdef OS_HOOK_PARAM_ACCESS
ObjectIDType  OSObjId= 0; 
#endif /*ifdef OS_HOOK_PARAM_ACCESS*/

#endif /*#ifdef OS_HOOK_ERROR*/ 

#ifdef OS_STACK_SEPARATE
#ifndef  OS_TASK_IDLE_STK_SIZE
#define  OS_TASK_IDLE_STK_SIZE  200
#endif/*#ifndef  OS_TASK_IDLE_STK_SIZE*/
OSSTK OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE];
#endif/*#ifdef OS_EVENT_EN*/

#ifdef OS_STACK_INIT
void  OSStartHighRdy();
#define OSStartFirstTask(curTcb) OSTaskHighRun()
#else /* #ifdef OS_STACK_INIT*/
#define OSStartFirstTask(curTcb) OSTaskRun(curTcb) /* ����������ȼ������� */
#endif/* #ifdef OS_STACK_INIT*/
/*�����������֧���ҵ�����ߵ�1������,�����ò��
osmaptblʵ������һ��������ֱ������8��λ��ĳһλΪ1��ֵ*/

#ifndef OS_CPU_MPC555
INT8U  const  OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/*һ��0��255֮��������ҵ�������ߵ�1����һ��λ��
���磬17�����Ʊ�ʾΪ0b10001,����ߵ�1�ڵ�5λ�ϣ�������Ѹ�ٵõ�*/
INT8U  const  OSUnMapTbl[] = {
    0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};
#endif/*#ifndef OS_CPU_MPC555*/
/*
***************************************************************************
*void  OSTaskIdle (void)
* ������idle������
* ���õĺ�������
* �������õ�λ�ã�OSInit
* ������	��
* ���أ�	��
* ע�����
* �����ˣ�����
* ʱ�䣺2003.12.23
* �޸��ˣ�����
* ʱ�䣺2005.1.17    
***************************************************************************
*/
void  OSTaskIdle (void)
{
    for(;;) 
    {
        ;  
    }    
}
/*
***************************************************************************
*void  OSInit (void)
* ����������ϵͳ�ĳ�ʼ��
* ���õĺ�������ģ��ĳ�ʼ������(countInit/ResourceInit/interrupt_init)
* �������õ�λ�ã�main����
* ������	��
* ���أ�	��
* ע�����
* �����ˣ�����
* ʱ�䣺2003.12.23
* �޸��ˣ�����
* ʱ�䣺2005.1.23    
***************************************************************************
*/
#ifndef OS_TASK_AUTO_DEFINE /*�û��Լ�����DefineTask��API������ʵ���ʱ����Ҫ�õ�*/
void  OSInit(void)
{
    INT16U     i;
#ifndef OS_CPU_MPC555   
    INT8U * prdytbl;
#endif

#ifndef OS_CPU_MPC555
    prdytbl = &OSRdyTbl[0];
    for (i = 0; i < OS_RDY_TBL_SIZE; i++) 
    {
        *prdytbl++ = 0x00;
    }  
#endif/* #ifdef OS_CPU_MPC555 */
    
    /*������������*/
    SET_TASK_PROPERTY(&OSTCBTbl[0],FULL_SCHED|AUTO_START);
    SET_TASK_STATUS(&OSTCBTbl[0],READY);
    SET_TASK_ENTRY(&OSTCBTbl[0],OSTaskIdle);
#if OS_STK_GROWTH == 1
    SET_TASK_STK(&OSTCBTbl[0],&OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1],OSTaskIdleStk);
#else /*#if OS_STK_GROWTH == 1*/
    SET_TASK_STK(&OSTCBTbl[0],OSTaskIdleStk,&OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1]);
#endif/*if OS_STK_GROWTH == 1*/
    OSTCBPrioTbl[0] = (OSTCBRef)&OSTCBTbl[0];
    /*��ʼ��������ص�ȫ�����ݽṹ*/
    for (i = 1; i < (OS_NUM_SYS_TASK) ; i++) 
    {     /* ��ʼ��TCB���ȼ���*/
        OSTCBPrioTbl[i] = (OSTCBRef)0;
        SET_TASK_STK(&OSTCBTbl[i],NULL,NULL);
    }

#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
    ResourceInit();
#endif

}
#endif/*#ifndef OS_TASK_AUTO_DEFINE*/


/*
***************************************************************************
void  StartOS (AppModeType mode)
* ��������������ϵͳ
* ���õĺ�����OSStartupHook()
* �������õ�λ�ã�main()
* ������	mode������ϵͳ���е�ģʽ
* ���أ�	��
* ע�����
* �����ˣ�����
* ʱ�䣺2003.12.23
* �޸��ˣ�����
* ʱ�䣺2005.1.23    
***************************************************************************
*/

void  StartOS(AppModeType mode)
{
    OSTCBRef curTcb=NULL;
    if(OS_NUM_SYS_TASK > OS_MAX_TASKS)/*�ж���������Ƿ񳬹�Ҫ��*/
        return ;
#ifdef OS_TASK_AUTO_DEFINE    
    OSTaskInit();
#endif
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
    ResourceInit();
#endif/*#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/    
#ifdef OS_ALARM_EN
    OSAlarmInit();
#endif/*#ifdef OS_ALARM_EN*/
#ifdef OS_EXCEPTION_EN
    OSISRInit();
#endif/*#ifdef OS_EXCEPTION_EN*/    
    OSStartupHook();
	  ActiveAppMode=mode;
    OSTCBCur=curTcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()];/* �ҵ�������ȼ�������*/
    GET_INT_RES(curTcb);   /*��ȡ�ڲ���Դ*/
    OSPreTaskHook();
    SET_TASK_STATUS(curTcb,RUN_READY);
    OSStartFirstTask(curTcb);
}



/*
***************************************************************************
*void  OSIntExit (void)
* �����������ж��˳�ʱ�Ĵ�����
* ���õĺ�����OSIntCtxSw��
* �������õ�λ�ã�DECHdlr��ExceptionRoutine
* ������	��
* ���أ�	��
* ע������ж���������������йص�apiΪActivateTask,SetEvent��GetResource��ReleaseResource
* �����ˣ�����
* ʱ�䣺2003.12.23
* �޸��ˣ��˺�ƽ
* ʱ�䣺2005.2.23   
* �޸��ˣ�����
* ʱ�䣺2005.6.9
* �޸ģ�������
* ʱ�䣺2005.12.13 
* ��ע��ע��ú�������ʱ�Ѿ������������ⲿ�ж�
***************************************************************************
*/

#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)
void  OSIntExit (void)
{
    OSTCBRef ptcb=NULL;
    OSTCBRef oldRunning=NULL;
    if (--OSIntNesting==0){
#ifndef OS_FULL_SCHEDULE  /*����ȫ��ռ����Ҫô�ǻ����ռ��Ҫô����ռ*/  
#ifdef OS_MIX_SCHEDULE  /*�����ռ*/
   	    if(FULL_SCHED!=GET_TASK_SCHED(OSTCBCur))/*������ռ����*/
   	    {
   		    return;/*�˳��ж�*/
   	    }
#else /*������ռ���ȵ����*/
   	    if(0!=GET_TASK_ID(OSTCBCur))/*��IDLE���񣬾Ͳ����е��ȣ����ΪIDLE�������о�Ҫ����*/
   	    {
   	        return; /*�˳��ж�*/    	
   	    }   	    
#endif /*#ifdef OS_MIX_SCHEDULE*/
#endif /*#ifndef OS_FULL_SCHEDULE*/   		 	
    	/*������ռ*/
        oldRunning=OSTCBCur;
        if((ptcb=OSTCBPrioTbl[OS_FIND_HIGHEST_TASK()] )!=oldRunning)
        {  /*�������ȼ���ռ*/
            OS_TASK_SWITCH(oldRunning);
            OSPostTaskHook();/*�ڵ�ǰ�����л�ǰ������PostTaskHook�������ﻹ�ܵõ���ǰ���������ID*/
            OSTCBCur=ptcb;
            GET_INT_RES(ptcb);
            OSPreTaskHook();/*����PreTaskHook*/
            OSCheckStack(oldRunning);   
#ifdef  OS_EVENT_EN  /*��ECC1�������*/                           
            if(RUN_READY!=GET_TASK_STATUS(ptcb))
#endif/*#ifdef  OS_EVENT_EN*/            
            {  /*���������û���й�*/
                SET_TASK_STATUS(ptcb,RUN_READY);/*����״̬*/
                OSTaskRun(ptcb);        	
            }
#ifdef  OS_EVENT_EN  /*��ECC1�������*/            
            else
            {
                OSTaskLoadRun(ptcb);        	
            }   
#endif/*#ifdef  OS_EVENT_EN*/             
        }
    }
}
#endif /* if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN) */


/*
***************************************************************************
*AppModeType GetActiveApplicationMode ( void )
* �������õ���ǰ���е�APPMODE
* ���õĺ�������
* �������õ�λ�ã�����һ���жϺ�ALARM�е�CALLBACK����������еط������Ե���
* ������	��
* ���أ�	��ǰ���е�APPMODE
* ע�����
* �����ˣ�����
* ʱ�䣺2003.12.23
* �޸��ˣ�����
* ʱ�䣺2005.1.17    
***************************************************************************
*/
AppModeType GetActiveApplicationMode ( void )
{
	return ActiveAppMode;
}

/*
***************************************************************************
*void ShutdownOS ( StatusType Error )
* �������ر�OS
* ���õĺ�������
* �������õ�λ�ã�TASK������ISR��ErrorHook��StartupHook���ڲ�����
* ������Error��������Ϣ
* ���أ���
* ע���������Error����ΪE_OK
* �����ˣ�������
* ʱ�䣺2004.1.23
* �޸��ˣ�����
* ʱ�䣺2005.1.17    
***************************************************************************
*/
void ShutdownOS ( StatusType Error )
{
    DEFINECRITICAL(cpu_sr); 
    Error=Error;/*����Ҫ�ж�Error�Ƿ�ΪE_OK�������û���Ҫע��ģ�����ʵ�ֵ�Ҫ��*/
#ifdef OS_HOOK_SHUTDOWN
    OSShutdownHook(Error); 
#endif /*ifdef OS_HOOK_SHUTDOWN*/
    OS_ENTER_CRITICAL(); 
	while(1)
	  ;
	OS_EXIT_CRITICAL();
}

#ifndef OS_EXCEPTION_EN
/*
***************************************************************************
* void UserException() 
* �������û��ж�����ڡ��ú���ֻ����û���ж�ģ���ʱ��������ң���ֹ�������
* ���õĺ�������
* �������õ�λ�ã���
* ��������
* ���أ���
* ע�����
* �����ˣ���ˬ
* ʱ�䣺2004.1.23
* �޸��ˣ�����
* ʱ�䣺2005.1.17    
***************************************************************************
*/

void UserException()
{

}

/*
***************************************************************************
*void SystemException()
* ������ϵͳ�ж�����ڡ��ú���ֻ����û���ж�ģ���ʱ��������ң���ֹ�������
* ���õĺ�������
* �������õ�λ�ã���
* ��������
* ���أ���
* ע�����
* �����ˣ���ˬ
* ʱ�䣺2004.1.23
* �޸��ˣ�����
* ʱ�䣺2005.1.17    
***************************************************************************
*/

void SystemException()
{

}


#endif /*#ifndef OS_EXCEPTION_EN*/


#endif /*#ifndef OS_CORE_C*/
