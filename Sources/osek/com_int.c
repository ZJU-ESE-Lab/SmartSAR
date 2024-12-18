/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：com_int.c
* 摘    要：内部通信模块的实现
*
* 当前版本：1.0
* 作    者：厉蒋
* 完成日期：2005年7月17日
*
*********************************************************************************
*/
#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

#define COM_MAX_FILTER_BYTE sizeof(INT64U)

#ifdef COM_SEND_STATIC_INTERNAL
extern COMMsgIntSenderCfg MsgIntSenderCfgTbl[];     /*内部发送配置数组，用户定义*/
MsgIntSender MsgInSenderTbl[COM_NUM_INT_SENDER];/*内部发送对象数组*/
#endif/*#ifdef COM_SEND_STATIC_INTERNAL*/

#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL) 

#if defined(COM_RECEIVE_QUEUED_INTERNAL)&&defined(COM_RECEIVE_UNQUEUED_INTERNAL) 
#define COM_NUM_INT_RECEIVER  ((COM_NUM_INT_QUEUED_RECEIVER)+(COM_NUM_INT_UNQUEUED_RECEIVER))/*定义内部接受者的数量*/

#elif defined(COM_RECEIVE_QUEUED_INTERNAL)&&(!defined(COM_RECEIVE_UNQUEUED_INTERNAL) )

#define COM_NUM_INT_RECEIVER  ((COM_NUM_INT_QUEUED_RECEIVER))/*定义内部接受者的数量*/

#elif defined(COM_RECEIVE_UNQUEUED_INTERNAL)&&(!defined(COM_RECEIVE_QUEUED_INTERNAL)) 

#define COM_NUM_INT_RECEIVER  ((COM_NUM_INT_UNQUEUED_RECEIVER))/*定义内部接受者的数量*/
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)&&defined(COM_RECEIVE_UNQUEUED_INTERNAL) */

extern COMMsgIntReceiverCfg MsgIntReceiverCfgTbl[]; /*内部接受配置数组，用户定义*/
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL) */

#if defined(COM_RECEIVE_UNQUEUED_INTERNAL) 
#define MSG_UQ_RECEIVER(object) ((MsgIntUqReceiver*)(object))
MsgIntUqReceiver MsgIntUqReceiverTbl[COM_NUM_INT_UNQUEUED_RECEIVER];/*内部非队列接受消息对象数组*/
static INT64U UqueuedOldData[COM_NUM_INT_UNQUEUED_RECEIVER];
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/

#if defined(COM_RECEIVE_QUEUED_INTERNAL)
#define MSG_QU_RECEIVER(object) ((MsgIntQuReceiver*)(object))
MsgIntQuReceiver MsgIntQuReceiverTbl[COM_NUM_INT_QUEUED_RECEIVER];/*内部队列接受消息对象数组*/
static INT64U QueuedOldData[COM_NUM_INT_QUEUED_RECEIVER];
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)*/

#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
/*通知机制的配置数组*/
static COMNotification COMNotificationTbl[COM_NUM_NOTIFICATION];/*通知机制*/
INT8U NtIndex=0;
#define COMInternalStop() (NtIndex = 0)
#else
#define COMInternalStop() 
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/

#if COM_NUM_FILTER>0
static COMFilter COMFilterTbl[COM_NUM_FILTER];/*过滤算法*/
#endif/*#if COM_NUM_FILTER>0*/

#if defined (COM_NOTIFICATION_FLAG)
FlagValue COMFlags[COM_NUM_FLAG];
#define	COMSetFlag(flag)    *(flag)=TRUE
#define COMResetFlag(flag)	*(flag)=FALSE
#endif/*#if defined (COM_NOTIFICATION_FLAG) || (COM_NOTIFICATIONERROR_FLAG)*/

/*
***************************************************************************
* static void COMSetFilter(COMFilter ** filter,COMFilterCfg *cfg)
* 描述：按照配置设置过滤器
* 调用的函数：无
* 允许被调用的位置：COMInternalInit
* 参数：	filter  要设置的过滤
*           cfg     过滤器的配置信息
* 返回：	无
* 注意事项：
* 创建人：厉蒋
* 时间：2005.7.20
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
     (*filter)->min=cfg->min; /*用于NEWISWITHIN和NEWISOUTSIDE算法*/
