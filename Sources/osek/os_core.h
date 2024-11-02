/*
*************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_core.h
* 摘    要：操作系统出错机制和核心常量的宏定义以及核心函数的声明
*
* 当前版本：1.1
* 作    者：厉蒋、潘杭平
* 完成日期：2005年1月20日
*
* 取代版本：1.0
* 原作者  ：陈威
* 完成日期：2003年12月4日
*************************************************************************
*/
#ifndef	 OS_CORE_H
#define	 OS_CORE_H

typedef INT8U       AppModeType;        /* Application mode 类型  */

extern AppModeType	ActiveAppMode;/*当前激活的APPMODE*/

/*****************************************************************************
*        系统堆栈
*******************************************************************************/
extern OSSTKRef OSSTKSystem ;

#define OSDEFAULTAPPMODE (AppModeType)0     /*系统默认APPMODE*/

#define OS_LOWEST_PRIO      0 /*最低优先级*/
#define  OS_TASK_IDLE_ID     (OS_LOWEST_PRIO)    /*Idle任务的id */
#define  OS_IDLE_PRIO       (OS_LOWEST_PRIO)     /*Idle任务的priority  */

#ifdef OS_SCHED_LOCK_EN 
void   OSSchedLock(void);
void   OSSchedUnlock(INT8U ReSched);
#endif

/****************************************************************
显示其他错误信息
****************************************************************/
#ifdef OS_SHOW_OTHER_ERROR/*显示其他错误信息*/

#define RETURN_OTHER_ERROR(error) return (error)

#else
#define RETURN_OTHER_ERROR(error) OSEKPrintString("\nStack Error");return  (E_OS_OTHER)
#endif/*ifdef OS_SHOW_OTHER_ERROR*/


#ifdef OS_HOOK_ERROR

extern INT8U  OSNestedErrorHook;
extern INT8U  OSNestedHook;
void ErrorHook(StatusType error);

#ifdef OS_HOOK_GET_SERVICEID

typedef INT8U  OSSERVICEIDTYPE;  /*系统服务标志的类型*/
extern OSSERVICEIDTYPE  ServiceId;

/* OSEK OS 服务组ID*/
#define OSSYSTEMSERVICEIDBASE       0x00    /* 控制服务*/
#define OSTASKSERVICEIDBASE         0x10    /* 任务管理服务 */
#define OSISRSERVICEIDBASE          0x20    /* 中断处理服务 */
#define OSRESOURCESERVICEIDBASE     0x30    /* 资源管理服务*/
#define OSEVENTSERVICEIDBASE        0x40    /* 事件管理服务*/
#define OSMESSAGESERVICEIDBASE      0x50    /* 消息服务*/
#define OSCOUNTERSERVICEIDBASE      0x70    /* 计数器服务*/
#define OSALARMSERVICEIDBASE        0x80    /* ALARMS服务 */
#define OSHOOKSERVICEIDBASE         0x90    /* HOOK服务*/

#define OSServiceId_NoService                   (OSSERVICEIDTYPE)0xFF
#define OSServiceId_StartOS                     (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x02 )
#define OSServiceId_ShutdownOS                  (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x04 )
#define OSServiceId_GetActiveApplicationMode    (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x06 )

#define OSServiceId_ActivateTask                (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x00 )
#define OSServiceId_TerminateTask               (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x02 )
#define OSServiceId_ChainTask                   (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x04 )
#define OSServiceId_Schedule                    (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x06 )
#define OSServiceId_GetTaskID                   (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x08 )
#define OSServiceId_GetTaskState                (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x0A )

#define OSServiceId_ResumeAllInterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x00 )
#define OSServiceId_SuspendAllinterrupts        (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x02 )
#define OSServiceId_ResumeOSInterrupts          (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x04 )
#define OSServiceId_SuspendOSinterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x06 )
#define OSServiceId_EnableAllInterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x08 )
#define OSServiceId_DisableAllInterrupts        (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x0A )

#define OSServiceId_DefineResource				(OSSERVICEIDTYPE)( OSRESOURCESERVICEIDBASE + 0x00 )
#define OSServiceId_GetResource                 (OSSERVICEIDTYPE)( OSRESOURCESERVICEIDBASE + 0x02 )
#define OSServiceId_ReleaseResource             (OSSERVICEIDTYPE)( OSRESOURCESERVICEIDBASE + 0x04 )

#define OSServiceId_SetEvent                    (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x00 )
#define OSServiceId_ClearEvent                  (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x02 )
#define OSServiceId_GetEvent                    (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x04 )
#define OSServiceId_WaitEvent                   (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x06 )

#define OSServiceId_GetAlarmBase                (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x00 )
#define OSServiceId_GetAlarm                    (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x02 )
#define OSServiceId_SetRelAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x04 )
#define OSServiceId_SetAbsAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x06 )
#define OSServiceId_CancelAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x08 )

#define OSServiceId_ErrorHook                   (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x00 )
#define OSServiceId_PreTaskHook                 (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x02 )
#define OSServiceId_PostTaskHook                (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x04 )
#define OSServiceId_StartupHook                 (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x06 )
#define OSServiceId_ShutdownHook                (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x08 )

#define OSErrorGetServiceId()  ServiceId
#endif  /*ifdef OS_HOOK_GET_SERVICEID*/
/**********************************************************************
HOOK参数获取的方法定义
***********************************************************************/
#ifdef OS_HOOK_PARAM_ACCESS
typedef INT32U ObjectIDType;
extern ObjectIDType  OSObjId; 
      /* 获取出错模式*/
