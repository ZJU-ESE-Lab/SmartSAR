/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_task_cpu.h
* 摘    要：task模块中与平台相关的宏定义以及相关操作,这里的平台是HCS12
*
* 当前版本：2.0
* 作    者：郁利吉
* 完成日期：2005年12月12日
*
*********************************************************************************
*/

#ifndef OS_TASK_CPU_H
#define OS_TASK_CPU_H

/*多一个堆栈用于解决chaitask问题*/
#define ExtraStackSize 100
//extern OSSTK ExtraStack[];

/********************其他平台************/
extern  INT8U   OSRdyGrp;                        /*就绪队列数组索引*/
extern  INT8U   OSRdyTbl[];       /*就绪队列子数组*/





OSSTKRef OSTaskStkInit(OSTaskEntry task, OSSTKRef ptos);   /*栈初始化*/
        
/*
*********************************************************************************************************
* 全局变量的声明
*********************************************************************************************************
*/
extern  OSTCBRef           OSTCBOld;				/*指向刚刚切换的任务*/

/*******************************平台相关的额外定义***********************/

extern OSSTK ExtraStack[];
extern OSTCBRef           OSTCBOld;/*指向刚刚切换的任务*/
extern OSSTKRef           FreeStkTop;
extern OSSTKRef           FreeStkBottom;

/***************************定义就绪表*******************************/
extern INT8U  OSRdyGrp;/* 就绪数组索引 */
extern INT8U  OSRdyTbl[];/* 就绪队列 */   
    
#endif /*#ifndef OS_TASK_CPU_H*/