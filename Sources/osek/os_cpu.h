/*
***************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_cpu.h
* 摘    要：HCS12底层数据结构定义和操作宏定义
*
* 当前版本：1.1
* 作    者：厉将
* 完成日期：2005年8月1日
*
* 取代版本：1.0
* 原作者  ：姜旭锋
* 完成日期：2004年12月4日
***************************************************************************
*/
#ifndef  OS_CPU_H
#define  OS_CPU_H

/*
*********************************************************************************************************
*                                              数据类型
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    
typedef signed   char  INT8S;                    
typedef unsigned int   INT16U;                   
typedef signed   int   INT16S;                  
typedef unsigned long  INT32U;                  
typedef signed   long  INT32S;                  
typedef float          FP32;                    
typedef double         FP64;                   
typedef unsigned long long INT64U;              

typedef INT16U         AddrType;                /*地址类型*/
typedef unsigned char  OSSTK;                   /*栈指针是1字节递增或递减的*/
typedef OSSTK * OSSTKRef;
typedef unsigned char  OS_CPU_SR;                
/*
*********************************************************************************************************
*                                              常量
*********************************************************************************************************
*/

#ifndef  FALSE
#define  FALSE    0
#endif

#ifndef  TRUE
#define  TRUE     1
#endif


#define  DEFINECRITICAL(cpu_sr)    OS_CPU_SR  cpu_sr=0; OS_CPU_SR reg_a
/*保存机器状态寄存器，屏蔽中断，进临界区*/
#define  OS_ENTER_CRITICAL()   __asm staa reg_a; __asm  tpa; __asm staa cpu_sr; __asm sei; __asm ldaa reg_a;                         
/*恢复机器状态寄存器，恢复中断，推临界区*/
#define  OS_EXIT_CRITICAL()    __asm staa reg_a; __asm  ldaa cpu_sr;  __asm tap;__asm ldaa reg_a;

#define  OS_TASK_SW() __asm swi;

#define  OS_STK_GROWTH        1                  /* 栈增长方向，由高到低*/

#endif
