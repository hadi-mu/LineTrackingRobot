/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Robot.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "camera.h"
#include "delay.h"
#include "LCD.h"
#include "PWM.h"
#include "LED.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
unsigned int checkT=1;//next turn direction
unsigned int checkO=0;//if obstacle
unsigned char left; // black pixels on left
unsigned char right; // black pixels on right
unsigned int lspeed; // black pixels on left
unsigned int rspeed; // black pixels on right
signed int dist =0;//how off centre we are
unsigned int lost;//lost state
/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();


    //initialsing different components
    camera_init();
    motor_init();
    LCD_init();
    LED_init();



    //Initialising PORTE and connecting PTE16 & PTE18
    SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
    PORTE->PCR[16]=PORT_PCR_MUX(1);
    PORTE->PCR[18]=PORT_PCR_MUX(1);






    while(1) {



    	//Read and process and display the information from the camera
    	camera_process(&left,&right);
    	LCDNumberLeft(left);
    	LCDNumberRight(right);

    	//Check if there is a turn coming up
    	checkT=checkTurn(checkT);

    	//Check if there is an obstacle
    	checkO=checkObst();

    	//If there is an obstacle reverse and turn until lost
    	if (checkO==1){
        	LED2(1);//Flash LED
        	motor(-255,-255);//Reverse
        	delay(4E6);//For 1 second
        	motor(255,-255);//Turn right for 1 second
        	delay(4E6);
        	checkT=1;//Keep turning right when lost

    	}
    	else{
    		LED2(0);//LED off
    	}



    	lost=checkLost(&left,&right);//Check if lost

    	if (lost==1){
   			LED1(1);
    		motor(0,0);//Stop
       	while(left==0 && right==0){//Until we can see any black pixels
       		if (checkT==0){//Turn according to previous detected turn
       		    LCDNumberLeft(left);
       		    LCDNumberRight(right);
    			motor(-255,255);
    			camera_process(&left,&right);}
       		else if (checkT==1){
       		    LCDNumberLeft(left);
       		    LCDNumberRight(right);
    			motor(255,-255);
    			camera_process(&left,&right);

       		}
    		}
    	}


    	else{
    		LED1(0);

    	dist=left-right;//calculate how off-centre we are


		if (dist<0){//if more to the right side than left

			dist=-dist;
			lspeed=255;
			rspeed=255 - 14*dist;//slow right wheel using proportional controller
		}
		else
		if (dist>=0){//if more to the left side than right
				rspeed=255;
				lspeed=255- 14*dist;//slow left wheel

			}

		motor(lspeed,rspeed);
    	}

    }
    return 0 ;



}
