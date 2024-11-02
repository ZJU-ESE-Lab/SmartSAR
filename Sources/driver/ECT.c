/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�ect.c
* ժ    Ҫ������ECTģ�����������
*
* ��ǰ�汾��1.0
* ��    �ߣ������
* ������ڣ�2005��2��20��
*********************************************************************************
*/
#ifndef _HCS12_ECT_C_
#define _HCS12_ECT_C_

#include "os_cpu.h"
#include "io_map.h"
#include "ect.h"

#define CH_0  0x01

#define CH_1  0x02

#define CH_2  0x04

#define CH_3  0x08



#define CH_HZ_MIN        32768

#define CH_HZ_MAX        39





#define ECT_10MS_EN      0x80



#define ECT_COUNT        1953 



INT8U   ectflag;



INT16U  EngineSpeed; 

INT16U  ShaftSpeed;

INT16U  VehicleSpeed1;

INT16U  VehicleSpeed2;



/*

=========================================================================

*/

//  ˵����

//  �����Ƕ�ʱ���жϵĺ���

/*

=========================================================================

*/





/*

***********************************************************

* ����ECTģ��ͨ��6��ʱ���жϣ���ʱʱ��5ms

***********************************************************

*/

void OS5msTickStart(){

  INT32U count;



  count=(INT32U)TCNT;

  count+=ECT_COUNT;

  TC6=(INT16U)count;

  TIE_C6I=1;

}





/*

***********************************************************

* ECTģ��ͨ��6��ʱ���жϷ�����򣬶�ʱʱ��5ms

***********************************************************

*/
#if 0

__interrupt void ECT_C6_Interrupt(void){

  INT32U count;

  

  //==========================================

  PORTB_BIT1=1;   //�����

  //==========================================

  

  TFLG1_C6F=1;           // ���жϱ�־

  

  count=(INT32U)TCNT;    //���¼�����һ���жϵļ���ֵ

  count+=ECT_COUNT;

  TC6=(INT16U)count;
 

  CPU_INT_EN();          // ʹ���жϣ����������ж�Ƕ��

  
  if (ectflag & ECT_10MS_EN){

    ectflag&=~ECT_10MS_EN;


  }else{

    ectflag|=ECT_10MS_EN;

  }

  

  CPU_INT_DIS();

  

  //==========================================  

  PORTB_BIT1=0;   //�����

  //========================================== 

}


#endif
/*

=========================================================================

*/

//  ˵����

//  ��ʱ���жϵĺ�������

/*

=========================================================================

*/













/*
*******************************************************************
* ECTģ���ʼ��
*******************************************************************
*/
void vInterface_ECTInit(){



  ectflag=0x0F;

  
  EngineSpeed=0;  

  ShaftSpeed=0;  

  VehicleSpeed1=0;

  VehicleSpeed2=0;



  
  TIOS=0xF0;           /* set pin 0,1,2,3 as input ,others as output*/      

  TCTL4_EDG0A =1;      /* Pin 0 Capture on rising edges only*/
  TCTL4_EDG1A =1;      /* Pin 1 Capture on rising edges only*/
  TCTL4_EDG2A =1;      /* Pin 2 Capture on rising edges only*/
  TCTL4_EDG3A =1;      /* Pin 3 Capture on rising edges only*/
  
  ICSYS_BUFEN=1;    

  
  TSCR2_PR=5;          /* Prescale factor set to 32,ref to Bus clock*/ 

  

  TSCR2_TOI=1;

  ICPAR=0x0F;

  ICSYS_PACMX=1;

  TSCR1_TEN=1;         /* Enable the ECT module*/
}



/*
***********************************************************
* ECTģ�����������ж�
***********************************************************
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED       /* Interrupt section for this module. Placement will be in NON_BANKED area. */

__interrupt void ECT_Overflow_ISR(void){

  INT8U count;

  

  TFLG2_TOF=1;

  

  count=PACN0;             /*  ���ͨ��0   */

  PACN0=0;

  if (count<=1){

    ectflag|=CH_0;

  }else{

    ectflag&=~CH_0;

  }

  

  count=PACN1;             /*  ���ͨ��1   */

  PACN1=0;

  if (count<=1){

    ectflag|=CH_1;

  }else{

    ectflag&=~CH_1;

  }  

  

  count=PACN2;             /*  ���ͨ��2   */

  PACN2=0;

  if (count<=1){

    ectflag|=CH_2;

  }else{

    ectflag&=~CH_2;

  }

  

  count=PACN3;            /*  ���ͨ��3   */

  PACN3=0;

  if (count<=1){

    ectflag|=CH_3;

  }else{

    ectflag&=~CH_3;

  }  

  

}

