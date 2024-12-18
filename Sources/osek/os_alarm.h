/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_alarm.h
* 摘    要：alarm模块的的宏开关以及API的声明
*  
* 当前版本：1.4
* 作    者：厉蒋
* 完成日期：2005年7月6日
*
* 当前版本：1.3
* 作    者：徐卢传
* 完成日期：2005年6月7日
*
* 取代版本：1.2
* 作    者：孟晓楠、厉蒋
* 完成日期：2005年3月1日
*
* 取代版本：1.1
* 作    者：潘杭平
* 完成日期：2004年9月27日
*
* 取代版本：1.0
* 原 作 者  ：林树新
* 完成日期：2003年12月23日
*********************************************************************************
*/

#ifndef OS_ALARM_H
#define OS_ALARM_H


#define AlarmUsed   1
#define AlarmUnUsed 0
#define ALARM_AUTO_START 1
/**********************************************************************************
以下三个宏在用户使用OSMAXALLOWEDVALUE_x等时，必须让其define 如下
#define OSMAXALLOWEDVALUE_1 GETMAXALLOWEDVALUE(1)
如果有多个，则循环定义OSMAXALLOWEDVALUE_x，从而实现OSEK标准中类似OSMAXALLOWEDVALUE_1的常量
*****************************************************************/
#define GETMAXALLOWEDVALUE(Index)  OSCounters[Index].CtrInfo->maxallowedvalue
#define GETTICKSPERBASE(Index)  OSCounters[Index].CtrInfo->ticksperbase
#define GETMINCYCLE(Index)  OSCounters[Index].CtrInfo->mincycle

/***************************************************************************
以下为OSEK标准规定的常量
****************************************************************************/
#define OSMAXALLOWEDVALUE  OSCounters[0].CtrInfo->maxallowedvalue
#define OSTICKSPERBASE  OSCounters[0].CtrInfo->ticksperbase
#define OSMINCYCLE  OSCounters[0].CtrInfo->mincycle
#define OSTICKDURATION (1000000000/(OS_TICKS_PER_SEC)) /*一Tick多少纳秒*/

#define ALARMCALLBACK(name) void name(void)    /*ALARMCALLBACK宏定义，用来定义一个alarm回叫函数*/

#define DeclareAlarm(AlarmIdentifier) extern AlarmType AlarmIdentifier


typedef INT32U TickType;


typedef TickType* TickRefType;     /*TickType指针*/

typedef struct AlarmOSCB*  AlarmType;          /*AlarmOSCB的指针，用于表示alarm的索引*/

/*********************************************************
该数据结构用于用户配置Counter信息，也是标准所要求的类型
***********************************************************/
typedef struct AlarmBase
{
    TickType maxallowedvalue;     /* 最大的计数值（以Ticks为单位的） */
    TickType ticksperbase;        /* 以多少ticks为一个counter特定的单位 */
#ifdef OS_EXTENDED_STATUS
    TickType mincycle;            /* 相关联的alarm允许的最小周期值，单位是tick，用于
                                    SetRelAlarm/SetAbsAlarm函数中的cycle参数有效性
                                    检查判断（只在扩展状态下才需要）。*/
#endif/*#ifdef OS_EXTENDED_STATUS*/
} AlarmBaseType;
typedef AlarmBaseType* AlarmBaseRefType;
typedef struct AlarmOSCB* AlarmCBRef;

/*********************************************************
该数据结构用于Counter的控制
***********************************************************/
typedef struct CtrOSCB
{
    TickType value;          /*当前该counter的tick值，该值应该位于区间[0, CtrInfo->maxallowedvalue]之间*/
    TickType RemainTicks;    /*还剩余的ticks,当为0时，表明就与其关联的启用的alarm中至少有一个
                               会被触发。注：如果head为空时，则RemainTicks无意义。*/    
    
    AlarmCBRef head;         /*关联的已启用的alarm链表的头指针*/
    AlarmBaseRefType CtrInfo;   /*对应的couter配置信息*/    
}CtrCB;

typedef CtrCB* CtrType;

typedef void(*OS_CALLBACK_ENTRY)(void); /*callback函数入口定义*/


/*********************************************************
该数据结构用于用户配置Alarm信息
***********************************************************/
typedef struct 
{
    INT8U CtrID;       /*GetAlarmBase函数会用到的*/
    INT8U AutoStart;      /*是否自启动*/
    TickType increment;   /*下一个触发点的tick值，如果为自启动，则需要定义*/
    TickType cycle;       /*周期，如果为自启动，则需要定义*/
    PriorityType TaskID;  /*关联的任务*/
    EventMaskRefType event;  /*关联的事件掩码*/
    OS_CALLBACK_ENTRY AlarmCallBack;   /*callback入口函数*/    
}OSAlarmCfg;

/*********************************************************
该数据结构用于Alarm的控制
***********************************************************/
typedef struct AlarmOSCB
{
    CtrType ctr;        /*GetAlarmBase函数会用到的*/  
    TickType delta;     /*下一个触发点的tick值*/
    TickType cycle;     /*周期*/    
    TaskType task;      /*关联的任务*/
#ifdef OS_ALARM_CALLBACK_EN
    OS_CALLBACK_ENTRY AlarmCallBack;   /*callback入口函数*/
#endif /*#ifdef OS_ALARM_CALLBACK_EN*/
    
#ifdef OS_EVENT_EN
    EventMaskRefType event;  /*关联的事件掩码*/
#endif /*#ifdef OS_EVENT_EN*/

    AlarmCBRef next;      /*指向关联的counter上面的下一个alarm*/
    AlarmCBRef prev;      /*为了链表的删除方便而设置该变量*/
    INT8U start;          /* 状态位，标志该Alarm是否启用, 0表示未被启用，非0相反 */

}AlarmCB;

