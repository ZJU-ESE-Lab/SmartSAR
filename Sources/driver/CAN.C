/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：can.c
* 摘    要：包含CAN模块的驱动程序
*
* 当前版本：1.0
* 作    者：姜旭锋
* 完成日期：2005年2月20日
*********************************************************************************
*/
#ifndef _HCS12_CAN_C_ 
#define _HCS12_CAN_C_

#include "os_cpu.h"
#include "io_map.h"
#include "can.h"

vCAN_FrameStruct vCAN_A_Frame[MSCANFRAME];
vCAN_FrameStruct vCAN_B_Frame[MSCANFRAME];


/*
************************************************************************
* 函数名   : MSCAN2_INIT
*
* 描  述   : CAN2通道的初始化函数。函数内部使用
*
* 参  数  : vCAN_INITStruct* init    CAN初始化结构体指针 
*
* 返回值   : CAN_REQ_SUCCESS         CAN2通道初始化成功
*            CAN_REQ_FAIL            CAN2通道初始化失败
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN2_INIT(vCAN_InitStruct* init)
{
CAN2CTL1=0x80;                                       // 使能MSCAN2 模块
CAN2CTL0|=0x01;                                      // 请求进入初始化模式
while(!(CAN2CTL1&0x01));                             // 等待进入初始化模式

                 
CAN2IDAC=(INT8U)((init->GlobalIDMask_Type&0x03)<<4); //设置Filter模式

// 设置Filter。

// 对于8位模式，init->GlboalID_AccpFilter[0]的高位对应IDAR0，低位对应IDAR1，以此类推

// 对于16位模式, init->GlboalID_AccpFilter[0]的位序正常.

// 对于32位模式, init->GlboalID_AccpFilter[0]为32位的高位，init->GlboalID_AccpFilter[1]为低位
CAN2IDAR1=(INT8U)(init->GlboalID_AccpFilter[0]&0x00FF);          
CAN2IDAR0=(INT8U)((init->GlboalID_AccpFilter[0]>>8)&0x00FF);

CAN2IDAR3=(INT8U)(init->GlboalID_AccpFilter[1]&0x00FF);
CAN2IDAR2=(INT8U)((init->GlboalID_AccpFilter[1]>>8)&0x00FF);

CAN2IDAR5=(INT8U)(init->GlboalID_AccpFilter[2]&0x00FF);
CAN2IDAR4=(INT8U)((init->GlboalID_AccpFilter[2]>>8)&0x00FF);

CAN2IDAR7=(INT8U)(init->GlboalID_AccpFilter[3]&0x00FF);
CAN2IDAR6=(INT8U)((init->GlboalID_AccpFilter[3]>>8)&0x00FF);

// 设置Mask.

// 对于8位模式，init->GlobalID_Mask[0]的高位对应IDMR0，低位对应IDMR1，以此类推

// 对于16位模式, init->GlobalID_Mask[0]的位序正常.

// 对于32位模式, init->GlobalID_Mask[0]为32位的高位，init->GlobalID_Mask[1]为低位
CAN2IDMR1=(INT8U)(init->GlobalID_Mask[0]&0x00FF);               
CAN2IDMR0=(INT8U)((init->GlobalID_Mask[0]>>8)&0x00FF);

CAN2IDMR3=(INT8U)(init->GlobalID_Mask[1]&0x00FF);
CAN2IDMR2=(INT8U)((init->GlobalID_Mask[1]>>8)&0x00FF);

CAN2IDMR5=(INT8U)(init->GlobalID_Mask[2]&0x00FF);
CAN2IDMR4=(INT8U)((init->GlobalID_Mask[2]>>8)&0x00FF);

CAN2IDMR7=(INT8U)(init->GlobalID_Mask[3]&0x00FF);
CAN2IDMR6=(INT8U)((init->GlobalID_Mask[3]>>8)&0x00FF);


CAN2CTL1_CLKSRC=1;                                  //  选择内部时钟源 Bus Clock

CAN2BTR0_BRP=init->CAN_SPEED;                       //  设置波特率
CAN2BTR0_SJW=0;
CAN2BTR1_SAMP=0;                                    
CAN2BTR1_TSEG_20=7;
CAN2BTR1_TSEG_10=15;



CAN2CTL0&=0xfe;                                     //   请求退出初始化模式
while(CAN2CTL1&0x01);                               //   等待退出初始化模式


while(!CAN2CTL0_SYNCH);                             //   等待总线同步

CAN2CTL0=0x08;
                                                    //   使能定时器

						    
if (init->Poll_Enable==ENABLE_POLL)
  CAN2RIER=0x00;                                    //   禁止接收中断请求
else 
  CAN2RIER=0x01;                                    //   使能接收中断请求


return CAN_REQ_SUCCESS;
}



/*
************************************************************************
* 函数名   : MSCAN2_SEND
*
* 描  述   : 由CAN2通道发送数据。函数内部使用
*
* 参  数   : vCAN_FrameStruct* msg    发送的数据包结构体指针 
*
* 返回值   : CAN_TX_NO_BUFFER         CAN2通道无可用的发送缓冲区
*            CAN_REQ_SUCCESS          CAN2通道发送数据成功
*            CAN_TX_FAIL              CAN2通道发送数据失败
*
* 编  写   : 姜旭锋
************************************************************************
*/
INT16U MSCAN2_SEND(vCAN_FrameStruct* msg){

INT32U MessageID;


/*   检查缓冲区      */
if (!(CAN2TFLG & 7))                 
    return CAN_TX_NO_BUFFER;      


/*   获得发送缓冲区  */
CAN2TBSEL=CAN2TFLG;                  

MessageID=((INT32U)(msg->ID_High))<<16 | (INT32U)msg->ID_Low;


/*   封装CAN包包头   */  
if ( msg->ID_Type  <= CAN_EXTENDED_REMOTE_ID)     /* Is it the extend frame? */
  *(INT32U *)&CAN2TXIDR0 = ( ((MessageID << 1) & 0x0007FFFF) | ((MessageID << 3) & 0xFFE00000) ) | 0x00080000; /* Set the message as "extended" */
else
  *(INT32U *)&CAN2TXIDR0 = (MessageID << 21);     /* Sets the message as "standard" */


 if ( msg->ID_Type  & 0x01) {                     /* Is it a remote frame? */
    if (msg->ID_Type  <= CAN_EXTENDED_REMOTE_ID)  /* Is it the extend frame? */
      CAN2TXIDR3 |= 1;                            /* If yes then set message type as "remote frame" */
    else
      CAN2TXIDR1 |= 16;   
  }
  else {                                          /* data frame */
                                                  /* If no then set message type as "remote frame" */
     CAN2TXDSR0 =msg->Data[0];
     CAN2TXDSR1 =msg->Data[1];
     CAN2TXDSR2 =msg->Data[2];
     CAN2TXDSR3 =msg->Data[3];
     CAN2TXDSR4 =msg->Data[4];
     CAN2TXDSR5 =msg->Data[5];
     CAN2TXDSR6 =msg->Data[6];
     CAN2TXDSR7 =msg->Data[7];

     CAN2TXDLR=(INT8U)(msg->Data_Len & 0x0F);
   
    if (msg->ID_Type  <= CAN_EXTENDED_REMOTE_ID)   /* Is it the extend frame? */
      CAN2TXIDR3 &= 254;               /* If yes then set message type as "extend data frame" */   
    else
      CAN2TXIDR1 &= 239;               /* If no then set message type as "standard data frame" */
 
  }
  
CAN2TXTBPR=msg->Tx_Priority;

CAN2TFLG=CAN2TBSEL;

return CAN_REQ_SUCCESS;

}



