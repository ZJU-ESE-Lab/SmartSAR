/*
****************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_res.h
* ժ    Ҫ��������Դģ������ݽṹ�Լ�������׼API
*
* ��ǰ�汾��1.2
* ��    �ߣ�����
* ������ڣ�2005��7��20��
*
* ��ǰ�汾��1.1
* ��    �ߣ�����
* ������ڣ�2005��1��20��
*
* ȡ���汾��1.0
* ԭ����  ������
* ������ڣ�2003��12��10��
****************************************************************
*/
#ifndef	 OS_RES_H
#define	 OS_RES_H

typedef struct OSResCB * ResourceType; /*��Դ����*/

/*************************************************************
     �ڲ���Դ��ص�һЩ����
***************************************************************/
#ifdef OS_INT_RES /*ʹ���ڲ���Դ*/


#ifdef OS_COMPILE_INLINE	/*֧��inline����*/

/*�����ڲ���Դ�Զ���ȡʱ�����ȼ��仯*/
inline void GET_INT_RES(OSTCBRef PTCB)  
{
    if((HOLD_INT_RES==GET_TASK_HOLD_INT_RES(PTCB))&& 
        (GET_TASK_ID(PTCB)<GET_TASK_RUN_PRIO(PTCB)))
        /*ռ���ڲ���Դ���ڲ���Դ���ȼ��ȱ���ID��*/
    {
    	PTCB->SavedTCB=&OSTCBTbl[PTCB->RunPrio];
	    OSTCBPrioTbl[PTCB->RunPrio]=PTCB;
	    OS_TASK_TO_READY(PTCB->RunPrio,PTCB->SavedTCB);
    }
}
/*�ͷ��ڲ���Դ�����ȼ��仯�Լ�TCB����仯*/
inline void RELEASE_INT_RES(OSTCBRef curTcb)
{
    if( (HOLD_INT_RES==GET_TASK_HOLD_INT_RES(curTcb))&& 
        (GET_TASK_ID(curTcb)<GET_TASK_RUN_PRIO(curTcb)))
       /*ռ���ڲ���Դ���ڲ���Դ���ȼ��ȱ���ID��*/
    { /*ռ���ڲ���Դ*/
        if(!IS_TASK_READY(&OSTCBTbl[GET_TASK_INT_RES(curTcb)])) 
        {  /*����ڲ���Դ���ȼ���Ӧ�������ھ���״̬���Ϳ��Խ������ȼ��Ӿ����������ó�*/
        	OS_TASK_TO_UNREADY(GET_TASK_INT_RES(curTcb),GET_TASK_SAVED_TCB(curTcb));
        }
        RESET_TASK_SAVED_TCB(curTcb);/*�ͷ��ڲ���Դ*/
    }    	
}

#else/*#ifdef OS_COMPILE_INLINE*/

/*�����ڲ���Դ�Զ���ȡʱ�����ȼ��仯*/
#define GET_INT_RES(PTCB)  \
    do{if((HOLD_INT_RES==GET_TASK_HOLD_INT_RES(PTCB))&&\
        (GET_TASK_ID(PTCB)<GET_TASK_RUN_PRIO(PTCB))){\
    	PTCB->SavedTCB=&OSTCBTbl[PTCB->RunPrio];\
	    OSTCBPrioTbl[PTCB->RunPrio]=PTCB;\
	    OS_TASK_TO_READY(PTCB->RunPrio,PTCB->SavedTCB);}}while(0)

/*�ͷ��ڲ���Դ�����ȼ��仯�Լ�TCB����仯*/
#define RELEASE_INT_RES(curTcb) \
    do{if((HOLD_INT_RES==GET_TASK_HOLD_INT_RES(curTcb))&&\
        (GET_TASK_ID(curTcb)<GET_TASK_RUN_PRIO(curTcb))){\
       /*ռ���ڲ���Դ���ڲ���Դ���ȼ��ȱ���ID��*/\
        if(!IS_TASK_READY(&OSTCBTbl[GET_TASK_INT_RES(curTcb)])){\
        	OS_TASK_TO_UNREADY(GET_TASK_INT_RES(curTcb),GET_TASK_SAVED_TCB(curTcb));\
        }\
        RESET_TASK_SAVED_TCB(curTcb);/*�ͷ��ڲ���Դ*/\
    }}while(0)    	
    
