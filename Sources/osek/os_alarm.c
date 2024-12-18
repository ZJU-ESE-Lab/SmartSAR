/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_alarm.c
* 摘    要：实现alarm模块的API
*
* 当前版本：1.4
* 作    者：徐卢传
* 完成日期：2005年6月7日
*
* 取代版本：1.3
* 作    者：孟晓楠
* 完成日期：2005年3月7日
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

#ifndef OS_ALARM_C
#define OS_ALARM_C

#ifndef OS_MASTER_FILE
#include "includes.h"
#endif

AlarmCB OSAlarms[OS_NUM_ALARM]; /*Alarm配置数组*/
CtrCB OSCounters[OS_NUM_CTR];   /*Counter配置数组*/

#ifdef OS_ALARM_AUTO_DEFINE
extern AlarmBaseType OSCounterCfgTbl[];
extern OSAlarmCfg OSAlarmCfgTbl[];
#endif/*#ifdef OS_ALARM_AUTO_DEFINE*/

void OSCheckAlarms( CtrType ctrId );
void OSNotifyAlarm(AlarmCBRef alarm);
void OSInsertAlarm(AlarmCBRef alarm);
/*
***************************************************************************
* void AlarmInit()
* 说明：用于Alarm的初始化
* 调用的函数：无
* 允许被调用的位置：StartOS
* 参数：无
* 返回：无
* 注意事项：
* 创建人：厉蒋
* 时间：2005.7.6
***************************************************************************
*/
#ifdef OS_ALARM_AUTO_DEFINE
void OSAlarmInit()
{
	AlarmType alarm=OSAlarms;
	OSAlarmCfg * alarmCfg=OSAlarmCfgTbl;
	CtrType counter =OSCounters;
	AlarmBaseRefType counterCfg=OSCounterCfgTbl;
	for(;counter<(OSCounters+OS_NUM_CTR);counter++,counterCfg++)
	{   /*初始化Counter*/
		counter->CtrInfo = counterCfg;
        counter->value = 0;
        counter->head = NULL;        
	}
	for(;alarm<OSAlarms+OS_NUM_ALARM;alarm++,alarmCfg++)
	{   /*初始化Alarm*/
        alarm->start = AlarmUnUsed;
        alarm->ctr = &OSCounters[alarmCfg->CtrID];
        alarm->task = &OSTCBTbl[alarmCfg->TaskID];
#ifdef OS_EVENT_EN
        alarm->event = alarmCfg->event;
#endif /*#ifdef OS_EVENT_EN*/

#ifdef OS_ALARM_CALLBACK_EN
        alarm->AlarmCallBack = alarmCfg->AlarmCallBack;
#endif /*#ifdef OS_ALARM_CALLBACK_EN*/
       if(alarmCfg->AutoStart==ALARM_AUTO_START)/*如果为自启动，则设置其delta和cycle*/
       {
           alarm->delta=alarmCfg->increment;
           alarm->cycle = alarmCfg->cycle;
           OSInsertAlarm(alarm);
       }
	}
	
}
#else/*#ifdef OS_ALARM_AUTO_DEFINE*/
static INT8U AlarmIndex = 0;    /*Alarm数组索引*/
static INT8U CounterIndex = 0;  /*Couter数组索引*/
/*
***************************************************************************
* AlarmType DefineAutoAlarm(CtrType ctr,TaskType id, EventMaskType event,
*        OS_CALLBACK_ENTRY callback , TickType increment,TickType cycle)
* 描述：    定义一个自启动的alarm,自启动和非自启动在oil文件中配置
*            将从oil文件中分析的alarm信息填充到alarm配置的数据结构中
* 调用的函数：OSInsertAlarm
* 允许被调用的位置：无
* 参数：ctr:counter
        id:关联的任务id,如果关联的是callback则为0
*       event:关联的事件掩码,没有则为0
*       callback:关联的callback函数入口,即函数指针,如果没有,则为0
*       increment和 cycle的含义与SetRelalarm函数中参数含义一样
* 返回：AlarmType
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：厉蒋
* 时间：2005.3.1
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

AlarmType DefineAutoAlarm(CtrType ctr,TaskType task,
#ifdef OS_EVENT_EN
                    EventMaskRefType event,
#endif /*#ifdef OS_EVENT_EN*/
#ifdef OS_ALARM_CALLBACK_EN
                    OS_CALLBACK_ENTRY callback,
