#ifndef _HCS12_CRG_H_
#define _HCS12_CRG_H_



#define CRG_PRESCALERATE_OFF   0x00

#define CRG_PRESCALERATE_2E10  0x10

#define CRG_PRESCALERATE_2E11  0x20

#define CRG_PRESCALERATE_2E12  0x30

#define CRG_PRESCALERATE_2E13  0x40

#define CRG_PRESCALERATE_2E14  0x50

#define CRG_PRESCALERATE_2E15  0x60

#define CRG_PRESCALERATE_2E16  0x70



#define CRG_SCALE_1            0x00

#define CRG_SCALE_2            0x01

#define CRG_SCALE_3            0x02

#define CRG_SCALE_4            0x03

#define CRG_SCALE_5            0x04

#define CRG_SCALE_6            0x05

#define CRG_SCALE_7            0x06

#define CRG_SCALE_8            0x07

#define CRG_SCALE_9            0x08

#define CRG_SCALE_10           0x09

#define CRG_SCALE_11           0x0A

#define CRG_SCALE_12           0x0B

#define CRG_SCALE_13           0x0C

#define CRG_SCALE_14           0x0D

#define CRG_SCALE_15           0x0E

#define CRG_SCALE_16           0x0F




void OSTickStart(void);
void PLLInit(void);


#endif