#endif/*#ifdef OS_CPU_MPC555*/

    
    

#else /*#ifdef OS_INT_RES*/

#define GET_INT_RES(PTCB)

#define RELEASE_INT_RES(curTcb)	
#endif/*#ifdef OS_INT_RES*/

/*************************************************************
     �ⲿ��Դ��صĶ����һЩ�����Ķ���
***************************************************************/

#if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER) /*ʹ���ⲿ��Դ���������Դ*/

#define OUT_RES_USED  1
#define OUT_RES_NOT_USED  0

#ifdef OS_ISR_RES /*�жϼ�����Դ����*/
typedef INT32U OSISRMaskType;/*MASK����*/
#endif/*ifdef OS_ISR_RES*/ 
/*�ⲿ��Դ���ƽṹ��*/
struct OSResCB 
{
    ResPrioType   priority;     /*��Դ���ȼ�*/
    PriorityType  OldRunPrio;   /*���ڱ���ԭ�����������ȼ�������ReleaseResourceʱ�ָ�*/
#ifdef OS_EXTENDED_STATUS    
    INT8U         IsUsed;     /*��־����Դ�Ƿ�ʹ��,0��ʾû����ʹ�ã������ʾ��ʹ��*/
#endif /*ifdef OS_EXTENDED_STATUS*/    
    OSTCBRef  SavedTCB;   /* �����TCBָ��*/
    OSRCBRef     NextRes;    /*��һ����Դ��ָ��*/
#ifdef OS_ISR_RES /*�жϼ�����Դ����*/
    OSISRMaskType SavedMask;/*�����MASK*/
#endif/*ifdef OS_ISR_RES*/      
};
extern OSRCB  OSResources[];


/*�õ���Դ���ȼ�*/
#define GET_RES_PRIO(PRCB) ((PRCB)->priority)
/*�õ�ԭ��������������ȼ�*/
#define GET_RES_OLD_RUN_PRIO(PRCB) (PRCB)->OldRunPrio
/*�õ������TCBָ��*/
#define GET_RES_SAVED_TCB(PRCB) ((PRCB)->SavedTCB)

/*�õ���һ����Դָ��*/
#define GET_RES_NEXT(PRCB)  ((PRCB)->NextRes)

#define SET_RES_NEXT(PRCB,next)  (PRCB)->NextRes=next

/********************************************************************
* �����ⲿ��Դ��ȡʱ�����ȼ��仯��
* 1)������Դ���ȼ���Ӧ��TCB,ע�⣺��������OSTCBTbl��ȡֵ�����ܴ�OSTCBPrioTblȡֵ����Ϊ
*   OSTCBPrioTbl�����Ѿ�����ȷ�ˣ��ڲ���Դ��ȡҲһ����
* 2)�滻��Դ���ȼ���Ӧ��TCBPrioTbl�е�ֵΪ��ǰ����
* 3)����ԭ�����������ȼ�������ReleaseResourceʱ�ָ�ԭ�����������ȼ�
* 4)�����������ȼ�
* 5)����Դ���ȼ������������
*********************************************************************/
#define SET_OUT_PRIORITY(RCB,TCB) \
	do{if(((RCB)->OldRunPrio = GET_TASK_RUN_PRIO(TCB) ) < GET_RES_PRIO(RCB))\
	{\
		(RCB)->SavedTCB=&OSTCBTbl[(RCB)->priority];\
		OSTCBPrioTbl[(RCB)->priority]=(TCB);\
		OS_TASK_TO_READY((RCB)->priority,(RCB)->SavedTCB);\
		SET_TASK_RUN_PRIO(TCB,(RCB)->priority);\
	}}while(0)

/********************************************************************
* �����ⲿ��Դ�ͷ�ʱ�����ȼ��仯
* 1)�ָ��������ȼ�������ֵ��Ϊԭ��������������ȼ�
* 2)��OSTCBPrioTbl�е���Դ���ȼ����������ֵ��Ϊԭ����TCB
*********************************************************************/ 	
#define RESET_OUT_PRIORITY(PTCB,RCB) \
    do{if(GET_TASK_RUN_PRIO(PTCB) > GET_RES_OLD_RUN_PRIO(RCB))\
    {\
    	SET_TASK_RUN_PRIO(PTCB,GET_RES_OLD_RUN_PRIO(RCB));\
    	OSTCBPrioTbl[(RCB)->priority]=(RCB)->SavedTCB;\
    }}while(0)
	