#endif/*#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)*/
#if defined(COM_ONEEVERYN)
     (*filter)->offset=cfg->offset; /*用于ONEEVERYN算法*/
#endif/*#if defined(COM_ONEEVERYN)*/ 
}
#endif/*#if COM_NUM_FILTER>0*/
/*
***************************************************************************
* static void COMSetNotification(COMNotification ** filter,COMNtCfg *cfg)
* 描述：按照配置设置过滤器
* 调用的函数：无
* 允许被调用的位置：COMInternalInit
* 参数：	nt 要设置的通知机制
*           cfg 配置信息
* 返回：	无
* 注意事项：
* 创建人：厉蒋
* 时间：2005.7.20
* 修改人：郁利吉
* 时间：2005.11.23 
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
    if(cfg->type == COM_NOTIFICATION_ACTIVATETASK)(*nt)->parameter.task=&OSTCBTbl[cfg->TaskID];  /*任务的指针*/
#endif/*#ifdef(COM_NOTIFICATION_ACTIVATETASK)*/

#ifdef COM_NOTIFICATION_SETEVENT
  if(cfg->type == COM_NOTIFICATION_SETEVENT)(*nt)->parameter.task=&OSTCBTbl[cfg->TaskID];  /*任务的指针*/
#endif/*#ifdef(COM_NOTIFICATION_SETEVENT)*/





#if defined(COM_NOTIFICATION_FLAG)
    if(cfg->type == COM_NOTIFICATION_FLAG)(*nt)->parameter.flag=&COMFlags[cfg->FlagID];	   /*标志位的指针，指向COMFlags[COM_NUM_FLAG]数组*/
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/
#if defined(COM_NOTIFICATION_COMCALLBACK)
    if(cfg->type == COM_NOTIFICATION_COMCALLBACK)(*nt)->parameter.CallBack=cfg->CallBack; /*CALLBACK函数名*/
#endif/*#if defined(COM_NOTIFICATION_COMCALLBACK)*/
#if	defined(COM_NOTIFICATION_SETEVENT)    
    if(cfg->type == COM_NOTIFICATION_SETEVENT)(*nt)->event=cfg->event;   /*事件的ID*/
#endif/*#if	defined(COM_NOTIFICATION_SETEVENT)  */
    NtIndex++;
}
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/
/*
***************************************************************************
* void COMInternalInit()
* 描述：内部通信初始化
* 调用的函数：无
* 允许被调用的位置：StartCOM
* 参数：	无
* 返回：	无
* 注意事项：该函数只适用于内部通信
* 创建人：厉蒋
* 时间：2005.7.17
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
    {    /*初始化发送消息对象数组*/
    	sender->property=COM_SEND_STATIC_INTERNAL;
    	sender->DataProperty=cfgSender->DataProperty;
    	sender->DataSize=cfgSender->DataSize; 
        sender->ReceiverList=NULL;
    }
