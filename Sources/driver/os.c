#ifndef   _HCS12_OS_C_
#define   _HCS12_OS_C_

 
#include "os_cpu.h"
#include "io_map.h"


INT8U cop_status=0;
/*
***********************************************************
* Function    :vInterface_ReadTickCount
* Description :Read the current time in unit of tick count.
*
***********************************************************
*/
INT16U vInterface_ReadTickCount(){
   return OSTime;
}


/*
***********************************************************
* Function    :vInterface_GetTickRes
* Description :Read the current time in unit of tick count.
*
***********************************************************
*/
INT16U vInterface_GetTickRes(){

}

/*
***********************************************************
* Function    :vInterface_ToggleCOP
* Description :Toggle the watchdog of the micro. 
*
***********************************************************
*/
void vInterface_ToggleCOP(void){
 if  (cop_status==0){  
   ARMCOP=0x55;
 	 cop_status=1;
 }else{
   ARMCOP=0xAA;
   cop_status=0;
 }
}

/*
***********************************************************
* Function    :vInterface_MicroReset
* Description :Reset the micro.
*
***********************************************************
*/
void vInterface_MicroReset(void){
	 COPCTL=0x03;
	 ARMCOP=0x00;
}


/*
***********************************************************
* Function    :vInterface_MicroStop
* Description :Stop the micro.
*
***********************************************************
*/
void vInterface_MicroStop(void){
  asm stop;
}


/*
***********************************************************
* Function    :vInterface_CalculateSum
* Description :Calculate a 16-bit word serial summation of
               all area between two addresses.
*
***********************************************************
*/
INT16U vInterface_CalculateSum(INT32U start_addr, INT32U end_addr){

}


/*
***********************************************************
* Function    : vInterface_PowerDown
* Description : This function configures all outputs to its 
*               proper state and signal Main Relay to stop
*               battery powerto the ECU.
*
***********************************************************
*/
void vInterface_PowerDown (void){

}


 #endif