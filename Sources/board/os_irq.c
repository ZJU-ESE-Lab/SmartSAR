/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：os_irq.c
* 摘    要：包含操作系统中断处理模块
*
* 作    者: 姜旭锋
*********************************************************************************
*/
#ifndef OS_GLOBALS
#include "includes.h"
#endif

#include "app_cfg.h"

#pragma CODE_SEG __NEAR_SEG NON_BANKED       /* Interrupt section for this module. Placement will be in NON_BANKED area. */


ISR_FP   ISRVector[ISR_TBL_SIZE];            /* 用户中断向量表 */
//OSSTK    SystemSTK[512];
//OSSTKRef SystemSTKHead;

static INT8U SuspendCount = 0; /*记录Suspend/ResumeAllInterupst嵌套层数*/
static INT8U OSSuspendCount = 0;/*记录Suspend/ResumeOSInterupst嵌套层数*/

INT32U OSISRCategory2Mask[2] = {0,0}; /*二类中断类别表*/

#ifdef OS_ISR_AUTO_DEFINE
extern OSISRCfg OSISRCfgTbl[OS_NUM_ISR];

/*
***************************************************************************
*void OSISRInit(void)
* 描述：初始化注册中断服务程序
* 调用的函数：无
* 允许被调用的位置：StartOS
* 参数：	无
* 返回：	无
* 注意事项：无
* 创建人：郁利吉
* 时间：2005.10.12
* 修改人：郁利吉
* 时间：2005.10.19 
* 备注：
***************************************************************************
*/
void OSISRInit(void)
{
	OSISRCfg * cfg=OSISRCfgTbl;
	SourceType source;
	INT32U i;
  for(i=0;i<ISR_TBL_SIZE;i++)
    ISRVector[i]=0;		
	
	for(;cfg<(OSISRCfgTbl+OS_NUM_ISR);cfg++)
	{
		source=cfg->source;
		ISRVector[source]=cfg->handle;  /* 注册中断函数   */
		if((source == OSISR_IRQ)&&(OSIRQ_EDGE==cfg->option))
		  INTCR_IRQE = 1;
		if (OSISRCategory2==cfg->category)
		  OSISRCategory2Mask[source/32]|=(0x80000000>>(source%32));
	}
}
#else/*#ifdef OS_ISR_AUTO_DEFINE*/

/*
******************************************************************
* INT8U RegisterISR(INT8U isr_code,ISR_FP fp,INT8U category,INT8U opt)
* 描述：注册一个用户中断服务程序
* 允许被调用的位置：用户配置中断服务程序时调用
* 参数：isr_code: 中断号
*       fp:       中断处理函数的指针
*       category：中断类型
*                 取值：
*                 OSISRCategory1，一类中断
*                 OSISRCategory2，二类中断
*       opt：     触发方式
*                 取值：
*                 OSIRQ_NULL,
*                 OSIRQ_LEVEL,
*                 OSIRQ_EDGE
*
* 返回值  OS_INT_ERR   : 表示注册失败,isr_code越界
*         OS_INT_NOERR : 表示ok
*
* 编写人  :  吕攀 
* 编写日期:  2005.05.18
* 修改人：郁利吉
* 时间：2005.10.19
* 备注：
******************************************************************
*/
INT8U RegisterISR(INT8U isr_code,ISR_FP fp,INT8U category,INT8U opt) 
{
    if (isr_code >= ISR_TBL_SIZE || isr_code == UNIMPISR ){               /*   中断号非法     */
       return OS_INT_ERR;
    }
               
	  ISRVector[isr_code]=fp;                      /*   注册中断函数   */
	  if((isr_code == OSISR_IRQ)&&(OSIRQ_EDGE==opt))
		  INTCR_IRQE = 1;
		if (OSISRCategory2==category)
		  OSISRCategory2Mask[isr_code/32]|=(0x80000000>>(isr_code%32));
	  return OS_INT_NOERR;
}

#endif/*#ifdef OS_ISR_AUTO_DEFINE*/