#endif/*#ifdef COM_SEND_STATIC_INTERNAL*/    
#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL)     
    for(;cfgReceiver>=MsgIntReceiverCfgTbl;cfgReceiver--)
    {  /*初始化接受消息对象数组*/
    	switch((MessageType)(cfgReceiver->property))/*判断其类型*/
    	{
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)     	
    		case COM_RECEIVE_UNQUEUED_INTERNAL:/*非队列接受*/
    			unqueueReceiver->property=COM_RECEIVE_UNQUEUED_INTERNAL;
    			unqueueReceiver->buffer=cfgReceiver->buffer;
#if (COM_NUM_FILTER)>0     			/*如果没有Filter就都设置为NULL*/
    			if(cfgReceiver->filter==NULL)
    			{
#endif/*#if COM_NUM_FILTER>0 */    			
    				unqueueReceiver->filter=NULL;
#if (COM_NUM_FILTER)>0      			
                }
    			else
    			{
    				COMSetFilter(&(unqueueReceiver->filter),cfgReceiver->filter);/*设置Filter*/
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
    			    COMSetNotification(&(unqueueReceiver->notification),cfgReceiver->notification);/*设置通知机制*/		
    			}
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/    			
    			if((cfgReceiver->notification!=NULL) &&          /*有过滤*/
    			      ((sender->DataSize)<COM_MAX_FILTER_BYTE))/*不是非负整数就不需要过滤，也就不需要记录旧数据*/
    			{
    			    unqueueReceiver->OldData=uqdata++;/*设置OldData指针*/
    			    //memcpy(unqueueReceiver->OldData,&(cfgReceiver->InitValue),sender->DataSize);/*记录老数据，用户过滤*/	
    			    *unqueueReceiver->OldData = cfgReceiver->InitValue;
    			}
    			sender=&MsgInSenderTbl[cfgReceiver->SendMsgIndex];
    			unqueueReceiver->DataSize=sender->DataSize;/*数据大小*/
    			/*插入发送者的链表*/
    			unqueueReceiver->next=sender->ReceiverList;
	            sender->ReceiverList=unqueueReceiver;
    			/*拷贝初始值*/
    			memcpy(unqueueReceiver->buffer,(INT8U *)&(cfgReceiver->InitValue)+4-sender->DataSize,sender->DataSize);
    			unqueueReceiver--;
    			break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/      			
#if defined(COM_RECEIVE_QUEUED_INTERNAL)      			
    		case COM_RECEIVE_QUEUED_INTERNAL:/*队列接受*/
    		    queueReceiver->property=COM_RECEIVE_QUEUED_INTERNAL;
    		    queueReceiver->QueueCounter=0; 
    		    queueReceiver->QueueStatus=COM_QUEUE_MSG_NONE;
    		    queueReceiver->QueueSize=cfgReceiver->QueueSize;
    			queueReceiver->QueueStart=queueReceiver->QueueWrite=queueReceiver->QueueRead=cfgReceiver->buffer;
    			queueReceiver->QueueEnd=cfgReceiver->buffer+cfgReceiver->QueueSize+1;/*末尾指针*/
#if (COM_NUM_FILTER)>0     			/*如果没有Filter就都设置为NULL*/    			
    			if(cfgReceiver->filter==NULL)
    			{
#endif/*#if COM_NUM_FILTER>0 */    			
    				queueReceiver->filter=NULL;	
#if (COM_NUM_FILTER)>0     				
    			}
    			else
    			{
    				COMSetFilter(&(queueReceiver->filter),cfgReceiver->filter);/*设置Filter*/
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
    			    COMSetNotification(&(queueReceiver->notification),cfgReceiver->notification);/*设置通知机制*/
    			}
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)\
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/    			
    			if((cfgReceiver->notification!=NULL) &&          /*有过滤*/
    			      ((sender->DataSize)<COM_MAX_FILTER_BYTE))/*不是非负整数就不需要过滤，也就不需要记录旧数据*/
    			{
    				queueReceiver->OldData=qdata++;/*设置OldData指针*/
    			    memcpy(queueReceiver->OldData,(INT8U*)&(cfgReceiver->InitValue) + 4 - sender->DataSize,sender->DataSize);/*记录老数据，用户过滤*/	
    			}    			
    			sender=&MsgInSenderTbl[cfgReceiver->SendMsgIndex];
    			queueReceiver->DataSize=sender->DataSize;/*数据大小*/
    			queueReceiver->next=sender->ReceiverList;/*插入发送者的链表*/
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
* 描述：            COM 标准 API
*					对消息根据值进行过滤，内部函数
*
* 调用的函数：      
*					
*
* 允许被调用的位置：COMSendIntMessage
*					
*
* 参数：	        filter 过滤器
*			        oldData 旧数据
*                   newData 当前发送数据
*                   occurrence 次数
*
* 返回：	        1: 消息通过
*		            0: 消息被丢弃
*
* 注意事项：	    内部发送以及零长度和动态长度的消息不能进行过滤	
*
* 创建人：          孙洁、厉蒋
*
* 时间：            2004年12月10日
*
* 修改人：          吴先明
*
* 时间：            2005年3月8日
*
* 修改人：          孟晓楠
*
* 时间：            2005年6月28日
*
* 修改人：          厉蒋
*
* 时间：            2005年7月17日
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
/*全部通过，为过滤算法的默认值*/
#ifdef	COM_ALWAYS
    case COM_ALWAYS:
        return 1;
#endif/*#ifdef COM_ALWAYS*/
/*全部不通过*/
#ifdef	COM_NEVER
    case COM_NEVER:
        return 0;
#endif /*#ifdef	COM_NEVER*/

