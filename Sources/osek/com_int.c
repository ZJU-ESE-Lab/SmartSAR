/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�com_int.c
* ժ    Ҫ���ڲ�ͨ��ģ���ʵ��
*
* ��ǰ�汾��1.0
* ��    �ߣ�����
* ������ڣ�2005��7��17��
*
*********************************************************************************
*/
#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

#define COM_MAX_FILTER_BYTE sizeof(INT64U)

#ifdef COM_SEND_STATIC_INTERNAL
extern COMMsgIntSenderCfg MsgIntSenderCfgTbl[];     /*�ڲ������������飬�û�����*/
MsgIntSender MsgInSenderTbl[COM_NUM_INT_SENDER];/*�ڲ����Ͷ�������*/
#endif/*#ifdef COM_SEND_STATIC_INTERNAL*/

#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL) 

#if defined(COM_RECEIVE_QUEUED_INTERNAL)&&defined(COM_RECEIVE_UNQUEUED_INTERNAL) 
#define COM_NUM_INT_RECEIVER  ((COM_NUM_INT_QUEUED_RECEIVER)+(COM_NUM_INT_UNQUEUED_RECEIVER))/*�����ڲ������ߵ�����*/

#elif defined(COM_RECEIVE_QUEUED_INTERNAL)&&(!defined(COM_RECEIVE_UNQUEUED_INTERNAL) )

#define COM_NUM_INT_RECEIVER  ((COM_NUM_INT_QUEUED_RECEIVER))/*�����ڲ������ߵ�����*/

#elif defined(COM_RECEIVE_UNQUEUED_INTERNAL)&&(!defined(COM_RECEIVE_QUEUED_INTERNAL)) 

#define COM_NUM_INT_RECEIVER  ((COM_NUM_INT_UNQUEUED_RECEIVER))/*�����ڲ������ߵ�����*/
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)&&defined(COM_RECEIVE_UNQUEUED_INTERNAL) */

extern COMMsgIntReceiverCfg MsgIntReceiverCfgTbl[]; /*�ڲ������������飬�û�����*/
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL) */

#if defined(COM_RECEIVE_UNQUEUED_INTERNAL) 
#define MSG_UQ_RECEIVER(object) ((MsgIntUqReceiver*)(object))
MsgIntUqReceiver MsgIntUqReceiverTbl[COM_NUM_INT_UNQUEUED_RECEIVER];/*�ڲ��Ƕ��н�����Ϣ��������*/
static INT64U UqueuedOldData[COM_NUM_INT_UNQUEUED_RECEIVER];
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/

#if defined(COM_RECEIVE_QUEUED_INTERNAL)
#define MSG_QU_RECEIVER(object) ((MsgIntQuReceiver*)(object))
MsgIntQuReceiver MsgIntQuReceiverTbl[COM_NUM_INT_QUEUED_RECEIVER];/*�ڲ����н�����Ϣ��������*/
static INT64U QueuedOldData[COM_NUM_INT_QUEUED_RECEIVER];
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)*/

#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
/*֪ͨ���Ƶ���������*/
static COMNotification COMNotificationTbl[COM_NUM_NOTIFICATION];/*֪ͨ����*/
INT8U NtIndex=0;
#define COMInternalStop() (NtIndex = 0)
#else
#define COMInternalStop() 
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/

#if COM_NUM_FILTER>0
static COMFilter COMFilterTbl[COM_NUM_FILTER];/*�����㷨*/
#endif/*#if COM_NUM_FILTER>0*/

#if defined (COM_NOTIFICATION_FLAG)
FlagValue COMFlags[COM_NUM_FLAG];
#define	COMSetFlag(flag)    *(flag)=TRUE
#define COMResetFlag(flag)	*(flag)=FALSE
#endif/*#if defined (COM_NOTIFICATION_FLAG) || (COM_NOTIFICATIONERROR_FLAG)*/

/*
***************************************************************************
* static void COMSetFilter(COMFilter ** filter,COMFilterCfg *cfg)
* �����������������ù�����
* ���õĺ�������
* �������õ�λ�ã�COMInternalInit
* ������	filter  Ҫ���õĹ���
*           cfg     ��������������Ϣ
* ���أ�	��
* ע�����
* �����ˣ�����
* ʱ�䣺2005.7.20
***************************************************************************
*/
#if COM_NUM_FILTER>0
static void COMSetFilter(COMFilter ** filter,COMFilterCfg *cfg)
{
	static INT8U FilterIndex=0;
	*filter=COMFilterTbl+FilterIndex;
	FilterIndex++;
	(*filter)->FilterAlgorithm=cfg->FilterAlgorithm;
#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)\
							||defined(COM_MASKEDNEWEQUALSMASKEDOLD)\
							||defined(COM_MASKEDNEWDIFFERSMASKEDOLD) 	
	(*filter)->parameter1.mask=cfg->mask;
#endif/* defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)\
							||defined(COM_MASKEDNEWEQUALSMASKEDOLD)\
							||defined(COM_MASKEDNEWDIFFERSMASKEDOLD) */	
	
#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)
     (*filter)->parameter1.max=cfg->max;
#endif/*#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)*/

#if defined(COM_ONEEVERYN)
     (*filter)->parameter1.period=cfg->period;
#endif/*#if defined(COM_ONEEVERYN)*/

#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)
     (*filter)->x=cfg->x;
#endif/*#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)*/
#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)    
     (*filter)->min=cfg->min; /*����NEWISWITHIN��NEWISOUTSIDE�㷨*/
#endif/*#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)*/
#if defined(COM_ONEEVERYN)
     (*filter)->offset=cfg->offset; /*����ONEEVERYN�㷨*/
#endif/*#if defined(COM_ONEEVERYN)*/ 
}
#endif/*#if COM_NUM_FILTER>0*/
/*
***************************************************************************
* static void COMSetNotification(COMNotification ** filter,COMNtCfg *cfg)
* �����������������ù�����
* ���õĺ�������
* �������õ�λ�ã�COMInternalInit
* ������	nt Ҫ���õ�֪ͨ����
*           cfg ������Ϣ
* ���أ�	��
* ע�����
* �����ˣ�����
* ʱ�䣺2005.7.20
* �޸��ˣ�������
* ʱ�䣺2005.11.23 
***************************************************************************
*/
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)

