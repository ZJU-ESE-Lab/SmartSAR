/*
***************************************************************************
* Copyright (c) 2005,�㽭��ѧǶ��ʽϵͳ�о���������
* All rights reserved.
*
* �ļ����ƣ�os_cpu.h
* ժ    Ҫ��HCS12�ײ����ݽṹ����Ͳ����궨��
*
* ��ǰ�汾��1.1
* ��    �ߣ�����
* ������ڣ�2005��8��1��
*
* ȡ���汾��1.0
* ԭ����  �������
* ������ڣ�2004��12��4��
***************************************************************************
*/
#ifndef  OS_CPU_H
#define  OS_CPU_H

/*
*********************************************************************************************************
*                                              ��������
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    
typedef signed   char  INT8S;                    
typedef unsigned int   INT16U;                   
typedef signed   int   INT16S;                  
typedef unsigned long  INT32U;                  
typedef signed   long  INT32S;                  
typedef float          FP32;                    
typedef double         FP64;                   
typedef unsigned long long INT64U;              

typedef INT16U         AddrType;                /*��ַ����*/
typedef unsigned char  OSSTK;                   /*ջָ����1�ֽڵ�����ݼ���*/
typedef OSSTK * OSSTKRef;
typedef unsigned char  OS_CPU_SR;                
/*
*********************************************************************************************************
*                                              ����
*********************************************************************************************************
*/

#ifndef  FALSE
#define  FALSE    0
#endif

#ifndef  TRUE
#define  TRUE     1
#endif


#define  DEFINECRITICAL(cpu_sr)    OS_CPU_SR  cpu_sr=0; OS_CPU_SR reg_a
/*�������״̬�Ĵ����������жϣ����ٽ���*/
#define  OS_ENTER_CRITICAL()   __asm staa reg_a; __asm  tpa; __asm staa cpu_sr; __asm sei; __asm ldaa reg_a;                         
/*�ָ�����״̬�Ĵ������ָ��жϣ����ٽ���*/
#define  OS_EXIT_CRITICAL()    __asm staa reg_a; __asm  ldaa cpu_sr;  __asm tap;__asm ldaa reg_a;

#define  OS_TASK_SW() __asm swi;

#define  OS_STK_GROWTH        1                  /* ջ���������ɸߵ���*/

#endif
