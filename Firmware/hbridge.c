/*
 * hbridge.c
 * Reference: https://electrosome.com/interfacing-dc-motor-atmega32-l293d/#Control_Signals_and_Motor_Status
 * Created: 11/1/2017 10:48:46 AM
 * Author : chamberlain
 */ 
#define F_CPU 8000000UL // 16MHz clock speed

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0xFF; //PORTC as Output
	while(1)
	{
		//Rotates Motor in AntiClockwise
		PORTC = 0x05; //00000101
		_delay_ms(10);

		//Stops Motor
		PORTC = 0x00; //00000000
		_delay_ms(10);

		//Rotates Motor in Clockwise
		PORTC = 0x0A; //00001010
		_delay_ms(10);

		//Stops Motor
		PORTC = 0x0F; //00001111
		_delay_ms(10);
	}
}