static void COMSetNotification(COMNotification ** nt,COMNtCfg *cfg)
{
    //static INT8U NtIndex=0;    
    (*nt)=COMNotificationTbl+NtIndex;
	(*nt)->type=cfg->type;
#ifdef COM_NOTIFICATION_ACTIVATETASK
    if(cfg->type == COM_NOTIFICATION_ACTIVATETASK)(*nt)->parameter.task=&OSTCBTbl[cfg->TaskID];  /*�����ָ��*/
#endif/*#ifdef(COM_NOTIFICATION_ACTIVATETASK)*/

#ifdef COM_NOTIFICATION_SETEVENT
  if(cfg->type == COM_NOTIFICATION_SETEVENT)(*nt)->parameter.task=&OSTCBTbl[cfg->TaskID];  /*�����ָ��*/
#endif/*#ifdef(COM_NOTIFICATION_SETEVENT)*/





#if defined(COM_NOTIFICATION_FLAG)
    if(cfg->type == COM_NOTIFICATION_FLAG)(*nt)->parameter.flag=&COMFlags[cfg->FlagID];	   /*��־λ��ָ�룬ָ��COMFlags[COM_NUM_FLAG]����*/
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/
#if defined(COM_NOTIFICATION_COMCALLBACK)
    if(cfg->type == COM_NOTIFICATION_COMCALLBACK)(*nt)->parameter.CallBack=cfg->CallBack; /*CALLBACK������*/
#endif/*#if defined(COM_NOTIFICATION_COMCALLBACK)*/
#if	defined(COM_NOTIFICATION_SETEVENT)    
    if(cfg->type == COM_NOTIFICATION_SETEVENT)(*nt)->event=cfg->event;   /*�¼���ID*/
#endif/*#if	defined(COM_NOTIFICATION_SETEVENT)  */
    NtIndex++;
}
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/
/*
***************************************************************************
* void COMInternalInit()
* �������ڲ�ͨ�ų�ʼ��
* ���õĺ�������
* �������õ�λ�ã�StartCOM
* ������	��
* ���أ�	��
* ע������ú���ֻ�������ڲ�ͨ��
* �����ˣ�����
* ʱ�䣺2005.7.17
***************************************************************************
*/
void COMInternalInit()
{
    DEFINECRITICAL(cpu_sr); 
#ifdef COM_SEND_STATIC_INTERNAL    
    COMMsgIntSenderCfg * cfgSender=MsgIntSenderCfgTbl;
    MsgIntSender * sender=MsgInSenderTbl;
#endif/*#ifdef COM_SEND_STATIC_INTERNAL*/
#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL)     
    COMMsgIntReceiverCfg *cfgReceiver=MsgIntReceiverCfgTbl+COM_NUM_INT_RECEIVER-1;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL) */    
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)    
    MsgIntUqReceiver * unqueueReceiver=MsgIntUqReceiverTbl+COM_NUM_INT_UNQUEUED_RECEIVER-1;
    INT64U * uqdata=UqueuedOldData;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/  
#if defined(COM_RECEIVE_QUEUED_INTERNAL)  
    MsgIntQuReceiver * queueReceiver=MsgIntQuReceiverTbl+COM_NUM_INT_QUEUED_RECEIVER-1;
    INT64U * qdata=QueuedOldData;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)*/    
    OS_ENTER_CRITICAL();
#ifdef COM_SEND_STATIC_INTERNAL   
    for(;cfgSender<MsgIntSenderCfgTbl+COM_NUM_INT_SENDER;cfgSender++,sender++)
    {    /*��ʼ��������Ϣ��������*/
    	sender->property=COM_SEND_STATIC_INTERNAL;
    	sender->DataProperty=cfgSender->DataProperty;
    	sender->DataSize=cfgSender->DataSize; 
        sender->ReceiverList=NULL;
    }
