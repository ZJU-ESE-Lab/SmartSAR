/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_irq.c
* ժ    Ҫ����������ϵͳ�жϴ���ģ��
*
* ��    ��: �����
*********************************************************************************
*/
#ifndef OS_GLOBALS
#include "includes.h"
#endif

#include "app_cfg.h"

#pragma CODE_SEG __NEAR_SEG NON_BANKED       /* Interrupt section for this module. Placement will be in NON_BANKED area. */


ISR_FP   ISRVector[ISR_TBL_SIZE];            /* �û��ж������� */
//OSSTK    SystemSTK[512];
//OSSTKRef SystemSTKHead;

static INT8U SuspendCount = 0; /*��¼Suspend/ResumeAllInterupstǶ�ײ���*/
static INT8U OSSuspendCount = 0;/*��¼Suspend/ResumeOSInterupstǶ�ײ���*/

INT32U OSISRCategory2Mask[2] = {0,0}; /*�����ж�����*/

#ifdef OS_ISR_AUTO_DEFINE
extern OSISRCfg OSISRCfgTbl[OS_NUM_ISR];

/*
***************************************************************************
*void OSISRInit(void)
* ��������ʼ��ע���жϷ������
* ���õĺ�������
* ���������õ�λ�ã�StartOS
* ������	��
* ���أ�	��
* ע�������
* �����ˣ�������
* ʱ�䣺2005.10.12
* �޸��ˣ�������
* ʱ�䣺2005.10.19 
* ��ע��
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
		ISRVector[source]=cfg->handle;  /* ע���жϺ���   */
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
* ������ע��һ���û��жϷ������
* ���������õ�λ�ã��û������жϷ������ʱ����
* ������isr_code: �жϺ�
*       fp:       �жϴ���������ָ��
*       category���ж�����
*                 ȡֵ��
*                 OSISRCategory1��һ���ж�
*                 OSISRCategory2�������ж�
*       opt��     ������ʽ
*                 ȡֵ��
*                 OSIRQ_NULL,
*                 OSIRQ_LEVEL,
*                 OSIRQ_EDGE
*
* ����ֵ  OS_INT_ERR   : ��ʾע��ʧ��,isr_codeԽ��
*         OS_INT_NOERR : ��ʾok
*
* ��д��  :  ���� 
* ��д����:  2005.05.18
* �޸��ˣ�������
* ʱ�䣺2005.10.19
* ��ע��
******************************************************************
*/
INT8U RegisterISR(INT8U isr_code,ISR_FP fp,INT8U category,INT8U opt) 
{
    if (isr_code >= ISR_TBL_SIZE || isr_code == UNIMPISR ){               /*   �жϺŷǷ�     */
       return OS_INT_ERR;
    }
               
	  ISRVector[isr_code]=fp;                      /*   ע���жϺ���   */
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
* �������������ж����жϵ�״̬���ر���Щ�ж�
* ���������õ�λ�ã��жϷ�����������
* ��������
* ����ֵ����
* ��д��  :  ������
* ��д����:  2005.10.14
* ��ע��û�б���״̬����δ����һ�����ж�
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
* �������ָ���SuspendOSInterrupts����Ķ����жϵ�״̬��������Щ�ж�
* ���������õ�λ�ã��жϷ�����������
* ��������
* ����ֵ����
* ��д��  :  ������
* ��д����:  2005.10.14
* ��ע��û�б���״̬����δ����һ�����ж�
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
* ���������������жϵ�״̬���ر���Щ�ж�
* ���������õ�λ�ã��жϷ������alarm�ص������������
* ��������
* ����ֵ����
* ��д��  :  ������
* ��д����:  2005.10.14
* ��ע��û�б���״̬
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
* �������ָ���SuspendAllInterrupts������жϵ�״̬��������Щ�ж�
* ���������õ�λ�ã��жϷ������alarm�ص������������
* ��������
* ����ֵ����
* ��д��  :  ������
* ��д����:  2005.10.14
* ��ע��û�б���״̬
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
* ���������������ж�
* ���������õ�λ�ã��жϷ�����������
* ��������
* ����ֵ����
* ��д��  :  ������
* ��д����:  2005.10.14
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
* �������ر������ж�
* ���������õ�λ�ã��жϷ�����������
* ��������
* ����ֵ����
* ��д��  :  ������
* ��д����:  2005.10.14
******************************************************************
*/
void DisableAllInterrupts ( void ) {
		__asm{
		  sei
		}
}



/*
************************************************************
* ECT����жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* ECTͨ��1�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* ECTͨ��2�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* ECTͨ��3�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* ECTͨ��4�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* ECTͨ��5�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* ECTͨ��1�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* ECTͨ��1�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* ECTͨ��1�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
*  Pulse accumulator A overflow �жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* Pulse accumulator input edge�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* MDCUF�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* PABOV�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* PWM�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* IRQ�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* AD0�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* SCI1�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* PTJ�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* PTH�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* PTP�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* SCI1�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CRGSCM�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* EEPROM�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* FLASHģ���жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* IICB�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* SPI0�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ס�
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
* SPI1�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* SCI0�жϷ������
*
* ���ߣ������			
*
* �޸ģ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* SCI1�жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN0�����жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN0�������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN0���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN0���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN1�����жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN1�������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN1���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN1���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN2�����жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN2�������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* ����ϵͳϵͳ��ʱ���жϷ������
*
* ˵�� :  ���жϷ�������Զ�Ƕ��
*
* ���� :  �����
*
* �޸ģ�������  2005.10.14
* �޸ģ�������  2005.11.2
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
  //�ж��Ƿ�Ϊ�����ж�
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
* CAN2���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN2���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN3�����жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN3�������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN3���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN3���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN4�����жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN4�������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN4���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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
* CAN4���������жϷ������
*			
* ���ߣ�����		    2005.5.19
*
* �޸ģ������			2005.5.20
* 
* ˵�������жϷ�������Զ�Ƕ�ף��û������жϷ����жϵ�״̬
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

