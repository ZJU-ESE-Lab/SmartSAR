/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_task.h
* ժ    Ҫ��ʵ������ģ������ݽṹ�Լ��ض������������ͺ�����Ķ���
*
* ��ǰ�汾��1.2
* ��    �ߣ�����
* ������ڣ�2005��5��22��
*
* ��ǰ�汾��1.1
* ��    �ߣ��˺�ƽ������
* ������ڣ�2005��3��1��
*
* ȡ���汾��1.0
* ԭ����  ������
* ������ڣ�2004��12��4��
*********************************************************************************
*/


#ifndef  OS_TASK_H
#define	 OS_TASK_H

/*
*********************************************************************************************************
*                              �궨��
*********************************************************************************************************
*/
#define OS_NUM_SYS_TASK ((OS_NUM_TASK)+1)
#define  OS_RDY_TBL_SIZE   (((OS_NUM_SYS_TASK)) / 8 + 1)   /*ready table �Ĵ�С�ɿ��� */
/*���64������*/
#define OS_MAX_TASKS 64


/*�����״̬��Ǻ�*/
#define  READY            0x00        
#define  WAITING          0x02        
#define  SUSPENDED        0x04        
#define  RUNNING          0x01        

#define  RUN_READY        (INT8U)(0x10)   /*�������е�Ready״̬���ô�״̬�ж��Ƿ��ǵ�һ������*/

#define  INVALID_TASK     (NULL)          /* ��Ч����*/

/*�Ƿ����ռ�ĺ궨��*/
#define	Preemptive	1
#define	NoPreemptive 0


/*����ģʽ�ĺ궨��*/
#define FULL_SCHED 0x10
/*��ʾ�������궨��*/
#define AUTO_START 0x20
/*��ʾռ���ڲ���Դ�궨��*/
#define HOLD_INT_RES 0x40
/*��ʾ��չ����궨��*/
#define EXTENDED 0x80

/***********************************************************************
                             OSEK��׼��������
***********************************************************************/
typedef struct OSTCB   *    TaskType;
typedef TaskType    *TaskRefType;
typedef INT8U       TaskStateType;
typedef TaskStateType       *TaskStateRefType;
/***********************************************************************
                             SmartOSEK�ڲ���������
***********************************************************************/
/*���ȼ�����*/
typedef INT8U PriorityType;
/*���ȼ�����*/
typedef INT8S ResPrioType;
/*tcbָ��*/
typedef struct OSTCB* OSTCBRef;

/*���������*/
typedef void (*OSTaskEntry)(void) ;


/********************************************
�ýṹ�������û�������������
�û����õ�������������Ӧ�ð����ȼ���С�������
********************************************/
typedef struct TaskCfg
{
    OSTaskEntry PTask;/*�������ָ��*/
#ifdef OS_STACK_SEPARATE	/*��BCC1������£�������Ҫ�����Ķ�ջ*/ 
    OSSTKRef    PStkHigh;/*��ջ��ָ��*/ 
    OSSTKRef    PStkLow;   /* ��ջ��ָ��*/
#endif/*#ifdef OS_EVENT_EN*/     
    PriorityType  ID; /*�ڲ���Դ���ȼ�*/
	  PriorityType  RunPrio; /*�ڲ���Դ���ȼ�*/
	  INT8U	InfoBit;	  /*��¼���������Լ�������Ϣ����һλ��ʾ������չ��
                                                                   �ڶ�λ��ʾ������չ��
                                                                   ����λ��ʾ������չ��
                                                                   ����λ��ʾ�Ƿ���ȫ����������
                                                                   ����λ��ʾ�Ƿ����ռ�����ȷ�ʽ��
                                                                   ����λ��ʾ�Ƿ���������                                                                   
                                                                   ����λ��ʾ�Ƿ�ռ���ڲ���Դ��
                                                                   �ڰ�λ��ʾ�Ƿ���չ����*/
}OSTaskCfg;

/*******************************************
* 		��Դģ��Ĳ��ֽṹ�嶨��
*
*�������ݽṹ��Ƶ�ԭ��tcb����Դ���ƽṹ��
*��Ա�����������á����������typedef
*
********************************************/
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*�ⲿ���������Դ*/
typedef struct OSResCB OSRCB;
typedef struct OSResCB* OSRCBRef;


