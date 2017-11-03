/*
 * Ultrasonic sensor HC-05 interfacing with ATmega328p display test
 * Reference: https://circuitdigest.com/microcontroller-projects/distance-measurement-using-hc-sr04-avr
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "LCD_16x2_H_file.h"/* Include LCD header file */

#define  Trigger_pin	PC0	/* Trigger pin */

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;		/* Increment Timer Overflow count */
}

int main(void)
{
	char string[10];
	long count;
	double distance;
	
	DDRC = 0x01;			/* Make trigger pin as output */
	PORTC = 0xFF;			/* Turn on Pull-up */
	
	LCD_Init();
	//LCD_Clear ();
	LCD_String_xy(1, 0, "Ultrasonic");
	
	sei();					/* Enable global interrupt */
	TIMSK1 = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;				/* Set all bit to zero Normal operation */
	
	while(1)
	{

		PORTC |= (1 << Trigger_pin);/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		_delay_us(10);
		PORTC &= (~(1 << Trigger_pin));
		/* calculation for distance*/
		TCNT1 = 0;			/* Clear Timer counter */
		TCCR1B = 0x41;		/* Setting for capture rising edge, No prescaler*/
		TIFR1 = 1<<ICF1;		/* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1<<TOV1;		/* Clear Timer Overflow flag */

		/*Calculate width of Echo by Input Capture (ICP) on PortD PD6 */
		
		while ((TIFR1 & (1 << ICF1)) == 0);	/* Wait for rising edge */
		TCNT1 = 0;			/* Clear Timer1 counter */
		TCCR1B = 0x01;		/* Setting for capture falling edge, No prescaler */
		TIFR1 = 1<<ICF1;		/* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1<<TOV1;		/* Clear Timer Overflow flag */
		TimerOverflow = 0;	/* Clear Timer overflow count */

		while ((TIFR1 & (1 << ICF1)) == 0); /* Wait for falling edge */
		count = ICR1 + (65535 * TimerOverflow);	/* Take value of capture register */
		/* 8MHz Timer freq, sound speed =343 m/s, calculation mentioned in doc. */
		distance = ((double)count / 466.47)/2;

		dtostrf(distance, 2, 2, string);/* Convert distance into string */
		strcat(string, " cm   ");
		LCD_String_xy(2, 0, "Dist = ");
		LCD_String_xy(2, 7, string);	/* Print distance on LDC16x2 */
		_delay_ms(200);
	}
}