#endif/*#ifdef OS_ALARM_CALLBACK_EN*/
                    TickType increment,TickType cycle)  /*定义一个自动启动的alarm*/
{
#ifdef OS_SHOW_OTHER_ERROR        /*显示其他错误信息*/
    if(AlarmIndex >= OS_NUM_ALARM)
        RETURN_OTHER_ERROR(E_OS_SYS_PARA);
#endif /*ifdef OS_SHOW_OTHER_ERROR*/
    AlarmType alarm = &OSAlarms[AlarmIndex];
    alarm->start = AlarmUsed;/*标记alarm已经启用*/
    alarm->ctr = ctr;
    alarm->task = task;
#ifdef OS_EVENT_EN
    alarm->event = event;
#endif /*#ifdef OS_EVENT_EN*/

#ifdef OS_ALARM_CALLBACK_EN
    alarm->AlarmCallBack = callback;
#endif /*#ifdef OS_ALARM_CALLBACK_EN*/

    alarm->delta = increment;
    alarm->cycle = cycle;

    OSInsertAlarm(alarm);/*插入到counter关联的链表中去*/

    AlarmIndex ++;
    return alarm;
}

/*
***************************************************************************
* AlarmType DefineAlarm(CtrType ctr,TaskType id, EventMaskType event,OS_CALLBACK_ENTRY callback )
* 描述：定义一个非自启动的alarm
*        将从oil文件中分析的alarm信息填充到alarm配置的数据结构中
* 调用的函数：无
* 允许被调用的位置：无
* 参数：
*    ctr:counter
*     id:关联的任务id,如果关联的是callback则为0
*    event:关联的事件掩码,没有则为0
*    callback:关联的callback函数入口,即函数指针,如果没有,则为0
* 返回：AlarmType
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：厉蒋
* 时间：2005.3.1
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

AlarmType DefineAlarm(CtrType ctr, TaskType id
#ifdef OS_EVENT_EN
                    , EventMaskRefType event
#endif /*#ifdef OS_EVENT_EN*/
#ifdef OS_ALARM_CALLBACK_EN
                    , OS_CALLBACK_ENTRY callback
#endif/*#ifdef OS_ALARM_CALLBACK_EN*/
                )
{

#ifdef OS_SHOW_OTHER_ERROR /*显示其他错误信息*/
    if(AlarmIndex >= OS_NUM_ALARM)
        RETURN_OTHER_ERROR(E_OS_SYS_PARA);
#endif /*ifdef OS_SHOW_OTHER_ERROR*/
    AlarmType alarm = &OSAlarms[AlarmIndex];
    alarm->start = AlarmUnUsed;
    alarm->ctr = ctr;
    alarm->task = id;

#ifdef OS_EVENT_EN
    alarm->event = event;
#endif /*#ifdef OS_EVENT_EN*/

#ifdef OS_ALARM_CALLBACK_EN
    alarm->AlarmCallBack = callback;
#endif /*#ifdef OS_ALARM_CALLBACK_EN*/

    AlarmIndex++;
    return alarm;
}


/*
***************************************************************************
* CtrType DefineCounter(TickType maxallowedvalue, TickType mincycle, TickType ticksperbase)
* 描述：定义一个counter
*    将从oil文件中分析的counter信息填充到counter配置的数据结构中
* 调用的函数：无
* 允许被调用的位置：无
* 参数：
*    maxallowedvalue: 所允许最大tick数
*    mincycle: 所允许的最小的周期
*    ticksperbase: 一个counter unit相当于多少个ticks
* 返回：CtrType
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

CtrType DefineCounter(TickType maxallowedvalue,
            TickType mincycle,
            TickType ticksperbase)
{

#ifdef OS_SHOW_OTHER_ERROR  /*显示其他错误信息*/
    if(CounterIndex >= OS_NUM_CTR)
        RETURN_OTHER_ERROR(E_OS_SYS_PARA);
