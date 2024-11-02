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
     �ⲿ��Դ��صĶ����һЩ�����Ķ���
***************************************************************/


#define OUT_RES_USED  1
#define OUT_RES_NOT_USED  0

struct OSResCB 
{
    ResPrioType   priority;     /*��Դ���ȼ�*/
    PriorityType  OldRunPrio;   /*���ڱ���ԭ�����������ȼ�������ReleaseResourceʱ�ָ�*/
#ifdef OS_EXTENDED_STATUS    
    INT8U         IsUsed;     /*��־����Դ�Ƿ�ʹ��,0��ʾû����ʹ�ã������ʾ��ʹ��*/
#endif /*ifdef OS_EXTENDED_STATUS*/    
    OSTCBRef  SavedTCB;   /* �����TCBָ��*/
    OSRCBRef     NextRes;    /*��һ����Դ��ָ��*/
 
};
extern OSRCB  OSResources[];



#define NO_RESOURCE ((OSRCBRef)0) /*���ⲿ��Դ*/

#define DeclareResource(ResId) extern ResourceType ResId
void ResourceInit(void);



extern  ResourceType RES_SCHEDULER;    /*��������Դ*/
 
#define GetResource(ResId) \
       (ResId==RES_SCHEDULER)?GetSchedulerResource(RES_SCHEDULER):GetOutResource(ResId)
       
#define ReleaseResource(ResId) \
       (ResId==RES_SCHEDULER)?ReleaseSchedulerResource(RES_SCHEDULER):ReleaseOutResource(ResId)
       
#endif /*ifndef	 OS_RES_H*/