/*
******************************************************************
* void SuspendOSInterrupts(void)
* 描述：保存所有二类中断的状态并关闭这些中断
* 允许被调用的位置：中断服务程序、任务层
* 参数：无
* 返回值：无
* 编写人  :  郁利吉
* 编写日期:  2005.10.14
* 备注：没有保存状态，尚未区分一二类中断
******************************************************************
*/
void SuspendOSInterrupts(void)
{
  OSSuspendCount++;
    if(1==OSSuspendCount)
    {
      __asm sei;
    }
   return;
}

/*
******************************************************************
* void ResumeOSInterrupts(void)
* 描述：恢复由SuspendOSInterrupts保存的二类中断的状态并开启这些中断
* 允许被调用的位置：中断服务程序、任务层
* 参数：无
* 返回值：无
* 编写人  :  郁利吉
* 编写日期:  2005.10.14
* 备注：没有保存状态，尚未区分一二类中断
******************************************************************
*/
void ResumeOSInterrupts(void)
{
  OSSuspendCount--;
   if(0==OSSuspendCount)
      __asm cli;
}

/*
******************************************************************
* void SuspendAllInterrupts(void)
* 描述：保存所有中断的状态并关闭这些中断
* 允许被调用的位置：中断服务程序、alarm回调函数、任务层
* 参数：无
* 返回值：无
* 编写人  :  郁利吉
* 编写日期:  2005.10.14
* 备注：没有保存状态
******************************************************************
*/

void SuspendAllInterrupts(void){
 SuspendCount++;
   if(1==SuspendCount)
      __asm sei;
}

/*
******************************************************************
* void ResumeAllInterrupts(void)
* 描述：恢复由SuspendAllInterrupts保存的中断的状态并开启这些中断
* 允许被调用的位置：中断服务程序、alarm回调函数、任务层
* 参数：无
* 返回值：无
* 编写人  :  郁利吉
* 编写日期:  2005.10.14
* 备注：没有保存状态
******************************************************************
*/

void ResumeAllInterrupts(void){
 SuspendCount--;
    if(0==SuspendCount)
    {
      __asm cli;
    }
   return;
}

/*
******************************************************************
* void EnableAllInterrupts ( void )
* 描述：开启所有中断
* 允许被调用的位置：中断服务程序、任务层
* 参数：无
* 返回值：无
* 编写人  :  郁利吉
* 编写日期:  2005.10.14
******************************************************************
*/

void EnableAllInterrupts ( void ) {
   __asm{
     cli
   }
}

/*
******************************************************************
* void DisableAllInterrupts ( void )
* 描述：关闭所有中断
* 允许被调用的位置：中断服务程序、任务层
* 参数：无
* 返回值：无
* 编写人  :  郁利吉
* 编写日期:  2005.10.14
******************************************************************
*/
void DisableAllInterrupts ( void ) {
		__asm{
		  sei
		}
}