/*使mask等于特定值的消息通过*/
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

/*使mask不等于特定值的消息通过*/
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

/*使数据没有改变的消息通过*/
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

/*使数据改变的消息通过*/
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
    case COM_ONEEVERYN:/*注意:此语句需要在每次发送和接收消息时对变量pmsg->MsgOccurrence赋值。2004715还未作，以后需补上*/
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
        oldData=oldData;/*防止Warning*/
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
* 描述：            OSEK COM 内部函数；实现通知机制
*
* 调用的函数：      
*
* 允许被调用的位置：COMSendIntMessage
*					COMSendIntZeroMessage
*
* 参数：	        notification 通知的数据结构
*
* 返回：	        E_OK 设置成功
*					E_COM_ID 设置失败
*
* 注意事项：	    
*
* 创建人：          厉蒋
*
* 时间：            2004年12月27日
*
* 修改人：          吴先明
*
* 时间：            2005年3月8日
*
* 修改人：          厉蒋
*
* 时间：            2005年7月17日
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
	/*设置FLAG*/
#ifdef	COM_NOTIFICATION_FLAG
    case COM_NOTIFICATION_FLAG:
        OS_ENTER_CRITICAL();
        COMSetFlag(notification->parameter.flag);/*设置FLAG*/
        OS_EXIT_CRITICAL();
        break;
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/
	
	/*任务调用*/
#ifdef	COM_NOTIFICATION_ACTIVATETASK
    case COM_NOTIFICATION_ACTIVATETASK:
        OS_ENTER_CRITICAL();
        OSActivateTask((TaskType)notification->parameter.task);
        OS_EXIT_CRITICAL();
        break;
