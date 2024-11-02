#include "includes.h"
#include "led.h"


void SetLED(byte LEDID){
   DDRB  |= LEDID;
   PORTB &= ~LEDID;
}

void ClrLED(byte LEDID){
   PORTB |= LEDID;
}

void PutInt(byte num){
   if (num>=0 && num<10){    
      switch(num){
        case 0: DDRB  |= 0x3f;
                PORTB |=~0x3f;
                PORTB &= ~0x3f;
                break; 
        case 1: DDRB  |= 0x06;
                PORTB |= ~0x06;
                PORTB &= ~0x06;
                break; 
        case 2: DDRB  |= 0x5b;
            PORTB |= ~0x5b;
                PORTB &= ~0x5b;
                break; 
        case 3: DDRB  |= 0x4f;
                PORTB |= ~0x4f; 
                PORTB &= ~0x4f;
                break; 
        case 4: DDRB  |= 0x66;
            PORTB |= ~0x66;
                PORTB &= ~0x66;
                break; 
        case 5: DDRB  |= 0x6d;
            PORTB |= ~0x6d;
                PORTB &= ~0x6d;
                break;   
        case 6: DDRB  |= 0x7d;
            PORTB |= ~0x7d;
                PORTB &= ~0x7d;
                break;  
        case 7: DDRB  |= 0x07;
            PORTB |= ~0x07;
                PORTB &= ~0x07;
                break;  
        case 8: DDRB  |= 0x7f;
            PORTB |= ~0x7f;
                PORTB &= ~0x7f;
                break; 
        case 9: DDRB  |= 0x6f;
                PORTB |= ~0x6f;
                PORTB &= ~0x6f;
                break;                                                                                 
      }
   }
}