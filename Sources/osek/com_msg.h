/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：com_msg.h
* 摘    要：定义与消息对象有关的数据结构
*
* 当前版本：1.1
* 作    者：厉蒋
* 完成日期：2005年7月17日
*********************************************************************************
*/
#ifndef COM_MSG_H
#define COM_MSG_H

#include "os_task.h"
//#include "os_evt.h"

#define COM_FILTER 0x01
#define COM_NON_FILTER 0x00
typedef INT8U   *DataPtrType;
typedef INT8U   DataType;
typedef INT64U  ValueType;
typedef	void	(*COMCallBack)(void);
typedef INT8U MessageType; 
typedef INT8U *LengthRef; /*用户数据长度的地址*/
typedef INT8U SenderType;
typedef INT8U FlagValue; /*flag当前的值*/ 
typedef enum {COM_FALSE,COM_TRUE}  CalloutReturnType; 
typedef void * SymbolicName;                                  

/*
*********************************************************************************
* 通知机制配置，用于用户的配置
*********************************************************************************
*/
typedef struct
{
	INT8U    type;/*指明类型，包括COM_NOTIFICATION_NONE, COM_NOTIFICATION_SETEVENT，
                        COM_NOTIFICATION_ACTIVATETASK，COM_NOTIFICATION_COMCALLBACK等*/
    PriorityType	TaskID;  /*任务的ID*/
    INT8U    FlagID;	   /*标志位的指针，指向COMFlags[COM_NUM_FLAG]数组*/
    EventMaskRefType event;   /*事件的ID*/
    COMCallBack CallBack; /*CALLBACK函数名*/
}COMNtCfg;

/*
*********************************************************************************
* 通知机制
*********************************************************************************
*/
typedef struct/*8或12字节*/
{
	INT8U    type;/*指明类型，包括COM_NOTIFICATION_NONE, COM_NOTIFICATION_SETEVENT，
                        COM_NOTIFICATION_ACTIVATETASK，COM_NOTIFICATION_COMCALLBACK等*/
    union
    {
        void * non;/*用于填充*/
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)
        TaskType	task;  /*任务的指针，当初始化时，需要将TaskID索引的指针赋给该值*/
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)*/
#if defined(COM_NOTIFICATION_FLAG)
        FlagValue *	flag;	   /*标志位的指针，指向COMFlags[COM_NUM_FLAG]数组*/
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/
#if defined(COM_NOTIFICATION_COMCALLBACK)
        COMCallBack CallBack; /*CALLBACK函数名*/
#endif/*#if defined(COM_NOTIFICATION_COMCALLBACK)*/
    }parameter;
#if	defined(COM_NOTIFICATION_SETEVENT)    
    EventMaskRefType	event;   /*事件的ID*/
#endif/*#if	defined(COM_NOTIFICATION_SETEVENT)  */
}COMNotification;

/*
*************************************************************************************
*
* 过滤器配置的结构，用于用户的配置
*
*************************************************************************************
*/
typedef struct 
{
    INT8U FilterAlgorithm;
    ValueType mask;
    ValueType max;
    ValueType period;
    ValueType x;
  	ValueType min; /*用于NEWISWITHIN和NEWISOUTSIDE算法*/
    ValueType offset; /*用于ONEEVERYN算法*/
}COMFilterCfg;
    
/*
*************************************************************************************
*
* 过滤器的结构 定义过滤算法和其子属性，该数据结构参考OIL进行设计
*
*************************************************************************************
*/
typedef struct 
{
    INT8U FilterAlgorithm;
    union
    {
#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)\
							||defined(COM_MASKEDNEWEQUALSMASKEDOLD)\
							||defined(COM_MASKEDNEWDIFFERSMASKEDOLD) 
        ValueType mask;
#endif/* defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)\
							||defined(COM_MASKEDNEWEQUALSMASKEDOLD)\
							||defined(COM_MASKEDNEWDIFFERSMASKEDOLD) */
							

#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)
        ValueType max;
#endif/*#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)*/

#if defined(COM_ONEEVERYN)
        ValueType period;
#endif/*#if defined(COM_ONEEVERYN)*/
        ValueType non;/*多余的，防止编译错误*/ 
    }parameter1;
    union
    {
#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)
        ValueType x;
#endif/*#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)*/
#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)    
    	ValueType min; /*用于NEWISWITHIN和NEWISOUTSIDE算法*/
#endif/*#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)*/
#if defined(COM_ONEEVERYN)
        ValueType offset; /*用于ONEEVERYN算法*/
#endif/*#if defined(COM_ONEEVERYN)*/ 
        ValueType non;/*多余的，防止编译错误*/   
        ValueType non1;/*多余的，防止编译错误*/     	
    }parameter2;
}COMFilter;