#endif /*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/


/******************************************************************************
 	                       ������ƿ�Ľṹ�嶨��
*******************************************************************************/
typedef struct  OSTCB
{
    OSSTKRef     OSTCBStkCur;         /* Ŀǰ��ջָ�룬��ջ��ָ��  */
    OSTaskEntry  PTask;               /* �������ָ��*/
    OSSTKRef     OSTCBStkTop;         /* ջ��ָ�룬��¼����ջ�Ķ�ָ�룬���ڳ�ʼ��ջ��*/

#ifdef OS_EVENT_EN
    EventMaskType   WEvent;             /* waited events������	    */
    EventMaskType   SEvent;             /* setted events������ 		*/
#endif /*#ifdef OS_EVENT_EN*/ 

#if defined(OS_CHECK_STACK) && defined(OS_STACK_SEPARATE) /*��ջ�ֿ���������²���ջ�ļ��*/  
    OSSTKRef     OSTCBStkBottom;        /* ��ջ��ָ��				    */
#endif /*#if defined(OS_CHECK_STACK) && defined(OS_EVENT_EN)*/    
 	
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*�ⲿ���������Դ*/
    OSRCBRef		resources;          /* �ⲿ��Դ����ͷָ��            */
#endif /*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/
 	
#ifdef OS_INT_RES
    OSTCBRef        SavedTCB;           /* ָ��ԭ���ڸ�λ�õ�tcbָ��     */
#endif
#if defined(OS_INT_RES)	||defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*���õ��ڲ���Դ���ⲿ��Դ��ʱ������һ���������ȼ�*/
	PriorityType    RunPrio;         /* �������ȼ�				*/
#endif/*#if defined(OS_INT_RES)	*//*||defined(OS_OUT_RES)*/	

    PriorityType       OSTCBId;            /* Task ID                       */
    INT8U          OSTCBStat;          /* ����״̬                      */

#ifndef OS_CPU_MPC555
    INT8U          OSTCBBitX;          /* ���ڷ��ʾ����������*/
    INT8U          OSTCBBitY;          /* ���ڷ��ʾ�������������� */
#endif/*#ifndef OS_CPU_MPC555*/

	INT8U			InfoBit;           /*��¼���������Լ�������Ϣ����һλ��ʾ������չ��
                                                                   �ڶ�λ��ʾ������չ��
                                                                   ����λ��ʾ������չ��
                                                                   ����λ��ʾ�Ƿ���ȫ����������
                                                                   ����λ��ʾ�Ƿ����ռ�����ȷ�ʽ��
                                                                   ����λ��ʾ�Ƿ���������                                                                   
                                                                   ����λ��ʾ�Ƿ�ռ���ڲ���Դ��
                                                                   �ڰ�λ��ʾ�Ƿ���չ����*/
		
} OSTCB;





 
/*�õ���������ͣ���չ���׼*/
#define GET_TASK_TYPE(PTCB) ((PTCB)->InfoBit & EXTENDED)

/*�õ�������ȷ�ʽ*/
#define GET_TASK_SCHED(PTCB)  ((PTCB)->InfoBit & FULL_SCHED)

/*�õ������Ƿ�ռ���ڲ���Դ*/
#define GET_TASK_HOLD_INT_RES(PTCB) ((PTCB)->InfoBit & HOLD_INT_RES)
/*������������*/
#define SET_TASK_PROPERTY(PTCB,PROPERTY) (PTCB)->InfoBit=(PROPERTY)
/*�������������*/
#define SET_TASK_ENTRY(PTCB,ENTRY) (PTCB)->PTask=(ENTRY)  
/*�õ�����״̬*/
#define GET_TASK_STATUS(PTCB)  ((PTCB)->OSTCBStat)
/*��������ID*/
#define SET_TASK_ID(PTCB,ID) (PTCB)->OSTCBId=(ID)
/*��������״̬*/
#define SET_TASK_STATUS(PTCB,STATUS) (PTCB)->OSTCBStat = (STATUS) 

/*�õ����������ȼ�������ID��*/
#define GET_TASK_ID(PTCB)  ((PTCB)->OSTCBId)


/*�ж������Ƿ����״̬��1��ʾ������0��ʾ�Ǿ���*/
#define IS_TASK_READY(PTCB) ( READY==(GET_TASK_STATUS(PTCB)&~RUN_READY) )

