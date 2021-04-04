#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/delay.h>
int adc_left, adc_right, adc_middle;
uint8_t pwm_left, pwm_right;


int main(void)
{
	//ADC configuration
	ADMUX =(1<<REFS0); //AVcc reference with external capacitor at AREF pin
	ADCSRA =(1<<ADEN) // enable
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	//PWM configuration
	TCCR1A = (1<<COM1A1)|(1<<COM1B1) //clear OC1B on Compare Match
	|(1<<WGM10);
	TCCR1B =(1<<WGM12)//Fast PWM 8-bit
	|(1<<CS10)|(1<<CS11); //prescaler = 64;
	OCR1B = 0;
	OCR1A = 0;
	//I/O configuration
	//pd7.pb0
	//pd6,5
	//DDRB&=~(1<<PB0);
	
	DDRB|=(1<<PB1)|(1<<PB2);
	DDRD|=(1<<PD5)|(1<<PD7);
	PORTD|=(1<<PD5)|(1<<PD7);
	DDRC&=~(1<<PC1)&(1<<PC0)&(1<<PC2);
	
	
	
	while (1)
	{
		ADMUX &=0b11110000;//ADC0
		ADCSRA |= (1<<ADSC);
		while(ADCSRA &(1<<ADSC));
		adc_left = ADC;
		
		_delay_ms(10);
		
		ADMUX |= (1<<MUX0); //ADC1
		ADCSRA |= (1<<ADSC);
		while(ADCSRA &(1<<ADSC));
		adc_middle = ADC;
		
		_delay_ms(10);
		
		ADMUX &=0b11110000;
		ADMUX |=(1<<MUX1);//ADC2
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_right = ADC;
		
		_delay_ms(10);
		
		if((adc_left>adc_right)&&(adc_left>adc_middle&&(adc_left>300)))
		{
			OCR1A = 0;
			OCR1B = 150;
		}
		
		else if((adc_right>adc_left)&&(adc_right>adc_middle)&&(adc_right>300))
		{
			
			OCR1A=150;
			OCR1B=0;
			
		}
		else if ((adc_middle>adc_left)&&(adc_middle>adc_right)&&(adc_middle>300))
		{
			OCR1A=150;
			OCR1B=150;
		}
		else
		{
			
			OCR1A=0;
			OCR1B=0;
		}
		
		
		_delay_ms(50);
	}
}