#endif/*#ifdef	COM_NOTIFICATION_ACTIVATETASK*/
	
	/*设置事件*/
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
        (notification->parameter.CallBack)();/*调用用户在设定的回调函数*/
        break;/*com301 p31 callback没有返回值*/
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
* 描述：            内部通信实现，利用sender将data发送给与sender相关的接受者。
*
* 调用的函数：      COMFilterMessage
*					COMSetNotification
*
* 允许被调用的位置：SendMessage
*
* 参数：	        sender   发送消息对象名称
*					data:     用户数据所在地址
*
* 返回：	        标准状态：E_OK          消息发送成功
* 
*				    扩展状态: E_COM_ID      消息参数sender越界；sender为接收消息对象；
*                                           sender为零长度或动态长度消息
*
* 注意事项：	    
*
* 创建人：          厉蒋
*
* 时间：            2005年7月17日
*
* 修改人：          郁利吉
* 
* 修改时间：        2005年11月29日 
*************************************************************************************
*/
StatusType COMSendIntMessage(SymbolicName sender,ApplicationDataRef data)
{
    DEFINECRITICAL(cpu_sr); 
    SymbolicName receiver=NULL;
#ifdef COM_EXTENDED_STATUS
    /*检查是否越界*/
    if( (sender<MsgInSenderTbl) || (sender>(MsgInSenderTbl+COM_NUM_INT_SENDER))
        || (((AddrType)sender-(AddrType)MsgInSenderTbl)%sizeof(MsgIntSender)!=0))
    {
        COMRETERROR(E_COM_ID,COMServiceId_SendMessage,sender);
    }
#endif/*#ifdef COM_EXTENDED_STATUS*/
  
    OS_ENTER_CRITICAL();
    for(receiver=((MsgIntSender*)sender)->ReceiverList;NULL!=receiver;)
    {/*循环遍历当前消息对象的接受者，把数据拷贝到其接受者的数据域中*/
        if(((MsgIntSender*)sender)->DataProperty == COM_FILTER) /*如果其数据类型可以过滤*/
        {
            /*内部接受端的过滤*/
            switch(*(MessageType*)(receiver))/*判断其类型*/
            {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)      	            
            	case COM_RECEIVE_QUEUED_INTERNAL:/*队列接受*/
            	    
            	    if( ((MSG_QU_RECEIVER(receiver)->filter)!=NULL)&&
            	        (0 == COMFilterMessage(MSG_QU_RECEIVER(receiver)->filter,data,MSG_QU_RECEIVER(receiver)->OldData)
#if defined(COM_ONEEVERYN)
                            ,MSG_QU_RECEIVER(receiver)->occurrence/*更新发送次数*/
#endif /*#if defined(COM_ONEEVERYN)*/           	    
            	         )
            	     )
                    {
                        /*当前接受者消息被丢弃*/
                        continue;/*下一个接受者*/
                    }
            	    break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)               	    
            	case COM_RECEIVE_UNQUEUED_INTERNAL:/*非队列接受*/
            	    if( ((MSG_UQ_RECEIVER(receiver)->filter)!=NULL)&&
            	     (0 == COMFilterMessage(MSG_UQ_RECEIVER(receiver)->filter,data,MSG_UQ_RECEIVER(receiver)->OldData)
#if defined(COM_ONEEVERYN)
                            ,MSG_QU_RECEIVER(receiver)->occurrence/*更新发送次数*/
#endif /*#if defined(COM_ONEEVERYN)*/           	    
            	      )      
            	    )
            	    
                    {
                        /*消息被丢弃*/
                        continue;
                    }           	    
                    break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)  */ 
                default:
                    break;                   
            }
            
        }
        switch(*(MessageType*)(receiver))/*判断其类型*/
        {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)           
        	case COM_RECEIVE_QUEUED_INTERNAL:/*队列接受*/
        	     
        	     if(MSG_QU_RECEIVER(receiver)->QueueCounter>=MSG_QU_RECEIVER(receiver)->QueueSize)/*队列已满*/
        	     {
        	         MSG_QU_RECEIVER(receiver)->QueueStatus=COM_QUEUE_MSG_OVERFLOW;/*设置标志位*/
        	     }
        	     else/*队列不满*/
        	     {
        	         memcpy((void*)(MSG_QU_RECEIVER(receiver)->QueueWrite),data,((MsgIntSender*)sender)->DataSize);
				     
                     if(MSG_QU_RECEIVER(receiver)->QueueWrite == MSG_QU_RECEIVER(receiver)->QueueEnd)
                     {    /*如果队列刚刚满，则把写指针指为队列头指针*/
                         MSG_QU_RECEIVER(receiver)->QueueWrite=MSG_QU_RECEIVER(receiver)->QueueStart;
                     }
                     else
                     {
                         MSG_QU_RECEIVER(receiver)->QueueWrite+=((MsgIntSender*)sender)->DataSize; /*因为按照字节流来拷贝*/
                     }

                     MSG_QU_RECEIVER(receiver)->QueueCounter++;
				     if(MSG_QU_RECEIVER(receiver)->OldData  && ((MsgIntSender*)sender)->DataSize<COM_MAX_FILTER_BYTE)
				     memcpy(MSG_QU_RECEIVER(receiver)->OldData,(DataPtrType)data,((MsgIntSender*)sender)->DataSize);/*更新用于过滤的旧数据*/
				     MSG_QU_RECEIVER(receiver)->QueueStatus=COM_QUEUE_MSG_NORMAL;/*状态重置，当原来是空消息状态的时候*/
        	     }
        	     receiver=MSG_QU_RECEIVER(receiver)->next; 
#if defined(COM_ONEEVERYN)
                 MSG_QU_RECEIVER(receiver)->occurrence++;/*更新发送次数*/
#endif /*#if defined(COM_ONEEVERYN)*/   
        	     break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */        	     
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL) 
        	case COM_RECEIVE_UNQUEUED_INTERNAL:/*非队列接受*/
        	     memcpy((void*)(MSG_UQ_RECEIVER(receiver)->buffer),data,((MsgIntSender*)sender)->DataSize);
        	     receiver=MSG_UQ_RECEIVER(receiver)->next;
#if defined(COM_ONEEVERYN)
                 MSG_UQ_RECEIVER(receiver)->occurrence++;/*更新发送次数*/
