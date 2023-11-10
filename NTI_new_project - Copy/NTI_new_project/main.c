#include "DIO.h"
#include "Led.h"
#include "Button.h"
#include "Keypad.h"
#include "seg.h"
#include "LCD.h"
#include "EX_Interrupt.h"
#include "ADC.h"
#include "sensors.h"
#include "DC_motor.h"
#include "TIMER.h"
//MARWA***********************************************************************************

#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

typedef enum
{
	PRESSED_ONCE,
	PRESSED_TWICE,
	SHORT_HOLD,
	LONG_HOLD

}Button_State_t;

static volatile Button_State_t   pb1_State_right = PRESSED_TWICE ;
static volatile Button_State_t   pb2_State_left  = PRESSED_TWICE ;
static volatile Button_State_t   pb3_State_front = PRESSED_TWICE ;
static volatile Button_State_t   pb4_State_back  = PRESSED_TWICE ;

typedef enum
{
	BUTTON_1=1,
	BUTTON_2=2,
	BUTTON_3=3,
	BUTTON_4=4,
	NO_BUTTON=5
}NO_of_button_t;

static volatile NO_of_button_t  button_pressed =0;

#define   RIGHT_LED    YELLOW_LED
#define   LEFT_LED     BLUE_LED
#define   FRONT_LED    GREEN_LED
#define   BACK_LED     RED_LED


static  u8 flag_PB=0;
static volatile u16 timer_counter =0;
static u32 counter=0;

void light_func (void)//0.5sec ,0.25sec
{
	timer_counter ++;
	TCNT0=6;
	static u8 right_short_flag=0;
	static u8 right_long_flag =0;

	static u8 left_short_flag=0;
	static u8 left_long_flag =0;

	static u8 front_short_flag=0;
	static u8 front_long_flag =0;

	static u8 back_short_flag=0;
	static u8 back_long_flag =0;


	if(timer_counter % 1000 == 0) //8000 ov = 1 sec      -> 1000 = 0.125 sec
	{
		switch(pb1_State_right)
		{
		  case PRESSED_ONCE :
		  LED_setmode(RIGHT_LED,HIGH);
		  right_short_flag=0;
		  right_long_flag=0 ;
		  break;

		case PRESSED_TWICE:
		LED_setmode(RIGHT_LED,LOW) ;
		right_short_flag=0;
		right_long_flag=0 ;
		break;

		case SHORT_HOLD   :  right_short_flag=1; right_long_flag=0 ;break;
		case LONG_HOLD    :  right_short_flag=0; right_long_flag=1 ;break;
		}

		switch(pb2_State_left)
		{
			case PRESSED_ONCE : LED_setmode(LEFT_LED,HIGH); left_short_flag=0; left_long_flag=0 ;break;
			case PRESSED_TWICE: LED_setmode(LEFT_LED,LOW) ; left_short_flag=0; left_long_flag=0 ;break;
			case SHORT_HOLD   :  left_short_flag=1; left_long_flag=0 ;break;
			case LONG_HOLD    :  left_short_flag=0; left_long_flag=1 ;break;
		}

		switch(pb3_State_front)//b3
		{
			case PRESSED_ONCE : LED_setmode(FRONT_LED,HIGH); front_short_flag=0; front_long_flag=0 ;break;
			case PRESSED_TWICE: LED_setmode(FRONT_LED,LOW) ; front_short_flag=0; front_long_flag=0 ;break;
			case SHORT_HOLD   :  front_short_flag=1; front_long_flag=0 ;break;
			case LONG_HOLD    :  front_short_flag=0; front_long_flag=1 ;break;
		}

		switch(pb4_State_back)
		{
			case PRESSED_ONCE : LED_setmode(BACK_LED,HIGH); back_short_flag=0; back_long_flag=0 ;break;
			case PRESSED_TWICE: LED_setmode(BACK_LED,LOW) ; back_short_flag=0; back_long_flag=0 ;break;
			case SHORT_HOLD   :  back_short_flag=1; back_long_flag=0 ;break;
			case LONG_HOLD    :  back_short_flag=0; back_long_flag=1 ;break;
		}


	}


	if(timer_counter % 2000 == 0)//each 0.25 sec     //long
	{
		if(right_long_flag == 1)//button
		{
		 LED_toggle(RIGHT_LED);
		}
		if(left_long_flag == 1)
		{
		 LED_toggle(LEFT_LED);
		}
		if(front_long_flag == 1)
		{
		 LED_toggle(FRONT_LED);
		}
		if(back_long_flag == 1)
		{
		 LED_toggle(BACK_LED);
		}
	}

	if(timer_counter % 4000==0 )//each 0.5 sec     //short
	{
		if(right_short_flag == 1)
		{
			LED_toggle(RIGHT_LED);
		}
		if(left_short_flag == 1)
		{
			LED_toggle(LEFT_LED);
		}
		if(front_short_flag == 1)
		{
			LED_toggle(FRONT_LED);
		}
		if(back_short_flag == 1)
		{
			LED_toggle(BACK_LED);
		}
		timer_counter=0;
	}
}