/*********************************************************
操作宏
***********************************************************/
#define GET_ALARM_COUNTER_HEAD(PALARM) (PALARM)->ctr->head
#define GET_ALARM_COUNTER_VALUE(PALARM) (PALARM)->ctr->value
#define GET_ALARM_COUNTER_MAXVALUE(PALARM) (PALARM)->ctr->CtrInfo->maxallowedvalue
#define GET_ALARM_COUNTER_MINCYCLE(PALARM) (PALARM)->ctr->CtrInfo->mincycle
#define GET_ALARM_DELTA(PALARM) (PALARM)->delta
#define GET_NEXT_ALARM(PALARM) (PALARM)->next
#define GET_PRV_ALARM(PALARM) (PALARM)->prev
#define GET_ALARM_CYCLE(PALARM) (PALARM)->cycle
#define GET_ALARM_EVENT(PALARM) (PALARM)->event
#define GET_ALARM_TASK(PALARM) (PALARM)->task
#define GET_ALARM_COUNTER_INFO(PALARM) (PALARM)->ctr->CtrInfo
#define GET_ALARM_STATE(PALARM) (PALARM)->start


#define SET_ALARM_COUNTER_HEAD(PALARM, ALM2SET) (PALARM)->ctr->head = (ALM2SET) 
#define SET_ALARM_USED(PALARM) (PALARM)->start = AlarmUsed
#define SET_ALARM_UNUSED(PALARM) (PALARM)->start = AlarmUnUsed
/*把ALM2SET插入到PALARM所在counter的ALARM表的表头*/
#define SET_ALARM_COUNTER_HEAD(PALARM, ALM2SET) (PALARM)->ctr->head = (ALM2SET)
#define SET_NEXT_ALARM(PALARM, NXTALM) (PALARM)->next = (NXTALM)
#define SET_PRV_ALARM(PALARM, PRVALM) (PALARM)->prev = (PRVALM)
#define SET_ALARM_CYCLE(PALARM, CYCLE) (PALARM)->cycle = (CYCLE)
#define SET_ALARM_DELTA(PALARM, DELTA) (PALARM)->delta = (DELTA)

#define GET_COUNTER_VALUE(PCTR) (PCTR)->value
#define GET_COUNTER_MAXVALUE(PCTR) (PCTR)->CtrInfo->maxallowedvalue
#define GET_HEAD_ALARM(PCTR) (PCTR)->head
#define GET_COUNTER_REMAIN_TICKS(PCTR) (PCTR)->RemainTicks


#define SET_COUNTER_VALUE(PCTR, VALUE) (PCTR)->value = (VALUE)
#define COUNTER_VALUE_GROW(PCTR) (PCTR)->value ++
#define REMAIN_TICKS_DECREASE(PCTR);(PCTR)->RemainTicks --
#define SET_HEAD_ALARM(PCTR, HDALARM) (PCTR)->head = (HDALARM)

#define SET_ALARM_COUNTER_REMAIN_TICKS() 
/*********************************************************
全局变量申明
***********************************************************/
extern AlarmCB OSAlarms[];/*Alarm配置数组*/   
extern CtrCB OSCounters[];/*Counter配置数组*/
#ifdef OS_ALARM_AUTO_DEFINE
void OSAlarmInit(void);
#else
CtrType DefineCounter(TickType  maxallowedvalue,
            TickType mincycle,         
            TickType ticksperbase);    /*OIL分析工具需要的函数，定义(配置)一个counter*/        
        
AlarmType DefineAlarm(CtrType ci, TaskType id
#ifdef OS_EVENT_EN
                    , EventMaskRefType event
#endif/*#ifdef OS_EVENT_EN*/
#ifdef OS_ALARM_CALLBACK_EN
                    , OS_CALLBACK_ENTRY callback
#endif/*#ifdef OS_ALARM_CALLBACK_EN*/
                ); /*定义一个alarm*/

AlarmType DefineAutoAlarm(CtrType ci,TaskType id, 
#ifdef OS_EVENT_EN
                    EventMaskRefType event,
#endif/*#ifdef OS_EVENT_EN*/
#ifdef OS_ALARM_CALLBACK_EN
                    OS_CALLBACK_ENTRY callback,
#endif/*#ifdef OS_ALARM_CALLBACK_EN*/
                    TickType increment,TickType cycle);    /*定义一个自动启动的alarm*/
#endif/*#ifdef OS_ALARM_AUTO_DEFINE*/
StatusType GetAlarm( AlarmType AlarmID, TickRefType tick );
StatusType CancelAlarm( AlarmType AlarmID );
StatusType GetAlarmBase ( AlarmType AlarmID,AlarmBaseRefType Info );
StatusType SetRelAlarm( AlarmType AlarmID, TickType increment, TickType cycle );
StatusType SetAbsAlarm( AlarmType AlarmID, TickType start, TickType cycle );

void OSCounterTrigger(CtrType isr);        /*每次中断对应的Counter的处理函数,OS_CPU_C.c中用到*/

#endif