#endif  /*ifdef OS_SHOW_OTHER_ERROR*/
    CtrType counter = &OSCounters[CounterIndex];
    counter->CtrInfo->maxallowedvalue = maxallowedvalue;
#ifdef OS_EXTENDED_STATUS
    counter->CtrInfo->mincycle = mincycle;
#else
    mincycle = mincycle;/*去除Warnning*/     
#endif /*#ifdef OS_EXTENDED_STATUS*/
    counter->CtrInfo->ticksperbase = ticksperbase;
    counter->value = 0;
    counter->head = NULL;

    CounterIndex++;
  
    return counter;
}

#endif/*#ifdef OS_ALARM_AUTO_DEFINE*/
/*
***************************************************************************
* void OSInsertAlarm(AlarmCBRef alarm)
* 说明：按下一次触发时间的顺序，将alarm加入它的Counter中的alarms链表中,
*       并且设置该alarm已经启动。如果新插入的alarm处于链表头，则更新其counter的
*       RemainTicks.
* 调用的函数：无
* 允许被调用的位置：SetAbsAlarm,SetRelAlarm,DefineAutoAlarm
* 参数：AlarmCBRef alarm
* 返回：无
* 注意事项：
* 创建人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/
void OSInsertAlarm(AlarmCBRef alarm)
{
    DEFINECRITICAL(cpu_sr);
    AlarmCBRef temp, last;
    AlarmCBRef head;
    TickType value ;

    OS_ENTER_CRITICAL();
    head = GET_ALARM_COUNTER_HEAD(alarm);
    value = GET_ALARM_COUNTER_VALUE(alarm);
    SET_ALARM_USED(alarm);

    /*如果链表为空，则直接将alarm放入链表头*/
    if (head == NULL)
    {
        SET_ALARM_COUNTER_HEAD(alarm, alarm);
        SET_NEXT_ALARM(alarm, NULL);
        SET_PRV_ALARM(alarm, NULL);

        /*链表头改变，更新RemainTicks.*/
        alarm->ctr->RemainTicks = GET_ALARM_DELTA(alarm) > value?
                                 GET_ALARM_DELTA(alarm) - value:
                                 GET_ALARM_COUNTER_MAXVALUE(alarm) - value + GET_ALARM_DELTA(alarm) + 1;
        OS_EXIT_CRITICAL();
        return;
    }

    /*将alarm排序插入到链表*/
    temp = head;
    last = NULL;/*用于保存处于temp之前的alarm*/
    /*1) 找到第一个将在'alarm'之后被触发的ALARM,用temp表示*/
    while (temp != NULL)
    {
        if (GET_ALARM_DELTA(temp) < value)
        {
            if (GET_ALARM_DELTA(alarm) < value)
            {
                if (GET_ALARM_DELTA(temp) >= GET_ALARM_DELTA(alarm))
                {
                    break;/*find*/
                }
            }
            else/* alarm->delta > value*/
            {
                break;/*find*/
            }
        }
        else/* temp->delta >value*/
        {
            if (GET_ALARM_DELTA(alarm) > value)
            {
                if (GET_ALARM_DELTA(temp) >= GET_ALARM_DELTA(alarm))
                {
                    break;/*find*/
                }
            }
        }
        last = temp;
        temp = GET_NEXT_ALARM(temp);
    }

    /*2) 将'alarm'插到'temp'之前*/
    alarm->prev = last;
    if (last) last->next = alarm;

    alarm->next = temp;
    if (temp) temp->prev = alarm;

    /*如果'last'为空，则表示'alarm'会被插到链表头，所以更新RemainTicks.*/
    if (!last)
    {
        SET_ALARM_COUNTER_HEAD(alarm, alarm);
        alarm->ctr->RemainTicks = GET_ALARM_DELTA(alarm) > value?
                                 GET_ALARM_DELTA(alarm) - value:
                                 GET_ALARM_COUNTER_MAXVALUE(alarm) - value + GET_ALARM_DELTA(alarm) + 1;
    }
    OS_EXIT_CRITICAL();
}

