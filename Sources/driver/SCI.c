/*
*********************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�sci.c
* ժ    Ҫ������SCIģ�����������
*
* ��ǰ�汾��1.0
* ��    �ߣ������
* ������ڣ�2005��2��20��
*********************************************************************************
*/
#ifndef _HCS12_SCI_C_
#define _HCS12_SCI_C_

#include "os_cpu.h"
#include "io_map.h"
#include "sci.h"


/*
*****************************************************************
*	 SCI0Init
*  ��ʼ������0
*  ���ò�����Ϊ9600
*
*****************************************************************
*/
void SCI0Init(void) {

  SCI0BDL = (volatile unsigned char)((16000000UL /* OSC freq */ / 2) / 9600 /* baud rate */ / 16 /*factor*/); 
  SCI0CR2 = 0x2C;	
  SCI0CR2_RIE =1;
  SCI0CR2_RE =1;
}


INT8U OSEKGetChar(){
 if (SCI0SR1_RDRF){
   SCI0SR1_RDRF=1;  
   return SCI0DRL;
 }
}

/*
*****************************************************************
* OSEKPrintChar
*****************************************************************
*/
void OSEKPrintChar(INT8U ch){
    while (!(SCI0SR1 & 0x80));  /* wait for output buffer empty */
    SCI0DRL = ch;
}


/*
*****************************************************************
*	 OSEKPrintString
*
*****************************************************************
*/
void OSEKPrintString(const char *text){
  while (*text != '\0'){
    while (!(SCI0SR1 & 0x80));  /* wait for output buffer empty */
        SCI0DRL = *text++;
  }
}

/*
*****************************************************************
*	 OSEKPrintInt
*
*****************************************************************
*/
void OSEKPrintInt(INT16U digit)
{
	INT8U out[10];
	int i,len,j;
	
	for(i=0;i<9;i++)
	    out[i]=0;
	
	i=0;
	len=0;
	
	if(digit==0)
	{
		OSEKPrintChar('0');
		
		return;
	}
		
	while(digit > 0)
	{
		
		j = digit%10;
		digit = digit/10;
		out[i++]= j;
		len++;
	}
	
	while((--len)>=0)
	{
		OSEKPrintChar((char)('0'+out[len]));
	}
	OSEKPrintString("\n\r");
	return;
}

#endif




