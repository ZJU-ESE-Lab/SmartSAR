/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_alarm.c
* ժ    Ҫ��ʵ��alarmģ���API
*
* ��ǰ�汾��1.4
* ��    �ߣ���¬��
* ������ڣ�2005��6��7��
*
* ȡ���汾��1.3
* ��    �ߣ������
* ������ڣ�2005��3��7��
*
* ȡ���汾��1.2
* ��    �ߣ�����骡�����
* ������ڣ�2005��3��1��
*
* ȡ���汾��1.1
* ��    �ߣ��˺�ƽ
* ������ڣ�2004��9��27��
*
* ȡ���汾��1.0
* ԭ �� ��  ��������
* ������ڣ�2003��12��23��
*********************************************************************************
*/

#ifndef OS_ALARM_C
#define OS_ALARM_C

#ifndef OS_MASTER_FILE
#include "includes.h"
#endif

AlarmCB OSAlarms[OS_NUM_ALARM]; /*Alarm��������*/
CtrCB OSCounters[OS_NUM_CTR];   /*Counter��������*/

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
* ˵��������Alarm�ĳ�ʼ��
* ���õĺ�������
* �������õ�λ�ã�StartOS
* ��������
* ���أ���
* ע�����
* �����ˣ�����
* ʱ�䣺2005.7.6
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
	{   /*��ʼ��Counter*/
		counter->CtrInfo = counterCfg;
        counter->value = 0;
        counter->head = NULL;        
	}
	for(;alarm<OSAlarms+OS_NUM_ALARM;alarm++,alarmCfg++)
	{   /*��ʼ��Alarm*/
        alarm->start = AlarmUnUsed;
        alarm->ctr = &OSCounters[alarmCfg->CtrID];
        alarm->task = &OSTCBTbl[alarmCfg->TaskID];
#ifdef OS_EVENT_EN
        alarm->event = alarmCfg->event;
#endif /*#ifdef OS_EVENT_EN*/

#ifdef OS_ALARM_CALLBACK_EN
        alarm->AlarmCallBack = alarmCfg->AlarmCallBack;
#endif /*#ifdef OS_ALARM_CALLBACK_EN*/
       if(alarmCfg->AutoStart==ALARM_AUTO_START)/*���Ϊ����������������delta��cycle*/
       {
           alarm->delta=alarmCfg->increment;
           alarm->cycle = alarmCfg->cycle;
           OSInsertAlarm(alarm);
       }
	}
	
}
#else/*#ifdef OS_ALARM_AUTO_DEFINE*/
static INT8U AlarmIndex = 0;    /*Alarm��������*/
static INT8U CounterIndex = 0;  /*Couter��������*/
/*
***************************************************************************
* AlarmType DefineAutoAlarm(CtrType ctr,TaskType id, EventMaskType event,
*        OS_CALLBACK_ENTRY callback , TickType increment,TickType cycle)
* ������    ����һ����������alarm,�������ͷ���������oil�ļ�������
*            ����oil�ļ��з�����alarm��Ϣ��䵽alarm���õ����ݽṹ��
* ���õĺ�����OSInsertAlarm
* �������õ�λ�ã���
* ������ctr:counter
        id:����������id,�����������callback��Ϊ0
*       event:�������¼�����,û����Ϊ0
*       callback:������callback�������,������ָ��,���û��,��Ϊ0
*       increment�� cycle�ĺ�����SetRelalarm�����в�������һ��
* ���أ�AlarmType
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ�����
* ʱ�䣺2005.3.1
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
***************************************************************************
*/

