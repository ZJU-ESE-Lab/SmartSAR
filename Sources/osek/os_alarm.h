/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_alarm.h
* ժ    Ҫ��alarmģ��ĵĺ꿪���Լ�API������
*  
* ��ǰ�汾��1.4
* ��    �ߣ�����
* ������ڣ�2005��7��6��
*
* ��ǰ�汾��1.3
* ��    �ߣ���¬��
* ������ڣ�2005��6��7��
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

#ifndef OS_ALARM_H
#define OS_ALARM_H


#define AlarmUsed   1
#define AlarmUnUsed 0
#define ALARM_AUTO_START 1
/**********************************************************************************
�������������û�ʹ��OSMAXALLOWEDVALUE_x��ʱ����������define ����
#define OSMAXALLOWEDVALUE_1 GETMAXALLOWEDVALUE(1)
����ж������ѭ������OSMAXALLOWEDVALUE_x���Ӷ�ʵ��OSEK��׼������OSMAXALLOWEDVALUE_1�ĳ���
*****************************************************************/
#define GETMAXALLOWEDVALUE(Index)  OSCounters[Index].CtrInfo->maxallowedvalue
#define GETTICKSPERBASE(Index)  OSCounters[Index].CtrInfo->ticksperbase
#define GETMINCYCLE(Index)  OSCounters[Index].CtrInfo->mincycle

/***************************************************************************
����ΪOSEK��׼�涨�ĳ���
****************************************************************************/
#define OSMAXALLOWEDVALUE  OSCounters[0].CtrInfo->maxallowedvalue
#define OSTICKSPERBASE  OSCounters[0].CtrInfo->ticksperbase
#define OSMINCYCLE  OSCounters[0].CtrInfo->mincycle
#define OSTICKDURATION (1000000000/(OS_TICKS_PER_SEC)) /*һTick��������*/

#define ALARMCALLBACK(name) void name(void)    /*ALARMCALLBACK�궨�壬��������һ��alarm�ؽк���*/

#define DeclareAlarm(AlarmIdentifier) extern AlarmType AlarmIdentifier


typedef INT32U TickType;


typedef TickType* TickRefType;     /*TickTypeָ��*/

typedef struct AlarmOSCB*  AlarmType;          /*AlarmOSCB��ָ�룬���ڱ�ʾalarm������*/

/*********************************************************
�����ݽṹ�����û�����Counter��Ϣ��Ҳ�Ǳ�׼��Ҫ�������
***********************************************************/
typedef struct AlarmBase
{
    TickType maxallowedvalue;     /* ���ļ���ֵ����TicksΪ��λ�ģ� */
    TickType ticksperbase;        /* �Զ���ticksΪһ��counter�ض��ĵ�λ */
#ifdef OS_EXTENDED_STATUS
    TickType mincycle;            /* �������alarm�������С����ֵ����λ��tick������
                                    SetRelAlarm/SetAbsAlarm�����е�cycle������Ч��
                                    ����жϣ�ֻ����չ״̬�²���Ҫ����*/
#endif/*#ifdef OS_EXTENDED_STATUS*/
} AlarmBaseType;
typedef AlarmBaseType* AlarmBaseRefType;
typedef struct AlarmOSCB* AlarmCBRef;

/*********************************************************
�����ݽṹ����Counter�Ŀ���
***********************************************************/
typedef struct CtrOSCB
{
    TickType value;          /*��ǰ��counter��tickֵ����ֵӦ��λ������[0, CtrInfo->maxallowedvalue]֮��*/
    TickType RemainTicks;    /*��ʣ���ticks,��Ϊ0ʱ��������������������õ�alarm��������һ��
                               �ᱻ������ע�����headΪ��ʱ����RemainTicks�����塣*/    
    
    AlarmCBRef head;         /*�����������õ�alarm�����ͷָ��*/
    AlarmBaseRefType CtrInfo;   /*��Ӧ��couter������Ϣ*/    
}CtrCB;

typedef CtrCB* CtrType;

