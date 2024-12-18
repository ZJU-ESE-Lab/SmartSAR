/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：osek.c
* 摘    要：包含各模块的*.C文件
*
* 当前版本：1.1
* 作    者：厉蒋
* 完成日期：2005年2月20日
*
* 取代版本：1.0
* 原作者  ：潘杭平
* 完成日期：2004年9月14日
*********************************************************************************
*/

#include "includes.h"
#define  OS_MASTER_FILE   /* 防止下列文件重复包含includes.h*/

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