/*
************************************************************
* ECT溢出中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/  
def_interrupt(OS_ECT_C0_ISR){

  IntEnter();
     
  if(ISRVector[OSISR_ECT0] != 0){
    ISRVector[OSISR_ECT0]();
  }
  else
    TIE_C0I=0;
  
  IntExit(); 
}

			

/*
************************************************************
* ECT通道1中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_ECT_C1_ISR){

  IntEnter(); 

  if(ISRVector[OSISR_ECT1] != 0){
    ISRVector[OSISR_ECT1]();        
  }
  else
    TIE_C1I=0;
  
  IntExit(); 
  
}


/*
************************************************************
* ECT通道2中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_ECT_C2_ISR){
 
  IntEnter();
    
  if(ISRVector[OSISR_ECT2] != 0){   
    ISRVector[OSISR_ECT2]();       
  }
  else
    TIE_C2I=0;
  
  IntExit(); 
}


/*
************************************************************
* ECT通道3中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_ECT_C3_ISR){

  IntEnter();
  if(ISRVector[OSISR_ECT3] != 0){ 
     ISRVector[OSISR_ECT3]();   
  }
  else{
    TIE_C3I=0; 
  }
  IntExit(); 
}


/*
************************************************************
* ECT通道4中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_ECT_C4_ISR){

  IntEnter();    
  if(ISRVector[OSISR_ECT4] != 0){
    ISRVector[OSISR_ECT4](); 
  }
  else
    TIE_C4I=0;
  OSIntExit(); 
}						


/*
************************************************************
* ECT通道5中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_ECT_C5_ISR){
 
  IntEnter();
  
  if(ISRVector[OSISR_ECT5] != 0){ 
    ISRVector[OSISR_ECT5](); 
  }         
  else
    TIE_C5I=0;  
  
  IntExit(); 
}


/*
************************************************************
* ECT通道1中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_ECT_C6_ISR){

  IntEnter();
   
  if(ISRVector[OSISR_ECT6] != 0)  
    ISRVector[OSISR_ECT6]();           
  else
     TIE_C6I=0; 
  
  IntExit(); 
}


/*
************************************************************
* ECT通道1中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_ECT_OF_ISR){

  IntEnter();
  
  if(ISRVector[OSISR_ECTO] != 0){
    ISRVector[OSISR_ECTO]();           
  }
  else
     TIE_C7I=0;
  
  IntExit(); 
}

/*
************************************************************
* ECT通道1中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_ECT_C7_ISR){

  IntEnter();
  
  if(ISRVector[OSISR_ECT7] != 0){
    ISRVector[OSISR_ECT7]();           
  }
  else
     TIE_C7I=0;
  
  IntExit(); 
}

/*
************************************************************
*  Pulse accumulator A overflow 中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_PAOV_ISR) 
{
  IntEnter();
  
  if(ISRVector[OSISR_PAOV] != 0) { 
     ISRVector[OSISR_PAOV]();        
  }
  else
  {
     PACTL_PAOVI = 0;
  } 
  
	IntExit();
}

/*
************************************************************
* Pulse accumulator input edge中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序自动嵌套。
************************************************************
*/ 
def_interrupt(OS_PAI_ISR) 
{
  IntEnter();
  
  if(ISRVector[OSISR_PAI] != 0)  
     ISRVector[OSISR_PAI]();           
  else
  {
     PACTL_PAI = 0;
  } 
  
	IntExit();
}



/*
************************************************************
* MDCUF中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_MDCUF_ISR)
{
  IntEnter();
   
  if(ISRVector[OSISR_MDCUF] != 0)  
     ISRVector[OSISR_MDCUF]();           
  else
  {
     MCCTL_MCZI = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* PABOV中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_PABOV_ISR)
{
  IntEnter();
  
  if(ISRVector[OSISR_PABOV] != 0)  
     ISRVector[OSISR_PABOV]();           
  else
  {
     PBCTL_PBOVI = 0;
  }
  
  IntExit();                
}

/*
************************************************************
* PWM中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_PWMES_ISR)
{
  IntEnter();  
   
  if(ISRVector[OSISR_PWMES] != 0)  
     ISRVector[OSISR_PWMES]();           
  else
  {
     PWMSDN_PWMIE = 0;
  }
  
  IntExit();               
}





/*
************************************************************
* IRQ中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套。
************************************************************
*/ 
def_interrupt(OS_IRQ_ISR) 
{
  IntEnter();
   
  if(ISRVector[OSISR_IRQ] != 0)  
     ISRVector[OSISR_IRQ]();           
  else
  {
    INTCR_IRQEN = 0;
  } 
  
	IntExit();
}


