/*
*********************************************************************************
* Copyright (c) 2005,浙江大学嵌入式系统研究开发中心
* All rights reserved.
*
* 文件名称：isr_vecotors.c
* 摘    要：中断向量表定义,请勿随意修改
*
* 作    者: 姜旭锋
*********************************************************************************
*/

#ifndef OS_GLOBALS
#include "includes.h"
#endif

extern void near _Startup(void);       /* Startup routine */
extern void near OSTaskSwitch(void);        /* Context Switch routine */


extern  __interrupt  void OSTickISR(void);

#define         CAN0_Tx_ISR   UnimplementedISR
#define         PWMES_ISR     UnimplementedISR                      /* Pwm emergency shutdown  */
#define         PTP_ISR       UnimplementedISR                      /* Port P    */   
           
#define         CAN0_Tx_ISR   UnimplementedISR                      /* MSCAN4 transmit  */
#define         CAN3_Rx_ISR   UnimplementedISR                      /* MSCAN4 receive   */
#define         CAN3_ER_ISR   UnimplementedISR                      /* MSCAN4 errors    */
#define         CAN3_WK_ISR   UnimplementedISR                      /* MSCAN4 wake up   */
        
#define         CAN3_Tx_ISR   UnimplementedISR                      /* MSCAN3 transmit  */
#define         CAN3_Rx_ISR   UnimplementedISR                      /* MSCAN3 receive ,系统预定义   */
#define         CAN3_ER_ISR   UnimplementedISR                      /* MSCAN3 errors    */
#define         CAN3_WK_ISR   UnimplementedISR                      /* MSCAN3 wake up   */
        
#define         CAN2_Tx_ISR   UnimplementedISR                      /* MSCAN2 transmit  */
#define         CAN2_Rx_ISR   UnimplementedISR                      /* MSCAN2 receive ,系统预定义   */
#define         CAN2_ER_ISR   UnimplementedISR                      /* MSCAN2 errors    */
#define         CAN2_WK_ISR   UnimplementedISR                      /* MSCAN2 wake up   */
        
#define         CAN1_Tx_ISR   UnimplementedISR                      /* MSCAN1 transmit  */
#define         CAN1_Rx_ISR   UnimplementedISR                      /* MSCAN1 receive   */
#define         CAN1_ER_ISR   UnimplementedISR                      /* MSCAN1 errors    */
#define         CAN1_WK_ISR   UnimplementedISR                      /* MSCAN1 wake up   */
        
#define         CAN0_Tx_ISR   UnimplementedISR                      /* MSCAN0 transmit  */
#define         CAN0_Rx_ISR   UnimplementedISR                      /* MSCAN0 receive   */
#define         CAN0_ER_ISR   UnimplementedISR                      /* MSCAN0 errors    */
#define         CAN0_WK_ISR   UnimplementedISR                      /* MSCAN0 wake up   */
        
#define         FLASH_ISR     UnimplementedISR                      /* FLASH     */
#define         EEPROM_ISR    UnimplementedISR                      /* EEPROM    */
        
#define         SPI1_ISR      UnimplementedISR                      /* SPI1      */
#define         IICB_ISR      UnimplementedISR                      /* IIC bus   */
#define         CRGSCM_ISR    UnimplementedISR                      /* SCME      */
#define         CRGPL_ISR     UnimplementedISR                      /* CRG lock  */
#define         PABOV_ISR     UnimplementedISR                      /* Pulse accumulator B overflow */
#define         MDCUF_ISR     UnimplementedISR                      /* Modulus Down Counter underflow */
#define         PTH_ISR       UnimplementedISR                      /* PORTH     */
#define         PTJ_ISR       UnimplementedISR                      /* PORTJ     */
#define         AD1_ISR       UnimplementedISR                      /* ATD1      */
#define         AD0_ISR       UnimplementedISR                      /* ATD0      */
#define         SCI1_ISR      UnimplementedISR                      /* SCI1      */
#define         SCI0_ISR      UnimplementedISR                      /* SCI0      */
#define         SPI0_ISR      UnimplementedISR                      /* SPI0      */
#define         PAI_ISR       UnimplementedISR                      /* Pulse accumulator input edge */
#define         PAOV_ISR      UnimplementedISR                      /* Pulse accumulator A overflow */
#define         ECT_OF_ISR    UnimplementedISR                      /* ECT Overflow  */
#define         ECT_C7_ISR    UnimplementedISR                      /* ECT Channel 7 */
#define         ECT_C6_ISR    UnimplementedISR                      /* ECT Channel 6 */
#define         ECT_C5_ISR    UnimplementedISR                      /* ECT Channel 5 */
#define         ECT_C4_ISR    UnimplementedISR                      /* ECT Channel 4 */
#define         ECT_C3_ISR    UnimplementedISR                      /* ECT Channel 3 */
#define         ECT_C2_ISR    UnimplementedISR                      /* ECT Channel 2 */
#define         ECT_C1_ISR    UnimplementedISR                      /* ECT Channel 1 */
#define         ECT_C0_ISR    UnimplementedISR                      /* ECT Channel 0 */
#define         RTI_ISR       OS_RTI_ISR//OSTickISR//                            /* Real time interrupt */
#define         IRQ_ISR       UnimplementedISR                      /* IRQ */



