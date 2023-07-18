/*
 * camera.c
 *
 *  Created on: 13 Jun 2023
 *      Author: ball5995
 */


#include "MKL46Z4.h"
#include "delay.h"
#include "ADC.h"
#include "camera.h"

#define CLOCK 1 // The camera clock
#define SI 2 // The camera sync pulse.
#define TRIG 4 //Output to trigger the scope

unsigned int n = 28; //DELAY
unsigned int itime = 100; //integration time delay
unsigned k; // Array index
unsigned char CCD[64]; // Array for storing ADC output
unsigned int UL = 130;//Upper and lower limits to detect saturation on camera
unsigned int LL = 120;
unsigned int thresh = 60; //Threshold for black colour
int rawL=0;//raw ADC data for left and right proximity sensors
int rawR=0;
int checkL=0;//check whether threshold for obstacle met by raw ADC data
int checkR=0;



//Initialising camera
void camera_init(void){
	ADCinit();//Initialise ADC
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);//Setting up PTB0,PTB1,PTB2
	PORTB->PCR[0]=PORT_PCR_MUX(1);
	PORTB->PCR[1]=PORT_PCR_MUX(1);
	PORTB->PCR[2]=PORT_PCR_MUX(1);
	GPIOB->PDDR = CLOCK | SI | TRIG; //Setting output

}

void camera_true(void){
	 GPIOB->PSOR |= SI | TRIG; //Initial clock and signal pulses
	 delay(n);
	 GPIOB->PSOR |= CLOCK;
	 delay(n);
	 delay(n);
	 GPIOB->PCOR |= SI | TRIG;
	 delay(n);
	 GPIOB->PCOR |= CLOCK;
	 delay(n);
	 for (k = 0; k < 64;k++){ //Rest of clock pulses
		 ADCstart(0); //Reading camera data and storing in CCD
		 CCD[k] = ADCread ();
		 GPIOB->PSOR |= CLOCK;
		 delay(n);
		 GPIOB->PCOR |= CLOCK;
		 delay(n);
	 }



}

//Same as camera_true but takes dummy reading
void camera_dummy(void){
	unsigned char DUM[64];
	GPIOB->PSOR |= SI;
		 delay(n);
	 GPIOB->PSOR |= CLOCK;
	 delay(n);
	 delay(n);
	 GPIOB->PCOR |= SI;
	 delay(n);
	 GPIOB->PCOR |= CLOCK;
	 delay(n);
	 for (k = 0; k < 64;k++){
		 ADCstart(0);
		 DUM[k] = ADCread ();
		 GPIOB->PSOR |= CLOCK;
		 delay(n);
		 GPIOB->PCOR |= CLOCK;
		 delay(n);
	 }
}


void camera_read(void){

	int max=0;
	camera_dummy(); //Clear accumulated output
	delay(itime); //Integration time
	camera_true(); //Read camera output into array
	max=camera_max(CCD); //Find max value



	if (max>UL){ //If max value greater than UL, decrease itime
		itime = (itime * 9)/10;
		if (itime==0){
			itime=1;
		}

	}

	if (max<LL){//If max value less than LL, increase itime
		itime=(itime*(11))/10;
		if (itime<10){
			itime++;
		}
	}
}

//finds maximum in array
int camera_max(unsigned char CCD[]){
	int maxValue = CCD[0];
	int i=0;
	for (i = 0; i < 64; ++i) {
	        if ( CCD[i] > maxValue ) {
	            maxValue = CCD[i];
	        }
	    }
	    return maxValue;


}

void camera_process(unsigned char *l, unsigned char *r){
	camera_read();//read in camera data
	*r=0;
	*l=0;
	int i=0;
	for (i = 0; i < 64; ++i) {
		        if ( CCD[i] < thresh ) {//check if black pixel
		           if (i>31){//check if black pixel in right
		        	   (*r)++;
		           }
		           else
		           if (i<=31){//check if black pixel in left
		        	   (*l)++;
		           }
		        }

		    }


}



int checkLost(unsigned char *l, unsigned char *r){
	if (*r==0 & *l==0){//if no black pixels seen, we are lost
		return 1;
	}
	else{
		return 0;
	}

}

int checkTurn(unsigned int check){
	if (CCD[31]<thresh & CCD[32]<thresh & CCD[63]<thresh){ //if middle pixels and far right are black, right turn coming up
		return 1;
	}
	else if (CCD[31]<thresh & CCD[32]<thresh & CCD[0]<thresh){//if middle pixels and far left are black, left turn coming up
		return 0;

	}
	else{
		return check; //otherwise return previous turn

	}

}

int checkObst(void)
{

	ADCstart(1);//read in ADC data
	rawL = ADCread();
	if (rawL>=70){//if threshold for obstacle met
		checkL=1; //then obstacle detected
	}
	else{
		checkL=0;
	}



	ADCstart(2);
	rawR=ADCread();
	if (rawR>=70){
			checkR=1;
		}
	else{
		checkR=0;
	}

	if (checkL || checkR){//if obstacle detected on either left or right sensor
		return 1;
	}
	else{return 0;}
}