/*
************************************************************************
* 函数名   : MSCAN2_STATUS
*
* 描  述   : 获取CAN2通道的当前状态。函数内部使用
*
* 返回值   : CAN_IN_INIT_STATE      CAN2处于初始化状态
*            CAN_IN_SLEEP_STATE     CAN2处于睡眠状态
*       CAN_TXERR_STATE    CAN2处于数据发送状态
*       CAN_RXERR_STATE    CAN2处于数据接收状态
*       CAN_BUSOFF_STATE    CAN2处于总线忙状态
*       CAN_OVERRUN_STATE   CAN2处于溢出状态
*       CAN_TX_EMPTY_STATE   CAN2处于发送缓冲区空状态
*       CAN_NORMAL_STATE    CAN2处于正常状态
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN2_STATUS(){
  if (CAN2CTL0&0x01 && CAN2CTL1&0x01)
     return CAN_IN_INIT_STATE;
  
  if (CAN2CTL0&0x02 && CAN2CTL1&0x02) 
    return CAN_IN_SLEEP_STATE ;
   
  if (CAN2RFLG_TSTAT==2)
     return CAN_TXERR_STATE;
  
  if (CAN2RFLG_RSTAT==2)
     return CAN_RXERR_STATE;
  
  if (CAN2RFLG_RSTAT==3 || CAN2RFLG_TSTAT==3)
     return CAN_BUSOFF_STATE;
  
  if (CAN2RFLG_OVRIF)
     return CAN_OVERRUN_STATE;
  
  if (!CAN2TFLG_TXE)
     return  CAN_TX_EMPTY_STATE;
  
  return CAN_NORMAL_STATE;
}


/*
************************************************************************
* 函数名   : MSCAN2_Sleep
*
* 描  述   : 使CAN2通道进入睡眠。函数内部使用
*
* 返回值   : CAN_REQ_SUCCESS  请求成功
*            CAN_REQ_FAIL     请求失败
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN2_Sleep(void){
  CAN2CTL0 |= 0x02;                //请求进入睡眠状态
  while(!(CAN2CTL1&0x02));         //等待进入睡眠状态
  if ((CAN2CTL0&0x02) && (CAN2CTL1&0x02)){

    return CAN_REQ_SUCCESS; 

  }else{

    return CAN_REQ_FAIL;  

  }
}



/*
************************************************************************
* 函数名   : MSCAN2_Poll
*
* 描  述   : CAN2通道以轮询方式接收数据。函数内部使用
*
* 返回值   : CAN_REQ_SUCCESS  请求成功
*            CAN_REQ_FAIL     请求失败
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN2_Poll(vCAN_FrameStruct* msg){

INT32U ID;
  
if (!(CAN2RFLG&0x01))

  return CAN_REQ_FAIL;        //检查是否接收到数据，未接收到数据，不阻塞，直接返回

  

ID = *(INT32U *)&CAN2RXIDR0;

if ((CAN2RXIDR1 & 0x08)) {                           //扩展帧        
  ID = ( ((ID >> 1) & 0x0003FFFF) | ((ID >> 3) & 0xFFFC0000) ); 
  msg->ID_Type = (CAN2RXIDR3 & 1)?  CAN_EXTENDED_REMOTE_ID : CAN_EXTENDED_DATA_ID; 
} else {                                             //标准帧                         
  ID >>= 21;                        
  msg->ID_Type = (CAN2RXIDR1 & 16)? CAN_STANDARD_REMOTE_ID : CAN_STANDARD_DATA_ID; 
}
  
msg->ID_High  = (INT16U)(ID>>16);
msg->ID_Low   = (INT16U)(ID &0x0000FFFF);  


if  (msg->ID_Type==CAN_EXTENDED_DATA_ID || msg->ID_Type==CAN_STANDARD_DATA_ID ){
  
   msg->Data_Len = CAN2RXDLR &0x0F;

/*     读取数据      */
   msg->Data[0]=CAN2RXDSR0;
   msg->Data[1]=CAN2RXDSR1;
   msg->Data[2]=CAN2RXDSR2;
   msg->Data[3]=CAN2RXDSR3;
   msg->Data[4]=CAN2RXDSR4;
   msg->Data[5]=CAN2RXDSR5;
   msg->Data[6]=CAN2RXDSR6;
   msg->Data[7]=CAN2RXDSR7;

}