/*
************************************************************
* AD0中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_AD0_ISR)
{
  IntEnter();  
    
  if(ISRVector[OSISR_ATD0] != 0)  
     ISRVector[OSISR_ATD0]();           
  else
  {
     ATD0DIEN_BIT = 0;
  }
  
  IntExit();                
}

/*
************************************************************
* SCI1中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_AD1_ISR)
{
  IntEnter();
  
  if(ISRVector[OSISR_ATD1] != 0)  
     ISRVector[OSISR_ATD1]();           
  else
  {
     ATD1DIEN_BIT = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* PTJ中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_PTJ_ISR)
{
  IntEnter();
  
  if(ISRVector[OSISR_PTJ] != 0)  
     ISRVector[OSISR_PTJ]();           
  else
  {
     PIEJ_PIEJ0 = 0;
     PIEJ_PIEJ1 = 0;
     PIEJ_PIEJ6 = 0;
     PIEJ_PIEJ7 = 0;
  }
  
  IntExit();                
}

/*
************************************************************
* PTH中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_PTH_ISR)
{
  IntEnter(); 
  
  if(ISRVector[OSISR_PTH] != 0)  
     ISRVector[OSISR_PTH]();           
  else
  {
     PIEH_PIEH = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* PTP中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_PTP_ISR)
{
  IntEnter();  
     
  if(ISRVector[OSISR_PTP] != 0)  
     ISRVector[OSISR_PTP]();           
  else
  {
     PIEP_PIEP = 0;
  }
  
  IntExit();              
}



/*
************************************************************
* SCI1中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CRGPL_ISR)
{
  IntEnter(); 
     
  if(ISRVector[OSISR_CRGPL] != 0)  
     ISRVector[OSISR_CRGPL]();           
  else
  {
     CRGINT_LOCKIE = 0;
  }
  
  IntExit();                
}

/*
************************************************************
* CRGSCM中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CRGSCM_ISR)
{

  IntEnter(); 
     
  if(ISRVector[OSISR_CRGSCM] != 0)  
     ISRVector[OSISR_CRGSCM]();           
  else
  {
     CRGINT_SCMIE = 0;
  }
  
  IntExit();                
}





/*
************************************************************
* EEPROM中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_EEPROM_ISR)
{
  IntEnter();  
   
  if(ISRVector[OSISR_EEPROM] != 0)  
     ISRVector[OSISR_EEPROM]();           
  else
  {
     ECNFG_CCIE = 0;
     ECNFG_CBEIE = 0;
  }
  IntExit(); 
                 
}


/*
************************************************************
* FLASH模块中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_FLASH_ISR)
{
  IntEnter();
  
  if(ISRVector[OSISR_FLASH] != 0)  
     ISRVector[OSISR_FLASH]();           
  else
  {
     FCNFG_CCIE = 0;
     FCNFG_CBEIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* IICB中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_IICB_ISR)
{
  IntEnter();
  
  if(ISRVector[OSISR_IICB] != 0)  
     ISRVector[OSISR_IICB]();           
  else
  {
     IBCR_IBIE = 0;
  }
  
  IntExit();                
}




/*
************************************************************
* SPI0中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套。
************************************************************
*/ 
def_interrupt(OS_SPI0_ISR) 
{
  IntEnter();
   
  if(ISRVector[OSISR_SPI0] != 0)  
     ISRVector[OSISR_SPI0]();           
  else
  {
     SPI0CR1_SPTIE = 0;
     SPI0CR1_SPIE = 0;
  } 
  
	IntExit();
}

