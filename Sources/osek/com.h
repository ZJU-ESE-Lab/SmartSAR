/*
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：com.h
* 摘    要：com模块数据结构及API定义
*
* 当前版本：1.1
* 作    者：吴先明
* 完成日期：2005年3月8日
*
* 取代版本：1.0
* 原作者  ：孙洁、厉蒋
* 完成日期：2004年12月10日
*/

#ifndef	 COM_H
#define	 COM_H
#include "com_msg.h"
/*
*************************************************************************************
* 全局变量的定义 
*               added by sj                         
*************************************************************************************
*/
/*added by sj,2004.6.15，以下为OSEK COM要求的返回值*/
#define E_OK            0
#define E_COM_ID        10 
#define E_COM_LIMIT     11      /*队列已满*/
#define E_COM_NOMSG     12      /*接收消息对象数据区空*/
#define E_COM_LENGTH    13
#define E_COM_FILTER    14      /*消息未通过过滤*/

#define	COM_QUEUE_MSG_NORMAL	0x0  /*队列消息的标志－正常*/
#define	COM_QUEUE_MSG_OVERFLOW	0x1  /*队列消息的标志－溢出*/
#define	COM_QUEUE_MSG_NONE		0x2  /*队列消息的标志－无消息*/
#define	E_COM_START_FAILED		0xFF /*初始化中的出错返回值，该值可以自定义的*/

typedef	void*	ApplicationDataRef;
typedef INT8U   COMApplicationModeType;
typedef enum    {COM_SHUTDOWN_IMMEDIATE}  COMShutdownModeType;
                                             /*COM关闭模式的标识符*/
                                            
       /*前16位为IPDU名称，其中前8位为ECU标识，
                                             后8位为IPDU内部标识。后16位为消息对象，
                                             前8位保留全0，后8位位消息对象内部标识
                                             MsgId。*/
											 
                                           
                                             /*当退出callout时IL是否还要继续处理当前
                                             消息：COM_FALSE 不处理，退出；
                                             COM_TRUE：继续处理*/                                        
#define COMCallback(name)  void name(void) /*ComCallout宏*/

#define  COMServiceId_NoService  (0x70)
#define  COMServiceId_StartCOM  ((COMServiceId_NoService)+0x01)
#define  COMServiceId_StopCOM		((COMServiceId_NoService)+0x02)

#define  COMServiceId_GetCOMApplicationMode		((COMServiceId_NoService)+0x03)
#define  COMServiceId_InitMessage		((COMServiceId_NoService)+0x04)
#define  COMServiceId_StartPeriodic		((COMServiceId_NoService)+0x05)
#define  COMServiceId_StopPeriodic		((COMServiceId_NoService)+0x06)
#define  COMServiceId_ResetFlag		((COMServiceId_NoService)+0x07)
#define  COMServiceId_SendMessage		((COMServiceId_NoService)+0x08)
#define  COMServiceId_ReceiveMessage		((COMServiceId_NoService)+0x09)
#define  COMServiceId_SendDynamicMessage		((COMServiceId_NoService)+0x0a)
#define  COMServiceId_ReceiveDynamicMessage		((COMServiceId_NoService)+0x0b)
#define  COMServiceId_SendZeroMessage		((COMServiceId_NoService)+0x0c)
#define  COMServiceId_GetMessageStatus		((COMServiceId_NoService)+0x0d)
#define  COMServiceId_COMErrorGetServiceId		((COMServiceId_NoService)+0x0e)
#define  COMServiceId_StartCOMExtension		((COMServiceId_NoService)+0x0f)
#define  COMServiceId_Callouts		((COMServiceId_NoService)+0x10)

typedef INT8U COMServiceIdType;/*OSEK COM Service唯一的标识符*/

#ifdef COM_NOTIFICATION_FLAG
   extern FlagValue COMFlags[COM_NUM_FLAG];
#endif/*#ifdef COM_NOTIFICATION_FLAG*/

/*
*************************************************************************************
*
* COMErrorHook
*
*************************************************************************************
*/
          
#ifdef COM_ERROR_HOOK
extern    INT8U  COMNestedErrorHook;
void COMErrorHook(StatusType Error );
#ifdef COM_HOOK_GET_SERVICEID

extern    COMServiceIdType  ServiceId;

#endif/*ifdef COM_HOOK_GET_SERVICEID*/

#ifdef COM_HOOK_PARA_ACCESS
 extern    INT32U    COMObjId;   