CAN2RFLG_RXF=1;                                    //  清中断标志

return CAN_RX_SUCCESS;

}

/*
************************************************************************
* 函数名   : MSCAN2_READ
*
* 描  述   : CAN2通道接收数据。函数内部使用
*
* 返回值   : CAN_REQ_SUCCESS  请求成功
*            CAN_NO_RX        无新数据到达
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN2_READ(vCAN_FrameStruct* msg){

vCAN_FrameStruct* buf;


buf=&vCAN_A_Frame[msg->Msg_Num];


CPU_INT_DIS();
  
if (buf->Count!=msg->Count){



  memcpy(msg,buf,sizeof(vCAN_FrameStruct));
  

  CPU_INT_EN();
  

  return CAN_RX_SUCCESS;
  
}else{


  CPU_INT_EN();


  return CAN_NO_RX;
}
}
/*
************************************************************************
* 函数名   : CAN2ERISR
*
* 描  述   : CAN2通道读取数据的中断服务程序，改服务程序为不可嵌套的
             中断服务程序。函数内部使用
*
* 编  写  : 姜旭锋
************************************************************************
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED       /* Interrupt section for this module. Placement will be in NON_BANKED area. */

void CAN2_RX_ISR(void){

INT32U ID;
INT16U high,low;
INT8U  type;
INT8S  index,i;
vCAN_FrameStruct* msg;

ID = *(INT32U *)&CAN2RXIDR0;

if ((CAN2RXIDR1 & 0x08)) {                           //扩展帧        
  ID = ( ((ID >> 1) & 0x0003FFFF) | ((ID >> 3) & 0xFFFC0000) ); 
  type = (CAN2RXIDR3 & 1)?  CAN_EXTENDED_REMOTE_ID : CAN_EXTENDED_DATA_ID; 
} else {                                             //标准帧                         
  ID >>= 21;                        
  type = (CAN2RXIDR1 & 16)? CAN_STANDARD_REMOTE_ID : CAN_STANDARD_DATA_ID; 
}
  
high  = (INT16U)(ID>>16);
low   = (INT16U)(ID & 0x0000FFFF);  


/*  查找对应ID的缓冲区   */

msg=&vCAN_A_Frame[MSCANFRAME-1];


for(i=0;i<MSCANFRAME;i++,msg--){
    if (msg->ID_High==high &&  msg->ID_Low==low){
      break;
    }
}

/*  该ID无对应缓冲区，则忽略*/
if (i==MSCANFRAME){

  CAN2RFLG&=0x01;     /*  清除中断标志位  */
  return;

}

/*  复制CAN数据报信息至i号缓冲区 */
msg->ID_Type=type;

if  (type==CAN_EXTENDED_DATA_ID || type==CAN_STANDARD_DATA_ID ){
  
   msg->Data_Len = CAN2RXDLR &0x0F;

/*     读取数据      */
   msg->Data[0]=CAN2RXDSR0;
   msg->Data[1]=CAN2RXDSR1;
   msg->Data[2]=CAN2RXDSR2;
   msg->Data[3]=CAN2RXDSR3;
   msg->Data[4]=CAN2RXDSR4;
   msg->Data[5]=CAN2RXDSR5;
   msg->Data[6]=CAN2RXDSR6;
   msg->Data[7]=CAN2RXDSR7;
   
   msg->Count++;

}



CAN2RFLG&=0x01;     /*  清除中断标志位  */

}