#define   PASSWORD     (1234) 

int main(void)
{
	DIO_vSetPinMode(PC0,OUTPUT_LOW);//MOTOR
	DIO_vSetPinMode(PC1,OUTPUT_LOW); 
	
	LCD_Init();
	Keypad_Init();

	TIMER0_Init(TIMER0_NORMAL_MODE,TIMER0_SCALER_8);//tick.time 0.5Msec,cpu freq =16
	TIMER0_OVF_InterruptEnable();
	TIMER0_OVF_SetCallBack(light_func);
	sei();

	u8 flag_Right=0, flag_Left=0,flag_Front=0,flag_Back=0;
	u8 flag_Twice_Right=0,count_Twice_Right=0;
	u8 flag_Twice_Left=0,count_Twice_Left=0;
	u8 flag_Twice_Front=0,count_Twice_Front=0;
	u8 flag_Twice_Back=0,count_Twice_Back=0;
	
	
	LED_Init(RED_LED);
	LED_Init(YELLOW_LED);
	LED_Init(GREEN_LED);
	LED_Init(BLUE_LED);


	DIO_vSetPinMode(PA0,INPUT_PULLUP);
	DIO_vSetPinMode(PA1,INPUT_PULLUP);
	DIO_vSetPinMode(PA7,INPUT_PULLUP);
	DIO_vSetPinMode(PB6,INPUT_PULLUP);

    LCD_Write_stringGoTo(0,0,"Enter password");
    LCD_goto(1,0);
	
	
	u16 num=0;
	u8 No_digit=1;	
	while(No_digit<=4)
	{
		u8 k=keypad_getkey();
		if(k>='0'&& k<='9')
		{
			num=(num*10)+k-'0'; //string to int
			LCD_goto(1,No_digit-1);
			LCD_WriteChar('*');
			No_digit++;
		}
		
		if(No_digit==5)
		{
			if(num==PASSWORD)
			{
				LCD_Write_stringGoTo(0,0,"Right password   ");
				_delay_ms(500);
				DIO_vSetPinMode(PC0,OUTPUT_HIGH);//MOTROR on
				DIO_vSetPinMode(PC1,OUTPUT_LOW);
				LCD_Clear();
				
				
			}
			else
			{
				LCD_Write_stringGoTo(0,0,"Wrong Password ! ");
				_delay_ms(500);
				LCD_Clear();
				LCD_Write_stringGoTo(0,0,"Enter password");
				No_digit=1;
				num=0;
				
			}
		}
	}
	
	LCD_Clear();
	LCD_Write_stringGoTo(0,0,"Right :    ");
	LCD_Write_stringGoTo(1,0,"LEFT  :    ");
	LCD_Write_stringGoTo(2,0,"FRONT :    ");
	LCD_Write_stringGoTo(3,0,"BACK  :    ");
	
	while (1)				  
	{
		counter=0;
		if(Pushbutton_state(pd_1)==PRESSED) //RIGHT_LED,YELLOW_LED
		{
			button_pressed=BUTTON_1;
			while (Pushbutton_state(pd_1)==PRESSED)
			{

				counter++;
				_delay_ms(1);
			}
		}
		else if(Pushbutton_state( pd_2)==PRESSED)//LEFT_LED,BLUE_LED
		{
			button_pressed=BUTTON_2;
			while (Pushbutton_state( pd_2)==PRESSED)
			{
				counter++;
				_delay_ms(1);
			}
		}
		else if(Pushbutton_state(pd_3)==PRESSED)//FRONT_LED,GREEN_LED
		{
			button_pressed=BUTTON_3;
			while (Pushbutton_state(pd_3)==PRESSED)
			{
				counter++;
				_delay_ms(1);
			}
		}
		else if(Pushbutton_state(pd_4)==PRESSED)//BACK_LED,RED_LED
		{
			button_pressed=BUTTON_4;
			while (Pushbutton_state(pd_4)==PRESSED)
			{
				counter++;
				_delay_ms(1);
			}
		}
		else
		{
			button_pressed=NO_BUTTON;
		}

		if((counter<1500)&&(counter>0))//PRESSED_ONCE,PRESSED_TWICE
		{
			
		   if( button_pressed==BUTTON_1)//pb1_RIGHT_WARNING
		  {	
				//if((pb1_State_right == LONG_HOLD) || (pb1_State_right == SHORT_HOLD) )
				//{
					//flag_Twice=1;
				//}
				if(flag_Twice_Right==0)
				{
					flag_Twice_Right=1;
					pb1_State_right = PRESSED_ONCE;
					
				}
				else if(flag_Twice_Right==1)
				{
					if(count_Twice_Right==0)
					{
						count_Twice_Right=1;
			
					}
					else if(count_Twice_Right ==1)
					{
						count_Twice_Right=0;
						flag_Twice_Right=0;
						pb1_State_right = PRESSED_TWICE;
					}
				}
				
				
		    }

		if( button_pressed==BUTTON_2)
		{
			if(flag_Twice_Left==0)
			{
				flag_Twice_Left=1;
				pb2_State_left = PRESSED_ONCE;
				
			}
			else if(flag_Twice_Left==1)
			{
				if(count_Twice_Left==0)
				{
					count_Twice_Left=1;
					
				}
				else if(count_Twice_Left ==1)
				{
					count_Twice_Left=0;
					flag_Twice_Left=0;
					pb2_State_left = PRESSED_TWICE;
				}
			}
		}

		if( button_pressed==BUTTON_3)
		{
                if(flag_Twice_Front==0)
                {
	                flag_Twice_Front=1;
	                pb3_State_front = PRESSED_ONCE;
	                
                }
                else if(flag_Twice_Front==1)
                {
	                if(count_Twice_Front==0)
	                {
		                count_Twice_Front=1;
		                
	                }
	                else if(count_Twice_Front ==1)
	                {
		                count_Twice_Front=0;
		                flag_Twice_Front=0;
		                pb3_State_front = PRESSED_TWICE;
	                }
				}
		}
		
		if( button_pressed==BUTTON_4)
		{
                  if(flag_Twice_Back==0)
                  {
	                  flag_Twice_Back=1;
	                  pb4_State_back = PRESSED_ONCE;
	                  
                  }
                  else if(flag_Twice_Back==1)
                  {
	                  if(count_Twice_Back==0)
	                  {
		                  count_Twice_Back=1;
		                  
	                  }
	                  else if(count_Twice_Back ==1)
	                  {
		                  count_Twice_Back=0;
		                  flag_Twice_Back=0;
		                  pb4_State_back = PRESSED_TWICE;
	                  }
                  }
		  
		}
	}
	
	else if(counter<3000)//short hold
	{
		
		if (button_pressed==BUTTON_1 )
		{
		    pb1_State_right = SHORT_HOLD ;
		}
		else if(button_pressed==BUTTON_2)
		{
			pb2_State_left =SHORT_HOLD ;
		}
		else if(button_pressed==BUTTON_3)
		{
			pb3_State_front = SHORT_HOLD ;
		}

		else if(button_pressed==BUTTON_4)
		{
			pb4_State_back = SHORT_HOLD ;
		}
		flag_Right=0;
		flag_Left=0;
		flag_Front=0;
		flag_Back=0;
	}

	else //long hold
	{
		if (button_pressed==BUTTON_1)
		{
			pb1_State_right = LONG_HOLD ;
		}
		else if(button_pressed==BUTTON_2)
		{
			pb2_State_left =LONG_HOLD ;
		}
		else if(button_pressed==BUTTON_3)
		{
			pb3_State_front = LONG_HOLD ;
		}

		else if(button_pressed==BUTTON_4)
		{
			pb4_State_back =LONG_HOLD ;
		}
		flag_Right=0;
		flag_Left=0;
		flag_Front=0;
		flag_Back=0;
	}

	switch(pb1_State_right)
	{
	case PRESSED_ONCE :LCD_Write_stringGoTo(0,8,"ON   ");break;
	case PRESSED_TWICE:LCD_Write_stringGoTo(0,8,"OFF  "); break;
	case SHORT_HOLD   : LCD_Write_stringGoTo(0,8,"SHORT  "); break;
	case LONG_HOLD    :LCD_Write_stringGoTo(0,8,"LONG    "); break;
	}

	switch(pb2_State_left)
	{
	case PRESSED_ONCE :LCD_Write_stringGoTo(1,8,"ON   ");break;
	case PRESSED_TWICE:LCD_Write_stringGoTo(1,8,"OFF  "); break;
	case SHORT_HOLD   : LCD_Write_stringGoTo(1,8,"SHORT  "); break;
	case LONG_HOLD    :LCD_Write_stringGoTo(1,8,"LONG    "); break;
	}
	switch(pb3_State_front)
	{
	case PRESSED_ONCE :LCD_Write_stringGoTo(2,8,"ON   ");break;
	case PRESSED_TWICE:LCD_Write_stringGoTo(2,8,"OFF  "); break;
	case SHORT_HOLD   : LCD_Write_stringGoTo(2,8,"SHORT  "); break;
	case LONG_HOLD    :LCD_Write_stringGoTo(2,8,"LONG    "); break;
	}
	switch(pb4_State_back)
	{
	case PRESSED_ONCE :LCD_Write_stringGoTo(3,8,"ON   ");break;
	case PRESSED_TWICE:LCD_Write_stringGoTo(3,8,"OFF  "); break;
	case SHORT_HOLD   : LCD_Write_stringGoTo(3,8,"SHORT  "); break;
	case LONG_HOLD    :LCD_Write_stringGoTo(3,8,"LONG    "); break;
	}
		
	}
}
