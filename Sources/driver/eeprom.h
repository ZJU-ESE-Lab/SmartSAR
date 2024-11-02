/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�eeprom.h
* ժ    Ҫ������EEPROMģ�����ض���
*
* ��ǰ�汾��1.0
* ��    �ߣ������
* ������ڣ�2005��2��20��
*********************************************************************************
*/

#ifndef _HCS12_EEPROM_H_

#define _HCS12_EEPROM_H_



typedef struct{
  INT16U *Dest_Addr; 
  INT16U *Source_Addr; 
  INT8U  Data_Len; 

}vEE_ReqStruct;  

#define  REQ_SUCCESS 	                   0x01
#define  REQ_FAIL                          0x00


#define  EEPROM_REQUEST_IDLE               0x01
#define  EEPROM_REQUEST_FAIL               0x02
#define  EEPROM_NOT_WRITABLE               0x03
#define  EEPROM_NOT_READABLE               0x04
#define  EEPROM_REQUEST_QUEUED             0x05
#define  EEPROM_REQUEST_IN_PROGRESS        0x06
#define  EEPROM_REQUEST_ACCERR             0x07
#define  EEPROM_REQUEST_PROTECT_ERR        0x08
#define  EEPROM_REQUEST_FAIL_UNKNOWN       0x09
#define  EEPROM_CMD_FINISH                 0x0A



void EPPROM_Write(INT16U *addr,INT16U value);



/*

************************************************************

* Prototype     : UInt8 vInterface_QueueReq(vEEReqStruct *ee_req)

* Purpose       : This function will initiate EEPROM update 

*                 request operations on the background.

* Return Values : REQ_SUCCESS �C EEPROM update request has been

*                 successfully queued.

*                 REQ_FAIL �C EEPROM update request could not be queued.

* Parameters    : struct vEE_ReqStruct;

* Restrictions  : Application, which is the caller, need to maintain 

*                 the ee_req content in its original memory location 

*                 until this request done, which is indicated by vEE_GetStatus()

* Information   : None

************************************************************

*/
INT8U vEE_QueueReq(vEE_ReqStruct *ee_req);
/*

************************************************************

* Prototype     : INT16U vEE_GetStatus(void)

* Purpose       : This function reports EEPROM update request 

*                 operation progress status.

* Return Values : EEPROM_REQUEST_IDLE �C no EEPROM operation currently on-going

*                 EEPROM_REQUEST_FAIL �C last EEPROM update request has failed

*                 EEPROM_NOT_WRITABLE �C EEPROM could not be updated currently

*                 EEPROM_NOT_READABLE �C EEPROM could not be read currently

*                 EEPROM_REQUEST_QUEUED �C EEPROM update request has been queued

*                 EEPROM_REQUEST_IN_PROGRESS �C EEPROM update request is currently under

*                 operation

*                 EEPROM_REQUEST_ACCERR �C The EEPROM operation fails for the access error.

*                 EEPROM_REQUEST_PROTECT_ERR �C The EEPROM operation fails for the

*                 protection error.

*                 EEPROM_REQUEST_FAIL_UNKNOWN: The EEPROM operation fails for the

*                 unknown error.

*                 EEPROM_CMD_FINISH �C EEPROM operation command has finished

* Parameters   : None

* Restrictions : None

* Information  : None

************************************************************

*/

INT16U vEE_GetStatus(void);

#endif