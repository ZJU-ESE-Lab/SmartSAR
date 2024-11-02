/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�com_msg.h
* ժ    Ҫ����������Ϣ�����йص����ݽṹ
*
* ��ǰ�汾��1.1
* ��    �ߣ�����
* ������ڣ�2005��7��17��
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
typedef INT8U *LengthRef; /*�û����ݳ��ȵĵ�ַ*/
typedef INT8U SenderType;
typedef INT8U FlagValue; /*flag��ǰ��ֵ*/ 
typedef enum {COM_FALSE,COM_TRUE}  CalloutReturnType; 
typedef void * SymbolicName;                                  

/*
*********************************************************************************
* ֪ͨ�������ã������û�������
*********************************************************************************
*/
typedef struct
{
	INT8U    type;/*ָ�����ͣ�����COM_NOTIFICATION_NONE, COM_NOTIFICATION_SETEVENT��
                        COM_NOTIFICATION_ACTIVATETASK��COM_NOTIFICATION_COMCALLBACK��*/
    PriorityType	TaskID;  /*�����ID*/
    INT8U    FlagID;	   /*��־λ��ָ�룬ָ��COMFlags[COM_NUM_FLAG]����*/
    EventMaskRefType event;   /*�¼���ID*/
    COMCallBack CallBack; /*CALLBACK������*/
}COMNtCfg;

/*
*********************************************************************************
* ֪ͨ����
*********************************************************************************
*/
typedef struct/*8��12�ֽ�*/
{
	INT8U    type;/*ָ�����ͣ�����COM_NOTIFICATION_NONE, COM_NOTIFICATION_SETEVENT��
                        COM_NOTIFICATION_ACTIVATETASK��COM_NOTIFICATION_COMCALLBACK��*/
    union
    {
        void * non;/*�������*/
#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)
        TaskType	task;  /*�����ָ�룬����ʼ��ʱ����Ҫ��TaskID������ָ�븳����ֵ*/
#endif/*#if defined(COM_NOTIFICATION_SETEVENT)||defined(COM_NOTIFICATION_ACTIVATETASK)*/
#if defined(COM_NOTIFICATION_FLAG)
        FlagValue *	flag;	   /*��־λ��ָ�룬ָ��COMFlags[COM_NUM_FLAG]����*/
#endif/*#ifdef	COM_NOTIFICATION_FLAG*/
#if defined(COM_NOTIFICATION_COMCALLBACK)
        COMCallBack CallBack; /*CALLBACK������*/
#endif/*#if defined(COM_NOTIFICATION_COMCALLBACK)*/
    }parameter;
#if	defined(COM_NOTIFICATION_SETEVENT)    
    EventMaskRefType	event;   /*�¼���ID*/
#endif/*#if	defined(COM_NOTIFICATION_SETEVENT)  */
}COMNotification;

/*
*************************************************************************************
*
* ���������õĽṹ�������û�������
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
  	ValueType min; /*����NEWISWITHIN��NEWISOUTSIDE�㷨*/
    ValueType offset; /*����ONEEVERYN�㷨*/
}COMFilterCfg;
    
/*
*************************************************************************************
*
* �������Ľṹ ��������㷨���������ԣ������ݽṹ�ο�OIL�������
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
        ValueType non;/*����ģ���ֹ�������*/ 
    }parameter1;
    union
    {
#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)
        ValueType x;
#endif/*#if defined(COM_MASKEDNEWEQUALSX)||defined(COM_MASKEDNEWDIFFERSX)*/
#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)    
    	ValueType min; /*����NEWISWITHIN��NEWISOUTSIDE�㷨*/
#endif/*#if defined(COM_NEWISWITHIN)||defined(COM_NEWISOUTSIDE)*/
#if defined(COM_ONEEVERYN)
        ValueType offset; /*����ONEEVERYN�㷨*/
#endif/*#if defined(COM_ONEEVERYN)*/ 
        ValueType non;/*����ģ���ֹ�������*/   
        ValueType non1;/*����ģ���ֹ�������*/     	
    }parameter2;
}COMFilter;


/*
************************************************************************************
* �ڲ����͵���Ϣ������������ݽṹ 
************************************************************************************
*/
typedef struct 
{
	INT8U    DataProperty;/*���������Ƿ�Ϊ�޷�������*/	
	INT32U   DataSize; /*�������͵Ĵ�С*/	
}COMMsgIntSenderCfg;