#endif /*#if defined(COM_ONEEVERYN)*/         	     
        	     break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)  */ 
        	default: 
        	     OS_EXIT_CRITICAL();
        	     
        	     OSEKPrintInt(*(MessageType*)(receiver));
        	     goto EXCEPTION;/*因为没有内部接受者，而发送者记录了内部接受者，所以异常*/
        	     break;
        }
    }
	OS_EXIT_CRITICAL();
    /*内部通信只有一类通知机制，即只有接受者的通知机制*/								
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)||\
    defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
    for(receiver=((MsgIntSender *)sender)->ReceiverList;NULL!=receiver;)
    {/*循环遍历当前消息对象的接受者，实现通知机制*/
        switch(*(MessageType*)(receiver))/*判断其类型*/
        {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)          
        	case COM_RECEIVE_QUEUED_INTERNAL:/*队列接受*/
        	     if(MSG_QU_RECEIVER(receiver)->notification!=NULL)
        	     {
        	         COMNotify(MSG_QU_RECEIVER(receiver)->notification);	
        	     }
        	     receiver=MSG_QU_RECEIVER(receiver)->next;
        	     break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */   
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)        	     
        	case COM_RECEIVE_UNQUEUED_INTERNAL:/*非队列接受*/
        	     if(MSG_UQ_RECEIVER(receiver)->notification!=NULL)
        	     {
        	         COMNotify(MSG_UQ_RECEIVER(receiver)->notification);	
        	     }
        	     receiver=MSG_UQ_RECEIVER(receiver)->next;
        	     break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)   */        	     
        	default: 
        	     goto EXCEPTION;/*因为没有内部接受者，而发送者记录了内部接受者，所以异常*/
        	     break;
        }    
   }/*通知机制*/
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)||\
            defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)*/
EXCEPTION:
    return E_OK;
}


/*
*************************************************************************************
* StatusType COMReceiveIntMessage(SymbolicName message,ApplicationDataRef data)
*
* 描述：            内部接受，将message所在的消息对象的数据拷贝到data
*
* 调用的函数：      COMFilterMessage
*
* 允许被调用的位置：
*
* 参数：	        message     接收消息对象名称
*					data     用户数据所在地址
*
* 返回：	        标准状态：  E_OK         消息接收成功
*					            E_COM_NOMSG  接收消息对象message为空
*                               E_COM_LIMIT  消息队列溢出
*
*         
*                   扩展状态：  E_COM_ID     接收消息对象message越界；message为发送
*                                            消息对象；该消息为零长度或为动态长度							             
*
* 注意事项：	    
*
* 创建人：          厉蒋
*
* 时间：            2005年7月18日
*
* 修改人：          郁利吉
* 
* 修改时间：        2005年11月29日 
*************************************************************************************
*/
StatusType COMReceiveIntMessage(SymbolicName message,ApplicationDataRef data)
{
    DEFINECRITICAL(cpu_sr);
    switch(*(MessageType*)message)
    {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)     
    	case COM_RECEIVE_QUEUED_INTERNAL:/*队列接受*/
#ifdef COM_EXTENDED_STATUS     	
    	    if( (message<MsgIntQuReceiverTbl )
    	       || (message >= (MsgIntQuReceiverTbl+COM_NUM_INT_QUEUED_RECEIVER))
    	       ||(((AddrType)message-(AddrType)MsgIntQuReceiverTbl)%sizeof(MsgIntQuReceiver)!=0) )
    	    {   /*检查是否越界*/
    	    	COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    	    }
#endif/*#ifdef COM_EXTENDED_STATUS*/ 
            OS_ENTER_CRITICAL();
            if(0==MSG_QU_RECEIVER(message)->QueueCounter)/*无消息*/
            {
                MSG_QU_RECEIVER(message)->QueueStatus=COM_QUEUE_MSG_NONE;
                COMRETERROR(E_COM_NOMSG,COMServiceId_ReceiveMessage,message);
            }        
            memcpy(data,(void*)(MSG_QU_RECEIVER(message)->QueueRead),MSG_QU_RECEIVER(message)->DataSize);
            if(MSG_QU_RECEIVER(message)->QueueRead==MSG_QU_RECEIVER(message)->QueueEnd)
            {    /*如果读到队列尾*/
                MSG_QU_RECEIVER(message)->QueueRead = MSG_QU_RECEIVER(message)->QueueStart;/*消息对象的起始指针*/
                MSG_QU_RECEIVER(message)->QueueCounter--;
            }
            else
            {
                MSG_QU_RECEIVER(message)->QueueRead += MSG_QU_RECEIVER(message)->DataSize;
                MSG_QU_RECEIVER(message)->QueueCounter--;

            }
            
#ifdef	COM_NOTIFICATION_FLAG
            if(MSG_QU_RECEIVER(message)->notification->type == COM_NOTIFICATION_FLAG)                
            COMResetFlag(MSG_QU_RECEIVER(message)->notification->parameter.flag);/*设置FLAG*/                    
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/

            if(MSG_QU_RECEIVER(message)->QueueStatus==COM_QUEUE_MSG_OVERFLOW)
            {     /*已经有溢出*/
                MSG_QU_RECEIVER(message)->QueueStatus=
                  (0==MSG_QU_RECEIVER(message)->QueueCounter)?COM_QUEUE_MSG_NONE:COM_QUEUE_MSG_NORMAL;
                OS_EXIT_CRITICAL();
                COMRETERROR(E_COM_LIMIT,COMServiceId_ReceiveMessage,message);
            }            
            if(0==MSG_QU_RECEIVER(message)->QueueCounter)/*如果读完消息，则置为无消息*/
            {
                MSG_QU_RECEIVER(message)->QueueStatus=COM_QUEUE_MSG_NONE;
            }
            
           
            OS_EXIT_CRITICAL();
            break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)  */  
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)      	
        case COM_RECEIVE_UNQUEUED_INTERNAL:/*非队列接受*/