#define OSError_StartOS_Mode()              (AppModeType)OSObjId
      /* 获取任务管理API出错信息 */ 
#define OSError_ActivateTask_TaskID()       (TaskType)OSObjId
#define OSError_ChainTask_TaskID()          (TaskType)OSObjId                   
#define OSError_GetTaskState_TaskID()       (TaskType)OSObjId                
     /* 获取资源管理API出错信息 */
#define OSError_GetResource_ResID()         (ResourceType)OSObjId
#define OSError_ReleaseResource_ResID()     (ResourceType)OSObjId             
     /* 获取事件管理API出错信息 */
#define OSError_SetEvent_TaskID()           (TaskType)OSObjId                    
#define OSError_GetEvent_TaskID()           (TaskType)OSObjId                                  
     /* 获取ALARM管理API出错信息 */
#define OSError_GetAlarmBase_AlarmID()      (AlarmType)OSObjId                
#define OSError_GetAlarm_AlarmID()          (AlarmType)OSObjId                    
#define OSError_SetRelAlarm_AlarmID()       (AlarmType)OSObjId                 
#define OSError_SetAbsAlarm_AlarmID()       (AlarmType)OSObjId                 
#define OSError_CancelAlarm_AlarmID()       (AlarmType)OSObjId  

#ifdef OS_HOOK_GET_SERVICEID      
#define OSSetServiceId(ID, param) \
        do{\
         if(ServiceId == OSServiceId_NoService)\
         {\
               ServiceId = ID; OSObjId = (ObjectIDType) param;\
         }}while(0)
#else  /*ifdef OS_HOOK_GET_SERVICEID*/

#define OSSetServiceId(ID, param)  OSObjId = (INT8U) param  
      
#endif /*ifdef OS_HOOK_GET_SERVICEID*/

#else /*define OS_HOOK_PARAM_ACCESS*/

#ifdef OS_HOOK_GET_SERVICEID
#define OSSetServiceId(ID, param) \
         do{\
         if(ServiceId == OSServiceId_NoService)\
         {\
               ServiceId = ID;\
         }}while(0) 
#else  /*ifdef OS_HOOK_GET_SERVICEID*/
#define OSSetServiceId(ID, param)

#endif /*ifdef OS_HOOK_GET_SERVICEID*/ 

#endif /*define OS_HOOK_PARAM_ACCESS*/ 
            
#ifdef OS_HOOK_GET_SERVICEID

#define OSCancelServiceId()  ServiceId = OSServiceId_NoService

#else /*define OS_HOOK_GET_SERVICEID*/
#define OSCancelServiceId() 
#endif /*OS_HOOK_GET_SERVICEID*/

/**********************************************************
ErrorHook
***********************************************************/                                    
#define OSErrorHook( error, ID, param )\
        if(OSNestedErrorHook == 0)\
        { \
            OSNestedErrorHook++;\
            if(0 == OSNestedHook)\
                SuspendOSInterrupts();\
            OSSetServiceId(ID, param);\
            ErrorHook(error);\
            OSCancelServiceId();\
            if(0 == OSNestedHook)\
                ResumeOSInterrupts();\
            OSNestedErrorHook--;\
         } 
#define OSRETERROR_( error, ID, param ) \
        OSErrorHook( error, ID, param )\
        return( error )    

#define OSRETERROR( error, ID, param )  OsLastError = error; OSRETERROR_( error, ID, param )


#else/*ifdef OSHOOKERROR*/

#define OSRETERROR_( error, ID, param ) return( error )    

#define OSRETERROR( error, ID, param )  OsLastError = error; OSRETERROR_( error, ID, param )

#endif/*ifdef OSHOOKERROR*/


/**********************************************************
PreTaskHook
***********************************************************/
void PreTaskHook(); /*OSPreTaskHook调用该PreTaskHook（os_core.c）*/

#define OSPreTaskHook() PreTaskHook()

/**********************************************************
PostTaskHook
***********************************************************/
  
void PostTaskHook(void);
  	
#define OSPostTaskHook() \
        PostTaskHook()

        

/**********************************************************
StartupHook
***********************************************************/
#ifdef OS_HOOK_STARTUP
void StartupHook(void);
#define OSStartupHook()		StartupHook()
#else /*#ifdef OS_HOOK_STARTUP*/
#define OSStartupHook()
#endif/*ifdef OS_HOOK_STARTUP*/

/**********************************************************
ShutdownHook
***********************************************************/
#ifdef OS_HOOK_SHUTDOWN
void ShutdownHook(StatusType error);
#define OSShutdownHook( error )\
  		SuspendOSInterrupts();\
        ShutdownHook(error);\
        CRGFLG_RTIF = 1;\
        ResumeOSInterrupts()
#else /*#ifdef OS_HOOK_SHUTDOWN*/
#define OSShutdownHook()        
#endif/*ifdef OS_HOOK_SHUTDOWN*/



/*内部函数声明*/
#ifndef OS_TASK_AUTO_DEFINE
void          OSInit(void);
#endif/*#ifndef OS_TASK_AUTO_DEFINE*/

//#if defined(OS_EXCEPTION_EN)|| defined(OS_ALARM_EN)
#define OSIntEnter() \
        asm inc OSIntNesting; \
        if(OSIntNesting == 1) \
        { \
             asm ldy OSTCBCur; \
             asm sts 0,y; \
        }
        
void  OSIntExit(void);

/*标准osek 的api*/
void  StartOS (AppModeType mode );
AppModeType GetActiveApplicationMode ( void );
void ShutdownOS ( StatusType Error);

void OSDebug();
#endif/*ifndef	 OS_CORE_H*/