/*
************************************************************************************
* �ڲ����͵���Ϣ����
************************************************************************************
*/
typedef struct /*��С12�ֽ�*/
{
	MessageType property;/*��Ϣ�������ͣ���������÷����ݽṹ����ǰ�棬
	                       ��Ϊ���Է����ͨ���ýṹ���ָ�����*/
	INT8U    DataProperty;/*���������Ƿ�Ϊ�޷�������*/	
	INT32U    DataSize;    /*�������͵Ĵ�С*/	
	SymbolicName  ReceiverList;/*����������*/	
}MsgIntSender;


/*
************************************************************************************
* �ڲ����н��ܵ���Ϣ������������ݽṹ
************************************************************************************
*/
typedef struct 
{
	MessageType property;/*��Ϣ�������ͣ���������÷����ݽṹ����ǰ�棬
	                       ��Ϊ���Է����ͨ���ýṹ���ָ�����*/
    INT8U  SendMsgIndex;/*����������*/	                       	
	INT32U    QueueSize;/*���д�С�����Ϊ�Ƕ��У���Ϊ1*/
	COMFilterCfg * filter; /*�����㷨*/	
	DataPtrType buffer; /*������buffer����ʼָ��*/
	COMNtCfg * notification;/*֪ͨ����*/	
     /*�ڲ�ͨ��û�д���֪ͨ*/
	ValueType    InitValue;/*��ʼ��ֵ*/
}COMMsgIntReceiverCfg;
	
/*
************************************************************************************
* �ڲ��Ƕ��н��ܵ���Ϣ����
************************************************************************************
*/
typedef struct /*��С32�ֽ�*/
{
	MessageType property;/*��Ϣ�������ͣ���������÷����ݽṹ����ǰ�棬
	                       ��Ϊ���Է����ͨ���ýṹ���ָ�����*/
	SymbolicName next;/*��ͬһ�����ߵĽ���������*/ /*!!!��������ڽṹ��ĵ�4���ֽڿ�ʼ��*/
	INT32U    DataSize;    /*�������͵Ĵ�С*/	
	COMFilter * filter; /*�����㷨*/		
	DataPtrType buffer; /*�����߻���*/
	COMNotification * notification;
#if defined(COM_ONEEVERYN)    
   	INT32U occurrence; /*��¼��Ϣ���󱻷��ͻ���յĴ��������ֶ�ֻ�������á�*/
#endif/*#if defined(COM_ONEEVERYN) */ 
    ValueType *   OldData;  /*���ڹ����㷨*/
}MsgIntUqReceiver;
/*
************************************************************************************
* �ڲ����н��ܵ���Ϣ����
************************************************************************************
*/
typedef struct /*��С52�ֽ�*/
{
	MessageType property;/*��Ϣ�������ͣ���������÷����ݽṹ����ǰ�棬
	                       ��Ϊ���Է����ͨ���ýṹ���ָ�����*/
	INT8U 		QueueStatus; /*����״̬,0x00:������0x01:�����0x10:����Ϣ*/
	SymbolicName next;/*��ͬһ�����ߵĽ���������*/ /*!!!��������ڽṹ��ĵ�4���ֽڿ�ʼ��*/
	INT32U     DataSize;    /*�������͵Ĵ�С*/	
	INT32U      QueueSize;/*���еĴ�С*/
	INT32U      QueueCounter;/*���е���д�ĸ���*/
	DataPtrType QueueStart;/*��¼Queue��ʼָ��*/
	DataPtrType QueueEnd;/*��¼Queueĩβָ��*/
	DataPtrType QueueWrite; /*����������д��ָ��*/
	DataPtrType QueueRead;   /*���������������ָ��*/
    COMFilter * filter; /*�����㷨*/
    COMNotification * notification; 
#if defined(COM_ONEEVERYN)    
   	INT32U occurrence; /*��¼��Ϣ���󱻷��ͻ���յĴ��������ֶ�ֻ�������á�*/
#endif/*#if defined(COM_ONEEVERYN) */   		
    
	INT64U *    OldData;  /*���ڹ����㷨*/
}MsgIntQuReceiver;

#ifdef COM_SEND_STATIC_INTERNAL
extern MsgIntSender MsgInSenderTbl[];/*�ڲ����Ͷ�������*/
#endif/*#ifdef COM_SEND_STATIC_INTERNAL*/

#if defined(COM_RECEIVE_UNQUEUED_INTERNAL) 
extern MsgIntUqReceiver MsgIntUqReceiverTbl[];/*�ڲ��Ƕ��н�����Ϣ��������*/
#endif/*#if defined(COM_RECEIVE_UNQUEUED_INTERNAL)*/

#if defined(COM_RECEIVE_QUEUED_INTERNAL)
extern MsgIntQuReceiver MsgIntQuReceiverTbl[];/*�ڲ����н�����Ϣ��������*/
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)*/



#endif/*ifndef COM_MSG_H*/