#ifdef COM_EXTENDED_STATUS     	
    	    if( (message<MsgIntUqReceiverTbl )
    	       || (message >= (MsgIntUqReceiverTbl+COM_NUM_INT_UNQUEUED_RECEIVER))
    	       ||(((AddrType)message-(AddrType)MsgIntUqReceiverTbl)%sizeof(MsgIntUqReceiver)!=0) )
    	    {   /*检查是否越界*/
    	    	COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    	    }
#endif/*#ifdef COM_EXTENDED_STATUS*/ 
            OS_ENTER_CRITICAL();    	    
    	    memcpy(data,MSG_UQ_RECEIVER(message)->buffer,MSG_UQ_RECEIVER(message)->DataSize);
#ifdef	COM_NOTIFICATION_FLAG
            if(MSG_UQ_RECEIVER(message)->notification->type == COM_NOTIFICATION_FLAG)                
            COMResetFlag(MSG_QU_RECEIVER(message)->notification->parameter.flag);/*设置FLAG*/                    
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
* 描述：            由于消息数据为零长度，没有发送动作；但需根据消息对象
*                   配置信息启动消息机制。
*
* 调用的函数：      
*
* 允许被调用的位置：SendZeroMessage
*
* 参数：	        sender:      消息对象名称
*
* 返回：	        标准状态：    E_OK:      消息接收成功
*					
*                   扩展状态：    E_COM_ID： 参数message越界或者它指向非零长度消息
*					
* 注意事项：	    用于内部通信
*
* 创建人：          厉蒋
*
* 时间：            2004年12月22日
*
* 修改人：          吴先明
*
* 时间：            2005年3月8日
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
    {    /*检查是否越界、非法，是否为COM_SEND_ZERO_INTERNAL*/
        COMRETERROR(E_COM_ID,COMServiceId_SendZeroMessage,message);
    }
