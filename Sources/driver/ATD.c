///////////////////////////////////////////////////////////
//ATC module Driver Function
//	    
//
//
//
//Author XF   2004.9.30
/////////////////////////////////////////////////////////////
volatile ATD0CTL23STR _ATD0CTL23 @(REG_BASE + 0x00000082);
volatile ATD0CTL45STR _ATD0CTL45 @(REG_BASE + 0x00000084);

//Status Register
volatile ATD0STAT0STR _ATD0STAT0 @(REG_BASE + 0x00000086);
volatile ATD0STAT1STR _ATD0STAT1 @(REG_BASE + 0x0000008B);

//Interrupt Regiter
volatile ATD0DIENSTR _ATD0DIEN @(REG_BASE + 0x0000008D);

volatile PORTAD0STR _PORTAD0 @(REG_BASE + 0x0000008F);

//DATA REGISTER
volatile ATD0DR0STR _ATD0DR0 @(REG_BASE + 0x00000090);
volatile ATD0DR1STR _ATD0DR1 @(REG_BASE + 0x00000092);
volatile ATD0DR2STR _ATD0DR2 @(REG_BASE + 0x00000094);
volatile ATD0DR3STR _ATD0DR3 @(REG_BASE + 0x00000096);
volatile ATD0DR4STR _ATD0DR4 @(REG_BASE + 0x00000098);
volatile ATD0DR5STR _ATD0DR5 @(REG_BASE + 0x0000009A);
volatile ATD0DR6STR _ATD0DR6 @(REG_BASE + 0x0000009C);
volatile ATD0DR7STR _ATD0DR7 @(REG_BASE + 0x0000009E);

void ATD_Init(void){


}

void ATD_Open(ChannleID channleid){


}

void ATD_Read(ChannleID channleid){


}

void ATD_Close(ChannleID channleid){


}