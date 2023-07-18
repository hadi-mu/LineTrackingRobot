/*
 * PWM.c
 *
 *  Created on: 12 Jun 2023
 *      Author: ball5995
 */
#include "MKL46Z4.h"
#include "PWM.h"

void PWM_init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1); //Enable Port B
	SIM->SCGC6 |= SIM_SCGC6_TPM2(1); //Enable TPM
	PORTB->PCR[18]=PORT_PCR_MUX(3); //Connect Port B to TPM
	PORTB->PCR[19]=PORT_PCR_MUX(3); //Connect Port B to TPM
	SIM->SOPT2 = SIM_SOPT2_TPMSRC(0b10); //Set TMP Clock Source
	// Set TPM_SC
	TPM2->SC = TPM_SC_CMOD(1) // Use internal clock source
	 | TPM_SC_PS(0b110); // prescale divide by ??
	//Set the period
	TPM2->MOD = 254;
	//Channel 0 Status and Control
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB(1) // PWM mode channel 0
	 | TPM_CnSC_MSA(0)
	 | TPM_CnSC_ELSB(1)
	 | TPM_CnSC_ELSA(0);
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB(1) // PWM mode channel 1
		 | TPM_CnSC_MSA(0)
		 | TPM_CnSC_ELSB(1)
		 | TPM_CnSC_ELSA(0);


}
//Duty cycle for left PWM
void PWMduty0(unsigned int pw)
{
// Set the duty cycle width
TPM2->CONTROLS[0].CnV = pw;
}
//Duty cycle for right PWM

void PWMduty1(unsigned int pw)
{
// Set the duty cycle width
TPM2->CONTROLS[1].CnV = pw;
}

void motor_init(void)
{
 PWM_init();
 // Enable Port C
 SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
 // Connect pin PTC10 and 11 to ALT1, the GPIO module. Setting them as digital outputs.
 PORTC->PCR[10]=PORT_PCR_MUX(1);
 PORTC->PCR[11]=PORT_PCR_MUX(1);
 GPIOC->PDDR = 1<<10 | 1<<11;
}

void motor(signed int ls,signed int rs)
{
//Sets motor speed
//LEFT MOTOR

/* Set the direction of the motor
 At the same time, make ls positive if needed */
 if (ls>0)
 { GPIOC->PSOR=1<<10;
 }
 else
 {
   GPIOC->PCOR=1<<10;
   ls=-ls;
 }



//If ls larger than 255, reduce to 255
 if (ls>255)
  { ls=255;
  }
//Set the PWM duty cycle pulse width
 PWMduty0(ls);



 //RIGHTMOTOR

  if (rs>0)
  { GPIOC->PSOR=1<<11;
  }
  else
  {
    GPIOC->PCOR=1<<11;
    rs=-rs;
  }
  if (rs>255)
   { rs=255;
   }
  PWMduty1(rs);
}