/*
************************************************************************
* 函数名   : MSCAN3_INIT
*
* 描  述   : CAN3通道的初始化函数。函数内部使用
*
* 参  数   : vCAN_INITStruct* init    CAN初始化结构体指针 
*
* 返回值   : CAN_REQ_SUCCESS         CAN3通道初始化成功
*            CAN_REQ_FAIL            CAN3通道初始化失败
*
* 编  写   : 姜旭锋
************************************************************************
*/
INT16U MSCAN3_INIT(vCAN_InitStruct* init)
{


CAN3CTL1=0x80;                                          // 使能MSCAN3 模块
CAN3CTL0|=0x01;
while(!(CAN3CTL1&0x01));

                  
CAN3IDAC=(INT8U)((init->GlobalIDMask_Type&0x03)<<4);    //设置Filter模式

// 设置Filter。

// 对于8位模式，init->GlboalID_AccpFilter[0]的高位对应IDAR0，低位对应IDAR1，以此类推

// 对于16位模式, init->GlboalID_AccpFilter[0]的位序正常.

// 对于32位模式, init->GlboalID_AccpFilter[0]为32位的高位，init->GlboalID_AccpFilter[1]为低位                             
CAN3IDAR1=(INT8U)(init->GlboalID_AccpFilter[0]&0x00FF);
CAN3IDAR0=(INT8U)((init->GlboalID_AccpFilter[0]>>8)&0x00FF);

CAN3IDAR3=(INT8U)(init->GlboalID_AccpFilter[1]&0x00FF);
CAN3IDAR2=(INT8U)((init->GlboalID_AccpFilter[1]>>8)&0x00FF);

CAN3IDAR5=(INT8U)(init->GlboalID_AccpFilter[2]&0x00FF);
CAN3IDAR4=(INT8U)((init->GlboalID_AccpFilter[2]>>8)&0x00FF);

CAN3IDAR7=(INT8U)(init->GlboalID_AccpFilter[3]&0x00FF);
CAN3IDAR6=(INT8U)((init->GlboalID_AccpFilter[3]>>8)&0x00FF);





// 设置Mask.

// 对于8位模式，init->GlobalID_Mask[0]的高位对应IDMR0，低位对应IDMR1，以此类推

// 对于16位模式, init->GlobalID_Mask[0]的位序正常.

// 对于32位模式, init->GlobalID_Mask[0]为32位的高位，init->GlobalID_Mask[1]为低位
CAN3IDMR1=(INT8U)(init->GlobalID_Mask[0]&0x00FF);
CAN3IDMR0=(INT8U)((init->GlobalID_Mask[0]>>8)&0x00FF);

CAN3IDMR3=(INT8U)(init->GlobalID_Mask[1]&0x00FF);
CAN3IDMR2=(INT8U)((init->GlobalID_Mask[1]>>8)&0x00FF);

CAN3IDMR5=(INT8U)(init->GlobalID_Mask[2]&0x00FF);
CAN3IDMR4=(INT8U)((init->GlobalID_Mask[2]>>8)&0x00FF);

CAN3IDMR7=(INT8U)(init->GlobalID_Mask[3]&0x00FF);
CAN3IDMR6=(INT8U)((init->GlobalID_Mask[3]>>8)&0x00FF);


CAN3CTL1_CLKSRC=1;                                   //  选择内部时钟源

CAN3BTR0_BRP=init->CAN_SPEED;                        //  设置波特率
CAN3BTR0_SJW=0;
CAN3BTR1_SAMP=0;
CAN3BTR1_TSEG_20=7;
CAN3BTR1_TSEG_10=15;

CAN3CTL0&=0xfe;                                      //   请求退出初始化模式
while(CAN3CTL1&0x01);                                //   等待退出初始化模式


while(!CAN3CTL0_SYNCH);                              //   等待总线同步

CAN3CTL0=0x08;                                       //   使能定时器
if (init->Poll_Enable==ENABLE_POLL)
  CAN3RIER=0x00;                                     //   禁止接收中断请求
else 
  CAN3RIER=0x01;                                     //   使能接收中断请求


return CAN_REQ_SUCCESS;
}



