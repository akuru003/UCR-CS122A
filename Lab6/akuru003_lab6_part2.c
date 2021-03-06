/*	Partner(s) Name & E-mail: Abraham Kuruvila akuru003@ucr.edu No Partner
 *	Lab Section: 21
 *	Assignment: Lab 6 Part 2
 *	Exercise Description: Lighting led matrix across depending on button press
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <usart_ATmega1284.h>
#include <avr/pgmspace.h> 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "keypad.h"
#include "croutine.h" 
#include "lcd.h"
unsigned char temp = 0x00;
int counter = 0;
int counter1 = 0;
unsigned char ground = 0xFF;
unsigned char lights = 0x00;


// rows go down, columns go right to left
void setGround()
{
	for(int i = 7; i >= 0; i--)
	{
		PORTC = 0x88;
		PORTD = 0x88;
		PORTC |= ((lights >> i) & 0x01);
		PORTD |= ((ground >> i) & 0x01);
		PORTC |= 0x44;
		PORTD |= 0x44;
	}
	PORTC |= 0x22;
	PORTD |= 0x22;
	PORTC = 0x00;
	PORTD = 0x00;
}




enum States {start, s1, s2, s3,s4, s5, s6, s7} State;	
void LEDS_Init(){
	State = start;
}
void LEDS_Tick()
{
	switch(State)
	{
		case start:
		ground = 0x00;
		lights = 0x80;
		setGround();
		if(GetBit(PINB, 0) == 0)
		{
			State = s1;
		}
		else
		{
			State = start;
		}
		break;
		
		case s1:
		ground = 0x00;
		lights = 0x40;
		setGround();
		if(GetBit(PINB, 0) == 0)
		{
			State = s2;
		}
		else if(GetBit(PINB, 1) == 0)
		{
			State = start;
		}
		else
		{
			State = s1;
		}
		break;
		
		case s2:
		ground = 0x00;
		lights = 0x20;
		setGround();
		if(GetBit(PINB, 0) == 0)
		{
			State = s3;
		}
		else if(GetBit(PINB, 1) == 0)
		{
			State = s1;
		}
		else
		{
			State = s2;
		}
		break;
		
		case s3:
		ground = 0x00;
		lights = 0x10;
		setGround();
		if(GetBit(PINB, 0) == 0)
		{
			State = s4;
		}
		else if(GetBit(PINB, 1) == 0)
		{
			State = s2;
		}
		else
		{
			State = s3;
		}
		break;
		
		case s4:
		ground = 0x00;
		lights = 0x08;
		setGround();
		if(GetBit(PINB, 0) == 0)
		{
			State = s5;
		}
		else if(GetBit(PINB, 1) == 0)
		{
			State = s3;
		}
		else
		{
			State = s4;
		}
		break;
		
		case s5:
		ground = 0x00;
		lights = 0x04;
		setGround();
		if(GetBit(PINB, 0) == 0)
		{
			State = s6;
		}
		else if(GetBit(PINB, 1) == 0)
		{
			State = s4;
		}
		else
		{
			State = s5;
		}
		break;
		
		case s6:
		ground = 0x00;
		lights = 0x02;
		setGround();
		if(GetBit(PINB, 0) == 0)
		{
			State = s7;
		}
		else if(GetBit(PINB, 1) == 0)
		{
			State = s5;
		}
		else
		{
			State = s6;
		}
		break;
		
		case s7:
		ground = 0x00;
		lights = 0x01;
		setGround();
		if(GetBit(PINB, 1) == 0)
		{
			State = s6;
		}
		else
		{
			State = s7;
		}
		break;
	}
}
void LedSecTask()
{
	LEDS_Init();
   for(;;) 
   { 	
	LEDS_Tick();
	vTaskDelay(100); 
   } 
}



void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	

int main(void) 
{ 
   DDRA = 0xFF; PORTA = 0x00;
   DDRB = 0x00; PORTB = 0xFF;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   
   StartSecPulse(1);
   //RunSchedular 
   vTaskStartScheduler();
   return 0; 
}



