#include "Button.h"
#include "DIO.h"

pb_state_t Pushbutton_state(pb_id_t   button)//7,6,5,3
{
	pb_state_t  state=NOTPRESSED;
	switch(button)
	{
		case pd_1:
		state=DIO_u8GetPinValue(PA0);
		break;
		
		case pd_2:
		state=DIO_u8GetPinValue(PA1);
		break;
		
		case pd_3:
		state=DIO_u8GetPinValue(PA7);
		break;
		
		case pd_4:
		state=DIO_u8GetPinValue(PB6);
		break;
	}
	return state;
	
}//presses/not

void Pushbutton_setup(pb_id_t   button ) //init //pin-->inpull
{
	switch(button )
	{
		case pd_1:
		DIO_vSetPinMode(PD0,INPUT_PULLUP);
		break;
		
		case pd_2:
		DIO_vSetPinMode(PD1,INPUT_PULLUP);
		break;
		
		case pd_3:
		DIO_vSetPinMode(PD2,INPUT_PULLUP);
		break;
		
		case pd_4:
		DIO_vSetPinMode(PD4,INPUT_PULLUP);
		break;
	}
	
	
}//Input,