/*
************************************************************************
* 函数名   : MSCAN3_SEND
*
* 描  述   : 由CAN3通道发送数据。函数内部使用
*
* 参  数  : vCAN_FrameStruct* msg    发送的数据包结构体指针 
*
* 返回值   : CAN_TX_NO_BUFFER         CAN3通道无可用的发送缓冲区
*            CAN_REQ_SUCCESS          CAN3通道发送数据成功
*       CAN_TX_FAIL           CAN3通道发送数据失败
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN3_SEND(vCAN_FrameStruct* msg){

INT32U MessageID;


/*   检查缓冲区      */
if (!(CAN3TFLG & 7))                 
    return CAN_TX_NO_BUFFER;      


/*   获得发送缓冲区  */
CAN3TBSEL=CAN3TFLG;                  

MessageID=((INT32U)(msg->ID_High))<<16 | (INT32U)msg->ID_Low;


/*   封装CAN包包头   */  
if ( msg->ID_Type  <= CAN_EXTENDED_REMOTE_ID)           /* Is it the extend frame? */
  *(INT32U *)&CAN3TXIDR0 = ( ((MessageID << 1) & 0x0007FFFF) | ((MessageID << 3) & 0xFFE00000) ) | 0x00080000; /* Set the message as "extended" */
else
  *(INT32U *)&CAN3TXIDR0 = (MessageID << 21); /* Sets the message as "standard" */


 if ( msg->ID_Type  & 0x01) {           /* Is it a remote frame? */
    if (msg->ID_Type  <= CAN_EXTENDED_REMOTE_ID)  /* Is it the extend frame? */
      CAN3TXIDR3 |= 1;                  /* If yes then set message type as "remote frame" */
    else
      CAN3TXIDR1 |= 16;   
  }
  else {                               /* data frame */
                                       /* If no then set message type as "remote frame" */
     CAN3TXDSR0 =msg->Data[0];
     CAN3TXDSR1 =msg->Data[1];
     CAN3TXDSR2 =msg->Data[2];
     CAN3TXDSR3 =msg->Data[3];
     CAN3TXDSR4 =msg->Data[4];
     CAN3TXDSR5 =msg->Data[5];
     CAN3TXDSR6 =msg->Data[6];
     CAN3TXDSR7 =msg->Data[7];

     CAN3TXDLR=(INT8U)(msg->Data_Len&0x0F);
   
    if (msg->ID_Type  <= CAN_EXTENDED_REMOTE_ID)   /* Is it the extend frame? */
      CAN3TXIDR3 &= 254;               /* If yes then set message type as "data frame" */   
    else
      CAN3TXIDR1 &= 239;               /* If no then set message type as "data frame" */
 
  }
  
CAN3TXTBPR=msg->Tx_Priority;

CAN3TFLG=CAN3TBSEL;

return CAN_REQ_SUCCESS;

}