#endif/*#ifdef COM_EXTENDED_STATUS*/
	
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)
    ||defined(COM_NOTIFICATION_COMCALLBACK)||defined(COM_NOTIFICATION_FLAG)
    for(receiver=((MsgIntSender *)sender)->ReceiverList;NULL!=receiver;)
    {/*循环遍历当前消息对象的接受者，实现通知机制*/
        switch(*(MessageType*)(receiver))/*判断其类型*/
        {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)          
        	case COM_RECEIVE_QUEUED_INTERNAL:/*队列接受*/
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
        	case COM_RECEIVE_UNQUEUED_INTERNAL:/*非队列接受*/
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
        	     goto EXCEPTION;/*因为没有内部接受者，而发送者记录了内部接受者，所以异常*/
        	     break;
        }    
    }/*通知机制*/
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
* 描述：            OSEK COM API。读取队列消息的状态，该状态由status字段保存
*
* 调用的函数：      
*
* 允许被调用的位置：
*
* 参数：	        
*
* 返回：	        标准状态：    E_OK:        非以下状况      			 
*		 			              E_COM_NOMSG: 消息队列空     
*		 			              E_COM_LIMIT: 消息队列满   
*					
*                   扩展状态：    E_COM_ID:    message参数越界或者它没有指向队列消息  
*
* 注意事项：	    只有队列接受者才能使用GetMessageStatus 
*
* 创建人：          厉蒋
*
* 时间：            2004年12月27日
*
* 修改人：          吴先明
*
* 时间：            2005年3月8日
*
* 修改人：          厉蒋
*
* 时间：            2005年7月18日
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
    {   /*检查是否越界,是否为*/
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
* 描述：            OSEK COM API。初始化消息对象
*
* 调用的函数：      
*
* 允许被调用的位置：
*
* 参数：	        
*
* 返回：	        标准状态：E_OK:     初始化消息成功
*        			
*                   扩展状态：E_COM_ID: message参数越界，内部发送消息或是零长度消息 
*					
* 注意事项：	    目前只实现内部通信的初始化，对外部通信中的Dynamic Message Object没有进行初始化
*
* 创建人：          厉蒋
*
* 时间：            2004年12月27日
*
* 修改人：          吴先明
*
* 时间：            2005年3月8日
*
* 修改人：          厉蒋
*
* 时间：            2005年7月19日
* 
* 修改人：          郁利吉
* 
* 修改时间：        2005年11月29日 
*************************************************************************************
*/
#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL)
StatusType InitMessage(SymbolicName message,ApplicationDataRef dataref)
{
    DEFINECRITICAL(cpu_sr);
    switch(*(MessageType*)(message))
    {
#if defined(COM_RECEIVE_QUEUED_INTERNAL)          
      	case COM_RECEIVE_QUEUED_INTERNAL:/*队列接受*/
#ifdef COM_EXTENDED_STATUS     	
    	    if( (message<MsgIntQuReceiverTbl )
    	       || (message >= (MsgIntQuReceiverTbl+COM_NUM_INT_QUEUED_RECEIVER))
    	       ||(((AddrType)message-(AddrType)MsgIntQuReceiverTbl)%sizeof(MsgIntQuReceiver)!=0) )
    	    {   /*检查是否越界*/
    	    	COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    	    }
#endif/*#ifdef COM_EXTENDED_STATUS*/ 
      	
      	     OS_ENTER_CRITICAL();
       	     MSG_QU_RECEIVER(message)->QueueCounter=0;/*消息中的接受到的数据为0*/
             MSG_QU_RECEIVER(message)->QueueStatus=COM_QUEUE_MSG_NONE;/*状态为空*/
			     if(MSG_QU_RECEIVER(message)->OldData && MSG_QU_RECEIVER(message)->DataSize < COM_MAX_FILTER_BYTE) 
             memcpy(MSG_QU_RECEIVER(message)->OldData, dataref, MSG_QU_RECEIVER(message)->DataSize);/*设置旧数据*/      	
             OS_EXIT_CRITICAL();
       	     break;
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)   */   
#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)        	     
        case COM_RECEIVE_UNQUEUED_INTERNAL:/*非队列接受*/
#ifdef COM_EXTENDED_STATUS     	
    	    if( (message<MsgIntUqReceiverTbl )
    	       || (message >= (MsgIntUqReceiverTbl+COM_NUM_INT_UNQUEUED_RECEIVER))
    	       ||(((AddrType)message-(AddrType)MsgIntUqReceiverTbl)%sizeof(MsgIntUqReceiver)!=0) )
    	    {   /*检查是否越界*/
    	    	COMRETERROR(E_COM_ID,COMServiceId_ReceiveMessage,message);
    	    }
#endif/*#ifdef COM_EXTENDED_STATUS*/         
             OS_ENTER_CRITICAL();
             memcpy(MSG_UQ_RECEIVER(message)->buffer, dataref, MSG_UQ_RECEIVER(message)->DataSize);/*设置数据*/
             if(MSG_UQ_RECEIVER(message)->OldData && MSG_UQ_RECEIVER(message)->DataSize < COM_MAX_FILTER_BYTE)
             memcpy(MSG_UQ_RECEIVER(message)->OldData, dataref, MSG_UQ_RECEIVER(message)->DataSize);/*设置旧数据*/      	
             OS_EXIT_CRITICAL();
             break;
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)   */        	     
        default: 
             cpu_sr=cpu_sr;/*防止编译Warning*/
             break;
    }
    
    return E_OK;
}
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)||defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/