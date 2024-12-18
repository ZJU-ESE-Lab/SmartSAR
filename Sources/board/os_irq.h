/*
******************************************************************
*
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名    : os_irq.h
*
*
* 摘要      : 中断机制是因平台而互异的,本中断机制是根据HCS12
*             硬件平台而编写的,主要管理HCS12的中断.在HCS12中
*             共有64个中断，并根据中断向量表的先后顺序确定中断
*             优先级。中断的嵌套只有在中断服务程序中用户开放中断
*             才会发生。
*             
*
* 当前版本：1.0
* 作    者：吕攀
* 完成日期：2005年5月xx日
* 
******************************************************************
*/
#ifndef _EXCEPTION_H
#define _EXCEPTION_H


#ifndef  NULL
#define  NULL                 0x0
#endif

#ifndef  TRUE
#define  TRUE                 0x1
#endif

#ifndef  FALSE
#define  FALSE                0x0
#endif

#define ISR(FuncName) void FuncName(void) /*ISR标准命名*/
#define ISR_TBL_SIZE   64
   

#define IntEnter()\
        asm  ldaa   PPAGE;\
        asm  psha;\
        OSIntEnter();
        
#define IntExit()\
        OSIntExit();\
        asm pula;\
        asm staa   PPAGE;


#define DIS() asm sei

/*
******************************************************************
*
*  中断服务程序编号定义
*
******************************************************************
*/ 

#define UNIMPISR      -1  /*未实现中断*/
#define OSISR_RESET    0
#define OSISR_CMF      UNIMPISR
#define OSISR_COPF     UNIMPISR
#define OSISR_UIT      UNIMPISR
#define OSISR_SWI      4
#define OSISR_XIRQ     UNIMPISR
#define OSISR_IRQ      6
#define OSISR_RTI      7
#define OSISR_ECT0     8
#define OSISR_ECT1     9
#define OSISR_ECT2     10
#define OSISR_ECT3     11
#define OSISR_ECT4     12
#define OSISR_ECT5     13
#define OSISR_ECT6     14
#define OSISR_ECT7     15

#define OSISR_ECTO     16
#define OSISR_PAOV     17
#define OSISR_PAI      18
#define OSISR_SPI0     19
#define OSISR_SCI0     20
#define OSISR_SCI1     21
#define OSISR_ATD0     22
#define OSISR_ATD1     23
#define OSISR_PTJ      24
#define OSISR_PTH      25
#define OSISR_MDCUF    26
#define OSISR_PABOV    27
#define OSISR_CRGPL    28
#define OSISR_CRGSCM   29
#define OSISR_BDLC     UNIMPISR			
#define OSISR_IICB     31

#define OSISR_SPI1     32
#define OSISR_SPI2     UNIMPISR	 
#define OSISR_EEPROM   34
#define OSISR_FLASH    35
#define OSISR_CAN0WK   36
#define OSISR_CAN0ER   37
#define OSISR_CAN0RE   38
#define OSISR_CAN0TR   39
#define OSISR_CAN1WK   UNIMPISR 
#define OSISR_CAN1ER   UNIMPISR 
#define OSISR_CAN1RE   UNIMPISR	 
#define OSISR_CAN1TR   UNIMPISR	 
#define OSISR_CAN2WK   44
#define OSISR_CAN2ER   45
#define OSISR_CAN2RE   46
#define OSISR_CAN2TR   47

#define OSISR_CAN3WK   UNIMPISR	 
#define OSISR_CAN3ER   UNIMPISR		
#define OSISR_CAN3RE   UNIMPISR		
#define OSISR_CAN3TR   UNIMPISR	 
#define OSISR_CAN4WK   UNIMPISR	 
#define OSISR_CAN4ER   UNIMPISR	 
#define OSISR_CAN4RE   UNIMPISR	
#define OSISR_CAN4TR   UNIMPISR	 
#define OSISR_PTP      56
#define OSISR_PWMES    57
#define OSISR_RSER0    UNIMPISR
#define OSISR_RSER1    UNIMPISR
#define OSISR_RSER2    UNIMPISR
#define OSISR_RSER3    UNIMPISR
#define OSISR_RSER4    UNIMPISR
#define OSISR_RSER5    UNIMPISR

/*
******************************************************************
*
*  中断模块错误码定义
*
******************************************************************
*/
#define  OS_INT_NOERR      0
#define  OS_INT_ERR        0xFF



/*
******************************************************************
*
*    系统中断向量表
*
******************************************************************
*/
typedef void(*ISR_FP)(void);

typedef enum
{
    OSISRCategory1=0,
    OSISRCategory2		
}CateType;/*Category类型*/

typedef INT8U SourceType;/*中断源类型*/

typedef enum
{
	OSIRQ_NULL,OSIRQ_LEVEL,OSIRQ_EDGE	
}OptionType;/*外部中断的触发选项定义*/

typedef struct
{
	ISR_FP handle;
	SourceType source;
	CateType category;
	OptionType option;	
}OSISRCfg;

#ifdef OS_ISR_AUTO_DEFINE
void OSISRInit(void);
#else/*#ifdef OS_ISR_AUTO_DEFINE*/
#define OSISRInit()
#endif/*#ifdef OS_ISR_AUTO_DEFINE*/

