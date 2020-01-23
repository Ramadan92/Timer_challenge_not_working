/*
 * timers.c
 *
 * Created: 1/20/2020 11:06:28 PM
 *  Author: ahmed
 */ 
#include "timers.h"
#include "interrupt.h"
#include "led.h"
En_timer0perscaler_t en_gprescal ;
static uint32_t u32_gOVFCounter;
uint8_t u8_gTCNT0PreloadValue;
uint16_t u16_gISRCount =1;



void timer0Init(En_timer0Mode_t en_mode,En_timer0OC_t en_OC0,En_timer0perscaler_t en_prescal, uint8_t u8_initialValue, uint8_t u8_outputCompare, En_timer0Interrupt_t en_interruptMask)
{
	en_gprescal = en_prescal;
	
	switch (en_mode)
	{
		case T0_NORMAL_MODE:
		TCCR0 |= T0_NORMAL_MODE;
		break;
		
		case T0_COMP_MODE:
		TCCR0 |= T0_COMP_MODE;
		break;
	}
	
	switch (en_OC0)
	{
		case T0_OC0_TOGGLE:
		TCCR0 |= T0_OC0_TOGGLE;
		break;
		
		case T0_OC0_SET:
		TCCR0 |= T0_OC0_SET;
		break;
		
		case T0_OC0_CLEAR:
		TCCR0 |= T0_OC0_CLEAR;
		break;
		
		case T0_OC0_DIS:
		TCCR0 |= T0_OC0_DIS;
		
	}
	
	switch (en_prescal)
	{
		case T0_NO_CLOCK:
		TCCR0 |= T0_NO_CLOCK;
		break;
		
		case T0_PRESCALER_NO:
		TCCR0 |= T0_PRESCALER_NO;
		break;
		
		case T0_PRESCALER_8:
		TCCR0 |= T0_PRESCALER_8;
		break;
		
		case T0_PRESCALER_64:
		TCCR0 |= T0_PRESCALER_64;
		break;
		
		case T0_PRESCALER_256:
		TCCR0 |= T0_PRESCALER_256;
		break;
		
		case T0_PRESCALER_1024:
		TCCR0 |= T0_PRESCALER_1024;
		break;
		
	}
	
	switch (en_interruptMask)
	{
		case T0_POLLING:
		TIMSK |= T0_POLLING;
		break;
		
		case T0_INTERRUPT_NORMAL:
		TIMSK |= T0_INTERRUPT_NORMAL;
		break;
		
		case T0_INTERRUPT_CMP:
		TIMSK |= T0_INTERRUPT_CMP;
		break;
	}
	
	TCNT0 = u8_initialValue;
	OCR0  = u8_outputCompare;
}

void timer0Set(uint8_t u8_value)
{
	TCNT0 = u8_value;
}

uint8_t timer0Read(void)
{
	return TCNT0;
}

void timer0Stop(void)
{
	TCCR0>>=3;
	TCCR0<<=3;
}

void timer0Start(void)
{
	TCCR0|=en_gprescal;
}

void timer0DelayMs(uint16_t u16_delay_in_ms)
{
	u8_gTCNT0PreloadValue=48;
	TCNT0 = u8_gTCNT0PreloadValue;
	u32_gOVFCounter = 8 * u16_delay_in_ms;
	timer0Start();
	while (!(u32_gOVFCounter==u16_gISRCount));
	Led_On(LED_3);
	timer0Stop();
}

void timer0DelayUs(uint32_t u32_delay_in_us)
{
	u8_gTCNT0PreloadValue=254;
	TCNT0 = u8_gTCNT0PreloadValue;
	timer0Start();
	u32_gOVFCounter = u32_delay_in_us;
	while (!(u32_gOVFCounter==u16_gISRCount));
	timer0Stop();
}

ISR(TIMER0_OVF_vect)
{
	/*if (u16_gISRCount==8)
	TCNT0 = u8_gTCNT0PreloadValue;
	
	if (u16_gISRCount == ++u32_gOVFCounter)
	{
		u16_gISRCount = 2;
		//u32_gOVFCounter= 1;
		
	}

	else 
	u16_gISRCount++; 
	//Led_On(LED_2); */
	
	//if (u16_gISRCount == 8000)
		Led_Off(LED_1);
	
	//else 
	//u16_gISRCount++;
}