#endif/*#ifdef COM_SEND_STATIC_INTERNAL*/    
#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL)     
    for(;cfgReceiver>=MsgIntReceiverCfgTbl;cfgReceiver--)
    {  /*��ʼ��������Ϣ��������*/
    	switch((MessageType)(cfgReceiver->property))/*�ж�������*/
    	{
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)     	
    		case COM_RECEIVE_UNQUEUED_INTERNAL:/*�Ƕ��н���*/
    			unqueueReceiver->property=COM_RECEIVE_UNQUEUED_INTERNAL;
    			unqueueReceiver->buffer=cfgReceiver->buffer;
#if (COM_NUM_FILTER)>0     			/*���û��Filter�Ͷ�����ΪNULL*/
    			if(cfgReceiver->filter==NULL)
    			{
#endif/*#if COM_NUM_FILTER>0 */    			
    				unqueueReceiver->filter=NULL;
#if (COM_NUM_FILTER)>0      			
                }
    			else
    			{
    				COMSetFilter(&(unqueueReceiver->filter),cfgReceiver->filter);/*����Filter*/
    			}
#endif/*#if COM_NUM_FILTER>0 */  
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
    			if(cfgReceiver->notification==NULL)
    			{
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/    			
    			    unqueueReceiver->notification=NULL;	
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
    			}
    			else
    			{
    			    COMSetNotification(&(unqueueReceiver->notification),cfgReceiver->notification);/*����֪ͨ����*/		
    			}
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/    			
    			if((cfgReceiver->notification!=NULL) &&          /*�й���*/
    			      ((sender->DataSize)<COM_MAX_FILTER_BYTE))/*���ǷǸ������Ͳ���Ҫ���ˣ�Ҳ�Ͳ���Ҫ��¼������*/
    			{
    			    unqueueReceiver->OldData=uqdata++;/*����OldDataָ��*/
    			    //memcpy(unqueueReceiver->OldData,&(cfgReceiver->InitValue),sender->DataSize);/*��¼�����ݣ��û�����*/	
    			    *unqueueReceiver->OldData = cfgReceiver->InitValue;
    			}
    			sender=&MsgInSenderTbl[cfgReceiver->SendMsgIndex];
    			unqueueReceiver->DataSize=sender->DataSize;/*���ݴ�С*/
    			/*���뷢���ߵ�����*/
    			unqueueReceiver->next=sender->ReceiverList;
	            sender->ReceiverList=unqueueReceiver;
    			/*������ʼֵ*/
    			memcpy(unqueueReceiver->buffer,(INT8U *)&(cfgReceiver->InitValue)+4-sender->DataSize,sender->DataSize);
    			unqueueReceiver--;
    			break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/      			
#if defined(COM_RECEIVE_QUEUED_INTERNAL)      			
    		case COM_RECEIVE_QUEUED_INTERNAL:/*���н���*/
    		    queueReceiver->property=COM_RECEIVE_QUEUED_INTERNAL;
    		    queueReceiver->QueueCounter=0; 
    		    queueReceiver->QueueStatus=COM_QUEUE_MSG_NONE;
    		    queueReceiver->QueueSize=cfgReceiver->QueueSize;
    			queueReceiver->QueueStart=queueReceiver->QueueWrite=queueReceiver->QueueRead=cfgReceiver->buffer;
    			queueReceiver->QueueEnd=cfgReceiver->buffer+cfgReceiver->QueueSize+1;/*ĩβָ��*/
#if (COM_NUM_FILTER)>0     			/*���û��Filter�Ͷ�����ΪNULL*/    			
    			if(cfgReceiver->filter==NULL)
    			{
#endif/*#if COM_NUM_FILTER>0 */    			
    				queueReceiver->filter=NULL;	
#if (COM_NUM_FILTER)>0     				
    			}
    			else
    			{
    				COMSetFilter(&(queueReceiver->filter),cfgReceiver->filter);/*����Filter*/
    			}
#endif/*#if COM_NUM_FILTER>0 */    			
    			
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
    			if(cfgReceiver->notification==NULL)
    			{
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/    			
    			    queueReceiver->notification=NULL;	
    			}
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
    			else
    			{
    			    COMSetNotification(&(queueReceiver->notification),cfgReceiver->notification);/*����֪ͨ����*/
    			}
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/    			
    			if((cfgReceiver->notification!=NULL) &&          /*�й���*/
    			      ((sender->DataSize)<COM_MAX_FILTER_BYTE))/*���ǷǸ������Ͳ���Ҫ���ˣ�Ҳ�Ͳ���Ҫ��¼������*/
    			{
    				queueReceiver->OldData=qdata++;/*����OldDataָ��*/
    			    memcpy(queueReceiver->OldData,(INT8U*)&(cfgReceiver->InitValue) + 4 - sender->DataSize,sender->DataSize);/*��¼�����ݣ��û�����*/	
    			}    			
    			sender=&MsgInSenderTbl[cfgReceiver->SendMsgIndex];
    			queueReceiver->DataSize=sender->DataSize;/*���ݴ�С*/
    			queueReceiver->next=sender->ReceiverList;/*���뷢���ߵ�����*/
	            sender->ReceiverList=queueReceiver;
    			queueReceiver--;
    		    break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)*/    		    
    		default:
    		    OSEKPrintInt((MessageType)(cfgReceiver->property));
    		    break;
    	}
    } 
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL) */    
    OS_EXIT_CRITICAL();
}

/*
*************************************************************************************
* static INT8U COMFilterMessage(COMMessage *pmsg,DataPtrType newData)
*
* ������            COM ��׼ API
*					����Ϣ����ֵ���й��ˣ��ڲ�����
*
* ���õĺ�����      
*					
*
* �������õ�λ�ã�COMSendIntMessage
*					
*
* ������	        filter ������
*			        oldData ������
*                   newData ��ǰ��������
*                   occurrence ����
*
* ���أ�	        1: ��Ϣͨ��
*		            0: ��Ϣ������
*
* ע�����	    �ڲ������Լ��㳤�ȺͶ�̬���ȵ���Ϣ���ܽ��й���	
*
* �����ˣ�          ��ࡢ����
*
* ʱ�䣺            2004��12��10��
*
* �޸��ˣ�          ������
*
* ʱ�䣺            2005��3��8��
*
* �޸��ˣ�          �����
*
* ʱ�䣺            2005��6��28��
*
* �޸��ˣ�          ����
*
* ʱ�䣺            2005��7��17��
*
*************************************************************************************
*/

    
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)||defined(COM_RECEIVE_QUEUED_INTERNAL)	

