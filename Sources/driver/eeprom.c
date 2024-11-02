/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* eeprom.c
* 摘    要：包含EEPROM模块的驱动程序
*
* 当前版本：1.0
* 作    者：姜旭锋
* 完成日期：2005年2月20日
*********************************************************************************
*/

#ifndef _HCS12_EEPROM_C_

#define _HCS12_EEPROM_C_

#include "os_cpu.h"
#include "io_map.h"
#include "eeprom.h"

void EPPROM_Write(INT16U *addr,INT16U value){
     if  (!ECLKDIV_EDIVLD){
        ECLKDIV=1<<7|11;       
        EPROT=0x88;     
     }
     while(!ECLKDIV_EDIVLD);   
     *addr=value;
     ECMD=0x20;
     ESTAT=0x80;     
}


/*

************************************************************

* Prototype     : INT16U vEE_GetStatus(void)

* Purpose       : This function reports EEPROM update request 

*                 operation progress status.

* Return Values : EEPROM_REQUEST_IDLE – no EEPROM operation currently on-going

*                 EEPROM_REQUEST_FAIL – last EEPROM update request has failed

*                 EEPROM_NOT_WRITABLE – EEPROM could not be updated currently

*                 EEPROM_NOT_READABLE – EEPROM could not be read currently

*                 EEPROM_REQUEST_QUEUED – EEPROM update request has been queued

*                 EEPROM_REQUEST_IN_PROGRESS – EEPROM update request is currently under

*                 operation

*                 EEPROM_REQUEST_ACCERR – The EEPROM operation fails for the access error.

*                 EEPROM_REQUEST_PROTECT_ERR – The EEPROM operation fails for the

*                 protection error.

*                 EEPROM_REQUEST_FAIL_UNKNOWN: The EEPROM operation fails for the

*                 unknown error.

*                 EEPROM_CMD_FINISH – EEPROM operation command has finished

* Parameters   : None

* Restrictions : None

* Information  : None

************************************************************

*/
INT16U vEE_GetStatus(void){

   return EEPROM_CMD_FINISH;

}



/*

************************************************************

* Prototype     : UInt8 vInterface_QueueReq(vEEReqStruct *ee_req)

* Purpose       : This function will initiate EEPROM update 

*                 request operations on the background.

* Return Values : REQ_SUCCESS – EEPROM update request has been

*                 successfully queued.

*                 REQ_FAIL – EEPROM update request could not be queued.

* Parameters    : struct vEE_ReqStruct;

* Restrictions  : Application, which is the caller, need to maintain 

*                 the ee_req content in its original memory location 

*                 until this request done, which is indicated by vEE_GetStatus()

* Information   : None

************************************************************

*/
INT8U vEE_QueueReq(vEE_ReqStruct *ee_req){
    INT8U i;
    INT16U *src,*dest;
    dest=(INT16U *)(ee_req->Dest_Addr);
    src =(INT16U *)(ee_req->Source_Addr); 
    for(i=0;i<ee_req->Data_Len;i++){
      EPPROM_Write(dest,*src);
      src++;
      dest++;
    }

    if (ESTAT_ACCERR){

      ESTAT_ACCERR=1;

      return REQ_FAIL;

    }

    else

      return REQ_SUCCESS;
}

#endif