typedef void(*OS_CALLBACK_ENTRY)(void); /*callback������ڶ���*/


/*********************************************************
�����ݽṹ�����û�����Alarm��Ϣ
***********************************************************/
typedef struct 
{
    INT8U CtrID;       /*GetAlarmBase�������õ���*/
    INT8U AutoStart;      /*�Ƿ�������*/
    TickType increment;   /*��һ���������tickֵ�����Ϊ������������Ҫ����*/
    TickType cycle;       /*���ڣ����Ϊ������������Ҫ����*/
    PriorityType TaskID;  /*����������*/
    EventMaskRefType event;  /*�������¼�����*/
    OS_CALLBACK_ENTRY AlarmCallBack;   /*callback��ں���*/    
}OSAlarmCfg;

/*********************************************************
�����ݽṹ����Alarm�Ŀ���
***********************************************************/
typedef struct AlarmOSCB
{
    CtrType ctr;        /*GetAlarmBase�������õ���*/  
    TickType delta;     /*��һ���������tickֵ*/
    TickType cycle;     /*����*/    
    TaskType task;      /*����������*/
#ifdef OS_ALARM_CALLBACK_EN
    OS_CALLBACK_ENTRY AlarmCallBack;   /*callback��ں���*/
#endif /*#ifdef OS_ALARM_CALLBACK_EN*/
    
#ifdef OS_EVENT_EN
    EventMaskRefType event;  /*�������¼�����*/
#endif /*#ifdef OS_EVENT_EN*/

    AlarmCBRef next;      /*ָ�������counter�������һ��alarm*/
    AlarmCBRef prev;      /*Ϊ�������ɾ����������øñ���*/
    INT8U start;          /* ״̬λ����־��Alarm�Ƿ�����, 0��ʾδ�����ã���0�෴ */

}AlarmCB;

/*********************************************************
������
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
/*��ALM2SET���뵽PALARM����counter��ALARM��ı�ͷ*/
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
ȫ�ֱ�������
***********************************************************/
extern AlarmCB OSAlarms[];/*Alarm��������*/   
extern CtrCB OSCounters[];/*Counter��������*/
#ifdef OS_ALARM_AUTO_DEFINE
void OSAlarmInit(void);
#else
CtrType DefineCounter(TickType  maxallowedvalue,
            TickType mincycle,         
            TickType ticksperbase);    /*OIL����������Ҫ�ĺ���������(����)һ��counter*/        
        
AlarmType DefineAlarm(CtrType ci, TaskType id
#ifdef OS_EVENT_EN
                    , EventMaskRefType event
#endif/*#ifdef OS_EVENT_EN*/
#ifdef OS_ALARM_CALLBACK_EN
                    , OS_CALLBACK_ENTRY callback
#endif/*#ifdef OS_ALARM_CALLBACK_EN*/
                ); /*����һ��alarm*/

AlarmType DefineAutoAlarm(CtrType ci,TaskType id, 
#ifdef OS_EVENT_EN
                    EventMaskRefType event,
#endif/*#ifdef OS_EVENT_EN*/
#ifdef OS_ALARM_CALLBACK_EN
                    OS_CALLBACK_ENTRY callback,
#endif/*#ifdef OS_ALARM_CALLBACK_EN*/
                    TickType increment,TickType cycle);    /*����һ���Զ�������alarm*/
#endif/*#ifdef OS_ALARM_AUTO_DEFINE*/
StatusType GetAlarm( AlarmType AlarmID, TickRefType tick );
StatusType CancelAlarm( AlarmType AlarmID );
StatusType GetAlarmBase ( AlarmType AlarmID,AlarmBaseRefType Info );
StatusType SetRelAlarm( AlarmType AlarmID, TickType increment, TickType cycle );
StatusType SetAbsAlarm( AlarmType AlarmID, TickType start, TickType cycle );

void OSCounterTrigger(CtrType isr);        /*ÿ���ж϶�Ӧ��Counter�Ĵ�����,OS_CPU_C.c���õ�*/

#endif