/*
***************************************************************************
* void OSDeleteAlarm(AlarmCBRef alarm)
* 说明：将alarm从它的Counter中的alarms链表中删除，并设置该alarm为非启动状态；
        如果删除的alarm原来处于链表头，则更新Counter的RemainTicks。
* 调用的函数：无
* 允许被调用的位置：CancelAlarm
* 参数：AlarmCBRef alarm
* 返回：无
* 注意事项：
* 创建人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/
void OSDeleteAlarm(AlarmCBRef alarm)
{
    DEFINECRITICAL(cpu_sr);

    OS_ENTER_CRITICAL();
    if(alarm == GET_ALARM_COUNTER_HEAD(alarm))
    {   /*'alarm' 是链表头*/

        /*从alarm的counter的alarm表中删除alarm*/
        SET_ALARM_COUNTER_HEAD(alarm, GET_NEXT_ALARM(alarm));

        /*如果alarm有下一个ALARM，则下一个ALARM会变成链表头，更新RemainTicks*/
        if(GET_NEXT_ALARM(alarm))
        {
            SET_PRV_ALARM(GET_NEXT_ALARM(alarm), NULL);
            alarm->ctr->RemainTicks = GET_ALARM_DELTA(GET_NEXT_ALARM(alarm)) > GET_ALARM_COUNTER_VALUE(alarm)?
                                     GET_ALARM_DELTA(GET_NEXT_ALARM(alarm)) - GET_ALARM_COUNTER_VALUE(alarm):
                                     GET_ALARM_COUNTER_MAXVALUE(alarm) - GET_ALARM_COUNTER_VALUE(alarm) + 1 + GET_ALARM_DELTA(GET_NEXT_ALARM(alarm));
        }
    }
    else
    {    /* 从链表中删除'alarm'*/
        SET_NEXT_ALARM(GET_PRV_ALARM(alarm),GET_NEXT_ALARM(alarm));
        if (GET_NEXT_ALARM(alarm))
        SET_PRV_ALARM(GET_NEXT_ALARM(alarm), GET_PRV_ALARM(alarm));
    }

    SET_NEXT_ALARM(alarm, NULL);
    SET_PRV_ALARM(alarm, NULL);
    SET_ALARM_UNUSED(alarm);        /*重新置位*/
    OS_EXIT_CRITICAL();

}


/*
***************************************************************************
* void OSCounterTrigger(CtrType ctr)
* 描述：将ctr计数值(value)加1，判断并触发此时expired的alarms。
* 调用的函数：OSCheckAlarms
* 允许被调用的位置：DECHandler（文件OS_CPU_C.c）
* 参数：无
* 返回：无
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

void OSCounterTrigger(CtrType ctr)
{
    DEFINECRITICAL(cpu_sr);
    OS_ENTER_CRITICAL();
    if (GET_COUNTER_VALUE(ctr) == GET_COUNTER_MAXVALUE(ctr))
    {
        /*达到maxallowedvalue，则置为0*/
        SET_COUNTER_VALUE(ctr,0);
    }
    else
    {
        COUNTER_VALUE_GROW(ctr);
    }
    if (GET_HEAD_ALARM(ctr) != NULL)/*判断是否有启用的alarms*/
    {
        REMAIN_TICKS_DECREASE(ctr);
        
        if (GET_COUNTER_REMAIN_TICKS(ctr) == 0)
        {
            /*至少有一个alarm会被触发，所以check一下，并更新RemainTicks*/
            OSCheckAlarms(ctr);
        }
        else
        {
            OS_EXIT_CRITICAL();
        }
    }
    else
    {
        OS_EXIT_CRITICAL();
    }
}

