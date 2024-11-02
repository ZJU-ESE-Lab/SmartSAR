/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：pim.c
* 摘    要：包含PIM模块的驱动程序
*
* 当前版本：1.0
* 作    者：姜旭锋
* 完成日期：2005年2月20日
*********************************************************************************
*/
#ifndef  _HCS12_PIM_C_
#define  _HCS12_PIM_C_

#include "os_cpu.h"
#include "io_map.h"
#include "pim.h"

/*
**********************************************************************
* PIM模块初始化
**********************************************************************
*/

void vInterface_PIMInit(){

   /* 15路IO输出 */

   PTH=0xFF;

   PORTB=0xFF;   

   
   DDRH =0xFF; 
   DDRB =0xFF;   

  

   DDRA =0xFF;

   DDRJ |=0x03;

   
   /* 7路IO输入  */ 
   DDRK =0x7C;
   PORTK |=0b00111100;

   DDRM &=0xF0;
}

/*
**********************************************************************
*
* 开关量输入
*
**********************************************************************
*/


/*
*******************************************
* 读刹车信号 1                          PS4
*******************************************
*/
INT8U vInterface_GetBrakeSwitch1(){
  return (INT8U)PORTK_BIT0;
}

/*
*******************************************
* 读刹车信号 2                          PS5
*******************************************
*/
INT8U vInterface_GetBrakeSwitch2(){
   return (INT8U)PORTK_BIT1;
 }
 
 
/*
*******************************************
* 读停车信号                            PS6
*******************************************
*/
INT8U vInterface_GetParkingSwitch(){
  return (INT8U)PORTK_BIT7;
}

      

/*
*******************************************
* 读压力信号                           取消
*******************************************
*/
INT8U vInterface_GetPressureSwitch(){
  return (INT8U)0;
}

/*
*******************************************
* 读怠速踏板信号                        PM2
*******************************************
*/
INT8U vInterface_GetPedalIdleSwitch(){
  return (INT8U)PTIM_PTIM2;
  
}

/*
*******************************************
* 读钥匙门信号                          PM3
*******************************************
*/
INT8U vInterface_GetIgnitionSwitch(){
   return (INT8U)PTIM_PTIM3;
}

/*
*******************************************
* 手动模式开关                          PM4
*******************************************
*/
INT8U vInterface_GetManualModeSwitch(){
     return (INT8U)PTIM_PTIM0; 
}


/*
*******************************************
* 读诊断请求信号                        PM5
*******************************************
*/
INT8U vInterface_GetDiagFlashSwitch(){
     return (INT8U)PTIM_PTIM1; 
}





/*
**********************************************************************
*
* 开关量输出
*
**********************************************************************
*/

/*
*******************************************
* 设置档位 1输出状态 
*******************************************
*/
void vInterface_SetGear1State(INT8U State){
  PTH_PTH0=State;
}

/*
*******************************************
* 设置档位 2输出状态 
*******************************************
*/
void vInterface_SetGear2State(INT8U State){
     PTH_PTH1=State;
}

/*
*******************************************
* 设置档位 3输出状态 
*******************************************
*/
void vInterface_SetGear3State(INT8U State){
      PTH_PTH2=State;
}

/*
*******************************************
* 设置档位 4输出状态 
*******************************************
*/
void vInterface_SetGear4State(INT8U State){
     PTH_PTH3=State;
}

/*
*******************************************
* 设置档位 5输出状态 
*******************************************
*/
void vInterface_SetGear5State(INT8U State){
     PTH_PTH4=State;
}

/*
*******************************************
* 设置档位 6输出状态 
*******************************************
*/
void vInterface_SetGear6State(INT8U State){
 PTH_PTH5=State;
}

/*
*******************************************
* 设置档位 R输出状态  
*******************************************
*/
void vInterface_SetGearRState(INT8U State){
 PTH_PTH6=State;
}


/*
*******************************************
* 设置档位 RN输出状态  
*******************************************
*/
void vInterface_SetGearRNState(INT8U State){
 PTH_PTH7=State;
}







/*
*******************************************
* 设置诊断灯输出状态
*******************************************
*/
void vInterface_SetLightState(INT8U State){
  PORTB_BIT2=State;
}

/*
*******************************************
* 设置报警器输出状态 
*******************************************
*/
void vInterface_SetAlarmState(INT8U State){
  PORTB_BIT3=State;  
}


/*
*******************************************
* 设置启动禁止继电器输出状态 
*******************************************
*/
void vInterface_SetStartupForbidRelayState (INT8U State){
  PORTB_BIT4=State;
}


/*
*******************************************
* 设置泵继电器输出状态 
*******************************************
*/
void vInterface_SetPumpRelayState (INT8U State){
  PORTB_BIT5=State;
}




/*
*******************************************
* 设置备用继电器 1输出状态
*******************************************
*/
void vInterface_SetSpareRelay1State(INT8U State){
 PORTB_BIT6=State;
}
/*
*******************************************
* 设置备用继电器 2输出状态 
*******************************************
*/
void vInterface_SetSpareRelay2State(INT8U State){
  PORTB_BIT7=State;
}




/*
*******************************************
* 设置主继电器输出状态
*******************************************
*/
void vInterface_SetMainRelayState(INT8U State){
  PORTB_BIT0=State;  
}


void vInterface_SetPortB1(INT8U State){

  PORTB_BIT1=State;  

}


/*
*********************************************************************************
* D/A Output Signal API
*********************************************************************************
*/
void vInterface_SetAD(INT16U value){

   INT8U high,low,i;
   
   value=value & 0x3FF;
   
   high=(INT8U)(value>>8);
   low=(INT8U)value;
  
   PORTK|=0b00111100;        //CLR(K2)=1,LDAC(K3)=1,CS(K4)=1,WR(K5)=1
  
   PORTA=(INT8U)(low & 0x3F);                            //write data  
   PTJ_PTJ1=0;                                        //write address
   PTJ_PTJ0=0;
  
   PORTK&=0b11000111;        //CLR(K2)=1,LDAC(K3)=0,CS(K4)=0,WR(K5)=0  
   for(i=0;i<1;i++);  
   PORTK|=0b00111100;        //CLR(K2)=1,LDAC(K3)=1,CS(K4)=1,WR(K5)=1
  
  
    
   PORTA=(INT8U)((low & 0xC0)|(high & 0x03));            //write data  
   PTJ_PTJ1=1;                                        //write address
   PTJ_PTJ0=1;
  
   PORTK&=0b11000111;       //CLR(K2)=1,LDAC(K3)=0,CS(K4)=0,WR(K5)=0  
   for(i=0;i<1;i++);  
   PORTK|=0b00111100;        //CLR(K2)=1,LDAC(K3)=1,CS(K4)=1,WR(K5)=1  
  
}


#endif
