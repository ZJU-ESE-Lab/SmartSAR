/*
***************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_cpu_c.c
* 摘    要：HCS12底层代码实现，上下文切换、栈初始化等
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
#include "includes.h"

/*
***************************************************************************
* OSSTK *OSTaskStkInit (void (*task)(void), OSSTK *ptos)
* 描述：初始化栈,模拟SWI指令后的栈的情况，具体可以参加SWI指令的说明
*         
*         OSTCBStkCur +  0  -->  CCR
*                     +  1       B
*                     +  2       A
*                     +  3       X (H)
*                     +  4       X (L)
*                     +  5       Y (H)
*                     +  6       Y (L)
*                     +  7       PC(H)
*                     +  8       PC(L)       
* 允许被调用的位置：在宏SET_TASK_READY中调用，也就是当任务要从挂起进入就绪状态时所要调用的
* 参数：task：任务函数体指针
*       ptos：栈顶指针
* 返回：入栈操作后的栈指针
* 注意事项：非OSEK标准函数，栈初始化函数，任务开始运行的关键
* 创建人：厉蒋
* 时间：2005.08.01
***************************************************************************
*/

OSSTK *OSTaskStkInit(void (*task)(void), OSSTK *ptos)
{
    INT16U *wstk;
    INT8U  *bstk;
    INT16U *OffsetAddress;
    INT8U regPage;
    wstk    = (INT16U *)ptos;          /* 获得栈指针*/
    OffsetAddress = (INT16U*)&task;
   	regPage = (INT8U)task;
    *--wstk = (INT16U)*OffsetAddress;
    *--wstk = (INT16U)*OffsetAddress;  /*将任务体函数指针压入栈顶*/

    *--wstk = (INT16U)0x2222;          /* Y 寄存器*/
    *--wstk = (INT16U)0x1111;          /* X 寄存器*/
    *--wstk = (INT16U)0xBBAA;          /* D 寄存器*/
    bstk    = (INT8U *)wstk;           /* 将Word转换为Byte指针*/
    *--bstk = 0x80;                    /* CCR 寄存器，禁止stop语句*/
    *--bstk = regPage;   
    return ((OSSTK *)bstk);            /* 返回新的栈顶指针 */
}

/*
***************************************************************************
* void OSTaskHighRun()
* 描述：运行最高优先级的任务，将SP指向最高优先级任务的当前栈指针，然后调用RTI       
* 允许被调用的位置：StartOS、OSTaskRun宏（在TerminateTask、ChainTask中会调用）
* 参数：无
* 返回：无
* 注意事项：
* 创建人：厉蒋
* 时间：2005.08.01
***************************************************************************
*/
#pragma CODE_SEG NON_BANKED
#pragma TRAP_PROC /*用于声明该函数为中断退出函数，返回时用RTI指令*/
void OSTaskHighRun(void) 
{
__asm{
    ldx    OSTCBCur               ;
    lds    0,x    
    pula
    staa   PPAGE                  ;
	 }
}

/*
***************************************************************************
* void OSTaskSwitch(void)
* 描述：进行任务切换，保存原有任务的运行环境，将SP指向新的最高优先级任务的当前栈指针，然后调用RTI
* 允许被调用的位置：该函数时SWI引起的软中断的服务程序
* 参数：无
* 返回：无
* 注意事项： OSTCBOld：要被切换走的任务，OSTCBCur要运行的任务
* 创建人：厉蒋
* 时间：2005.08.01
***************************************************************************
*/

#pragma TRAP_PROC  /*用于声明该函数为中断退出函数，返回时用RTI指令*/
void OSTaskSwitch(void)
{
__asm{
    ldaa   PPAGE
    psha   
    ldx    OSTCBOld                   ;  OSTCBCur->OSTCBStkCur = 栈指针                   
    sts    0,x                        ;         
    ldx    OSTCBCur                   ;                           
    lds    0,x                        ;  读取新的栈指针到SP中 
    pula
    staa   PPAGE                                 
	}
}


/*
**********************************************************************
* 中断级任务退出的上下文切换
* 
* 作者 : 姜旭锋
**********************************************************************
*/
#pragma TRAP_PROC
void OSIntCtxSw(void)
{                   
__asm{                
    ldx    OSTCBCur                                        
    lds    0,x    
    
    pula
    staa   PPAGE                                              
	}
}



/*
***********************************************************************
* 操作系统系统定时器中断服务程序
*
* 说明 :  该中断服务程序自动嵌套
*
* 作者 :  姜旭锋
***********************************************************************
*/
def_interrupt(OSTickISR)
{  
  IntEnter(); 
  
  CRGFLG_RTIF=1;
 
  OSCounterTrigger(OSCounters);

  IntExit();    
                    
}