static INT8U COMFilterMessage(COMFilter *filter,DataPtrType newData,INT64U * oldData
#ifdef COM_ONEEVERYN
                              ,INT32U occurrence
#endif/*#ifdef COM_ONEEVERYN*/                              
                              )
{
    
#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)\
							||defined(COM_MASKEDNEWEQUALSMASKEDOLD)\
							||defined(COM_MASKEDNEWDIFFERSMASKEDOLD) 
    INT8U mask=filter->parameter1.mask;
#endif /*defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)\
							||defined(COM_MASKEDNEWEQUALSMASKEDOLD)\
							||defined(COM_MASKEDNEWDIFFERSMASKEDOLD) */

#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)
    INT8U x=filter->parameter2.x;
#endif /*defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)*/

#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)
    INT8U max=filter->parameter1.max;
    INT8U min=filter->parameter2.min;
#endif /*defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)*/

#if defined(COM_ONEEVERYN)
    INT8U period=filter->parameter1.period;
    INT8U offset=filter->parameter2.offset;
    INT8U occurrence=filter->parameter3.occurrence;
#endif/*defined(COM_ONEEVERYN)*/

	
switch (filter->FilterAlgorithm)
{
/*ȫ��ͨ����Ϊ�����㷨��Ĭ��ֵ*/
#ifdef	COM_ALWAYS
    case COM_ALWAYS:
        return 1;
#endif/*#ifdef COM_ALWAYS*/
/*ȫ����ͨ��*/
#ifdef	COM_NEVER
    case COM_NEVER:
        return 0;
#endif /*#ifdef	COM_NEVER*/

/*ʹmask�����ض�ֵ����Ϣͨ��*/
#ifdef	COM_MASKEDNEWEQUALSX
    case COM_MASKEDNEWEQUALSX:
        if((mask&*newData)==x)
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif/*#ifdef	COM_MASKEDNEWEQUALSX*/

/*ʹmask�������ض�ֵ����Ϣͨ��*/
#ifdef	COM_MASKEDNEWDIFFERSX	
    case COM_MASKEDNEWDIFFERSX:
        if((mask&*newData)!=x)
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif/*#ifdef	COM_MASKEDNEWDIFFERSX*/

/*ʹ����û�иı����Ϣͨ��*/
#ifdef	COM_NEWISEQUAL
    case COM_NEWISEQUAL:
        if(*newData==*oldData)
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif/*#ifdef	COM_NEWISEQUAL*/

/*ʹ���ݸı����Ϣͨ��*/
#ifdef	COM_NEWISDIFFERENT
    case COM_NEWISDIFFERENT:
        if(*newData!=*oldData)
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif/*#ifdef	COM_NEWISDIFFERENT*/

#ifdef	COM_MASKEDNEWEQUALSMASKEDOLD		
	case COM_MASKEDNEWEQUALSMASKEDOLD:
        if((mask&*newData)==(mask&*oldData))
        {
            return 1;
        }
        else
        {
            return 0;
        }

#endif/*#ifdef	COM_MASKEDNEWEQUALSMASKEDOLD*/

#ifdef	COM_MASKEDNEWDIFFERSMASKEDOLD		
    case COM_MASKEDNEWDIFFERSMASKEDOLD:
        if((mask&*newData)!=(mask&*oldData))
        {
            return 1;
        }
        else
        {
         	return 0;
        }
#endif/*#ifdef	COM_MASKEDNEWDIFFERSMASKEDOLD*/

#ifdef	COM_NEWISWITHIN		
    case COM_NEWISWITHIN:
        if((min<=*newData) && (*newData<=max))
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif/*#ifdef	COM_NEWISWITHIN*/

#ifdef	COM_NEWISOUTSIDE		
     case COM_NEWISOUTSIDE:
        if((*newData<min) || (*newData>max))
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif/*#ifdef	COM_NEWISOUTSIDE*/

#ifdef	COM_NEWISGREATER		
    case COM_NEWISGREATER:
        if(*newData>*oldData)
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif/*#ifdef	COM_NEWISGREATER*/

#ifdef	COM_NEWISLESSOREQUAL		
    case COM_NEWISLESSOREQUAL:
        if(*newData<=*oldData)
        {
            return 1;
        }
        else
        {
			return 0;
	    }
#endif/*#ifdef	COM_NEWISLESSOREQUAL*/

#ifdef	COM_NEWISLESS		
    case COM_NEWISLESS:
        if(*newData<*oldData)
        {
            return 1;
        }
        else
        {
            return 0;
        }

#endif/*#ifdef	COM_NEWISLESS*/

#ifdef	COM_NEWISGREATEROREQUAL	
    case COM_NEWISGREATEROREQUAL:
        if(*newData>=*oldData)
        {
            return 1;
        }
        else
        {
            return 0;
        }

#endif/*#ifdef	COM_NEWISGREATEROREQUAL	*/

#ifdef	COM_ONEEVERYN		
    case COM_ONEEVERYN:/*ע��:�������Ҫ��ÿ�η��ͺͽ�����Ϣʱ�Ա���pmsg->MsgOccurrence��ֵ��2004715��δ�����Ժ��貹��*/
        if((occurrence%period)==offset)
        {
            return 1;
        }
        else
        {
            return 0;
        }
#endif/*#ifdef	COM_ONEEVERYN*/
    default:
        oldData=oldData;/*��ֹWarning*/
        newData=newData;
        filter=filter;
        return 1;	
	}

}
#endif/*#if defined(RECEIVE_UNQUEUED_INTERNAL)||defined(RECEIVE_QUEUED_INTERNAL)	*/

/*
*************************************************************************************
* static void  COMNotify(COMNotification *notification)
*
* ������            OSEK COM �ڲ�������ʵ��֪ͨ����
*
* ���õĺ�����      
*
* �������õ�λ�ã�COMSendIntMessage
*					COMSendIntZeroMessage
*
* ������	        notification ֪ͨ�����ݽṹ
*
* ���أ�	        E_OK ���óɹ�
*					E_COM_ID ����ʧ��
*
* ע�����	    
*
* �����ˣ�          ����
*
* ʱ�䣺            2004��12��27��
*
* �޸��ˣ�          ������
*
* ʱ�䣺            2005��3��8��
*
* �޸��ˣ�          ����
*
* ʱ�䣺            2005��7��17��
*************************************************************************************
*/
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)||\
										defined(COM_NOTIFICATION_COMCALLBACK)||\
										defined(COM_NOTIFICATION_FLAG)
static void	COMNotify(COMNotification *notification)
{
    DEFINECRITICAL(cpu_sr);
    switch(notification->type)
    {
	/*����FLAG*/
#ifdef	COM_NOTIFICATION_FLAG
    case COM_NOTIFICATION_FLAG:
        OS_ENTER_CRITICAL();
        COMSetFlag(notification->parameter.flag);/*����FLAG*/
        OS_EXIT_CRITICAL();
        break;
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/
	
	/*�������*/
#ifdef	COM_NOTIFICATION_ACTIVATETASK
    case COM_NOTIFICATION_ACTIVATETASK:
        OS_ENTER_CRITICAL();
        OSActivateTask((TaskType)notification->parameter.task);
        OS_EXIT_CRITICAL();
        break;
#endif/*#ifdef	COM_NOTIFICATION_ACTIVATETASK*/
	
	/*�����¼�*/
#if defined(COM_NOTIFICATION_SETEVENT)
    case COM_NOTIFICATION_SETEVENT:
#ifdef OS_EVENT_EN
        OS_ENTER_CRITICAL();
        OSSetEvent(notification->parameter.task, notification->event);
        OS_EXIT_CRITICAL();
#endif/*#ifdef OS_EVENT_EN*/
        break;
#endif/*#if defined(COM_NOTIFICATION_SETEVENT) */
	
	/*CALLBACK*/
#ifdef	COM_NOTIFICATION_COMCALLBACK
    case COM_NOTIFICATION_COMCALLBACK:
        (notification->parameter.CallBack)();/*�����û����趨�Ļص�����*/
        break;/*com301 p31 callbackû�з���ֵ*/
#endif/*#ifdef	COM_NOTIFICATION_COMCALLBACK*/
	
    default:
        notification=notification;
        cpu_sr=cpu_sr;
        break;	
    }
}
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)||\
										defined(COM_NOTIFICATION_COMCALLBACK)||\
										defined(COM_NOTIFICATION_FLAG)*/
