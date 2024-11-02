/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_task_cpu.h
* ժ    Ҫ��taskģ������ƽ̨��صĺ궨���Լ���ز���,�����ƽ̨��HCS12
*
* ��ǰ�汾��2.0
* ��    �ߣ�������
* ������ڣ�2005��12��12��
*
*********************************************************************************
*/

#ifndef OS_TASK_CPU_H
#define OS_TASK_CPU_H

/*��һ����ջ���ڽ��chaitask����*/
#define ExtraStackSize 100
//extern OSSTK ExtraStack[];

/********************����ƽ̨************/
extern  INT8U   OSRdyGrp;                        /*����������������*/
extern  INT8U   OSRdyTbl[];       /*��������������*/





OSSTKRef OSTaskStkInit(OSTaskEntry task, OSSTKRef ptos);   /*ջ��ʼ��*/
        
/*
*********************************************************************************************************
* ȫ�ֱ���������
*********************************************************************************************************
*/
extern  OSTCBRef           OSTCBOld;				/*ָ��ո��л�������*/

/*******************************ƽ̨��صĶ��ⶨ��***********************/

extern OSSTK ExtraStack[];
extern OSTCBRef           OSTCBOld;/*ָ��ո��л�������*/
extern OSSTKRef           FreeStkTop;
extern OSSTKRef           FreeStkBottom;

/***************************���������*******************************/
extern INT8U  OSRdyGrp;/* ������������ */
extern INT8U  OSRdyTbl[];/* �������� */   
    
#endif /*#ifndef OS_TASK_CPU_H*/