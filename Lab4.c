#define F_CPU 16000000UL

#include <avr/io.h>

int main(void){
	
	DDRB |= (1<<DDB5); //Sets up port B5 for the LED
	DDRB &= ~(1 << DDB7); //Sets up port D7 (button) as input
	PORTD |= (1 << PORTD7); //Enables pull up resistor
	
	TCCR0A |= (1 << WGM01); // Set the Timer Mode to CTC
	
	OCR0A = 0x9B; //represents 10ms timer
	OCR0B = 0x00; //represents 50% duty cycle
	
	TCCR0B |= (CS02 << 1) | (0 << CS01) | (1 << CS00); // set prescaler to 1024 and start the timer
	
	
	
	while (1){
		if ((PINB & (1 << PINB7)) == 0) {      //if button is being pressed
			if(OCR0B < 0x9A){	//don't over increment
				OCR0B++;
			}
		} 
		else {
			if(OCR0B > 0x00){	//don't increment below 0
				OCR0B--;
			}
		}
		//CREATE A LOGIC 1 SIGNAL (E.G. TURN ON LED)
		while ( (TIFR0 & (1 << OCF0B) ) == 0 && OCR0B > 0){
			PORTB |= (1<<PORTB5); //Set port bit B5 to 1 to turn on the LED
		} // wait for OCR0B overflow event
		
		TIFR0 |= (1 << OCF0B); // reset OCR0B overflow flag
		
		//CREATE A LOGIC 0 SIGNAL (E.G. TURN OFF LED)
		while ( (TIFR0 & (1 << OCF0A) ) == 0 /*&& OCR0B > 0*/){
			PORTB &= ~(1<<PORTB5); //Clear port bit B5 to 0 to turn off the LED
		} // wait for OCR0A overflow event
		
		TIFR0 |= (1 << OCF0A); // reset OCR0A overflow flag
		
	}
}