/*
*************************************************************************************
* StatusType COMSendIntMessage(SymbolicName sender,ApplicationDataRef data)
*
* ������            �ڲ�ͨ��ʵ�֣�����sender��data���͸���sender��صĽ����ߡ�
*
* ���õĺ�����      COMFilterMessage
*					COMSetNotification
*
* �������õ�λ�ã�SendMessage
*
* ������	        sender   ������Ϣ��������
*					data:     �û��������ڵ�ַ
*
* ���أ�	        ��׼״̬��E_OK          ��Ϣ���ͳɹ�
* 
*				    ��չ״̬: E_COM_ID      ��Ϣ����senderԽ�磻senderΪ������Ϣ����
*                                           senderΪ�㳤�Ȼ�̬������Ϣ
*
* ע�����	    
*
* �����ˣ�          ����
*
* ʱ�䣺            2005��7��17��
*
* �޸��ˣ�          ������
* 
* �޸�ʱ�䣺        2005��11��29�� 
*************************************************************************************
*/
StatusType COMSendIntMessage(SymbolicName sender,ApplicationDataRef data)
{
    DEFINECRITICAL(cpu_sr); 
    SymbolicName receiver=NULL;
#ifdef COM_EXTENDED_STATUS
    /*����Ƿ�Խ��*/
    if( (sender<MsgInSenderTbl) || (sender>(MsgInSenderTbl+COM_NUM_INT_SENDER))
        || (((AddrType)sender-(AddrType)MsgInSenderTbl)%sizeof(MsgIntSender)!=0))
    {
        COMRETERROR(E_COM_ID,COMServiceId_SendMessage,sender);
    }
#endif/*#ifdef COM_EXTENDED_STATUS*/
  
    OS_ENTER_CRITICAL();
    for(receiver=((MsgIntSender*)sender)->ReceiverList;NULL!=receiver;)
    {/*ѭ��������ǰ��Ϣ����Ľ����ߣ������ݿ�����������ߵ���������*/
        if(((MsgIntSender*)sender)->DataProperty == COM_FILTER) /*������������Ϳ��Թ���*/
        {
            /*�ڲ����ܶ˵Ĺ���*/
            switch(*(MessageType*)(receiver))/*�ж�������*/
            {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)      	            
            	case COM_RECEIVE_QUEUED_INTERNAL:/*���н���*/
            	    
            	    if( ((MSG_QU_RECEIVER(receiver)->filter)!=NULL)&&
            	        (0 == COMFilterMessage(MSG_QU_RECEIVER(receiver)->filter,data,MSG_QU_RECEIVER(receiver)->OldData)
#if defined(COM_ONEEVERYN)
                            ,MSG_QU_RECEIVER(receiver)->occurrence/*���·��ʹ���*/
#endif /*#if defined(COM_ONEEVERYN)*/           	    
            	         )
            	     )
                    {
                        /*��ǰ��������Ϣ������*/
                        continue;/*��һ��������*/
                    }
            	    break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)               	    
            	case COM_RECEIVE_UNQUEUED_INTERNAL:/*�Ƕ��н���*/
            	    if( ((MSG_UQ_RECEIVER(receiver)->filter)!=NULL)&&
            	     (0 == COMFilterMessage(MSG_UQ_RECEIVER(receiver)->filter,data,MSG_UQ_RECEIVER(receiver)->OldData)
#if defined(COM_ONEEVERYN)
                            ,MSG_QU_RECEIVER(receiver)->occurrence/*���·��ʹ���*/
#endif /*#if defined(COM_ONEEVERYN)*/           	    
            	      )      
            	    )
            	    
                    {
                        /*��Ϣ������*/
                        continue;
                    }           	    
                    break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)  */ 
                default:
                    break;                   
            }
            
        }
        switch(*(MessageType*)(receiver))/*�ж�������*/
        {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)           
        	case COM_RECEIVE_QUEUED_INTERNAL:/*���н���*/
        	     
        	     if(MSG_QU_RECEIVER(receiver)->QueueCounter>=MSG_QU_RECEIVER(receiver)->QueueSize)/*��������*/
        	     {
        	         MSG_QU_RECEIVER(receiver)->QueueStatus=COM_QUEUE_MSG_OVERFLOW;/*���ñ�־λ*/
        	     }
        	     else/*���в���*/
        	     {
        	         memcpy((void*)(MSG_QU_RECEIVER(receiver)->QueueWrite),data,((MsgIntSender*)sender)->DataSize);
				     
                     if(MSG_QU_RECEIVER(receiver)->QueueWrite == MSG_QU_RECEIVER(receiver)->QueueEnd)
                     {    /*������иո��������дָ��ָΪ����ͷָ��*/
                         MSG_QU_RECEIVER(receiver)->QueueWrite=MSG_QU_RECEIVER(receiver)->QueueStart;
                     }
                     else
                     {
                         MSG_QU_RECEIVER(receiver)->QueueWrite+=((MsgIntSender*)sender)->DataSize; /*��Ϊ�����ֽ���������*/
                     }

                     MSG_QU_RECEIVER(receiver)->QueueCounter++;
				     if(MSG_QU_RECEIVER(receiver)->OldData  && ((MsgIntSender*)sender)->DataSize<COM_MAX_FILTER_BYTE)
				     memcpy(MSG_QU_RECEIVER(receiver)->OldData,(DataPtrType)data,((MsgIntSender*)sender)->DataSize);/*�������ڹ��˵ľ�����*/
				     MSG_QU_RECEIVER(receiver)->QueueStatus=COM_QUEUE_MSG_NORMAL;/*״̬���ã���ԭ���ǿ���Ϣ״̬��ʱ��*/
        	     }
        	     receiver=MSG_QU_RECEIVER(receiver)->next; 