/*
***************************************************************************
* void OSCheckAlarms( CtrType ctr )
* 描述：触发crtId索引的Counter的所关联的当前expired的所有alarms。
* 调用的函数：OSNotifyAlarm
* 允许被调用的位置：OSISRCounterTrigger
* 参数：CtrType ctrId
* 返回：无
* 注意事项：
* 创建人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

void OSCheckAlarms( CtrType ctr )
{

    AlarmCBRef alarmRef, alarmRef2;
    AlarmCBRef head = NULL;
    AlarmCBRef temp, temp2, last;
    TickType value = GET_COUNTER_VALUE(ctr);

    alarmRef = alarmRef2 = GET_HEAD_ALARM(ctr);

    /*遍历前面k个被触发的 alarms，并触发它(k>=1) */
    while (alarmRef != NULL)
    {
        
        if (GET_ALARM_DELTA(alarmRef) != value)
        {
            break;
        }
        /*触发alarm,如果为周期alarm，则更新它的delta值（下一次触发时间）*/
        OSNotifyAlarm(alarmRef);
        
        alarmRef = GET_NEXT_ALARM(alarmRef);
    }
    /*指向剩余未触发alarms的链表的第一个alarm的指针*/
    head = alarmRef;
    /*第二次遍历前面k个刚刚被触发的alarms, 将其插入原来的未被触发的alarms链表中*/
    while (alarmRef2 != alarmRef)
    {
        /*保存一下 alrmRef2->next*/
        temp2 = GET_NEXT_ALARM(alarmRef2);
        
        if (GET_ALARM_CYCLE(alarmRef2) == 0)
        {
            /*一次性的alarm, 则直接删除它*/
            SET_NEXT_ALARM(alarmRef2, NULL); 
            SET_PRV_ALARM(alarmRef2, NULL);
            SET_ALARM_UNUSED(alarmRef2);

            alarmRef2 = temp2;
            continue;
        }

        /*如果剩余链表为空，则直接将alarmRef2设为链表头*/
        if (head == NULL)
        {
            head = alarmRef2;
            SET_NEXT_ALARM(alarmRef2, NULL); 
            SET_PRV_ALARM(alarmRef2, NULL);            
            
            alarmRef2 = temp2;
            continue;
        }

        /*将alarmRef2排序插入链表*/
        temp = head;
        last = NULL;
        
        /*找到第一个将在'alarmRef2'之后被触发的ALARM,用temp表示alarmRef2*/
        while (temp != NULL)
        {
            if (GET_ALARM_DELTA(temp) < value)
            {
                if (GET_ALARM_DELTA(alarmRef2) < value)
                {
                    if (GET_ALARM_DELTA(temp) >= GET_ALARM_DELTA(alarmRef2))
                    {
                        break;
                    }
                }
                else/* alarmRef2 > value*/
                {
                    break;
                }
            }
            else/* temp->delta >value*/
            {
                if (GET_ALARM_DELTA(alarmRef2) > value)
                {
                    if (GET_ALARM_DELTA(temp) >= GET_ALARM_DELTA(alarmRef2))
                    {
                        break;
                    }
                }
            }
            last = temp;
            temp = GET_NEXT_ALARM(temp);
        }

        alarmRef2->prev = last;
        if (last) last->next = alarmRef2;

        alarmRef2->next = temp;
        if (temp) temp->prev = alarmRef2;

        if (!last) head = alarmRef2;
        /*将下一个ALARM赋给alarmRef2*/
        alarmRef2 = temp2;
    }

    SET_HEAD_ALARM(ctr,head);
    if (head)
    {
        ctr->RemainTicks = GET_ALARM_DELTA(head) >= value?
                           GET_ALARM_DELTA(head) - value:
                           GET_COUNTER_MAXVALUE(ctr) - value + 1 + GET_ALARM_DELTA(head);
                                   
                         
    }
}


