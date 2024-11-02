/*
****************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_res.h
* 摘    要：定义资源模块的数据结构以及声明标准API
*
* 当前版本：1.2
* 作    者：厉蒋
* 完成日期：2005年7月20日
*
* 当前版本：1.1
* 作    者：厉蒋
* 完成日期：2005年1月20日
*
* 取代版本：1.0
* 原作者  ：陈威
* 完成日期：2003年12月10日
****************************************************************
*/
#ifndef	 OS_RES_H
#define	 OS_RES_H

typedef struct OSResCB * ResourceType; /*资源类型*/


/*************************************************************
     外部资源相关的定义和一些操作的定义
***************************************************************/


#define OUT_RES_USED  1
#define OUT_RES_NOT_USED  0

struct OSResCB 
{
    ResPrioType   priority;     /*资源优先级*/
    PriorityType  OldRunPrio;   /*用于保存原来的运行优先级，方便ReleaseResource时恢复*/
#ifdef OS_EXTENDED_STATUS    
    INT8U         IsUsed;     /*标志该资源是否被使用,0表示没有在使用，非零表示在使用*/
#endif /*ifdef OS_EXTENDED_STATUS*/    
    OSTCBRef  SavedTCB;   /* 保存的TCB指针*/
    OSRCBRef     NextRes;    /*下一个资源的指针*/
 
};
extern OSRCB  OSResources[];



#define NO_RESOURCE ((OSRCBRef)0) /*无外部资源*/

#define DeclareResource(ResId) extern ResourceType ResId
void ResourceInit(void);



extern  ResourceType RES_SCHEDULER;    /*调度器资源*/
 
#define GetResource(ResId) \
       (ResId==RES_SCHEDULER)?GetSchedulerResource(RES_SCHEDULER):GetOutResource(ResId)
       
#define ReleaseResource(ResId) \
       (ResId==RES_SCHEDULER)?ReleaseSchedulerResource(RES_SCHEDULER):ReleaseOutResource(ResId)
       
#endif /*ifndef	 OS_RES_H*/
