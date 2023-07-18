#include "MKL46Z4.h"

void LED_init()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA(1) ;		// Enable Port A

	PORTA->PCR[1]=PORT_PCR_MUX(1);		// Connect PTA1 to ALT1 the GPIO module

	PORTA->PCR[2]=PORT_PCR_MUX(1);		// Connect PTA2 to ALT1 the GPIO module

	GPIOA->PDDR = 1<<1 | 1<<2;		//Set PTA1 and PTA1 to be outputs
}


/* This function is used turn the LEDs on and off.
There are no outputs and 2 inputs called a and b.*/

void LED1( unsigned char a)
{if (a == 1)
	//the LED connected to pin 1 is turned on
	{
	GPIOA->PCOR = 1<<1;
	}

else
{
	//the LED connected to pin 1 is turned off
	GPIOA->PSOR = 1<<1;
}}

void LED2(unsigned char b )


{

if (b ==1)

{
	//the LED connected to pin 2 is turned on
	GPIOA->PCOR = 1<<2;
}

else
{
	//the LED connected to pin 2 is turned off
	GPIOA->PSOR = 1<<2;
}
}