#if defined(COM_ONEEVERYN)
                 MSG_QU_RECEIVER(receiver)->occurrence++;/*���·��ʹ���*/
#endif /*#if defined(COM_ONEEVERYN)*/   
        	     break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */        	     
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL) 
        	case COM_RECEIVE_UNQUEUED_INTERNAL:/*�Ƕ��н���*/
        	     memcpy((void*)(MSG_UQ_RECEIVER(receiver)->buffer),data,((MsgIntSender*)sender)->DataSize);
        	     receiver=MSG_UQ_RECEIVER(receiver)->next;
#if defined(COM_ONEEVERYN)
                 MSG_UQ_RECEIVER(receiver)->occurrence++;/*���·��ʹ���*/
#endif /*#if defined(COM_ONEEVERYN)*/         	     
        	     break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)  */ 
        	default: 
        	     OS_EXIT_CRITICAL();
        	     
        	     OSEKPrintInt(*(MessageType*)(receiver));
        	     goto EXCEPTION;/*��Ϊû���ڲ������ߣ��������߼�¼���ڲ������ߣ������쳣*/
        	     break;
        }
    }
	OS_EXIT_CRITICAL();
    /*�ڲ�ͨ��ֻ��һ��֪ͨ���ƣ���ֻ�н����ߵ�֪ͨ����*/								
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)||\
    defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
    for(receiver=((MsgIntSender *)sender)->ReceiverList;NULL!=receiver;)
    {/*ѭ��������ǰ��Ϣ����Ľ����ߣ�ʵ��֪ͨ����*/
        switch(*(MessageType*)(receiver))/*�ж�������*/
        {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)          
        	case COM_RECEIVE_QUEUED_INTERNAL:/*���н���*/
        	     if(MSG_QU_RECEIVER(receiver)->notification!=NULL)
        	     {
        	         COMNotify(MSG_QU_RECEIVER(receiver)->notification);	
        	     }
        	     receiver=MSG_QU_RECEIVER(receiver)->next;
        	     break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */   
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)        	     
        	case COM_RECEIVE_UNQUEUED_INTERNAL:/*�Ƕ��н���*/
        	     if(MSG_UQ_RECEIVER(receiver)->notification!=NULL)
        	     {
        	         COMNotify(MSG_UQ_RECEIVER(receiver)->notification);	
        	     }
        	     receiver=MSG_UQ_RECEIVER(receiver)->next;
        	     break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)   */        	     
        	default: 
        	     goto EXCEPTION;/*��Ϊû���ڲ������ߣ��������߼�¼���ڲ������ߣ������쳣*/
        	     break;
        }    
   }/*֪ͨ����*/
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)||\
            defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/
EXCEPTION:
    return E_OK;
}


/*
*************************************************************************************
* StatusType COMReceiveIntMessage(SymbolicName message,ApplicationDataRef data)
*
* ������            �ڲ����ܣ���message���ڵ���Ϣ��������ݿ�����data
*
* ���õĺ�����      COMFilterMessage
*
* �������õ�λ�ã�
*
* ������	        message     ������Ϣ��������
*					data     �û��������ڵ�ַ
*
* ���أ�	        ��׼״̬��  E_OK         ��Ϣ���ճɹ�
*					            E_COM_NOMSG  ������Ϣ����messageΪ��
*                               E_COM_LIMIT  ��Ϣ�������
*
*         
*                   ��չ״̬��  E_COM_ID     ������Ϣ����messageԽ�磻messageΪ����
*                                            ��Ϣ���󣻸���ϢΪ�㳤�Ȼ�Ϊ��̬����							             
*
* ע�����	    
*
* �����ˣ�          ����
*
* ʱ�䣺            2005��7��18��
*
* �޸��ˣ�          ������
* 
* �޸�ʱ�䣺        2005��11��29�� 
*************************************************************************************
*/
StatusType COMReceiveIntMessage(SymbolicName message,ApplicationDataRef data)
{
    DEFINECRITICAL(cpu_sr);
    switch(*(MessageType*)message)
    {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)     
    	case COM_RECEIVE_QUEUED_INTERNAL:/*���н���*/
#ifdef COM_EXTENDED_STATUS     	
    	    if( (message<MsgIntQuReceiverTbl )
    	       || (message >= (MsgIntQuReceiverTbl+COM_NUM_INT_QUEUED_RECEIVER))
    	       ||(((AddrType)message-(AddrType)MsgIntQuReceiverTbl)%sizeof(MsgIntQuReceiver)!=0) )
    	    {   /*����Ƿ�Խ��*/
    	    	COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    	    }
#endif/*#ifdef COM_EXTENDED_STATUS*/ 
            OS_ENTER_CRITICAL();
            if(0==MSG_QU_RECEIVER(message)->QueueCounter)/*����Ϣ*/
            {
                MSG_QU_RECEIVER(message)->QueueStatus=COM_QUEUE_MSG_NONE;
                COMRETERROR(E_COM_NOMSG,COMServiceId_ReceiveMessage,message);
            }        
            memcpy(data,(void*)(MSG_QU_RECEIVER(message)->QueueRead),MSG_QU_RECEIVER(message)->DataSize);
            if(MSG_QU_RECEIVER(message)->QueueRead==MSG_QU_RECEIVER(message)->QueueEnd)
            {    /*�����������β*/
                MSG_QU_RECEIVER(message)->QueueRead = MSG_QU_RECEIVER(message)->QueueStart;/*��Ϣ�������ʼָ��*/
                MSG_QU_RECEIVER(message)->QueueCounter--;
            }
            else
            {
                MSG_QU_RECEIVER(message)->QueueRead += MSG_QU_RECEIVER(message)->DataSize;
                MSG_QU_RECEIVER(message)->QueueCounter--;

            }
            
#ifdef	COM_NOTIFICATION_FLAG
            if(MSG_QU_RECEIVER(message)->notification->type == COM_NOTIFICATION_FLAG)                
            COMResetFlag(MSG_QU_RECEIVER(message)->notification->parameter.flag);/*����FLAG*/                    
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/

            if(MSG_QU_RECEIVER(message)->QueueStatus==COM_QUEUE_MSG_OVERFLOW)
            {     /*�Ѿ������*/
                MSG_QU_RECEIVER(message)->QueueStatus=
                  (0==MSG_QU_RECEIVER(message)->QueueCounter)?COM_QUEUE_MSG_NONE:COM_QUEUE_MSG_NORMAL;
                OS_EXIT_CRITICAL();
                COMRETERROR(E_COM_LIMIT,COMServiceId_ReceiveMessage,message);
            }            
            if(0==MSG_QU_RECEIVER(message)->QueueCounter)/*���������Ϣ������Ϊ����Ϣ*/
            {
                MSG_QU_RECEIVER(message)->QueueStatus=COM_QUEUE_MSG_NONE;
            }
            
           
            OS_EXIT_CRITICAL();
            break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)  */  
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)      	
        case COM_RECEIVE_UNQUEUED_INTERNAL:/*�Ƕ��н���*/