/*�����������Դ������������Դ*/
#define ADD_RES_LIST(TCB,RCB) \
    SET_RES_NEXT(res,(TCB)->resources);\
    SET_TASK_OUT_RES((TCB),res)

/*�����������Դ������ɾ����Դ*/   
#define DELETE_RES_LIST(Tcb,RCB) (Tcb)->resources=RCB->NextRes

/***********************************************************
���жϼ���Դ��һЩ����
*************************************************************/
#ifdef OS_ISR_RES
/*������Դ���ж�����*/
extern OSRCBRef OSISRLastResource;
#define SET_RES_ISR_MASK(RCB,mask) (RCB)->SavedMask=mask
/*�õ���Դ���ж�����*/
#define GET_RES_ISR_MASK(RCB) ((RCB)->SavedMask)
/*�����жϼ���Դ������*/
#define ADD_RES_ISR_LIST(RCB) \
        (RCB)->NextRes= OSISRLastResource; /*��Դ����*/\
        OSISRLastResource = (RCB)

#else/*#ifdef OS_ISR_RES*/
#define SET_RES_ISR_MASK(RCB,mask) 
#endif /*#ifdef OS_ISR_RES*/

/***********************************************************
��չ״̬�µ�һЩ����
*************************************************************/

#ifdef OS_EXTENDED_STATUS 
/*�õ���Դʹ��״̬*/
#define GET_RES_USED(PRCB) ((PRCB)->IsUsed)
/*������Դ��ʹ��*/
#define SET_RES_USED(PRCB) (PRCB)->IsUsed=OUT_RES_USED
/*������Դδʹ��*/
#define SET_RES_UNUSED(PRCB) (PRCB)->IsUsed=OUT_RES_NOT_USED
#else
/*������Դ��ʹ��*/
#define SET_RES_USED(PRCB) 
/*������Դδʹ��*/
#define SET_RES_UNUSED(PRCB) 

#endif/*#ifdef OS_EXTENDED_STATUS */
 
 
    	
#define NO_RESOURCE ((OSRCBRef)0) /*���ⲿ��Դ*/

#define DeclareResource(ResId) extern ResourceType ResId
void ResourceInit(void);

#endif/*if defined(OS_OUT_RES)||defined(OS_RES_SCHEDULER)*/ 



#if defined(OS_RES_SCHEDULER)&&defined(OS_OUT_RES) 

extern  ResourceType RES_SCHEDULER;    /*��������Դ*/
 
#define GetResource(ResId) \
       (ResId==RES_SCHEDULER)?GetSchedulerResource(RES_SCHEDULER):GetOutResource(ResId)
       
#define ReleaseResource(ResId) \
       (ResId==RES_SCHEDULER)?ReleaseSchedulerResource(RES_SCHEDULER):ReleaseOutResource(ResId)
       
#endif /*defined(OS_RES_SCHEDULER)&&defined(OS_OUT_RES) */

#if defined(OS_RES_SCHEDULER)&&!defined(OS_OUT_RES) /*ֻʹ�õ�������Դ����ʹ�������κ���Դ**/

extern  ResourceType RES_SCHEDULER;    /*��������Դ*/ 

#define GetResource(ResId) GetSchedulerResource(ResId)

#define ReleaseResource(ResId) ReleaseSchedulerResource(ResId)

#endif /*#if !defined(OS_RES_SCHEDULER)&&defined(OS_RES_SCHEDULER) */

#if defined(OS_OUT_RES)&&!defined(OS_RES_SCHEDULER)/*��ʹ�õ�������Դ��ֻʹ�������κ���Դ**/

#define GetResource(ResId) GetOutResource(ResId)
#define ReleaseResource(ResId) ReleaseOutResource(ResId)

#endif /*#if defined(OS_RES_SCHEDULER)&&!defined(OS_RES_SCHEDULER)*/


#endif /*ifndef	 OS_RES_H*/