/*
************************************************************
* SPI1中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_SPI1_ISR)
{
 
  IntEnter();
   
  if(ISRVector[OSISR_SPI1] != 0)  
     ISRVector[OSISR_SPI1]();           
  else
  {
     SPI1CR1_SPIE = 0;
     SPI1CR1_SPTIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* SCI0中断服务程序
*
* 作者：姜旭锋			
*
* 修改：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/
def_interrupt(OS_SCI0_ISR) 
{ 	
  IntEnter();
   
  if(ISRVector[OSISR_SCI0] != 0){ 
     ISRVector[OSISR_SCI0](); 
  }
  else
  {
    SCI0CR2_ILIE  = 0;
    SCI0CR2_RIE   = 0;
    SCI0CR2_TCIE  = 0;
    SCI0CR2_SCTIE = 0;			  
  }  
    
	IntExit();        
} 


/*
************************************************************
* SCI1中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_SCI1_ISR) 
{
  IntEnter(); 

  if(ISRVector[OSISR_SCI1] != 0)  
     ISRVector[OSISR_SCI1]();           
  else
  {
    SCI1CR2_ILIE  = 0;
    SCI1CR2_RIE   = 0;
    SCI1CR2_TCIE  = 0;
    SCI1CR2_SCTIE = 0;
  } 
  
	IntExit();
}





/*
************************************************************
* CAN0唤醒中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN0_WK_ISR)
{
  IntEnter();
  
  if(ISRVector[OSISR_CAN0WK] != 0)  
     ISRVector[OSISR_CAN0WK]();           
  else
  {
     CAN0RIER_WUPIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN0错误处理中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN0_ER_ISR)
{
  IntEnter();
       
  if(ISRVector[OSISR_CAN0ER] != 0)  
     ISRVector[OSISR_CAN0ER]();           
  else
  {
     CAN0RIER_OVRIE = 0;
     CAN0RIER_CSCIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN0接收数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN0_Rx_ISR)
{
  IntEnter(); 
   
  if(ISRVector[OSISR_CAN0RE] != 0)  
     ISRVector[OSISR_CAN0RE]();           
  else
  {
     CAN0RIER_RXFIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN0发送数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/                                  
def_interrupt(OS_CAN0_Tx_ISR)
{
  IntEnter();
    
  if(ISRVector[OSISR_CAN0TR] != 0)  
     ISRVector[OSISR_CAN0TR]();           
  else
  {
     CAN0TIER_TXEIE = 0;
  }
  
  IntExit();                
}



/*
************************************************************
* CAN1唤醒中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN1_WK_ISR)
{
  IntEnter();
   
  if(ISRVector[OSISR_CAN1WK] != 0)  
     ISRVector[OSISR_CAN1WK]();           
  else
  {
     CAN1RIER_WUPIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN1错误处理中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN1_ER_ISR)
{
 
  IntEnter();
  
  if(ISRVector[OSISR_CAN1ER] != 0)  
     ISRVector[OSISR_CAN1ER]();           
  else
  {
     CAN1RIER_OVRIE = 0;
     CAN1RIER_CSCIE = 0;
  }
  
  IntExit();              
}

/*
************************************************************
* CAN1接受数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 

def_interrupt(OS_CAN1_Rx_ISR)
{
  IntEnter();
    
  if(ISRVector[OSISR_CAN1RE] != 0)  
     ISRVector[OSISR_CAN1RE]();           
  else
  {
     CAN1RIER_RXFIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN1发送数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN1_Tx_ISR)
{
 
  IntEnter();
  
  if(ISRVector[OSISR_CAN1TR] != 0)  
     ISRVector[OSISR_CAN1TR]();           
  else
  {
     CAN1TIER_TXEIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN2唤醒中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN2_WK_ISR)
{
  IntEnter(); 
  
  if(ISRVector[OSISR_CAN2WK] != 0)  
     ISRVector[OSISR_CAN2WK]();           
  else
  {
     CAN2RIER_WUPIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN2错误处理中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN2_ER_ISR)
{
  IntEnter();  
   
  if(ISRVector[OSISR_CAN2ER] != 0)  
     ISRVector[OSISR_CAN2ER]();           
  else
  {
     CAN2RIER_OVRIE = 0;
     CAN2RIER_CSCIE = 0;
  }
  
  IntExit();              
}

/*
***********************************************************************
* 操作系统系统定时器中断服务程序
*
* 说明 :  该中断服务程序自动嵌套
*
* 作者 :  姜旭锋
*
* 修改：郁利吉  2005.10.14
* 修改：郁利吉  2005.11.2
***********************************************************************
*/
def_interrupt(OS_RTI_ISR)
{
  TaskStateType state;
  
  IntEnter(); 
  
  CRGFLG_RTIF = 1;  
  
  if(ISRVector[OSISR_RTI] != 0)  
     ISRVector[OSISR_RTI]();
  
  
  OSCounterTrigger(OSCounters);
  #ifndef OS_ALARM_EN
  //判断是否为二类中断
  if (OSISRCategory2Mask[OSISR_RTI/32]&(0x80000000>>(OSISR_RTI%32))) 
  {    
    IntExit();
  }
  else
  {
    OSIntNesting--;
    asm pula;
    asm staa   PPAGE;
  }
  #else
  IntExit();
  #endif /*ifndef OS_ALARM_EN*/                    
}