#endif /*ifdef COM_HOOK_PARA_ACCESS*/

                         
           
 
/*
*************************************************************************************
*
* 调用COMErrorHook的COM API的参数获取方法
*
*************************************************************************************
*/ 
#ifdef COM_HOOK_PARA_ACCESS

#define COMError_StartCOM_Mode()             (COMApplicationModeType)COMObjId
#define COMError_StopCOM_Mode()              (COMShutdownModeType)COMObjId
     
#define COMError_SendMessage_Message()       (SymbolicName)COMObjId
#define COMError_SendMessage_DataRef()       (ApplicationDataRef)COMObjId                   
#define COMError_ReceiveMessage_Message()    (SymbolicName)COMObjId                
#define COMError_ReceiveMessage_DataRef()    (ApplicationDataRef)COMObjId
#define COMError_SendZeroMessage_Message()   (SymbolicName)COMObjId  
    
#define COMError_GetMessageStatus_Message()  (SymbolicName)COMObjId
                    
#define COMError_InitMessage_Message()       (SymbolicName)COMObjId  
#define COMError_InitMessage_DataRef()       (ApplicationDataRef)COMObjId 

#ifdef COM_HOOK_GET_SERVICEID
      
#define COMSetServiceId(ID, param)\
        do{\
        if(ServiceId == COMServiceId_NoService)\
        {\
            ServiceId = ID; COMObjId = (INT32U) param;\
        }}while(0)

#else/*ifdef COM_HOOK_GET_SERVICEID*/

#define COMSetServiceId(ID, param)  COMObjId = (INT32U) param

#endif/*ifdef COM_HOOK_GET_SERVICEID*/  
      
#else /*#ifdef COM_HOOK_PARA_ACCESS*/

#ifdef COM_HOOK_GET_SERVICEID

#define COMSetServiceId(ID, param)\
        do{\
        if(ServiceId == COMServiceId_NoService)\
        {\
            ServiceId = ID;\
        }}while(0)
#else /*#ifdef COM_HOOK_GET_SERVICEID*/

#define COMSetServiceId(ID, param) 

#endif  /*#ifdef COM_HOOK_GET_SERVICEID*/

#endif /*#ifdef COM_HOOK_PARA_ACCESS*/             

#ifdef COM_HOOK_GET_SERVICEID

#define COMCancelServiceId()  ServiceId = COMServiceId_NoService

#else /*#ifdef COM_HOOK_GET_SERVICEID*/

#define COMCancelServiceId()  

#endif  /*COM_HOOK_GET_SERVICEID*/
#define SYSCOMErrorHook(error, ID, param) \
        if(COMNestedErrorHook == 0)\
        {\
            COMNestedErrorHook++;\
            SuspendOSInterrupts();\
            COMSetServiceId(ID, param);\
            COMErrorHook(error);\
            COMCancelServiceId();\
            ResumeOSInterrupts();\
            COMNestedErrorHook--;\
        }     
#define COMRETERROR(error, ID, param) \
        SYSCOMErrorHook(error, ID, param)\
        return error 

#else/*#ifdef COM_ERROR_HOOK*/

#define COMRETERROR(error, ID, param) return error

#endif/*#ifdef COM_ERROR_HOOK*/

/*
*************************************************************************************
*
* COM API的定义
*
*************************************************************************************
*/
StatusType StartCOM(COMApplicationModeType mode);
StatusType StopCOM  ( COMShutdownModeType mode );
COMApplicationModeType GetCOMApplicationMode (void);
StatusType InitMessage ( SymbolicName message, ApplicationDataRef dataref );
StatusType SendMessage ( SymbolicName message, ApplicationDataRef dataref );
StatusType ReceiveMessage ( SymbolicName message, ApplicationDataRef data);

#if defined(COM_RECEIVE_QUEUED_INTERNAL)
StatusType GetMessageStatus ( SymbolicName message );
#endif/*#if defined(COM_RECEIVE_QUEUED_INTERNAL)*/

#if defined(COM_SEND_ZERO_INTERNAL)
StatusType SendZeroMessage( SymbolicName message );
#endif/*#if defined(COM_SEND_ZERO_INTERNAL)*/

#ifdef COM_HOOK_GET_SERVICEID
    #define COMErrorGetServiceId(void) ServiceId
#endif/*#ifdef COM_USE_GETSERVICEID*/

#ifdef	COM_START_EXTENTSION
    StatusType StartCOMExtension(void);
#endif/*#ifdef	COM_START_EXTENTSION*/




/*
*************************************************************************************
*
* COM_H 结束
*
*************************************************************************************
*/
#endif/*#ifndef	 COM_H*/