/******************************************************************
             �õ������������ȼ�
*********************************************************************/

/******************************************************************
             TCB�����ڲ���Դ��صĲ���
*********************************************************************/
#ifdef OS_INT_RES

/*�õ�������ڲ���Դ���ȼ�*/
#define GET_TASK_INT_RES(PTCB)  ((PTCB)->RunPrio)

/*�õ�����ı����TCBָ��*/
#define GET_TASK_SAVED_TCB(PTCB)  ((PTCB)->SavedTCB)

/*�ڲ���Դ���ȼ�������*/
#define SET_TASK_INT_RES(PTcb,ResPrio) ((PTcb)->RunPrio)=ResPrio

/*���ñ����TCBָ��*/	
#define RESET_TASK_SAVED_TCB(PTCB) \
	OSTCBPrioTbl[GET_TASK_INT_RES(PTCB)]=GET_TASK_SAVED_TCB(PTCB)

#else /*#ifdef OS_INT_RES*/
#define RESET_TASK_SAVED_TCB(PTCB)	
#define SET_TASK_INT_RES(PTcb,ResPrio) 
#endif /*#define OS_INT_RES*/	
/******************************************************************
             TCB�����ⲿ��Դ��صĲ���
*********************************************************************/
#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
/*�õ�����ռ�е��ⲿ��Դ�����ͷָ��*/
#define GET_TASK_OUT_RES(PTCB) ((PTCB)->resources)
/*��������ռ�е��ⲿ��Դ�����ͷָ��*/
#define SET_TASK_OUT_RES(PTCB,Res) (PTCB)->resources=Res

#endif /*#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/

/**********************************************************************
             TCB�����ڲ����ⲿ��Դ��صĲ���
*************************************************************************/
#if defined(OS_INT_RES) ||defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)
#define GET_TASK_RUN_PRIO(PTCB) ((PTCB)->RunPrio)
#define SET_TASK_RUN_PRIO(PTCB,PRIO) ((PTCB)->RunPrio)=((PriorityType)(PRIO) )
#else/*#if defined(OS_INT_RES) ||defined(OS_OUT_RES)*/
#define GET_TASK_RUN_PRIO(PTCB) GET_TASK_ID(PTCB)
#define SET_TASK_RUN_PRIO(PTCB,PRIO) 
#endif/*#if defined(OS_INT_RES) ||defined(OS_OUT_RES)*/
/******************************************************************
             TCB����EVENT��صĲ���
*********************************************************************/

#ifdef OS_EVENT_EN
/*���TCB�е����ù��������¼�*/
#define CLEAR_TASK_EVENT(PTCB)  (PTCB)->SEvent=(NOSETEVENT)

/*����TCB�еĴ������¼�*/
#define SET_TASK_SET_EVENT(PTCB,mask)  (PTCB)->SEvent|=(EventMaskType)(mask)

/*����TCB�еĵȴ����¼�*/
#define SET_TASK_WAIT_EVENT(PTCB,mask)  (PTCB)->WEvent|=(EventMaskType)(mask)


/*�õ�TCB�еĴ������¼�*/
#define GET_TASK_SET_EVENT(PTCB)  ((PTCB)->SEvent)

/*�õ�TCB�еĵȴ����¼�*/
#define GET_TASK_WAIT_EVENT(PTCB)  ((PTCB)->WEvent)

/*���TCB�е����ù���MASKֵΪmask���¼�*/
#define CLERA_TASK_MASK_EVENT(PTCB,mask)  (PTCB)->SEvent&=(EventMaskType)(~Mask)


#else/*#ifdef OS_EVENT_EN*/

#define CLEAR_TASK_EVENT(PTCB)

#define SET_TASK_EVENT(PTCB,SEvent)

#endif /*#ifdef OS_EVENT_EN*/

