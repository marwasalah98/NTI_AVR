#include "ADC.h"

static ADC_VoltRef_type vref = VREF_VCC;
static ADC_Prescaler_type scaler=ADC_SCALER_64;
	
void ADC_Init(void)
{
	
	//v ref
	switch(vref)
	{
		case VREF_AREF:
		CLR_BIT(ADMUX,REFS1);
		CLR_BIT(ADMUX,REFS0);
		break;
		
		case VREF_VCC:
		CLR_BIT(ADMUX,REFS1);
		SET_BIT(ADMUX,REFS0);
		break;
		
		case VREF_256:
		SET_BIT(ADMUX,REFS1);
		SET_BIT(ADMUX,REFS0);
		break;
	}
	
	
	//prescaler
	ADCSRA&=0xf8;
	ADCSRA|=(scaler&(0x07));
	 10101111
	 00000111
	
	
	
	//ADC Enable 
	SET_BIT(ADCSRA,ADEN);
	
}


u16 ADC_Read(ADC_Channel_type channel)
{
	//select channel 
	ADMUX&=0xE0;
	ADMUX|=(channel & 0x1F);
	
	//start coversion
	SET_BIT(ADCSRA,ADSC);
	
	//wait until conversion compete
	while(GET_BIT(ADCSRA,ADSC));
	
	
	//get reding 
	return ADC;
	
	
	
	
}