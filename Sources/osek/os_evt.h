/*
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_evt.h
* ժ    Ҫ���������¼�(Event)ģ��ʹ�õĽṹ�ͱ�׼API�ӿں���
*
* ��ǰ�汾��1.1
* ��    �ߣ���ˬ
* ������ڣ�2005��1��17��
*
* ȡ���汾��1.0
* ԭ����  ��ԭ���ߣ���
* ������ڣ�2004��12��10��
*/

#ifndef	 OS_EVT_H
#define	 OS_EVT_H

#define NOSETEVENT  0

#define DeclareEvent(Event) extern EventMaskType Event

#ifdef OS_ALARM_EN
void OSSetEvent ( TaskType TaskID,EventMaskRefType Mask );
#endif/*#ifdef OS_ALARM_EN*/

/*     
*��׼API�ӿں���
*/                             
StatusType SetEvent ( TaskType TaskID,EventMaskType Mask );

StatusType ClearEvent ( EventMaskType Mask );

StatusType GetEvent ( TaskType TaskID,EventMaskRefType Event );

StatusType WaitEvent ( EventMaskType Mask );


#endif