/************************************************************
* �����������õ���صĺ궨��
* SET_TASK_READY(TaskId,PTCB) 
* SET_TASK_UNREADY(TaskId,PTCB,Status)
* ��������Ϊ�˱�֤����ֲ��
****************************************************/
#ifdef OS_STACK_INIT
/*�ı�����״̬��������������������*//*���ﻹ��Ҫʵ��ջ�ĳ�ʼ��*/
#define SET_TASK_READY(TaskId,PTCB) SET_TASK_STATUS(PTCB,READY);OS_TASK_TO_READY(TaskId,PTCB);SET_TASK_STK_INIT(PTCB)
#else /*#ifdef OS_STACK_INIT*/
/*�ı�����״̬��������������������*/
#define SET_TASK_READY(TaskId,PTCB) SET_TASK_STATUS(PTCB,READY);OS_TASK_TO_READY(TaskId,PTCB)
#endif/*#ifdef OS_STACK_INIT*/
/*�ı�����״̬(Suspend/Waiting)����������Ӿ���������ȥ��*/
#define SET_TASK_UNREADY(TaskId,PTCB,Status) SET_TASK_STATUS(PTCB,Status);OS_TASK_TO_UNREADY(TaskId,PTCB)





/****************************************************************
����ջ
****************************************************************/
#if defined(OS_CHECK_STACK)&&defined(OS_STACK_SEPARATE)&&defined(OS_CPU_MPC555)

INT8U OSStackOverFlow(OSSTKRef StkBottom);
#define OSCheckStack(PTCB)   \
        do\
        {\
            if(OSStackOverFlow(GET_TASK_STK_BOTTOM(PTCB)))\
            {\
        	    OSEKPrintInt(GET_TASK_ID(PTCB));\
        	    RETURN_OTHER_ERROR(E_OS_STACK);\
            }\
        }while(0)
#else/*#if defined(OS_CHECK_STACK)&&defined(OS_EVENT_EN)*/
#define OSCheckStack(PTCB) 
#endif/*#ifdef  OS_CHECK_STACK*/     


/*
*********************************************************************************************************
* ȫ�ֱ���������
*********************************************************************************************************
*/
extern  OSTCBRef           OSTCBCur;                  /* ��ǰ����tcbָ��          */
extern  OSTCBRef           OSTCBPrioTbl[(OS_NUM_SYS_TASK)];/* ���ȼ�������tcbָ������  */
extern  OSTCB              OSTCBTbl[(OS_NUM_SYS_TASK) ];   /* TCB����                  */

#ifndef OS_STACK_INIT 

/*********************************************************************************************************
�������л��ĺ������������嶨����OS_CPU_A.ASM��
����OSEK��׼�����������л���Ϊ4�������
1.Saveԭ������Load������ �ʺ���Schedule��WaitEvent��SetEvent��ReleaseResource
2.Saveԭ�����񣬲�Load�������ʺ���ActivateTask��Schedule��WaitEvent��ReleaseResource
3.��Saveԭ������Load�������ʺ���TerminateTask��ChainTask�������ж��з��ر���ռ
4.ֱ�����У�����ҪSave��Load���ʺ���TerminateTask��ChainTask�������ж��з��ر���ռ
**************************************************************************************************/

/*���1.Saveԭ������Load������*/
void OSTaskSaveLoadRun(OSTCBRef PSavedTcb,OSTCBRef PLoadTcb);
/*���2.Saveԭ�����񣬲�Load������*/
void OSTaskSaveRun(OSTCBRef PSavedTcb,OSTCBRef PLoadTcb);
/*���3.��Saveԭ������Load������*/
void OSTaskLoadRun(OSTCBRef PRunTcb);
/*���4.ֱ�����У�����ҪSave��Load*/
void OSTaskRun(OSTCBRef PRunTcb);
#endif/*#ifndef OS_STACK_INIT*/

/*TASK�궨�壬��������һ������*/
#define TASK(name) void Func##name(void)
/*definetask�ĺ꣬��һ���������иı�*/
#define DefineTask(p1,p2,p3,p4,p5,p6) FuncDefineTask(Func##p1,p2,p3,p4,p5,p6)

#ifdef OS_ALARM_EN
void OSActivateTask(TaskType TaskId);
#endif

/*
*********************************************************************************************************
* ������ģ����õĺ���
*********************************************************************************************************
*/
void OSTaskInit(void);

/*
*********************************************************************************************************
* ���ⲿ���õķǱ�׼api
*********************************************************************************************************
*/
INT8U  FuncDefineTask (void (*PTask)(void ),OSSTKRef PStkHigh,OSSTKRef PStkLow,INT8U Priority,INT8U Property,PriorityType ResPrio);

/*
*********************************************************************************************************
* ��׼��osek api
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
