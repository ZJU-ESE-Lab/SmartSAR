/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：ad.c
* 摘    要：包含ATD模块的驱动程序
*
* 当前版本：1.0
* 作    者：姜旭锋
* 完成日期：2005年2月20日
*********************************************************************************
*/
#ifndef _HCS12_ATD_C_
#define _HCS12_ATD_C_

#include "os_cpu.h"
#include "io_map.h"
#include "ad.h"
/*
***************************************************************

* 函数名 ： AD_Init 
* 描  述 ： AD模块初始化

* 采样时间计算：

* 12.5M bus clock

* 1/(12.5M/8)*10=6.4us
***************************************************************
*/
void vInterface_ADInit(void)
{ 
  INT8U i;

  /* ATD0 模块初始化   */
 
  ATD0DIEN =0;
  ATD0CTL2_AFFC=1;   																	  
  ATD0CTL2_ASCIE=0;                     /* 中断禁止                          */  
  ATD0CTL2_ETRIGE=0;
 
  ATD0CTL3_FIFO=0; 
  ATD0CTL3_S1C=1;
  ATD0CTL3_S2C=0;
  ATD0CTL3_S4C=0;
  ATD0CTL3_S8C=0;  
  
 
  ATD0CTL4_SRES8=0;                     /* 10位分辨率                         */
  ATD0CTL4_SMP=2;	                /* 8个AD采样时钟， 合计10个采样周期   */  
  ATD0CTL4_PRS=3;                       /* 预分频                             */
    
  ATD0CTL2_ADPU = 1;  
  for (i=0;i<100;i++);                  /* 恢复延迟                           */ 


  /* ATD1 模块初始化   */
   
  ATD1DIEN =0;
  ATD1CTL2_AFFC=1;   																	  
  ATD1CTL2_ASCIE=0;                     /* 中断禁止                           */  
  ATD1CTL2_ETRIGE=0;
 
  ATD1CTL3_FIFO=0; 
  ATD1CTL3_S1C=1;
  ATD1CTL3_S2C=0;
  ATD1CTL3_S4C=0;
  ATD1CTL3_S8C=0;  
  
 
  ATD1CTL4_SRES8=0;                     /* 10位分辨率                         */
  ATD1CTL4_SMP=2;                       /* 8个AD采样时钟， 合计10个采样周期   */  
  ATD1CTL4_PRS=3;                       /* 预分频                             */
    
  ATD1CTL2_ADPU = 1;  
  for (i=0;i<100;i++);                  /* 恢复延迟                           */ 
  
}





/*
***************************************************************

* 函数名 ： vInterface_GetAccelCtrlSensor
* 描  述 ： 读加速器控制踏板位置传感器 
***************************************************************
*/
INT16U vInterface_GetAccelCtrlSensor(){

  ATD0CTL5=0x80;
  while(!ATD0STAT1_CCF0);
  return ATD0DR0;
}




/*
***************************************************************

* 函数名 ： vInterface_GetThrottlePositionSensor
* 描  述 ： 读油门位置传感器 
***************************************************************
*/
INT16U vInterface_GetThrottlePositionSensor(){


  ATD0CTL5=0x81;  
  while(!ATD0STAT1_CCF0);
  return ATD0DR0;
}




/*
***************************************************************

* 函数名 ： vInterface_GetClutchPosSensor
* 描  述 ： 读离合器位置传感器
***************************************************************
*/
INT16U vInterface_GetClutchPosSensor(){

  ATD0CTL5=0x82;
  while(!ATD0STAT1_CCF0);
  return ATD0DR0;
}
/*
***************************************************************

* 函数名 ： vInterface_GetOilTempSenso
* 描  述 ： 读液压油温度传感器 
***************************************************************
*/
INT16U vInterface_GetOilTempSenso(){

  ATD0CTL5=0x83;  
  while(!ATD0STAT1_CCF0);
  return ATD0DR0;
}




/*
***************************************************************

* 函数名 ： vInterface_GetGear12Sensor
* 描  述 ： 读档位 12传感器 
***************************************************************
*/
INT16U vInterface_GetGear12Sensor(){

  ATD0CTL5=0x84;    
  while(!ATD0STAT1_CCF0);
  return ATD0DR0;
}




/*
***************************************************************

* 函数名 ： vInterface_GetGear34Sensor
* 描  述 ： 读档位 34传感器 
***************************************************************
*/
INT16U vInterface_GetGear34Sensor (){

  ATD0CTL5=0x85; 
  while(!ATD0STAT1_CCF0);
  return ATD0DR0;
}

/*
***************************************************************

* 函数名 ： vInterface_GetGear56Sensor
* 描  述 ： 读档位 56传感器
***************************************************************
*/
INT16U vInterface_GetGear56Sensor (){

  ATD0CTL5=0x86;
  while(!ATD0STAT1_CCF0);
  return ATD0DR0;
}

/*
***************************************************************

* 函数名 ： vInterface_GetRNSensor
* 描  述 ： 读档位 RN传感器
***************************************************************
*/
INT16U vInterface_GetGearRRNSensor(){

  ATD0CTL5=0x87;
  while(!ATD0STAT1_CCF0);

  return ATD0DR0;
}




/*
***************************************************************

* 函数名 ： vInterface_GetTMAPSensor
* 描  述 ： 读 TMAP（油压）传感器  
***************************************************************
*/
INT16U vInterface_GetTMAPSensor(){

  ATD1CTL5=0x80;
  while(!ATD1STAT1_CCF0);
  return ATD1DR0;
}

/*
***************************************************************
* 未使用 
***************************************************************
*/
INT16U vInterface_GetGearHandle(){

  ATD1CTL5=0x81;
  while(!ATD1STAT1_CCF0);
  return ATD1DR0;
}



/*
***************************************************************

* 函数名 ： vInterface_GetBatteryVoltage
* 描  述 ： Read Battery Voltage
***************************************************************
*/
INT16U vInterface_GetBatteryVoltage(){

  ATD1CTL5=0x82;
  while(!ATD1STAT1_CCF0);
  return ATD1DR0;
}



/*
***************************************************************
* Read Substrate Temprature Sensor
***************************************************************
*/
INT16U vInterface_GetSubTemp(){

  ATD1CTL5=0x83;
  while(!ATD1STAT1_CCF0);
  return ATD1DR0;
}

/*
***************************************************************
* 离合器结合比例电磁阀1*反馈 
***************************************************************
*/
INT16U vInterface_C12(){

  ATD1CTL5=0x84;
  while(!ATD1STAT1_CCF0);
  return ATD1DR0;
}

/*
***************************************************************
* 离合器结合比例电磁阀2*反馈 
***************************************************************
*/
INT16U vInterface_C13(){

  ATD1CTL5=0x85;
  while(!ATD1STAT1_CCF0);
  return ATD1DR0;
}

/*
***************************************************************
* 离合器结合比例电磁阀*反馈 
***************************************************************
*/
INT16U vInterface_C14(){

  ATD1CTL5=0x86;
  while(!ATD1STAT1_CCF0);
  return ATD1DR0;
}

/*
***************************************************************
* 备用比例电磁阀1反馈 
***************************************************************
*/
INT16U vInterface_C15(){

  ATD1CTL5=0x87;
  while(!ATD1STAT1_CCF0);
  return ATD1DR0;
}


#endif