#pragma off


/*

==============================================================

*/

//  ˵����

//  �����Ǳ�׼��API

/*

==============================================================

*/


/*
*********************************************************
*
* �����������������������ź�
* ���룺      ��
* ����ֵ��    �޷���16λ
* ������ʽ��  vInterface_GetEngineSpeed()
* ����������  �������ط����������������źţ���ߴﵽ10KHz��
* Ƶ��ֵ������ֵ������Χ��10KHz��100 us����10Hz��100ms����
* ��Ӧʱ��Ϊ2.56 us/bit�����ز�����������������Ϊ10ms��
* ���������ת�ٴ����������ź����ڸ���100 us����������ֵΪ10KHz��
* ��ͼ�⾫��Ϊ0.1�������������ת�ٴ����������ź����ڵ���100ms��
* ������ת�ٴ�����������ֵΪ10Hz��
*
*********************************************************
*/
INT16U vInterface_GetEngineSpeed(void){

 INT32U start,end;

 INT8U  count;

 INT16U returnvalue;

 

 if (ectflag & CH_0){

  returnvalue=CH_HZ_MIN;

 }else if (TFLG1_C0F){     

 

  TFLG1_C0F=1;

  

  count=PACN0;

  

  end=(INT32U)TC0;  

  start=(INT32U)TC0H;


  PACN0=count;

  

  if (count!=0xFF){ 

  

    if (end==start || end==0 || start==0){

      returnvalue=EngineSpeed; 

    }else{

      if (end>start)
       returnvalue=(INT16U)(end-start);
      else
       returnvalue=(INT16U)(end+0x00010000-start); 

       

       

      if (returnvalue>CH_HZ_MIN)

       returnvalue=CH_HZ_MIN;

    

      if (returnvalue<CH_HZ_MAX)

       returnvalue=CH_HZ_MAX;      

    }

    

  }else{                       //PACN0==0xFF

    PACN0=2;

    returnvalue=EngineSpeed; 

  }

 }else{

    returnvalue=EngineSpeed; 

 }

 

 EngineSpeed=returnvalue;
 return returnvalue;




}


/*
*********************************************************
* �������ᴫ�������������ź�
* ���룺      ��
* ����ֵ��    �޷���16λ
* ������ʽ��  vInterface_GetShaftSpeed()
* ����������  �������������ᴫ���������źţ���ߴﵽ10KHz��
* Ƶ��ֵ������ֵ������Χ��10KHz��100 us����10Hz��100ms����
* ��Ӧʱ��Ϊ2.56 us/bit�����ز�����������������Ϊ10ms��
* ���������ת�ٴ����������ź����ڸ���100 us����������ֵΪ10KHz��
* ��ͼ�⾫��Ϊ0.1�������������ת�ٴ����������ź����ڵ���100ms
* ��������ת�ٴ�����������ֵΪ10Hz��
*
*********************************************************
*/ 
INT16U vInterface_GetShaftSpeed(void){
 INT32U start,end;

 INT8U  count;

 INT16U returnvalue;

 

 if (ectflag & CH_1){

  returnvalue=CH_HZ_MIN;

 }else if (TFLG1_C1F){     

 

  TFLG1_C1F=1;

  

  count=PACN1;

  

  end=(INT32U)TC1;  

  start=(INT32U)TC1H;
 

  

  PACN1=count;

  

  if (count!=0xFF){  

  

    if (end==start || end==0 || start==0){

      returnvalue=ShaftSpeed; 

    }else{

      if (end>start)
       returnvalue=(INT16U)(end-start);
      else
       returnvalue=(INT16U)(end+0x00010000-start); 

       

       

      if (returnvalue>CH_HZ_MIN)

       returnvalue=CH_HZ_MIN;

    

      if (returnvalue<CH_HZ_MAX)

       returnvalue=CH_HZ_MAX;      

    }  

    

  }else{                       //PACN1==0xFF

    PACN1=2;

    returnvalue=ShaftSpeed; 

  }

 }else{

    returnvalue=ShaftSpeed; 

 }

 

 ShaftSpeed=returnvalue;
 return returnvalue;
}
 