AlarmType DefineAutoAlarm(CtrType ctr,TaskType task,
#ifdef OS_EVENT_EN
                    EventMaskRefType event,
#endif /*#ifdef OS_EVENT_EN*/
#ifdef OS_ALARM_CALLBACK_EN
                    OS_CALLBACK_ENTRY callback,
#endif/*#ifdef OS_ALARM_CALLBACK_EN*/
                    TickType increment,TickType cycle)  /*����һ���Զ�������alarm*/
{
#ifdef OS_SHOW_OTHER_ERROR        /*��ʾ����������Ϣ*/
    if(AlarmIndex >= OS_NUM_ALARM)
        RETURN_OTHER_ERROR(E_OS_SYS_PARA);
#endif /*ifdef OS_SHOW_OTHER_ERROR*/
    AlarmType alarm = &OSAlarms[AlarmIndex];
    alarm->start = AlarmUsed;/*���alarm�Ѿ�����*/
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

    OSInsertAlarm(alarm);/*���뵽counter������������ȥ*/

    AlarmIndex ++;
    return alarm;
}

/*
***************************************************************************
* AlarmType DefineAlarm(CtrType ctr,TaskType id, EventMaskType event,OS_CALLBACK_ENTRY callback )
* ����������һ������������alarm
*        ����oil�ļ��з�����alarm��Ϣ��䵽alarm���õ����ݽṹ��
* ���õĺ�������
* �������õ�λ�ã���
* ������
*    ctr:counter
*     id:����������id,�����������callback��Ϊ0
*    event:�������¼�����,û����Ϊ0
*    callback:������callback�������,������ָ��,���û��,��Ϊ0
* ���أ�AlarmType
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ�����
* ʱ�䣺2005.3.1
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
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

#ifdef OS_SHOW_OTHER_ERROR /*��ʾ����������Ϣ*/
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
* ����������һ��counter
*    ����oil�ļ��з�����counter��Ϣ��䵽counter���õ����ݽṹ��
* ���õĺ�������
* �������õ�λ�ã���
* ������
*    maxallowedvalue: ���������tick��
*    mincycle: ���������С������
*    ticksperbase: һ��counter unit�൱�ڶ��ٸ�ticks
* ���أ�CtrType
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
***************************************************************************
*/