/*
***************************************************************************
* void OSNotifyAlarm(AlarmCBRef alarm)
* 描述：执行alarm所对应的操作；如果是cycle alarm，则重新设定delta值。
* 调用的函数：SetEvent, ActivateTask, AlarmCallBack
* 允许被调用的位置：OSCheckAlarms
* 参数：AlarmCBRef alarm
* 返回：无
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：厉蒋
* 时间：2005.3.1
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

void OSNotifyAlarm(AlarmCBRef alarm)
{
    TickType temp;
#ifdef OS_ALARM_CALLBACK_EN
    if (alarm->AlarmCallBack != NULL)
    {
        alarm->AlarmCallBack();
    }
    else
    {
#endif

#ifdef OS_EVENT_EN
    if (GET_ALARM_EVENT(alarm) != NULL)
    {
        OSSetEvent(GET_ALARM_TASK(alarm),(GET_ALARM_EVENT(alarm)));
    }
    else
    {
#endif
        OSActivateTask(GET_ALARM_TASK(alarm));
#ifdef OS_EVENT_EN
    }
#endif

#ifdef OS_ALARM_CALLBACK_EN
    }
#endif
    if(GET_ALARM_CYCLE(alarm) != 0)
    {
        temp = GET_ALARM_DELTA(alarm) + GET_ALARM_CYCLE(alarm);
        alarm->delta = temp <= GET_ALARM_COUNTER_MAXVALUE(alarm) ?
                       temp:
                       temp - GET_ALARM_COUNTER_MAXVALUE(alarm) -1;
    }
    return;
}

/*
***************************************************************************
* StatusType GetAlarmBase ( AlarmType Alarm, AlarmBaseRefType Info )
* 描述：查询counter状态,获得ID号为<Alarm>的Alarm的counter的信息
* 调用的函数：无
* 允许被调用的位置：无
* 参数：AlarmType  Alarm, AlarmBaseRefType Info
* 返回： E_OK:    调用成功
*        E_OS_ID: Alarm越界
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

StatusType GetAlarmBase (AlarmType  Alarm, AlarmBaseRefType Info )
{

#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)    /* 判断Alarm是否有效 */
    {
        OSRETERROR( E_OS_ID, OSServiceId_GetAlarmBase, Alarm );
    }
#endif
    /* 返回Counter的基本信息 */
    memcpy(Info,(GET_ALARM_COUNTER_INFO(Alarm)),sizeof(AlarmBaseType));
    return E_OK;
}

/*
***************************************************************************
* StatusType GetAlarm( AlarmType Alarm, TickRefType Tick )
* 描述：查询alarm状态，获得ID号为<Alarm>的Alarm现在时刻到触发时间的相对ticks值
* 调用的函数：无
* 允许被调用的位置：无
* 参数：AlarmType Alarm, TickRefType  tick
* 返回：    E_OK：调用成功
*           E_OS_NOFUNC,：ID号为<Alarm>的Alarm未被启用
*           E_OS_ID：Alarm越界
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/


StatusType GetAlarm( AlarmType Alarm, TickRefType Tick )
{

#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)
    {
        OSRETERROR( E_OS_ID, OSServiceId_GetAlarm, Alarm );
    }
#endif/*ifdef OS_EXTENDED_STATUS */

    if(GET_ALARM_STATE(Alarm) == AlarmUnUsed)
    {
        OSRETERROR( E_OS_NOFUNC, OSServiceId_GetAlarm, Alarm );   /* 表明ID号为Alarm的alarm未被启用 */
    }

    *Tick = GET_ALARM_DELTA(Alarm) >= GET_ALARM_COUNTER_VALUE(Alarm)?
            GET_ALARM_DELTA(Alarm) - GET_ALARM_COUNTER_VALUE(Alarm):
            GET_ALARM_COUNTER_MAXVALUE(Alarm) - GET_ALARM_COUNTER_VALUE(Alarm) + 1 + GET_ALARM_DELTA(Alarm);

    return E_OK;
}/* End of GetAlarm */