/*
*********************************************************
*
* �����ٴ�����1���������ź�
* ���룺      ��
* ����ֵ��    �޷���16λ
* ������ʽ��  vInterface_GetVehicleSpeed1()
* ����������  �������س��ٴ�����1�����źţ���ߴﵽ10KHz��
* Ƶ��ֵ������ֵ������Χ��10KHz��100 us����10Hz��100ms����
* ��Ӧʱ��Ϊ2.56 us/bit�����ز�����������������Ϊ10ms��
* ������ٴ�����1���ڸ���100 us����������ֵΪ10KHz�����
* ��⾫��Ϊ0.1����������ٴ�����1����100ms��������ת��
* ������������ֵΪ10Hz��
*
*********************************************************
*/ 
INT16U vInterface_GetVehicleSpeed1(void){
 INT32U start,end;

 INT8U  count;

 INT16U returnvalue;

 
 if (ectflag & CH_2){

  returnvalue=CH_HZ_MIN;

 }else if (TFLG1_C2F){     

 

  TFLG1_C2F=1;

  

  count=PACN2;

  

  end=(INT32U)TC2;  

  start=(INT32U)TC2H;
 

  

  PACN2=count;

  

  if (count!=0xFF){  

  

    if (end==start || end==0 || start==0){

      returnvalue=VehicleSpeed1; 

    }else{

      if (end>start)
       returnvalue=(INT16U)(end-start);
      else
       returnvalue=(INT16U)(end+0x00010000-start); 

       

       

      if (returnvalue>CH_HZ_MIN)

       returnvalue=CH_HZ_MIN;

    

      if (returnvalue<CH_HZ_MAX)

       returnvalue=CH_HZ_MAX;      

    }  

    

  }else{                       //PACN2==0xFF

    PACN2=2;

    returnvalue=VehicleSpeed1; 

  }

 }else{

    returnvalue=VehicleSpeed1; 

 }

 VehicleSpeed1=returnvalue;
 return returnvalue;
}


/*
*********************************************************
*
* �����ٴ�����2���������ź�
* ���룺      ��
* ����ֵ��    �޷���16λ
* ������ʽ��  vInterface_GetVehicleSpeed2()
* ����������  �������س��ٴ�����2�����źţ���ߴﵽ10KHz��
* Ƶ��ֵ������ֵ������Χ��10KHz��100 us����10Hz��100ms����
* ��Ӧʱ��Ϊ2.56 us/bit�����ز�����������������Ϊ10ms��
* ������ٴ�����2���ڸ���100 us����������ֵΪ10KHz����ͼ��
* ����Ϊ0.1����������ٴ�����2����100ms��������ת�ٴ���
* ��������ֵΪ10Hz��
*
*********************************************************
*/ 
INT16U vInterface_GetVehicleSpeed2(void){
 INT32U start,end;

 INT8U  count;

 INT16U returnvalue;

 
 if (ectflag & CH_3){

  returnvalue=CH_HZ_MIN;

 }else if (TFLG1_C3F){     

 

  TFLG1_C3F=1;

  

  count=PACN3;

  

  end=(INT32U)TC3;  

  start=(INT32U)TC3H;
 

  

  PACN3=count;

  

  if (count!=0xFF){  

  

    if (end==start || end==0 || start==0){

      returnvalue=VehicleSpeed1; 

    }else{

      if (end>start)
       returnvalue=(INT16U)(end-start);
      else
       returnvalue=(INT16U)(end+0x00010000-start); 

       

       

      if (returnvalue>CH_HZ_MIN)

       returnvalue=CH_HZ_MIN;

    

      if (returnvalue<CH_HZ_MAX)

       returnvalue=CH_HZ_MAX;      

    }      

    

  }else{                       //PACN3==0xFF

    PACN3=2;

    returnvalue=VehicleSpeed2; 

  }

 }else{

    returnvalue=VehicleSpeed2; 

 }

 

 VehicleSpeed2=returnvalue;

 return returnvalue;
}


#endif