/*
************************************************************
* CAN2接受数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN2_Rx_ISR)
{
  IntEnter();
     
  if(ISRVector[OSISR_CAN2RE] != 0)  
     ISRVector[OSISR_CAN2RE]();           
  else
  {
     CAN2RIER_RXFIE = 0;
  }
  
  IntExit();                
}

/*
************************************************************
* CAN2发送数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN2_Tx_ISR)
{
  IntEnter();  
   
  if(ISRVector[OSISR_CAN2TR] != 0)  
     ISRVector[OSISR_CAN2TR]();           
  else
  {
     CAN2TIER_TXEIE = 0;
  }
  
  IntExit();                
}



/*
************************************************************
* CAN3唤醒中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN3_WK_ISR)
{
  IntEnter();
   
  if(ISRVector[OSISR_CAN3WK] != 0)  
     ISRVector[OSISR_CAN3WK]();           
  else
  {
     CAN3RIER_WUPIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN3错误处理中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN3_ER_ISR)
{
  IntEnter(); 
   
  if(ISRVector[OSISR_CAN3ER] != 0)  
     ISRVector[OSISR_CAN3ER]();           
  else
  {
     CAN3RIER_OVRIE = 0;
     CAN3RIER_CSCIE = 0;
  }
  IntExit();              
}

/*
************************************************************
* CAN3接受数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN3_Rx_ISR)
{
  IntEnter();
     
  if(ISRVector[OSISR_CAN3RE] != 0)  
     ISRVector[OSISR_CAN3RE]();           
  else
  {
     CAN3RIER_RXFIE = 0;
  }
  
  IntExit();                
}

/*
************************************************************
* CAN3发送数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN3_Tx_ISR)
{
 
  IntEnter();  
   
  if(ISRVector[OSISR_CAN3TR] != 0)  
     ISRVector[OSISR_CAN3TR]();           
  else
  {
     CAN3TIER_TXEIE = 0;
  }
  
  IntExit();                
}



/*
************************************************************
* CAN4唤醒中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN4_WK_ISR)
{
  IntEnter();
   
  if(ISRVector[OSISR_CAN4WK] != 0)  
     ISRVector[OSISR_CAN4WK]();           
  else
  {
     CAN4RIER_WUPIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN4错误处理中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN4_ER_ISR)
{
 
  IntEnter(); 
   
  if(ISRVector[OSISR_CAN4ER] != 0)  
     ISRVector[OSISR_CAN4ER]();           
  else
  {
     CAN4RIER_OVRIE = 0;
     CAN4RIER_CSCIE = 0;
  }
  
  IntExit();              
}

/*
************************************************************
* CAN4接受数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN4_Rx_ISR)
{
 
  IntEnter();  
   
  if(ISRVector[OSISR_CAN4RE] != 0)  
     ISRVector[OSISR_CAN4RE]();           
  else
  {
     CAN4RIER_RXFIE = 0;
  }
  
  IntExit();                
}


/*
************************************************************
* CAN4发送数据中断服务程序
*			
* 作者：吕攀		    2005.5.19
*
* 修改：姜旭锋			2005.5.20
* 
* 说明：该中断服务程序不自动嵌套，用户自行判断发生中断的状态
************************************************************
*/ 
def_interrupt(OS_CAN4_Tx_ISR)
{
 
  IntEnter(); 
   
  if(ISRVector[OSISR_CAN4TR] != 0)  
     ISRVector[OSISR_CAN4TR]();           
  else
  {
     CAN4TIER_TXEIE = 0;
  }
  
  IntExit();                
}


