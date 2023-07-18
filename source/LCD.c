/*
 * LCD.c
 *
 *  Created on: 17 Nov 2017
 *      Author: Eric Peasley
 */

#include <stdio.h>
#include "MKL46Z4.h"

// Lookup table for different character displays
const unsigned char wfa[16] = {14,6,12,14,6,10,10,14,14,14,14,2,8,6,8,8};
const unsigned char wfb[16] = {11,0,7,5,12,13,15,0,15,13,14,15,11,7,15,14};

void LCD_init(void)
{
  /*
   * Initialise the LCD controller.
   */


  /* Enable the clock to the LCD controller */
  SIM->SCGC5 |= SIM_SCGC5_SLCD(1);

  /* Configure the general control Register*/
  LCD->GCR = LCD_GCR_CPSEL(1) |       // Enable the LCD charge pump
      LCD_GCR_FFR(1)     |     // Fast Frame Rate
      LCD_GCR_SOURCE(1)  |     // Use alternate clock source
      LCD_GCR_LCLK(4)    |     // Select LCD prescaler
      LCD_GCR_DUTY(3);         // Four phases

  /* Select the pins used*/
  /* Pin Enable Register Low   LCD_P0 to LCD_P31 */
  LCD->PEN[0] = 1<<7   |         //LCD_P7      -> Pin 7  s401
      1<<8   |         //LCD_P8      -> Pin 8  s401
      1<<10  |         //LCD_P10     -> Pin 11 s401
      1<<11  |         //LCD_P11     -> Pin 12 s401
      1<<17  |         //LCD_P17     -> Pin 6  s401
      1<<18  |         //LCD_P18     -> Pin 4  s401
      1<<19 ;          //LCD_P19     -> Pin 3  s401

  /* Pin Enable Register High   LCD_P32 to LCD_P63
	   bit = pin-32 */
  LCD->PEN[1] = 1<<5   |         //LCD_P37     -> Pin 5 s401
      1<<6   |         //LCD_P38     -> Pin 7 s401
      1<<8   |         //LCD_P40     -> Pin 1 s401
      1<<20  |         //LCD_P52     -> Pin 2 s401
      1<<21 ;          //LCD_P53     -> Pin 9 s401

  /* Select the pins to use for Backplane */
  /* Backplane Enable Register Low   LCD_P0 to LCD_P31 */
  LCD->BPEN[0] =   1<<18  |         //LCD_P18    COM3
      1<<19 ;          //LCD_P19    COM2

  /* Backplane Enable Register High   LCD_P32 to LCD_P63
	   bit = pin-32 */
  LCD->BPEN[1] =    1<<8   |         //LCD_P40   COM0
      1<<20;           //LCD_P52   COM1

  /*  Waveform registers for the backplane pins
	    Set which phase the backplane signals are active in
	    only one COM signal is active in each phase*/
  LCD->WF8B[40] = 0b0001;   // COM0 is active in the first phase
  LCD->WF8B[52] = 0b0010;   // COM1                  second phase
  LCD->WF8B[19] = 0b0100;   // COM2                  third phase
  LCD->WF8B[18] = 0b1000;   // COM3                  last phase

  //Enable the LCD controller
  LCD->GCR |= LCD_GCR_LCDEN(1);
}

void HexDigit(unsigned char c)
{
  /*Display just one hexadecimal digit on the LCD*/

  //Digit 1       The most significant digit
  LCD->WF8B[37] = wfb[c];   // Waveform register for pin 5 of the LCD
  LCD->WF8B[17] = wfa[c];   // Waveform register for pin 6 of the LCD

  //Digit 1       The most significant digit
  //LCD->WF8B[37] = wfb[1];   // Waveform register for pin 5 of the LCD
  //LCD->WF8B[17] = wfa[1];   // Waveform register for pin 6 of the LCD
}


void LCDNumberLeft(unsigned char n)
{
  /*Display a two digit decimal number on the LCD*/

  unsigned short d1;
  unsigned short d2;

  //Put your code here
  //Convert n into its separate digits
d1 = n/10;
d2 = n%10;

  //Digit 1       The most significant digit
  LCD->WF8B[37] = wfb[d1];   // Waveform register for pin 5 of the LCD
  LCD->WF8B[17] = wfa[d1];   // Waveform register for pin 6 of the LCD

  //Digit 2
  LCD->WF8B[7] = wfb[d2];   // Waveform register for pin 7 of the LCD
  LCD->WF8B[8] = wfa[d2];   // Waveform register for pin 8 of the LCD

}

void LCDNumberRight(unsigned char n)
{
  /*Display a two digit decimal number on the LCD*/

  unsigned short d3;
  unsigned short d4;

  // Put your code here
  d3 = n/10;
  d4 = n%10;


  //Digit 3     The most significant digit
  LCD->WF8B[53] = wfb[d3];   // Waveform register for pin 9 of the LCD
  LCD->WF8B[38] = wfa[d3];   // Waveform register for pin 10 of the LCD

  //Digit 4
  LCD->WF8B[10] = wfb[d4];   // Waveform register for pin 11 of the LCD
  LCD->WF8B[11] = wfa[d4]; // Waveform register for pin 12 of the LCD
  // Add one to turn the colon between the two digits on
  // LCD->WF8B[11] = wfa[d4] + 1;   // Waveform register for pin 12 of the LCD

}