/*
************************************************************************
* 函数名   : MSCAN3_STATUS
*
* 描  述   : 获取CAN3通道的当前状态。函数内部使用
*
* 返回值   : CAN_IN_INIT_STATE      CAN3处于初始化状态
*            CAN_IN_SLEEP_STATE     CAN3处于睡眠状态
*            CAN_TXERR_STATE        CAN3处于数据发送状态
*            CAN_RXERR_STATE        CAN3处于数据接收状态
*            CAN_BUSOFF_STATE       CAN3处于总线忙状态
*            CAN_OVERRUN_STATE      CAN3处于溢出状态
*            CAN_TX_EMPTY_STATE     CAN3处于发送缓冲区空状态
*            CAN_NORMAL_STATE       CAN3处于正常状态
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN3_STATUS(){
  if (CAN3CTL0&0x01 && CAN3CTL1&0x01)
     return CAN_IN_INIT_STATE;
  
  if (CAN3CTL0&0x02 && CAN3CTL1&0x02) 
    return CAN_IN_SLEEP_STATE ;
   
  if (CAN3RFLG_TSTAT==2)
     return CAN_TXERR_STATE;
  
  if (CAN3RFLG_RSTAT==2)
     return CAN_RXERR_STATE;
  
  if (CAN3RFLG_RSTAT==3 || CAN3RFLG_TSTAT==3)
     return CAN_BUSOFF_STATE;
  
  if (CAN3RFLG_OVRIF)
     return CAN_OVERRUN_STATE;
  
  if (!CAN3TFLG_TXE)
     return  CAN_TX_EMPTY_STATE;
  
  return CAN_NORMAL_STATE;
}


/*
************************************************************************
* 函数名   : MSCAN3_Sleep
*
* 描  述   : 使CAN3通道进入睡眠。函数内部使用
*
* 返回值   : CAN_REQ_SUCCESS  请求成功
*            CAN_REQ_FAIL     请求失败
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN3_Sleep(void){
  CAN3CTL0 |= 0x02;
  while(!(CAN3CTL1&0x02));
  if ((CAN3CTL0&0x02) && (CAN3CTL1&0x02)){

    return CAN_REQ_SUCCESS; 

  }else{

    return CAN_REQ_FAIL;  

  }
}



/*
************************************************************************
* 函数名   : MSCAN3_Poll
*
* 描  述   : CAN3通道以轮询方式接收数据。函数内部使用
*
* 返回值   : CAN_REQ_SUCCESS  请求成功
*            CAN_REQ_FAIL     请求失败
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN3_Poll(vCAN_FrameStruct* msg){

INT32U ID;
  
if (!(CAN3RFLG&0x01))

  return CAN_REQ_FAIL;

ID = *(INT32U *)&CAN3RXIDR0;

if ((CAN3RXIDR1 & 0x08)) {                           //扩展帧        
  ID = ( ((ID >> 1) & 0x0003FFFF) | ((ID >> 3) & 0xFFFC0000) ); 
  msg->ID_Type = (CAN3RXIDR3 & 1)?  CAN_EXTENDED_REMOTE_ID : CAN_EXTENDED_DATA_ID; 
} else {                                             //标准帧                         
  ID >>= 21;                        
  msg->ID_Type = (CAN3RXIDR1 & 16)? CAN_STANDARD_REMOTE_ID : CAN_STANDARD_DATA_ID; 
}
  
msg->ID_High  = (INT16U)(ID>>16);
msg->ID_Low   = (INT16U)(ID & 0x0000FFFF);  


if  (msg->ID_Type==CAN_EXTENDED_DATA_ID || msg->ID_Type==CAN_STANDARD_DATA_ID ){
  
   msg->Data_Len = CAN3RXDLR &0x0F;

   /*     读取数据      */
   msg->Data[0]=CAN3RXDSR0;
   msg->Data[1]=CAN3RXDSR1;
   msg->Data[2]=CAN3RXDSR2;
   msg->Data[3]=CAN3RXDSR3;
   msg->Data[4]=CAN3RXDSR4;
   msg->Data[5]=CAN3RXDSR5;
   msg->Data[6]=CAN3RXDSR6;
   msg->Data[7]=CAN3RXDSR7;

}

CAN3RFLG_RXF=1;

return CAN_RX_SUCCESS;

}

/*
************************************************************************
* 函数名   : MSCAN3_READ
*
* 描  述   : CAN3通道接收数据。函数内部使用
*
* 返回值   : CAN_REQ_SUCCESS  请求成功
*            CAN_NO_RX        无新数据到达
*
* 编  写  : 姜旭锋
************************************************************************
*/
INT16U MSCAN3_READ(vCAN_FrameStruct* msg){

vCAN_FrameStruct* buf;



buf=&vCAN_B_Frame[msg->Msg_Num];

CPU_INT_DIS();
  
if (buf->Count!=msg->Count){



  memcpy(msg,buf,sizeof(vCAN_FrameStruct));


  CPU_INT_EN();


  return CAN_RX_SUCCESS;
  
}else{


  CPU_INT_EN();

  
  return CAN_NO_RX;
}
}
/*
************************************************************************
* 函数名   : MSCAN3_READ_ISR
*
* 描  述   : CAN3通道读取数据的中断服务程序，改服务程序为不可嵌套的
             中断服务程序。函数内部使用
*
* 编  写  : 姜旭锋
************************************************************************
*/
 #pragma CODE_SEG __NEAR_SEG NON_BANKED       /* Interrupt section for this module. Placement will be in NON_BANKED area. */

