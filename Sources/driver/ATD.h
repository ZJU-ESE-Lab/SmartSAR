/////////////////////////////////////////////////////////////
//
//		         MC9S12DP256B ATD MODULE HEADERFILE
//
//                       ZJU.ESE
//
//						 AUTHOR  JIANG.XUFENG
//						                                      2004.10.4
//
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//
//	 channle define 
//
//  ADC module has 8 channles
//
/////////////////////////////////////////////////////////////
typedef INT16U ChannleID;

#define Channle0  0x01
#define Channle1  0x02
#define Channle2  0x04
#define Channle3  0x08
#define Channle4  0x10
#define Channle5  0x20
#define Channle6  0x40
#define Channle7  0x80

void ATD_Init(void);
void ATD_Open(ChannleID channleid);
void ATD_Read(ChannleID channleid);
void ATD_Close(ChannleID channleid);

