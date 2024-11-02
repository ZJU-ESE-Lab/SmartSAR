#ifndef LED_H
#define	LED_H
#define LED0  0x01
#define LED1  0x02
#define LED2  0x04
#define LED3  0x08
#define LED4  0x10
#define LED5  0x20
#define LED6  0x40
#define LED7  0x80

void SetLED(byte LEDID);
void ClrLED(byte LEDID);
void PutInt(byte num);
#endif