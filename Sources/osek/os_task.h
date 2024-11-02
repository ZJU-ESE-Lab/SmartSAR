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

/*�����״̬��Ǻ�*/
#define  READY            0x00        
#define  WAITING          0x02        
#define  SUSPENDED        0x04        
#define  RUNNING          0x01        

#define  RUN_READY        (INT8U)(0x10)   /*�������е�Ready״̬���ô�״̬�ж��Ƿ��ǵ�һ������*/

#define  INVALID_TASK     (NULL)          /* ��Ч����*/

#define  INVALID_RESOURCE     (NULL)          /* ��Ч����*/
#define  INVALID_ALARM     (NULL)          /* ��Ч����*/

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
    OSSTKRef    PStkHigh;/*��ջ��ָ��*/ 
    OSSTKRef    PStkLow;   /* ��ջ��ָ��*/
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
//#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*�ⲿ���������Դ*/
typedef struct OSResCB OSRCB;
typedef struct OSResCB* OSRCBRef;


//#endif /*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/


/******************************************************************************
 	                       ������ƿ�Ľṹ�嶨��
*******************************************************************************/
typedef struct  OSTCB
{
    OSSTKRef     OSTCBStkCur;         /* Ŀǰ��ջָ�룬��ջ��ָ��  */
    OSTaskEntry  PTask;               /* �������ָ��*/
    OSSTKRef     OSTCBStkTop;         /* ջ��ָ�룬��¼����ջ�Ķ�ָ�룬���ڳ�ʼ��ջ��*/

    EventMaskType   WEvent;             /* waited events������	    */
    EventMaskType   SEvent;             /* setted events������ 		*/

    OSSTKRef     OSTCBStkBottom;        /* ��ջ��ָ��				    */
 	
    OSRCBRef		resources;          /* �ⲿ��Դ����ͷָ��            */
 	
    OSTCBRef        SavedTCB;           /* ָ��ԭ���ڸ�λ�õ�tcbָ��     */
	PriorityType    RunPrio;         /* �������ȼ�				*/

    PriorityType       OSTCBId;            /* Task ID                       */
    INT8U          OSTCBStat;          /* ����״̬                      */

    INT8U          OSTCBBitX;          /* ���ڷ��ʾ����������*/
    INT8U          OSTCBBitY;          /* ���ڷ��ʾ�������������� */

	INT8U			InfoBit;           /*��¼���������Լ�������Ϣ����һλ��ʾ������չ��
                                                                   �ڶ�λ��ʾ������չ��
                                                                   ����λ��ʾ������չ��
                                                                   ����λ��ʾ�Ƿ���ȫ����������
                                                                   ����λ��ʾ�Ƿ����ռ�����ȷ�ʽ��
                                                                   ����λ��ʾ�Ƿ���������                                                                   
                                                                   ����λ��ʾ�Ƿ�ռ���ڲ���Դ��
                                                                   �ڰ�λ��ʾ�Ƿ���չ����*/
		
} OSTCB;

/*
*********************************************************************************************************
* ȫ�ֱ���������
*********************************************************************************************************
*/
extern  OSTCBRef           OSTCBCur;                  /* ��ǰ����tcbָ��          */
extern  OSTCBRef           OSTCBPrioTbl[];/* ���ȼ�������tcbָ������  */
extern  OSTCB              OSTCBTbl[];   /* TCB����                  */

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

//#ifdef OS_ALARM_EN
void OSActivateTask(TaskType TaskId);
//#endif

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