#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void UnimplementedISR(void)
{
   asm BGND;
}

typedef void (*near tIsrFunc)(void);
const tIsrFunc _vect[] @0xFF80 = {        /* Interrupt table */
        UnimplementedISR,                 /* vector 63 */
        UnimplementedISR,                 /* vector 62 */
        UnimplementedISR,                 /* vector 61 */
        UnimplementedISR,                 /* vector 60 */
        UnimplementedISR,                 /* vector 59 */
        UnimplementedISR,                 /* vector 58 */
        PWMES_ISR,                        /* Pwm emergency shutdown  */
        PTP_ISR,                          /* Port P    */   
           
        CAN0_Tx_ISR,                      /* MSCAN4 transmit  */
        CAN3_Rx_ISR,                      /* MSCAN4 receive   */
        CAN3_ER_ISR,                      /* MSCAN4 errors    */
        CAN3_WK_ISR,                      /* MSCAN4 wake up   */
        
        CAN3_Tx_ISR,                      /* MSCAN3 transmit  */
        CAN3_Rx_ISR,                      /* MSCAN3 receive ,系统预定义   */
        CAN3_ER_ISR,                      /* MSCAN3 errors    */
        CAN3_WK_ISR,                      /* MSCAN3 wake up   */
        
        CAN2_Tx_ISR,                      /* MSCAN2 transmit  */
        CAN2_Rx_ISR,                      /* MSCAN2 receive ,系统预定义   */
        CAN2_ER_ISR,                      /* MSCAN2 errors    */
        CAN2_WK_ISR,                      /* MSCAN2 wake up   */
        
        CAN1_Tx_ISR,                      /* MSCAN1 transmit  */
        CAN1_Rx_ISR,                      /* MSCAN1 receive   */
        CAN1_ER_ISR,                      /* MSCAN1 errors    */
        CAN1_WK_ISR,                      /* MSCAN1 wake up   */
        
        CAN0_Tx_ISR,                      /* MSCAN0 transmit  */
        CAN0_Rx_ISR,                      /* MSCAN0 receive   */
        CAN0_ER_ISR,                      /* MSCAN0 errors    */
        CAN0_WK_ISR,                      /* MSCAN0 wake up   */
        
        FLASH_ISR,                        /* FLASH     */
        EEPROM_ISR,                       /* EEPROM    */
        
        UnimplementedISR,                 /* SPI2      */
        SPI1_ISR,                         /* SPI1      */
        IICB_ISR,                         /* IIC bus   */
        UnimplementedISR,                 /* DLC       */
        CRGSCM_ISR,                       /* SCME      */
        CRGPL_ISR,                        /* CRG lock  */
        PABOV_ISR,                        /* Pulse accumulator B overflow */
        MDCUF_ISR,                        /* Modulus Down Counter underflow */
        PTH_ISR,                          /* PORTH     */
        PTJ_ISR,                          /* PORTJ     */
        AD1_ISR,                          /* ATD1      */
        AD0_ISR,                          /* ATD0      */
        SCI1_ISR,                         /* SCI1      */
        SCI0_ISR,                         /* SCI0      */
        SPI0_ISR,                         /* SPI0      */
        PAI_ISR,                          /* Pulse accumulator input edge */
        PAOV_ISR,                         /* Pulse accumulator A overflow */
        ECT_OF_ISR,                       /* ECT Overflow  */
        ECT_C7_ISR,                       /* ECT Channel 7 */
        ECT_C6_ISR,                       /* ECT Channel 6 */
        ECT_C5_ISR,                       /* ECT Channel 5 */
        ECT_C4_ISR,                       /* ECT Channel 4 */
        ECT_C3_ISR,                       /* ECT Channel 3 */
        ECT_C2_ISR,                       /* ECT Channel 2 */
        ECT_C1_ISR,                       /* ECT Channel 1 */
        ECT_C0_ISR,                       /* ECT Channel 0 */
        RTI_ISR,                          /* Real time interrupt */
        IRQ_ISR,                          /* IRQ */
        UnimplementedISR,                 /* XIRQ */
        OSTaskSwitch,                     /* SWI */
        _Startup,                         /* Unimplemented instruction trap */
        _Startup,                         /* COP failure reset */
        _Startup,                         /* Clock monitor fail reset */
        _Startup                          /* Reset vector */
   };