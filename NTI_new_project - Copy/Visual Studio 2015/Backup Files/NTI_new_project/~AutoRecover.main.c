#include "DIO.h"
#include "Led.h"
#include "Button.h"
#include "Keypad.h"
#include "seg.h"
#include "LCD.h"
#include "EX_Interrupt.h"

#define F_CPU 16000000
#include <util/delay.h>


static volatile u16 seg_counter=0;


void seg_func(void)
{
	
}

void f1(void)
{
	LED_setmode(YELLOW_LED,HIGH);
	_delay_ms(2000);
	LED_setmode(YELLOW_LED,LOW);
}


/*projects:

task 1:
2*7 seg count using interrupt - 0 - 99
initial value is 88
one press -> 00
2 presses -> 01

 
 task 2:
DC motor 2 directions with 1 button 

 
 *    
 *
 *
 *
 */

int main (void)
{
	Keypad_Init();
	//LCD_Init();
	seg_Init();
	u8 key = NO_KEY;
	//LCD_WriteString("ABdallah");
 	//LCD_WriteString_multiLine("Ya rab ne5las ya marwa ");
	// LCD_WriteString_multiLine("")


	while(1)
	{
		seg_write(1579);
		/*
		//seg_write(1923);
		key=keypad_getkey();
		if(key!=NO_KEY)
		{
			LCD_goto(0,0);
			LCD_WriteChar(key);
		}
		*/
	}
}

int main3 (void)
{
	EXI_Init();
	EXI_TriggerEdge(EX_INT0,FALLING_EDGE);
	EXI_SetCallBack(EX_INT0,f1);
	
	
	LED_Init(BLUE_LED);
	LED_Init(RED_LED);
	LED_Init(YELLOW_LED);
	
	LED_setmode(BLUE_LED,LOW);
	LED_setmode(RED_LED,LOW);
	
	EXI_Enable(EX_INT0);
	GLOBAL_ENABLE();
	
	while(1)
	{
// 		LED_setmode(YELLOW_LED,HIGH);
// 		_delay_ms(500);
// 		LED_setmode(YELLOW_LED,LOW);
// 		_delay_ms(500);
		
	}	
	/*
	LCD_Init();
	///Keypad_Init();
	LED_Init(RED_LED);
	
	//u8 key=NO_KEY;
	LED_setmode(RED_LED,HIGH);
	LCD_WriteChar('0');
	LCD_WriteString("I am abdallah");
	LCD_goto(1,5);
	LCD_WriteNumber(99);
	_delay_ms(1000);
	LED_setmode(RED_LED,LOW);
	while(1)
	{
		
	}
	/*
	while(1)
	{
		key=keypad_getkey();
		
		if(key!=NO_KEY)
		{
			LCD_goto(0,0);
			LCD_WriteChar(key);
		}
			
	}
	*/
}







int main2(void)
{
	Pushbutton_setup(pd_1);
	Pushbutton_setup(pd_2);
	Pushbutton_setup(pd_3);
	Pushbutton_setup(pd_4);
	LED_Init(RED_LED);
	LED_Init(GREEN_LED);
	LED_Init(BLUE_LED);
	LED_Init(YELLOW_LED);
	Keypad_Init();
    while (1) 
    {
		u8 key =keypad_getkey();
		if(key!=NO_KEY)
		{
			LED_setmode(YELLOW_LED,HIGH);
			_delay_ms(500);
			LED_setmode(YELLOW_LED,LOW);
		}
		/*for(int i=0;i<4;i++)
		{
			LED_setmode(i,HIGH);
			_delay_ms(200);
			LED_setmode(i,LOW);	
		}
		/*if(Pushbutton_state(pd_1)==PRESSED)
		{
			 LED_setmode(RED_LED,HIGH);
		}
		else
		{
			//_delay_ms(1000);
			 LED_setmode(RED_LED,LOW);
			// _delay_ms(1000);
		}
		
		if(Pushbutton_state(pd_2)==NOTPRESSED)
		{
			LED_setmode(BLUE_LED,HIGH);
		}
		else
		{
			//_delay_ms(1000);
			LED_setmode(BLUE_LED,LOW);
			// _delay_ms(1000);
		}*/
			
    }
}