void CAN3_RX_ISR(void){

INT32U ID;
INT16U high,low;
INT8U  type;
INT8S  index,i;
vCAN_FrameStruct* msg;

ID = *(INT32U *)&CAN3RXIDR0;

if ((CAN3RXIDR1 & 0x08)) {                           //扩展帧        
  ID = ( ((ID >> 1) & 0x0003FFFF) | ((ID >> 3) & 0xFFFC0000) ); 
  type = (CAN3RXIDR3 & 1)?  CAN_EXTENDED_REMOTE_ID : CAN_EXTENDED_DATA_ID; 
} else {                                             //标准帧                         
  ID >>= 21;                        
  type = (CAN3RXIDR1 & 16)? CAN_STANDARD_REMOTE_ID : CAN_STANDARD_DATA_ID; 
}
  
high  = (INT16U)(ID>>16);
low   = (INT16U)(ID & 0x0000FFFF);  


/*  查找对应ID的缓冲区   */

msg=&vCAN_B_Frame[MSCANFRAME-1];


for(i=0;i<MSCANFRAME;i++,msg--){
    if (msg->ID_High==high &&  msg->ID_Low==low){
      break;
    }
}

/*  该ID无对应缓冲区，则忽略*/
if (i==MSCANFRAME){

  CAN3RFLG&=0x01;     /*  清除中断标志位  */
  return;

}

/*  复制CAN数据报信息至i号缓冲区 */
msg->ID_Type=type;

if  (type==CAN_EXTENDED_DATA_ID || type==CAN_STANDARD_DATA_ID ){
  
   msg->Data_Len = CAN3RXDLR &0x0F;

/*     读取数据      */
   msg->Data[0]=CAN3RXDSR0;
   msg->Data[1]=CAN3RXDSR1;
   msg->Data[2]=CAN3RXDSR2;
   msg->Data[3]=CAN3RXDSR3;
   msg->Data[4]=CAN3RXDSR4;
   msg->Data[5]=CAN3RXDSR5;
   msg->Data[6]=CAN3RXDSR6;
   msg->Data[7]=CAN3RXDSR7;
   
   msg->Count++;

}



CAN3RFLG&=0x01;     /*  清除中断标志位  */

}


/*

===========================================================================

*/

// 说明：
// 以下为一汽标准API定义。
/*

===========================================================================

*/



/*
******************************************************************
* vCAN_INITStruct* init_value， 8位无符号数  
* 返回值：16位无符号数error_code
* 命令格式：vCAN_ Init (vCAN_INITStruct*, unsigned 8-bit)
* 功能描述：本功能根据指定的init_value 结构来初始化指定的CAN设备
* （CAN0 或 CAN1），初始化包括CAN比特率和mask/过滤器规范。如果初始
* 化失败，返回值error_code 为非零。
******************************************************************
*/
INT16U vCAN_Init(vCAN_InitStruct* init,INT16U device){


  switch(device) {
    case CAN_A:return MSCAN2_INIT(init);break;
    case CAN_B:return MSCAN3_INIT(init);break;  
    default:return CAN_REQ_FAIL;break; 
  }
  
}


/*
*********************************************************************
* CAN Rx 信息缓冲器初始化
* 输入：vCAN_FrameStruct* rx_msg，8位无符号数  
* 返回值：16位无符号数error_code
* 命令格式：vCAN_ InitRxMsgBuffer (vCAN_FrameStruct*, unsigned 8-bit)
* 功能描述：本功能根据用户指定的rx_msg结构，为指定的CAN设备（CAN0 或 
* CAN1）准备好接收信息的缓冲器。用户至少要在此结构中指定ID信息。如果
* Poll_Enable被设置为禁止，在CAN被初始化前此功能必须被调用。
*********************************************************************
*/
INT16U vCAN_InitRxMsgBuffer(vCAN_FrameStruct* rx_msg,INT8U device){
   if (device==CAN_A){ 
      vCAN_A_Frame[rx_msg->Msg_Num].ID_High=rx_msg->ID_High;
      vCAN_A_Frame[rx_msg->Msg_Num].ID_Low=rx_msg->ID_Low;

      vCAN_A_Frame[rx_msg->Msg_Num].Msg_Num=rx_msg->Msg_Num;

      vCAN_A_Frame[rx_msg->Msg_Num].ID_Type=rx_msg->ID_Type;

      vCAN_A_Frame[rx_msg->Msg_Num].Tx_Priority=rx_msg->Tx_Priority;

      vCAN_A_Frame[rx_msg->Msg_Num].Count=0;  
   }else if (device==CAN_B){
      vCAN_B_Frame[rx_msg->Msg_Num].ID_High=rx_msg->ID_High;
      vCAN_B_Frame[rx_msg->Msg_Num].ID_Low=rx_msg->ID_Low;

      vCAN_B_Frame[rx_msg->Msg_Num].Msg_Num=rx_msg->Msg_Num;

      vCAN_B_Frame[rx_msg->Msg_Num].ID_Type=rx_msg->ID_Type;

      vCAN_B_Frame[rx_msg->Msg_Num].Tx_Priority=rx_msg->Tx_Priority;
      vCAN_B_Frame[rx_msg->Msg_Num].Count=0;        
   }  

   

   rx_msg->Count=0;

   return 0;
}


