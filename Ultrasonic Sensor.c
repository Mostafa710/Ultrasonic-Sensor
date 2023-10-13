#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "std_macros.h"
#include "LCD.h"

int main(void)
{
	unsigned short a,b;
	unsigned short distance,high;
	LCD_init();
	DIO_setPINDir('D',7,1);
	TCCR1A=0;
	/* Clear the flag bit (Important) */
	SET_BIT(TIFR,ICF1);
    while(1)
    {
		/* Send trigger pulse */
		DIO_writePIN('D',7,1);
		_delay_us(50);
		DIO_writePIN('D',7,0);
		/* Detect rising edge of Echo pulse, Frequency of timer 1 = 1MHz, Noise canceler ON */
		TCCR1B=0xC2;
		while(READ_BIT(TIFR,ICF1) == 0);
		SET_BIT(TIFR,ICF1);
		a=ICR1;
		/* Detect falling edge of Echo pulse, Frequency of timer 1 = 1MHz, Noise canceler ON */
		TCCR1B=0x82;
		while(READ_BIT(TIFR,ICF1) == 0);
		SET_BIT(TIFR,ICF1);
		b=ICR1;
		high = b-a;
		distance = (34600*high*4)/F_CPU;
		if(distance <= 80)
		{
			if(distance >= 10)
			{
				LCD_clear_screen();
				LCD_send_string("Distance=");
				LCD_send_char(distance/10+48);
				LCD_send_char(distance%10+48);
				LCD_send_string("cm");
				_delay_ms(1000);
			}
			else
			{
				LCD_clear_screen();
				LCD_send_string("Distance=");
				LCD_send_char(distance+48);
				LCD_send_string("cm");
				_delay_ms(1000);
			}
		}
		else
		{
			LCD_clear_screen();
			LCD_send_string("No Object");
			_delay_ms(1000);
		}
    }
}