/*
***************************************************************************
* StatusType    SetRelAlarm( AlarmType Alarm, TickType increment, TickType cycle )
* 描述：该模块被调用以后，将占用id号为<Alarm>的Alarm，然后在经过<increment>个时钟ticks之后，
*        将触发相应的任务或者相应的事件将被置位
* 调用的函数：OSInsertAlarm
* 允许被调用的位置：无
* 参数：AlarmType Alarm, TickType increment, TickType cycle
* 返回：
*            E_OK：成功
*            E_OS_STATE：ID号为<Alarm>的Alarm被占用
*           E_OS_ID：Alarm无效
*           E_OS_VALUE：increment或cycle的值越界
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：厉蒋
* 时间：2004.3.27 临界区匹配保护，以及返回值修改
* 修改人：徐卢传
* 时间：2005.6.7
* 修改人：厉蒋
* 时间：2005.7.6
***************************************************************************
*/


StatusType SetRelAlarm( AlarmType Alarm, TickType increment, TickType cycle )
{
    DEFINECRITICAL(cpu_sr);

#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)
    {
        OSRETERROR( E_OS_ID, OSServiceId_SetRelAlarm, Alarm );/* Alarm越界 */
    }
    if((increment > GET_ALARM_COUNTER_MAXVALUE(Alarm) ) 
        || ((cycle > GET_ALARM_COUNTER_MAXVALUE(Alarm))
        || (cycle != 0 && cycle < GET_ALARM_COUNTER_MINCYCLE(Alarm))))
    {
        OSRETERROR( E_OS_VALUE, OSServiceId_SetRelAlarm, Alarm );/*increment或cycle越界*/
    }                                            /* increment和cycle越界 */
#endif/*ifdef OS_EXTENDED_STATUS*/

    /*test and set 操作*/
    OS_ENTER_CRITICAL();
    if( GET_ALARM_STATE(Alarm) == AlarmUsed)
    {
        OS_EXIT_CRITICAL();
        OSRETERROR( E_OS_STATE, OSServiceId_SetRelAlarm, Alarm );/* 该Alarm已经被其他任务占用 */
    }
    OS_EXIT_CRITICAL();

     /*当increment为0的时候要特别处理,马上触发alarm*/
    if(increment == 0)
    {
      OSNotifyAlarm(Alarm);/*触发alarm*/
      if (cycle == 0)
      {
          /*一次性alarm*/
          return E_OK;
      }
      increment = cycle; /*下一次被触发所需经过的ticks*/
    }

    OS_ENTER_CRITICAL();
    Alarm->delta = increment <= GET_ALARM_COUNTER_MAXVALUE(Alarm) - GET_ALARM_COUNTER_VALUE(Alarm) ?
                              increment + GET_ALARM_COUNTER_VALUE(Alarm):
                              increment - (GET_ALARM_COUNTER_MAXVALUE(Alarm) - GET_ALARM_COUNTER_VALUE(Alarm))- 1;
    SET_ALARM_CYCLE(Alarm,cycle);
    OS_EXIT_CRITICAL();

    OSInsertAlarm(Alarm);

    return E_OK;

}/* End of SetRelAlarm */


/*
***************************************************************************
* StatusType SetAbsAlarm( AlarmType Alarm, TickType start, TickType cycle )
* 描述：该模块被调用以后，将占用id号为<Alarm>的Alarm，然后在当系统时钟数为<start>时，
*        将触发相应的任务或者相应的事件将被置位
* 调用的函数：OSInsertAlarm
* 允许被调用的位置：无
* 参数：    Alarm        标识Alarm的唯一id
*           start          Ticks的绝对值
*           cycle         周期（用于周期Alarm，如果alarm是单触发，则值为0）
* 返回：    若调用成功，则调用调度器进行重新调度
             E_OK：成功
*            E_OS_STATE: ID号为<Alarm>的Alarm被占用
*            E_OS_NOFUNC：ID号为<Alarm>的Alarm未被启用
*            E_OS_ID：Alarm无效
*            E_OS_VALUE：start或者cycle的值越界
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：潘杭平
* 时间：2005.3.17 修改对is_pass的赋值，对OSAlarms[ Alarm ].start临界区保护
* 修改人：厉蒋
* 时间：2004.3.27 临界区匹配保护，以及返回值
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

StatusType SetAbsAlarm( AlarmType Alarm, TickType start, TickType cycle )
{
    DEFINECRITICAL(cpu_sr);

#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)
    {   /* Alarm越界 */
        OSRETERROR( E_OS_ID, OSServiceId_SetAbsAlarm, Alarm );
    }
