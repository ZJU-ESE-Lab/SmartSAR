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


#ifndef _HCS12_MSCAN_H_
#define _HCS12_MSCAN_H_


/*

---------------------------------------------------------------------------------

| 说明 ： 当前版本只定义CAN_A,CAN_B，分别应MSCAN2，MSCAN3

---------------------------------------------------------------------------------

*/
#define CAN_A                            0
#define CAN_B                            1

/*

---------------------------------------------------------------------------------

| 说明 ： 当前版本只定义3种传输波特率

---------------------------------------------------------------------------------

*/
#define CAN_500KBPS                      0
#define CAN_250KBPS                      1
#define CAN_125KBPS	                 3




/*

---------------------------------------------------------------------------------

| 说明 ： MSCAN接收方式定义

---------------------------------------------------------------------------------

*/
#define ENABLE_POLL                      0
#define DISABLE_POLL                     1 

/*

---------------------------------------------------------------------------------

| 说明 ： CAN协议中规定的帧类型

---------------------------------------------------------------------------------

*/
#define CAN_EXTENDED_DATA_ID             0x00
#define CAN_EXTENDED_REMOTE_ID           0x01

#define CAN_STANDARD_DATA_ID             0x02
#define CAN_STANDARD_REMOTE_ID           0x03


/*

---------------------------------------------------------------------------------

| 说明 ： HCS12 MSCAN模块定义的过滤方式

---------------------------------------------------------------------------------

*/
#define TWO_32BIT_ACCP_FILTER              0
#define FOUR_16BIT_ACCP_FILTER             1
#define EIGHT_8BIT_ACCP_FILTER             2
#define FILTER_CLOSED                      3





/*

---------------------------------------------------------------------------------

| 说明 ： HCS12 MSCAN模块的状态定义

---------------------------------------------------------------------------------

*/
#define CAN_NORMAL_STATE               0x0000
#define CAN_BUSOFF_STATE               0x0001
#define CAN_TXERR_STATE                0x0002
#define CAN_RXERR_STATE                0x0004
#define CAN_OVERRUN_STATE              0x0008
#define CAN_IN_SLEEP_STATE             0x0010
#define CAN_IN_INIT_STATE              0x0020
#define CAN_TX_EMPTY_STATE             0x0040
#define CAN_INIT_FAIL_STATE            0x0080


/*

---------------------------------------------------------------------------------

| 说明 ： HCS12 MSCAN模块API的返回值定义

---------------------------------------------------------------------------------

*/
#define CAN_REQ_SUCCESS                0x0000   
#define CAN_TX_NO_BUFFER               0x0001
#define CAN_TX_FAIL                    0x0002
#define CAN_RX_SUCCESS                 0x0004 
#define CAN_NO_RX                      0x0008
#define CAN_REQ_FAIL                   0x0002


#define MSCANNUM                          2
#define MSCANFRAME                       10




typedef struct FRAMEStruct
{
INT16U  ID_High;
INT16U  ID_Low;
INT8U   Data[8];
INT8U   Data_Len;
INT8U   Tx_Priority;
INT16U  TimeStamp;   
INT16U  Count;
INT8U   ID_Type;
INT8U   Msg_Num;       
}vCAN_FrameStruct;


typedef struct  INITStruct
{
INT8U  CAN_SPEED;
INT8U  GlobalIDMask_Type;
INT16U GlboalID_AccpFilter[4];
INT16U GlobalID_Mask[4];
INT8U  Poll_Enable;
}vCAN_InitStruct;



#endif 
