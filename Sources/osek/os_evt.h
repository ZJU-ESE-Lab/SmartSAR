/*
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_evt.h
* 摘    要：申明了事件(Event)模块使用的结构和标准API接口函数
*
* 当前版本：1.1
* 作    者：高爽
* 完成日期：2005年1月17日
*
* 取代版本：1.0
* 原作者  ：原作者？？
* 完成日期：2004年12月10日
*/

#ifndef	 OS_EVT_H
#define	 OS_EVT_H

#define NOSETEVENT  0

#define DeclareEvent(Event) extern EventMaskType Event

#ifdef OS_ALARM_EN
void OSSetEvent ( TaskType TaskID,EventMaskRefType Mask );
#endif/*#ifdef OS_ALARM_EN*/

/*     
*标准API接口函数
*/                             
StatusType SetEvent ( TaskType TaskID,EventMaskType Mask );

StatusType ClearEvent ( EventMaskType Mask );

StatusType GetEvent ( TaskType TaskID,EventMaskRefType Event );

StatusType WaitEvent ( EventMaskType Mask );


#endif