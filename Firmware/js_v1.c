/*
 * joystick.c
 * Controlling the wheelchair with joystick 
 * Four direction: Forward, Backward, Left, Right
 * Reference: https://circuitdigest.com/microcontroller-projects/joystick-interfacing-with-atmega8
 * Created: 10/31/2017 3:43:23 PM
 * Author : Khang Nguyen
 */ 

#include <avr/io.h>
#define F_CPU 1000000//Internal clock at 1MHZ
#include <util/delay.h>



int main(void)
{
		//Set Port B and D as output
		DDRD=0x0F;
		PORTD = 0;
		_delay_ms(50); //delay of 50ms
		//Set Port C as input
		DDRC=0; 
		PORTC = (00 << PINC1);

		int H=141;// Set starting position on x-axis
		int V=151;// set starting position on y-axis
		int xmov=0;
		int ymov=0;

		//Setting reference of ADC
		ADMUX|=(1<<REFS0);
		//Enable ADC and set pre-scalar to 128
		ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<ADPS1)|(1<<ADPS0);

		while(1)
		{
			//switch between channel
			switch(ADMUX)
			{
				//When ADMUX== 0x40
				case 0x40:
				{
					//Start ADC conversion
					ADCSRA|=(1<<ADSC);
					//Wait for ADC conversion
					while(!(ADCSRA&(1<<ADIF)))
					{	//Horizontal moving value
						xmov=ADC; 
						//Reset ADC reg
						ADC=0;
						//Change channel
						ADMUX=0x41;
						break;
					}
				}

				//When ADMUX==0x41
				case 0x41:
				{
					//Start ADC conversion
					ADCSRA!=(1<<ADSC);
					//Wait til ADC conversion
					while(!(ADCSRA&(1<<ADIF)));
					{
						//Vertical value
						ymov=ADC;
						//Reset ADC reg;
						ADC=0;
						//Change channel
						ADMUX=0x40;
						break;
					}

				}
			}
			//Moving backward
		if(ymov<V-50)
		{
			PORTD|=(1<<PIND3);
			_delay_ms(5);
			//PORTD &= ~(1<<PIND3);
			PORTD = 0;
		}
		//Moving forward
		else if(ymov>V+50)
		{
			PORTD |=(1<<PIND2);
			_delay_ms(5);
			PORTD &= ~(1<<PIND2);
		}
		//Moving left
		else if(xmov<H+50)
		{
			PORTD |=(1<<PIND1);
			_delay_ms(5);
			PORTD &= ~(1<<PIND1);
		}
		//Moving right
		else if(xmov>H-50)
		{
			PORTD|=(1<<PIND0);
			_delay_ms(5);
			PORTD&= ~(1<<PIND0);
		}
		//Default: No movement, LEDS off
		else{
			PORTD &= ~(1111<<PIND3);
		}
	}
}