#endif

#ifdef OS_EXTENDED_STATUS
    if((cycle > GET_ALARM_COUNTER_MAXVALUE(Alarm)) || (cycle != 0 && cycle < GET_ALARM_COUNTER_MINCYCLE(Alarm) )
         || (start > GET_ALARM_COUNTER_MAXVALUE(Alarm)))
    {
        OSRETERROR( E_OS_VALUE, OSServiceId_SetAbsAlarm, Alarm );/* start和cycle越界 */
    }
#endif
    /*test and set 操作*/
    OS_ENTER_CRITICAL();
    if(GET_ALARM_STATE(Alarm) == AlarmUsed)
    {
        OS_EXIT_CRITICAL();
        OSRETERROR( E_OS_STATE, OSServiceId_SetAbsAlarm, Alarm );/* 该Alarm已经被其他任务占用 */
    }
    OS_EXIT_CRITICAL();

    /*当start为当前Counter的值的时候，特别处理，马上触发该alarm*/
    if(start == GET_ALARM_COUNTER_VALUE(Alarm))
    {
      OSNotifyAlarm(Alarm);/*触发alarm*/
      if (cycle == 0)
      {
          /*一次性alarm*/
          return E_OK;
      }
      /*start: 下一次被触发的时间*/
      if (start <= GET_ALARM_COUNTER_MAXVALUE(Alarm) - cycle)
      {
          start += cycle;
      }
      else
      {
          start -= GET_ALARM_COUNTER_MAXVALUE(Alarm) - cycle +1;
      }
    }

    OS_ENTER_CRITICAL();
    SET_ALARM_DELTA(Alarm, start);
    SET_ALARM_CYCLE(Alarm, cycle);
    OS_EXIT_CRITICAL();

    OSInsertAlarm(Alarm);

    return E_OK;

}/* End of SetAbsAlarm */


/*
***************************************************************************
* StatusType CancelAlarm( AlarmType Alarm )
* 描述：取消ID号为<Alarm>的Alarm的使用
* 调用的函数：OSDeleteAlarm
* 允许被调用的位置：无
* 参数：    Alarm        标识Alarm的唯一id
* 返回：    E_OK：调用成功
*           E_OS_NOFUNC：ID号为<Alarm>的Alarm未被启用
*           E_OS_ID：Alarm越界
* 注意事项：
* 创建人：潘杭平
* 时间：2004.12.4
* 修改人：孟晓楠
* 时间：2005.1.19
* 修改人：潘杭平
* 时间：2004.3.17 临界区保护
* 修改人：厉蒋
* 时间：2004.3.27 临界区匹配保护
* 修改人：徐卢传
* 时间：2005.6.7
***************************************************************************
*/

StatusType CancelAlarm( AlarmType Alarm )
{
#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)
    {
        OSRETERROR( E_OS_ID, OSServiceId_CancelAlarm, Alarm );/* Alarm越界 */
    }
#endif

    if(GET_ALARM_STATE(Alarm) == AlarmUnUsed )
    {
        OSRETERROR( E_OS_NOFUNC, OSServiceId_CancelAlarm, Alarm );/* 表明ID号为Alarm的alarm未被启用 */
    }

    OSDeleteAlarm(Alarm);

    return    E_OK;
}/* End of CancelAlarm */


#endif /*#ifndef  OS_ALARM_C */