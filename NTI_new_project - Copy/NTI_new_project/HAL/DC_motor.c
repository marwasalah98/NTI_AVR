
#include "STD_types.h"
#include "DIO.h"
#include "DC_motor.h"

void motor_state(MOTOR_MODES mode)
{
	//pwm 100%
	DIO_vSetPinMode(MOTOR_PWM,OUTPUT_HIGH);
	
	switch(mode)
	{
		case MOTOR_ON_CW : DIO_vSetPinMode(MOTOR_EN,OUTPUT_HIGH); DIO_vSetPinMode(MOTOR_DIR,OUTPUT_HIGH);break;
		case MOTOR_ON_CCW: DIO_vSetPinMode(MOTOR_EN,OUTPUT_HIGH); DIO_vSetPinMode(MOTOR_DIR,OUTPUT_LOW) ;break;
		case MOTOR_OFF   : DIO_vSetPinMode(MOTOR_EN,OUTPUT_LOW);  break;
	}
	
}