#ifdef COM_EXTENDED_STATUS     	
    	    if( (message<MsgIntUqReceiverTbl )
    	       || (message >= (MsgIntUqReceiverTbl+COM_NUM_INT_UNQUEUED_RECEIVER))
    	       ||(((AddrType)message-(AddrType)MsgIntUqReceiverTbl)%sizeof(MsgIntUqReceiver)!=0) )
    	    {   /*����Ƿ�Խ��*/
    	    	COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    	    }
#endif/*#ifdef COM_EXTENDED_STATUS*/ 
            OS_ENTER_CRITICAL();    	    
    	    memcpy(data,MSG_UQ_RECEIVER(message)->buffer,MSG_UQ_RECEIVER(message)->DataSize);
#ifdef	COM_NOTIFICATION_FLAG
            if(MSG_UQ_RECEIVER(message)->notification->type == COM_NOTIFICATION_FLAG)                
            COMResetFlag(MSG_QU_RECEIVER(message)->notification->parameter.flag);/*����FLAG*/                    
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/
    	    OS_EXIT_CRITICAL();
            break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)      	*/            
        default:
            break;
    }    
    
    return E_OK;
}

/*
*************************************************************************************
* StatusType SendZeroMessage(SymbolicName sender)
*
* ������            ������Ϣ����Ϊ�㳤�ȣ�û�з��Ͷ��������������Ϣ����
*                   ������Ϣ������Ϣ���ơ�
*
* ���õĺ�����      
*
* �������õ�λ�ã�SendZeroMessage
*
* ������	        sender:      ��Ϣ��������
*
* ���أ�	        ��׼״̬��    E_OK:      ��Ϣ���ճɹ�
*					
*                   ��չ״̬��    E_COM_ID�� ����messageԽ�������ָ����㳤����Ϣ
*					
* ע�����	    �����ڲ�ͨ��
*
* �����ˣ�          ����
*
* ʱ�䣺            2004��12��22��
*
* �޸��ˣ�          ������
*
* ʱ�䣺            2005��3��8��
*************************************************************************************
*/
#if defined(COM_SEND_ZERO_INTERNAL)

StatusType SendZeroMessage(SymbolicName sender)
{
    DEFINECRITICAL(cpu_sr);

#ifdef COM_EXTENDED_STATUS
    
    if( (sender<MsgInSenderTbl) || (sender>(MsgInSenderTbl+COM_NUM_INT_SENDER))
        || (((AddrType)sender-(AddrType)MsgInSenderTbl)%sizeof(MsgIntSender)!=0))
        || ((*(MessageType*)message)!=COM_SEND_ZERO_INTERNAL))
    {    /*����Ƿ�Խ�硢�Ƿ����Ƿ�ΪCOM_SEND_ZERO_INTERNAL*/
        COMRETERROR(E_COM_ID,COMServiceId_SendZeroMessage,message);
    }
#endif/*#ifdef COM_EXTENDED_STATUS*/
	
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
    for(receiver=((MsgIntSender *)sender)->ReceiverList;NULL!=receiver;)
    {/*ѭ��������ǰ��Ϣ����Ľ����ߣ�ʵ��֪ͨ����*/
        switch(*(MessageType*)(receiver))/*�ж�������*/
        {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)          
        	case COM_RECEIVE_QUEUED_INTERNAL:/*���н���*/
#ifdef COM_NOTIFICATION_NONE        	
        	     if(MSG_QU_RECEIVER(receiver)->notification->type!=COM_NOTIFICATION_NONE)
#endif/*#ifdef COM_NOTIFICATION_NONE  */        	     
        	     {
        	         COMNotify(MSG_QU_RECEIVER(receiver)->notification);	
        	     }
        	     receiver=MSG_QU_RECEIVER(receiver)->next;
        	     break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */   
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)        	     
        	case COM_RECEIVE_UNQUEUED_INTERNAL:/*�Ƕ��н���*/
#ifdef COM_NOTIFICATION_NONE        	
        	     if(MSG_UQ_RECEIVER(receiver)->notification->type!=COM_NOTIFICATION_NONE)
#endif/*#ifdef COM_NOTIFICATION_NONE*/        	     
        	     {
        	         COMNotify(MSG_QU_RECEIVER(receiver)->notification);	
        	     }
        	     receiver=MSG_UQ_RECEIVER(receiver)->next;
        	     break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)   */        	     
        	default: 
        	     goto EXCEPTION;/*��Ϊû���ڲ������ߣ��������߼�¼���ڲ������ߣ������쳣*/
        	     break;
        }    
    }/*֪ͨ����*/
EXCEPTION: 
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)||\
            defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/
    return E_OK;
}
#endif/*#if defined(SEND_ZERO_INTERNAL)*/