CtrType DefineCounter(TickType maxallowedvalue,
            TickType mincycle,
            TickType ticksperbase)
{

#ifdef OS_SHOW_OTHER_ERROR  /*��ʾ����������Ϣ*/
    if(CounterIndex >= OS_NUM_CTR)
        RETURN_OTHER_ERROR(E_OS_SYS_PARA);
#endif  /*ifdef OS_SHOW_OTHER_ERROR*/
    CtrType counter = &OSCounters[CounterIndex];
    counter->CtrInfo->maxallowedvalue = maxallowedvalue;
#ifdef OS_EXTENDED_STATUS
    counter->CtrInfo->mincycle = mincycle;
#else
    mincycle = mincycle;/*ȥ��Warnning*/     
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
* ˵��������һ�δ���ʱ���˳�򣬽�alarm��������Counter�е�alarms������,
*       �������ø�alarm�Ѿ�����������²����alarm��������ͷ���������counter��
*       RemainTicks.
* ���õĺ�������
* �������õ�λ�ã�SetAbsAlarm,SetRelAlarm,DefineAutoAlarm
* ������AlarmCBRef alarm
* ���أ���
* ע�����
* �����ˣ���¬��
* ʱ�䣺2005.6.7
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

    /*�������Ϊ�գ���ֱ�ӽ�alarm��������ͷ*/
    if (head == NULL)
    {
        SET_ALARM_COUNTER_HEAD(alarm, alarm);
        SET_NEXT_ALARM(alarm, NULL);
        SET_PRV_ALARM(alarm, NULL);

        /*����ͷ�ı䣬����RemainTicks.*/
        alarm->ctr->RemainTicks = GET_ALARM_DELTA(alarm) > value?
                                 GET_ALARM_DELTA(alarm) - value:
                                 GET_ALARM_COUNTER_MAXVALUE(alarm) - value + GET_ALARM_DELTA(alarm) + 1;
        OS_EXIT_CRITICAL();
        return;
    }

    /*��alarm������뵽����*/
    temp = head;
    last = NULL;/*���ڱ��洦��temp֮ǰ��alarm*/
    /*1) �ҵ���һ������'alarm'֮�󱻴�����ALARM,��temp��ʾ*/
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

    /*2) ��'alarm'�嵽'temp'֮ǰ*/
    alarm->prev = last;
    if (last) last->next = alarm;

    alarm->next = temp;
    if (temp) temp->prev = alarm;

    /*���'last'Ϊ�գ����ʾ'alarm'�ᱻ�嵽����ͷ�����Ը���RemainTicks.*/
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
* ˵������alarm������Counter�е�alarms������ɾ���������ø�alarmΪ������״̬��
        ���ɾ����alarmԭ����������ͷ�������Counter��RemainTicks��
* ���õĺ�������
* �������õ�λ�ã�CancelAlarm
* ������AlarmCBRef alarm
* ���أ���
* ע�����
* �����ˣ���¬��
* ʱ�䣺2005.6.7
***************************************************************************
*/
void OSDeleteAlarm(AlarmCBRef alarm)
{
    DEFINECRITICAL(cpu_sr);

    OS_ENTER_CRITICAL();
    if(alarm == GET_ALARM_COUNTER_HEAD(alarm))
    {   /*'alarm' ������ͷ*/

        /*��alarm��counter��alarm����ɾ��alarm*/
        SET_ALARM_COUNTER_HEAD(alarm, GET_NEXT_ALARM(alarm));

        /*���alarm����һ��ALARM������һ��ALARM��������ͷ������RemainTicks*/
        if(GET_NEXT_ALARM(alarm))
        {
            SET_PRV_ALARM(GET_NEXT_ALARM(alarm), NULL);
            alarm->ctr->RemainTicks = GET_ALARM_DELTA(GET_NEXT_ALARM(alarm)) > GET_ALARM_COUNTER_VALUE(alarm)?
                                     GET_ALARM_DELTA(GET_NEXT_ALARM(alarm)) - GET_ALARM_COUNTER_VALUE(alarm):
                                     GET_ALARM_COUNTER_MAXVALUE(alarm) - GET_ALARM_COUNTER_VALUE(alarm) + 1 + GET_ALARM_DELTA(GET_NEXT_ALARM(alarm));
        }
    }
    else
    {    /* ��������ɾ��'alarm'*/
        SET_NEXT_ALARM(GET_PRV_ALARM(alarm),GET_NEXT_ALARM(alarm));
        if (GET_NEXT_ALARM(alarm))
        SET_PRV_ALARM(GET_NEXT_ALARM(alarm), GET_PRV_ALARM(alarm));
    }

    SET_NEXT_ALARM(alarm, NULL);
    SET_PRV_ALARM(alarm, NULL);
    SET_ALARM_UNUSED(alarm);        /*������λ*/
    OS_EXIT_CRITICAL();

}


/*
***************************************************************************
* void OSCounterTrigger(CtrType ctr)
* ��������ctr����ֵ(value)��1���жϲ�������ʱexpired��alarms��
* ���õĺ�����OSCheckAlarms
* �������õ�λ�ã�DECHandler���ļ�OS_CPU_C.c��
* ��������
* ���أ���
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
***************************************************************************
*/

void OSCounterTrigger(CtrType ctr)
{
    DEFINECRITICAL(cpu_sr);
    OS_ENTER_CRITICAL();
    if (GET_COUNTER_VALUE(ctr) == GET_COUNTER_MAXVALUE(ctr))
    {
        /*�ﵽmaxallowedvalue������Ϊ0*/
        SET_COUNTER_VALUE(ctr,0);
    }
    else
    {
        COUNTER_VALUE_GROW(ctr);
    }
    if (GET_HEAD_ALARM(ctr) != NULL)/*�ж��Ƿ������õ�alarms*/
    {
        REMAIN_TICKS_DECREASE(ctr);
        
        if (GET_COUNTER_REMAIN_TICKS(ctr) == 0)
        {
            /*������һ��alarm�ᱻ����������checkһ�£�������RemainTicks*/
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
* ����������crtId������Counter���������ĵ�ǰexpired������alarms��
* ���õĺ�����OSNotifyAlarm
* �������õ�λ�ã�OSISRCounterTrigger
* ������CtrType ctrId
* ���أ���
* ע�����
* �����ˣ���¬��
* ʱ�䣺2005.6.7
***************************************************************************
*/

void OSCheckAlarms( CtrType ctr )
{

    AlarmCBRef alarmRef, alarmRef2;
    AlarmCBRef head = NULL;
    AlarmCBRef temp, temp2, last;
    TickType value = GET_COUNTER_VALUE(ctr);

    alarmRef = alarmRef2 = GET_HEAD_ALARM(ctr);

    /*����ǰ��k���������� alarms����������(k>=1) */
    while (alarmRef != NULL)
    {
        
        if (GET_ALARM_DELTA(alarmRef) != value)
        {
            break;
        }
        /*����alarm,���Ϊ����alarm�����������deltaֵ����һ�δ���ʱ�䣩*/
        OSNotifyAlarm(alarmRef);
        
        alarmRef = GET_NEXT_ALARM(alarmRef);
    }
    /*ָ��ʣ��δ����alarms������ĵ�һ��alarm��ָ��*/
    head = alarmRef;
    /*�ڶ��α���ǰ��k���ոձ�������alarms, �������ԭ����δ��������alarms������*/
    while (alarmRef2 != alarmRef)
    {
        /*����һ�� alrmRef2->next*/
        temp2 = GET_NEXT_ALARM(alarmRef2);
        
        if (GET_ALARM_CYCLE(alarmRef2) == 0)
        {
            /*һ���Ե�alarm, ��ֱ��ɾ����*/
            SET_NEXT_ALARM(alarmRef2, NULL); 
            SET_PRV_ALARM(alarmRef2, NULL);
            SET_ALARM_UNUSED(alarmRef2);

            alarmRef2 = temp2;
            continue;
        }

        /*���ʣ������Ϊ�գ���ֱ�ӽ�alarmRef2��Ϊ����ͷ*/
        if (head == NULL)
        {
            head = alarmRef2;
            SET_NEXT_ALARM(alarmRef2, NULL); 
            SET_PRV_ALARM(alarmRef2, NULL);            
            
            alarmRef2 = temp2;
            continue;
        }

        /*��alarmRef2�����������*/
        temp = head;
        last = NULL;
        
        /*�ҵ���һ������'alarmRef2'֮�󱻴�����ALARM,��temp��ʾalarmRef2*/
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
        /*����һ��ALARM����alarmRef2*/
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
* ������ִ��alarm����Ӧ�Ĳ����������cycle alarm���������趨deltaֵ��
* ���õĺ�����SetEvent, ActivateTask, AlarmCallBack
* �������õ�λ�ã�OSCheckAlarms
* ������AlarmCBRef alarm
* ���أ���
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ�����
* ʱ�䣺2005.3.1
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
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
* ��������ѯcounter״̬,���ID��Ϊ<Alarm>��Alarm��counter����Ϣ
* ���õĺ�������
* �������õ�λ�ã���
* ������AlarmType  Alarm, AlarmBaseRefType Info
* ���أ� E_OK:    ���óɹ�
*        E_OS_ID: AlarmԽ��
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
***************************************************************************
*/

StatusType GetAlarmBase (AlarmType  Alarm, AlarmBaseRefType Info )
{

#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)    /* �ж�Alarm�Ƿ���Ч */
    {
        OSRETERROR( E_OS_ID, OSServiceId_GetAlarmBase, Alarm );
    }
#endif
    /* ����Counter�Ļ�����Ϣ */
    memcpy(Info,(GET_ALARM_COUNTER_INFO(Alarm)),sizeof(AlarmBaseType));
    return E_OK;
}

/*
***************************************************************************
* StatusType GetAlarm( AlarmType Alarm, TickRefType Tick )
* ��������ѯalarm״̬�����ID��Ϊ<Alarm>��Alarm����ʱ�̵�����ʱ������ticksֵ
* ���õĺ�������
* �������õ�λ�ã���
* ������AlarmType Alarm, TickRefType  tick
* ���أ�    E_OK�����óɹ�
*           E_OS_NOFUNC,��ID��Ϊ<Alarm>��Alarmδ������
*           E_OS_ID��AlarmԽ��
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
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
        OSRETERROR( E_OS_NOFUNC, OSServiceId_GetAlarm, Alarm );   /* ����ID��ΪAlarm��alarmδ������ */
    }

    *Tick = GET_ALARM_DELTA(Alarm) >= GET_ALARM_COUNTER_VALUE(Alarm)?
            GET_ALARM_DELTA(Alarm) - GET_ALARM_COUNTER_VALUE(Alarm):
            GET_ALARM_COUNTER_MAXVALUE(Alarm) - GET_ALARM_COUNTER_VALUE(Alarm) + 1 + GET_ALARM_DELTA(Alarm);

    return E_OK;
}/* End of GetAlarm */


/*
***************************************************************************
* StatusType    SetRelAlarm( AlarmType Alarm, TickType increment, TickType cycle )
* ��������ģ�鱻�����Ժ󣬽�ռ��id��Ϊ<Alarm>��Alarm��Ȼ���ھ���<increment>��ʱ��ticks֮��
*        ��������Ӧ�����������Ӧ���¼�������λ
* ���õĺ�����OSInsertAlarm
* �������õ�λ�ã���
* ������AlarmType Alarm, TickType increment, TickType cycle
* ���أ�
*            E_OK���ɹ�
*            E_OS_STATE��ID��Ϊ<Alarm>��Alarm��ռ��
*           E_OS_ID��Alarm��Ч
*           E_OS_VALUE��increment��cycle��ֵԽ��
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ�����
* ʱ�䣺2004.3.27 �ٽ���ƥ�䱣�����Լ�����ֵ�޸�
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
* �޸��ˣ�����
* ʱ�䣺2005.7.6
***************************************************************************
*/


StatusType SetRelAlarm( AlarmType Alarm, TickType increment, TickType cycle )
{
    DEFINECRITICAL(cpu_sr);

#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)
    {
        OSRETERROR( E_OS_ID, OSServiceId_SetRelAlarm, Alarm );/* AlarmԽ�� */
    }
    if((increment > GET_ALARM_COUNTER_MAXVALUE(Alarm) ) 
        || ((cycle > GET_ALARM_COUNTER_MAXVALUE(Alarm))
        || (cycle != 0 && cycle < GET_ALARM_COUNTER_MINCYCLE(Alarm))))
    {
        OSRETERROR( E_OS_VALUE, OSServiceId_SetRelAlarm, Alarm );/*increment��cycleԽ��*/
    }                                            /* increment��cycleԽ�� */
#endif/*ifdef OS_EXTENDED_STATUS*/

    /*test and set ����*/
    OS_ENTER_CRITICAL();
    if( GET_ALARM_STATE(Alarm) == AlarmUsed)
    {
        OS_EXIT_CRITICAL();
        OSRETERROR( E_OS_STATE, OSServiceId_SetRelAlarm, Alarm );/* ��Alarm�Ѿ�����������ռ�� */
    }
    OS_EXIT_CRITICAL();

     /*��incrementΪ0��ʱ��Ҫ�ر���,���ϴ���alarm*/
    if(increment == 0)
    {
      OSNotifyAlarm(Alarm);/*����alarm*/
      if (cycle == 0)
      {
          /*һ����alarm*/
          return E_OK;
      }
      increment = cycle; /*��һ�α��������辭����ticks*/
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
* ��������ģ�鱻�����Ժ󣬽�ռ��id��Ϊ<Alarm>��Alarm��Ȼ���ڵ�ϵͳʱ����Ϊ<start>ʱ��
*        ��������Ӧ�����������Ӧ���¼�������λ
* ���õĺ�����OSInsertAlarm
* �������õ�λ�ã���
* ������    Alarm        ��ʶAlarm��Ψһid
*           start          Ticks�ľ���ֵ
*           cycle         ���ڣ���������Alarm�����alarm�ǵ���������ֵΪ0��
* ���أ�    �����óɹ�������õ������������µ���
             E_OK���ɹ�
*            E_OS_STATE: ID��Ϊ<Alarm>��Alarm��ռ��
*            E_OS_NOFUNC��ID��Ϊ<Alarm>��Alarmδ������
*            E_OS_ID��Alarm��Ч
*            E_OS_VALUE��start����cycle��ֵԽ��
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ��˺�ƽ
* ʱ�䣺2005.3.17 �޸Ķ�is_pass�ĸ�ֵ����OSAlarms[ Alarm ].start�ٽ�������
* �޸��ˣ�����
* ʱ�䣺2004.3.27 �ٽ���ƥ�䱣�����Լ�����ֵ
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
***************************************************************************
*/

StatusType SetAbsAlarm( AlarmType Alarm, TickType start, TickType cycle )
{
    DEFINECRITICAL(cpu_sr);

#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)
    {   /* AlarmԽ�� */
        OSRETERROR( E_OS_ID, OSServiceId_SetAbsAlarm, Alarm );
    }
#endif

#ifdef OS_EXTENDED_STATUS
    if((cycle > GET_ALARM_COUNTER_MAXVALUE(Alarm)) || (cycle != 0 && cycle < GET_ALARM_COUNTER_MINCYCLE(Alarm) )
         || (start > GET_ALARM_COUNTER_MAXVALUE(Alarm)))
    {
        OSRETERROR( E_OS_VALUE, OSServiceId_SetAbsAlarm, Alarm );/* start��cycleԽ�� */
    }
#endif
    /*test and set ����*/
    OS_ENTER_CRITICAL();
    if(GET_ALARM_STATE(Alarm) == AlarmUsed)
    {
        OS_EXIT_CRITICAL();
        OSRETERROR( E_OS_STATE, OSServiceId_SetAbsAlarm, Alarm );/* ��Alarm�Ѿ�����������ռ�� */
    }
    OS_EXIT_CRITICAL();

    /*��startΪ��ǰCounter��ֵ��ʱ���ر������ϴ�����alarm*/
    if(start == GET_ALARM_COUNTER_VALUE(Alarm))
    {
      OSNotifyAlarm(Alarm);/*����alarm*/
      if (cycle == 0)
      {
          /*һ����alarm*/
          return E_OK;
      }
      /*start: ��һ�α�������ʱ��*/
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
* ������ȡ��ID��Ϊ<Alarm>��Alarm��ʹ��
* ���õĺ�����OSDeleteAlarm
* �������õ�λ�ã���
* ������    Alarm        ��ʶAlarm��Ψһid
* ���أ�    E_OK�����óɹ�
*           E_OS_NOFUNC��ID��Ϊ<Alarm>��Alarmδ������
*           E_OS_ID��AlarmԽ��
* ע�����
* �����ˣ��˺�ƽ
* ʱ�䣺2004.12.4
* �޸��ˣ������
* ʱ�䣺2005.1.19
* �޸��ˣ��˺�ƽ
* ʱ�䣺2004.3.17 �ٽ�������
* �޸��ˣ�����
* ʱ�䣺2004.3.27 �ٽ���ƥ�䱣��
* �޸��ˣ���¬��
* ʱ�䣺2005.6.7
***************************************************************************
*/

StatusType CancelAlarm( AlarmType Alarm )
{
#ifdef OS_EXTENDED_STATUS
    if( (Alarm >= (OSAlarms + OS_NUM_ALARM)) || (Alarm < OSAlarms) 
        || (((AddrType)Alarm-(AddrType)OSAlarms)%sizeof(AlarmCB))!=0)
    {
        OSRETERROR( E_OS_ID, OSServiceId_CancelAlarm, Alarm );/* AlarmԽ�� */
    }
#endif

    if(GET_ALARM_STATE(Alarm) == AlarmUnUsed )
    {
        OSRETERROR( E_OS_NOFUNC, OSServiceId_CancelAlarm, Alarm );/* ����ID��ΪAlarm��alarmδ������ */
    }

    OSDeleteAlarm(Alarm);

    return    E_OK;
}/* End of CancelAlarm */


#endif /*#ifndef  OS_ALARM_C */