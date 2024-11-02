/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：pwm.c
* 摘    要：包含PWM模块的驱动程序
*
* 当前版本：1.0
* 作    者：姜旭锋
* 完成日期：2005年2月20日
*********************************************************************************
*/
#ifndef  _HCS12_PWM_C_
#define  _HCS12_PWM_C_

#include "os_cpu.h"
#include "io_map.h"
#include "pwm.h"


#define  PWM_START_LEVEL    1

#define  PWM_PRESCALE       0x55        

// Bus Clock is 1/2 of System Clock


/*
*********************************************************************
*
* 设置离合器结合1输出状态
* 输入：16位无符号数duty_cycle   
* 返回值：无
* 命令格式：vInterface_ SetClutchComb1State (UInt16)
* 功能描述：本功能根据duty_cycle值设置脉宽调制输出离合器1的状态。
* 输入值duty_cycle范围在0~167.7696ms，分辨率为2.00us/bit。
* 脉宽调制频率为定值。
*
*********************************************************************
*/
void vInterface_InitiateClutchComb1(INT16U period){
  PWMCTL_CON01=1;
  PWMCNT01 = 0;
  PWMPER01 = period;                    /* and to the period register */
  PWMPRCLK = PWM_PRESCALE;              /* Set prescaler register */
  PWMSCLB  = 0;                         /* Set scale register */

  PWMPOL_PPOL1=PWM_START_LEVEL;         /* Set the default polarity*/
  PWMCLK_PCLK1 = 0;                     /* Select clock source */
}


/*
*********************************************************************
*
* 离合器结合1输出状态初始化
* 输入：16位无符号数period  
* 返回值：无
* 命令格式：vInterface_ InitiateClutchComb1 (UInt16)
* 功能描述：本功能根据period值初始化脉宽调制输出离合器1。
* 输入值period范围在0~167.7696ms，分辨率为2.00us/bit。
* 在每次点火周期中，此功能一旦被启动，脉宽调制频率就不再改变。
*
*********************************************************************
*/
void vInterface_SetClutchComb1State(INT16U duty){
  PWMDTY01 = duty;                      /* Store initial value to the duty-compare register */
  PWME|=0x02;
}


/*
*********************************************************************
*
* 离合器结合2输出状态初始化
* 输入：16位无符号数period  
* 返回值：无
* 命令格式：vInterface_InitiateClutchComb2 (UInt16)
* 功能描述：本功能根据period值初始化脉宽调制输出离合器2。
* 输入值period范围在0~167.7696ms，分辨率为2.00us/bit。
* 在每次点火周期中，此功能一旦被启动，脉宽调制频率就不再改变。
*
*********************************************************************
*/
void vInterface_InitiateClutchComb2(INT16U period){
  PWMCTL_CON23=1;
  PWMCNT23 = 0;
  PWMPER23 = period;                   /* and to the period register */
  PWMPRCLK = PWM_PRESCALE;             /* Set prescaler register */
  PWMSCLB = 0;                         /* Set scale register */

  PWMPOL_PPOL3=PWM_START_LEVEL;        /* Set the default polarity*/
  PWMCLK_PCLK3 = 0;                    /* Select clock source */
}
  



/*
*********************************************************************
*
* 设置离合器结合2输出状态
* 输入：16位无符号数duty_cycle   
* 返回值：无
* 命令格式：vInterface_SetClutchComb2State (UInt16)
* 功能描述：本功能根据duty_cycle值设置脉宽调制输出离合器2的状态。

*           输入值duty_cycle范围在0~167.7696ms，分辨率为2.00us/bit。

*           脉宽调制频率为定值。
*
*********************************************************************
*/
void vInterface_SetClutchComb2State(INT16U duty){
  PWMDTY23 = duty;                      /* Store initial value to the duty-compare register */
  PWME|=0x08;
}



/*
*********************************************************************
*
* 离合器分离输出状态初始化
* 输入：16位无符号数period   
* 返回值：无
* 命令格式：vInterface_InitiateClutchRel(UInt16)
* 功能描述：本功能根据period值初始化释放离合器脉宽调制的输出。
* 输入值period范围在0~167.7696ms，分辨率为2.00us/bit。
* 在每次点火周期中，此功能一旦被启动，脉宽调制频率就不再改变。
*
*********************************************************************
*/
void vInterface_InitiateClutchRel(INT16U period){
  PWMCTL_CON45=1;
  PWMCNT45 = 0;
  PWMPER45 = period;                   /* and to the period register */
  PWMPRCLK = PWM_PRESCALE;             /* Set prescaler register */
  PWMSCLB = 0;                         /* Set scale register */

  PWMPOL_PPOL5=PWM_START_LEVEL;        /* Set the default polarity*/
  PWMCLK_PCLK5 = 0;                    /* Select clock source */
}  




/*
*********************************************************************
*
* 设置离合器分离输出状态
* 输入：16位无符号数duty_cycle   
* 返回值：无
* 命令格式：vInterface_ SetClutchRelState (UInt16)
* 功能描述：本功能根据duty_cycle值设置脉宽调制输出离合器释放状态。
* 输入值duty_cycle范围在0~167.7696ms，分辨率为2.00us/bit。
* 脉宽调制频率为定值。
*
*********************************************************************
*/
void vInterface_SetClutchRelState(INT16U duty){
  PWMDTY45 = duty;                      /* Store initial value to the duty-compare register */
  PWME|=0x20;
}



/*
*********************************************************************
*
* 冗余脉宽调制输出状态初始化
* 输入：16位无符号数period   
* 返回值：无
* 命令格式：vInterface_ InitiateSparePWM(UInt16)
* 功能描述：本功能根据period值初始化冗余脉宽调制的输出。
* 输入值period范围在0~167.7696ms，分辨率为2.00us/bit。
* 在每次点火周期中，此功能一旦被启动，脉宽调制频率就不再改变。
*
*********************************************************************
*/
void vInterface_InitiateSparePWM(INT16U period){
  PWMCTL_CON67=1;
  PWMCNT67 = 0;
  PWMPER67 = period;                   /* and to the period register */
  PWMPRCLK = PWM_PRESCALE;             /* Set prescaler register */
  PWMSCLB = 0;                         /* Set scale register */

  PWMPOL_PPOL7=PWM_START_LEVEL;        /* Set the default polarity*/
  PWMCLK_PCLK7 = 0;                    /* Select clock source */
}
  



/*
*********************************************************************
*
* 设置备用脉宽调制输出状态
* 输入：16位无符号数duty_cycle   
* 返回值：无
* 命令格式：vInterface_ SetSparePWMState (UInt16)
* 功能描述：本功能根据duty_cycle值设置脉宽调制输出状态。
* 输入值duty_cycle范围在0~167.7696ms，分辨率为2.00us/bit。
* 脉宽调制频率为定值。
*
*********************************************************************
*/
void vInterface_SetSparePWMState(INT16U duty){
  PWMDTY67 = duty;                      /* Store initial value to the duty-compare register */
  PWME|=0x80;
}


#endif