/*
**********************************************************************
* 传送的数据队列信息
* 输入：vCANFrameStruct* Tx_CANFrame， 8位无符号数  
* 返回值：16位无符号数error_code
* 命令格式：vCAN_ TxMsg(vCANFrameStruct*, unsigned 8-bit)
* 功能描述：此程序用于排列存储在硬件传送缓冲器中的指定的CAN设备（CAN0 或 
* CAN1）的信息。本功能需要用于传送的信息内容。输入参数Tx_CANFrame属于
* vCANFrameStruct结构类型。如果信息能够在硬件缓冲器中排列，并且进行传送，
* 则本功能返回成功信息。如果由于硬件缓冲器满，信息不能进行排列，则此程序
* 返回失败信息。
**********************************************************************
*/
INT16U vCAN_TxMsg(vCAN_FrameStruct *msg, INT16U device){
  switch(device) {
    case CAN_A:return MSCAN2_SEND(msg);break;
    case CAN_B:return MSCAN3_SEND(msg);break;  
    default:break; 
  }

}

/*
***********************************************************************
*
* 读信息
* 输入：vCANFrameStruct* msg， 8位无符号数  
* 返回值：16位无符号数error_code
* 命令格式：vCAN_RxMsg(vCANFrameStruct *, unsigned 8-bit)
* 功能描述：本功能用于读取指定CAN设备（CAN0 或 CAN1）中指定的信息缓冲器
* 内部的内容。用户必须在用户指南提供的信息结构中指定Msg_Num （信息缓冲器
* 号）或信息ID号。此功能读取信息ID号，数据字节，数据长度和信息的时间戳（如
* 果允许的话），时间戳在用户信息结构中的缓冲器中出现。如果指定的信息缓冲器
* 号和ID号不符，则本功能将返回非零的错误码。
*
***********************************************************************
*/
INT16U vCAN_RxMsg(vCAN_FrameStruct *msg,INT16U device){
  switch(device) {
    case CAN_A:return MSCAN2_READ(msg);break;
    case CAN_B:return MSCAN3_READ(msg);break;     
    default:break; 
  }
}

/*
***********************************************************************
*
* 读信息
* 输入：vCANFrameStruct* msg， 8位无符号数  
* 返回值：16位无符号数error_code
* 命令格式：vCAN_RxMsg(vCANFrameStruct *, unsigned 8-bit)
* 功能描述：本功能用于读取指定CAN设备（CAN0 或 CAN1）中指定的信息缓冲器
* 内部的内容。用户必须在用户指南提供的信息结构中指定Msg_Num （信息缓冲器
* 号）或信息ID号。此功能读取信息ID号，数据字节，数据长度和信息的时间戳（如
* 果允许的话），时间戳在用户信息结构中的缓冲器中出现。如果指定的信息缓冲器
* 号和ID号不符，则本功能将返回非零的错误码。
*
***********************************************************************
*/
INT16U vCAN_PollMsg(vCAN_FrameStruct *msg,INT16U device){
  switch(device) {
    case CAN_A:return MSCAN2_Poll(msg);break;
    case CAN_B:return MSCAN3_Poll(msg);break;     
    default:break; 
  }
}


/*
***********************************************************************
*
* 使CAN进入睡眠状态
* 输入NT16U device  8位无符号数  
* 返回值：16位无符号数error_code
* 命令格式：vCAN_Sleep (INT16U device)
* 功能描述：本功能用于使指定CAN通道进入睡眠状态
*
***********************************************************************
*/
INT16U vCAN_Sleep (INT16U device){
  switch(device) {
    case CAN_A:return MSCAN2_Sleep();break;
    case CAN_B:return MSCAN3_Sleep();break;     
    default:break; 
  }
}

/*
***********************************************************************
*
* 获得CAN通道状态信息
* 输入  INT16U device  8位无符号数  
* 返回值：16位无符号数error_code
* 命令格式CAN_GetStatus (INT16U device)
* 功能描述：本功能用于获取指定CAN通道的状态信息
*
***********************************************************************
*/
INT16U vCAN_GetStatus (INT16U device){

  switch(device) {
    case CAN_A:return MSCAN2_STATUS();break;
    case CAN_B:return MSCAN3_STATUS();break;     
    default:break; 
  }
}
#endif
