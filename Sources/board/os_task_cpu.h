/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_task_cpu.h
* ժ    Ҫ��taskģ������ƽ̨��صĺ궨���Լ���ز���,�����ƽ̨��MPC555
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
extern OSSTK ExtraStack[];

/********************����ƽ̨************/
extern  INT8U   OSRdyGrp;                        /*����������������*/
extern  INT8U   OSRdyTbl[OS_RDY_TBL_SIZE];       /*��������������*/


/*�������ھ������в���������*/
#define SET_TASK_MASK(ptcb,prio) (ptcb)->OSTCBBitY= OSMapTbl[(prio) >> 3]; (ptcb)->OSTCBBitX=OSMapTbl[(prio) & 0x07]      
#define OS_TASK_TO_READY(TaskId,PTCB)  OSRdyGrp|= PTCB->OSTCBBitY; OSRdyTbl[TaskId >>3] |= PTCB->OSTCBBitX
    
#define OS_TASK_TO_UNREADY(TaskId,PTCB) if (( OSRdyTbl[TaskId >>3]&=~PTCB->OSTCBBitX) == 0x00) OSRdyGrp &= ~PTCB->OSTCBBitY

#define	OS_FIND_HIGHEST_TASK() (INT8U)((OSUnMapTbl[OSRdyGrp] << 3)+OSUnMapTbl[OSRdyTbl[OSUnMapTbl[OSRdyGrp]]])



#ifdef OS_STACK_INIT
OSSTKRef OSTaskStkInit(OSTaskEntry task, OSSTKRef ptos);   /*ջ��ʼ��*/
        
#define SET_TASK_STK_INIT(PTCB)	 (PTCB)->OSTCBStkCur=OSTaskStkInit((PTCB)->PTask,(PTCB)->OSTCBStkTop)
#else /*#ifdef OS_STACK_INIT*/
#define SET_TASK_STK_INIT(PTCB)
#endif/*#ifdef OS_STACK_INIT*/


/*���������ջ*/
#if defined(OS_CHECK_STACK) && defined(OS_STACK_SEPARATE)

#ifdef OS_STACK_INIT
#define SET_TASK_STK(PTCB,SP,SB)  (PTCB)->OSTCBStkTop=SP;(PTCB)->OSTCBStkBottom=(SB)
#else /*#ifdef OS_STACK_INIT*/
#define SET_TASK_STK(PTCB,SP,SB)  (PTCB)->OSTCBStkCur=SP;(PTCB)->OSTCBStkBottom=(SB)
#endif/*#ifdef OS_STACK_INIT*/

#define GET_TASK_STK_BOTTOM(PTCB) ((PTCB)->OSTCBStkBottom)

#else /*#if defined(OS_CHECK_STACK) && defined(OS_STACK_SEPARATE) */

#ifdef OS_STACK_INIT
#define SET_TASK_STK(PTCB,SP,SB)  (PTCB)->OSTCBStkTop=SP
#else/*#ifdef OS_STACK_INIT*/
#define SET_TASK_STK(PTCB,SP,SB)  (PTCB)->OSTCBStkCur=SP
#endif/*#ifdef OS_STACK_INIT*/

#endif /*#if defined(OS_CHECK_STACK) && defined(OS_STACK_SEPARATE)*/


#ifdef OS_STACK_INIT
/*��ȡ����ջ��*/
#define GET_TASK_STK_TOP(PTCB)	 (PTCB)->OSTCBStkTop
/*���������*/
#define SET_TASK_OLD(PTCB)   OSTCBOld=(PTCB)
#else
#define SET_TASK_OLD(PTCB)   
#endif/*#ifdef OS_STACK_INIT*/

/********************************************************
* �����л�ʱ��ƽ̨��صĴ���
************************************************************/

#define OS_TASK_SWITCH(OldTcb) SET_TASK_OLD(OldTcb)
        
       

/*
*********************************************************************************************************
* ȫ�ֱ���������
*********************************************************************************************************
*/
#ifdef OS_STACK_INIT /*����������л���Ҫջ��ʼ���ȹ���*/
extern  OSTCBRef           OSTCBOld;				/*ָ��ո��л�������*/
#endif/*#ifdef OS_STACK_INIT*/

#ifdef OS_STACK_INIT /*��������л���ͨ����ջ�ͳ�ջʵ�ֵģ��Ǿ�ͨ�����ж���ʵ��*/
#define OSTaskRun(PRunTcb) OSTaskHighRun()
#define OSTaskSaveRun(PSavedTcb,PRunTcb) OS_TASK_SW() 
#define OSTaskLoadRun(PRunTcb) OSIntCtxSw() 
#define OSTaskSaveLoadRun(PSavedTcb,PRunTcb) OS_TASK_SW() 
#endif/*#ifdef OS_STACK_INIT*/




/***************************************************************************
* void  OSTaskInit() �е�ƽ̨��ز���
***************************************************************************/

/*****************************�������ʼ��*******************************/
#define INITRDYTBL()  \
INT8U * prdytbl; \
INT8U     i; \
prdytbl = &OSRdyTbl[0];\
for (i = 0; i < OS_RDY_TBL_SIZE; i++) *prdytbl++ = 0x00

/*����idle����*/
#define IDLEREADY(PTCB) \
    SET_TASK_MASK(PTCB, 0); /*�������ھ������в���������*/\
    SET_TASK_READY(0,PTCB)
    



    
#endif /*#ifndef OS_TASK_CPU_H*/