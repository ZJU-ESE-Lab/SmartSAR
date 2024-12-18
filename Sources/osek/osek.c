/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�osek.c
* ժ    Ҫ��������ģ���*.C�ļ�
*
* ��ǰ�汾��1.1
* ��    �ߣ�����
* ������ڣ�2005��2��20��
*
* ȡ���汾��1.0
* ԭ����  ���˺�ƽ
* ������ڣ�2004��9��14��
*********************************************************************************
*/

#include "includes.h"
#define  OS_MASTER_FILE   /* ��ֹ�����ļ��ظ�����includes.h*/

#include "global.c"
#include "os_core.c"
#include "os_task.c"

#ifdef OS_RESOURCE_EN
#include "os_res.c"
#endif

#ifdef OS_EVENT_EN
#include "os_evt.c"
#endif

#ifdef OS_ALARM_EN
#include "os_alarm.c"
#endif

#ifdef OS_EXCEPTION_EN
#include "os_irq.c"
#endif

#ifdef WatchSystemInfo
#include "WatchSystemInfo.c"
#endif

#ifdef	COM_EN
#include "com.c"
#endif