/*
************************************************************************************
* 内部发送的消息对象的配置数据结构 
************************************************************************************
*/
typedef struct 
{
	INT8U    DataProperty;/*表明数据是否为无符号整数*/	
	INT32U   DataSize; /*数据类型的大小*/	
}COMMsgIntSenderCfg;


/*
************************************************************************************
* 内部发送的消息对象
************************************************************************************
*/
typedef struct /*大小12字节*/
{
	MessageType property;/*消息对象类型，该数据域得放数据结构的最前面，
	                       因为可以方便得通过该结构体的指针访问*/
	INT8U    DataProperty;/*表明数据是否为无符号整数*/	
	INT32U    DataSize;    /*数据类型的大小*/	
	SymbolicName  ReceiverList;/*接受者链表*/	
}MsgIntSender;


/*
************************************************************************************
* 内部队列接受的消息对象的配置数据结构
************************************************************************************
*/
typedef struct 
{
	MessageType property;/*消息对象类型，该数据域得放数据结构的最前面，
	                       因为可以方便得通过该结构体的指针访问*/
    INT8U  SendMsgIndex;/*发送者索引*/	                       	
	INT32U    QueueSize;/*队列大小，如果为非队列，则为1*/
	COMFilterCfg * filter; /*过滤算法*/	
	DataPtrType buffer; /*接受者buffer的起始指针*/
	COMNtCfg * notification;/*通知机制*/	
     /*内部通信没有错误通知*/
	ValueType    InitValue;/*初始化值*/
}COMMsgIntReceiverCfg;
	
/*
************************************************************************************
* 内部非队列接受的消息对象
************************************************************************************
*/
typedef struct /*大小32字节*/
{
	MessageType property;/*消息对象类型，该数据域得放数据结构的最前面，
	                       因为可以方便得通过该结构体的指针访问*/
	SymbolicName next;/*与同一发送者的接受者链表*/ /*!!!该域必须在结构体的第4个字节开始处*/
	INT32U    DataSize;    /*数据类型的大小*/	
	COMFilter * filter; /*过滤算法*/		
	DataPtrType buffer; /*接受者缓存*/
	COMNotification * notification;
#if defined(COM_ONEEVERYN)    
   	INT32U occurrence; /*记录消息对象被发送或接收的次数，此字段只供过滤用。*/
#endif/*#if defined(COM_ONEEVERYN) */ 
    ValueType *   OldData;  /*用于过滤算法*/
}MsgIntUqReceiver;
/*
************************************************************************************
* 内部队列接受的消息对象
************************************************************************************
*/
typedef struct /*大小52字节*/
{
	MessageType property;/*消息对象类型，该数据域得放数据结构的最前面，
	                       因为可以方便得通过该结构体的指针访问*/
	INT8U 		QueueStatus; /*队列状态,0x00:正常，0x01:溢出，0x10:无消息*/
	SymbolicName next;/*与同一发送者的接受者链表*/ /*!!!该域必须在结构体的第4个字节开始处*/
	INT32U     DataSize;    /*数据类型的大小*/	
	INT32U      QueueSize;/*队列的大小*/
	INT32U      QueueCounter;/*队列的已写的个数*/
	DataPtrType QueueStart;/*记录Queue起始指针*/
	DataPtrType QueueEnd;/*记录Queue末尾指针*/
	DataPtrType QueueWrite; /*队列数据域写的指针*/
	DataPtrType QueueRead;   /*队列数据域读的首指针*/
    COMFilter * filter; /*过滤算法*/
    COMNotification * notification; 
#if defined(COM_ONEEVERYN)    
   	INT32U occurrence; /*记录消息对象被发送或接收的次数，此字段只供过滤用。*/
#endif/*#if defined(COM_ONEEVERYN) */   		
    
	INT64U *    OldData;  /*用于过滤算法*/
}MsgIntQuReceiver;

#ifdef COM_SEND_STATIC_INTERNAL
extern MsgIntSender MsgInSenderTbl[];/*内部发送对象数组*/
#endif/*#ifdef COM_SEND_STATIC_INTERNAL*/

#if defined(COM_RECEIVE_UNQUEUED_INTERNAL) 
extern MsgIntUqReceiver MsgIntUqReceiverTbl[];/*内部非队列接受消息对象数组*/
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/

#if defined(COM_RECEIVE_QUEUED_INTERNAL)
extern MsgIntQuReceiver MsgIntQuReceiverTbl[];/*内部队列接受消息对象数组*/
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)*/



#endif/*ifndef COM_MSG_H*/