/*
******************************************************************
*
*
*    中断模块API函数
*
*
******************************************************************
*/

                
INT8U RegisterISR(INT8U isr_code,ISR_FP fp,INT8U category,INT8U opt);   /* 注册一个中断处理函数*/


/*
******************************************************************
*
*  OSEK标准API
*
*
******************************************************************
*/
void EnableAllInterrupts ( void );
void DisableAllInterrupts ( void );
void ResumeAllInterrupts(void);
void SuspendAllInterrupts(void);
void SuspendOSInterrupts(void);
void ResumeOSInterrupts(void);
//void OSISRInit(void);


#define ref_interrupt(NAME)\
      	extern  __interrupt  void NAME(void) 
    		 
#define def_interrupt(NAME)\
         __interrupt  void NAME(void)		 



ref_interrupt(OS_PWMES_ISR);
ref_interrupt(OS_PTP_ISR);                          /* Port P    */   
ref_interrupt(OS_CAN0_Tx_ISR);                      /* MSCAN4 transmit  */
ref_interrupt(OS_CAN3_Rx_ISR);                      /* MSCAN4 receive   */
ref_interrupt(OS_CAN3_ER_ISR);                      /* MSCAN4 errors    */
ref_interrupt(OS_CAN3_WK_ISR);                      /* MSCAN4 wake up   */
ref_interrupt(OS_CAN3_Tx_ISR);                      /* MSCAN3 transmit  */
ref_interrupt(OS_CAN3_Rx_ISR);                      /* MSCAN3 receive ,系统预定义   */
ref_interrupt(OS_CAN3_ER_ISR);                      /* MSCAN3 errors    */
ref_interrupt(OS_CAN3_WK_ISR);                      /* MSCAN3 wake up   */
ref_interrupt(OS_CAN2_Tx_ISR);                      /* MSCAN2 transmit  */
ref_interrupt(OS_CAN2_Rx_ISR);                      /* MSCAN2 receive ,系统预定义   */
ref_interrupt(OS_CAN2_ER_ISR);                      /* MSCAN2 errors    */
ref_interrupt(OS_CAN2_WK_ISR);                      /* MSCAN2 wake up   */
ref_interrupt(OS_CAN1_Tx_ISR);                      /* MSCAN1 transmit  */
ref_interrupt(OS_CAN1_Rx_ISR);                      /* MSCAN1 receive   */
ref_interrupt(OS_CAN1_ER_ISR);                      /* MSCAN1 errors    */
ref_interrupt(OS_CAN1_WK_ISR);                      /* MSCAN1 wake up   */
ref_interrupt(OS_CAN0_Tx_ISR);                      /* MSCAN0 transmit  */
ref_interrupt(OS_CAN0_Rx_ISR);                      /* MSCAN0 receive   */
ref_interrupt(OS_CAN0_ER_ISR);                      /* MSCAN0 errors    */
ref_interrupt(OS_CAN0_WK_ISR);                      /* MSCAN0 wake up   */
ref_interrupt(OS_FLASH_ISR);                        /* FLASH     */
ref_interrupt(OS_EEPROM_ISR);                       /* EEPROM    */
ref_interrupt(OS_SPI1_ISR);                         /* SPI1      */
ref_interrupt(OS_IICB_ISR);                         /* IIC bus   */
ref_interrupt(OS_CRGSCM_ISR);                       /* SCME      */
ref_interrupt(OS_CRGPL_ISR);                        /* CRG lock  */
ref_interrupt(OS_PABOV_ISR);                        /* Pulse accumulator B overflow */
ref_interrupt(OS_MDCUF_ISR);                        /* Modulus Down Counter underflow */
ref_interrupt(OS_PTH_ISR);                          /* PORTH     */
ref_interrupt(OS_PTJ_ISR);                          /* PORTJ     */
ref_interrupt(OS_AD1_ISR);                          /* ATD1      */
ref_interrupt(OS_AD0_ISR);                          /* ATD0      */
ref_interrupt(OS_SCI1_ISR);                         /* SCI1      */
ref_interrupt(OS_SCI0_ISR);                         /* SCI0      */
ref_interrupt(OS_SPI0_ISR);                         /* SPI0      */
ref_interrupt(OS_PAI_ISR);                          /* Pulse accumulator input edge */
ref_interrupt(OS_PAOV_ISR);                         /* Pulse accumulator A overflow */
ref_interrupt(OS_ECT_OF_ISR);                       /* ECT Overflow  */
ref_interrupt(OS_ECT_C7_ISR);                       /* ECT Channel 7 */
ref_interrupt(OS_ECT_C6_ISR);                       /* ECT Channel 6 */
ref_interrupt(OS_ECT_C5_ISR);                       /* ECT Channel 5 */
ref_interrupt(OS_ECT_C4_ISR);                       /* ECT Channel 4 */
ref_interrupt(OS_ECT_C3_ISR);                       /* ECT Channel 3 */
ref_interrupt(OS_ECT_C2_ISR);                       /* ECT Channel 2 */
ref_interrupt(OS_ECT_C1_ISR);                       /* ECT Channel 1 */
ref_interrupt(OS_ECT_C0_ISR);                       /* ECT Channel 0 */
ref_interrupt(OS_RTI_ISR);                          /* Real time interrupt */



#endif