/*
*************************************************************************************
* StatusType GetMessageStatus(SymbolicName message)
*
* ������            OSEK COM API����ȡ������Ϣ��״̬����״̬��status�ֶα���
*
* ���õĺ�����      
*
* �������õ�λ�ã�
*
* ������	        
*
* ���أ�	        ��׼״̬��    E_OK:        ������״��      			 
*		 			              E_COM_NOMSG: ��Ϣ���п�     
*		 			              E_COM_LIMIT: ��Ϣ������   
*					
*                   ��չ״̬��    E_COM_ID:    message����Խ�������û��ָ�������Ϣ  
*
* ע�����	    ֻ�ж��н����߲���ʹ��GetMessageStatus 
*
* �����ˣ�          ����
*
* ʱ�䣺            2004��12��27��
*
* �޸��ˣ�          ������
*
* ʱ�䣺            2005��3��8��
*
* �޸��ˣ�          ����
*
* ʱ�䣺            2005��7��18��
*************************************************************************************
*/
#ifdef COM_RECEIVE_QUEUED_INTERNAL
StatusType GetMessageStatus(SymbolicName message)
{
    DEFINECRITICAL(cpu_sr);

#ifdef COM_EXTENDED_STATUS
    if( (message<MsgIntQuReceiverTbl )
    	  || (message >= (MsgIntQuReceiverTbl+COM_NUM_INT_QUEUED_RECEIVER))
    	  ||(((AddrType)message-(AddrType)MsgIntQuReceiverTbl)%sizeof(MsgIntQuReceiver)!=0) )
    {   /*����Ƿ�Խ��,�Ƿ�Ϊ*/
    	  COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    }
#endif/*#ifdef COM_EXTENDED_STATUS*/ 
	
    OS_ENTER_CRITICAL();
    switch(MSG_QU_RECEIVER(message)->QueueStatus)
    {
        case COM_QUEUE_MSG_NORMAL:
                return E_OK;
        case COM_QUEUE_MSG_NONE:                
                COMRETERROR(E_COM_NOMSG,COMServiceId_GetMessageStatus,message);
        case COM_QUEUE_MSG_OVERFLOW:                
                COMRETERROR(E_COM_LIMIT,COMServiceId_GetMessageStatus,message);
        default:
                return	E_OK;         
    }
}
#endif/*#ifdef COM_RECEIVE_QUEUED_INTERNAL*/
/*
*************************************************************************************
* StatusType InitMessage(SymbolicName message,ApplicationDataRef dataref)
*
* ������            OSEK COM API����ʼ����Ϣ����
*
* ���õĺ�����      
*
* �������õ�λ�ã�
*
* ������	        
*
* ���أ�	        ��׼״̬��E_OK:     ��ʼ����Ϣ�ɹ�
*        			
*                   ��չ״̬��E_COM_ID: message����Խ�磬�ڲ�������Ϣ�����㳤����Ϣ 
*					
* ע�����	    Ŀǰֻʵ���ڲ�ͨ�ŵĳ�ʼ�������ⲿͨ���е�Dynamic Message Objectû�н��г�ʼ��
*
* �����ˣ�          ����
*
* ʱ�䣺            2004��12��27��
*
* �޸��ˣ�          ������
*
* ʱ�䣺            2005��3��8��
*
* �޸��ˣ�          ����
*
* ʱ�䣺            2005��7��19��
* 
* �޸��ˣ�          ������
* 
* �޸�ʱ�䣺        2005��11��29�� 
*************************************************************************************
*/
#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL)
StatusType InitMessage(SymbolicName message,ApplicationDataRef dataref)
{
    DEFINECRITICAL(cpu_sr);
    switch(*(MessageType*)(message))
    {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)          
      	case COM_RECEIVE_QUEUED_INTERNAL:/*���н���*/
#ifdef COM_EXTENDED_STATUS     	
    	    if( (message<MsgIntQuReceiverTbl )
    	       || (message >= (MsgIntQuReceiverTbl+COM_NUM_INT_QUEUED_RECEIVER))
    	       ||(((AddrType)message-(AddrType)MsgIntQuReceiverTbl)%sizeof(MsgIntQuReceiver)!=0) )
    	    {   /*����Ƿ�Խ��*/
    	    	COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    	    }
#endif/*#ifdef COM_EXTENDED_STATUS*/ 
      	
      	     OS_ENTER_CRITICAL();
       	     MSG_QU_RECEIVER(message)->QueueCounter=0;/*��Ϣ�еĽ��ܵ�������Ϊ0*/
             MSG_QU_RECEIVER(message)->QueueStatus=COM_QUEUE_MSG_NONE;/*״̬Ϊ��*/
			     if(MSG_QU_RECEIVER(message)->OldData && MSG_QU_RECEIVER(message)->DataSize < COM_MAX_FILTER_BYTE) 
             memcpy(MSG_QU_RECEIVER(message)->OldData, dataref, MSG_QU_RECEIVER(message)->DataSize);/*���þ�����*/      	
             OS_EXIT_CRITICAL();
       	     break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */   
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)        	     
        case COM_RECEIVE_UNQUEUED_INTERNAL:/*�Ƕ��н���*/
#ifdef COM_EXTENDED_STATUS     	
    	    if( (message<MsgIntUqReceiverTbl )
    	       || (message >= (MsgIntUqReceiverTbl+COM_NUM_INT_UNQUEUED_RECEIVER))
    	       ||(((AddrType)message-(AddrType)MsgIntUqReceiverTbl)%sizeof(MsgIntUqReceiver)!=0) )
    	    {   /*����Ƿ�Խ��*/
    	    	COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    	    }
#endif/*#ifdef COM_EXTENDED_STATUS*/         
             OS_ENTER_CRITICAL();
             memcpy(MSG_UQ_RECEIVER(message)->buffer, dataref, MSG_UQ_RECEIVER(message)->DataSize);/*��������*/
             if(MSG_UQ_RECEIVER(message)->OldData && MSG_UQ_RECEIVER(message)->DataSize < COM_MAX_FILTER_BYTE)
             memcpy(MSG_UQ_RECEIVER(message)->OldData, dataref, MSG_UQ_RECEIVER(message)->DataSize);/*���þ�����*/      	
             OS_EXIT_CRITICAL();
             break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)   */        	     
        default: 
             cpu_sr=cpu_sr;/*��ֹ����Warning*/
             break;
    }
    
    return E_OK;
}
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/