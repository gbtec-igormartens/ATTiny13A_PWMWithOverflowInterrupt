#include <avr/io.h>
#include <avr/interrupt.h>

#define PULSE_RESET_VAL 750

#define MIN_ROTARY_VAL 22
#define MAX_ROTARY_VAL 88

volatile uint16_t pulseResetCounter = 0;
volatile uint16_t setLow = MIN_ROTARY_VAL; // When the pwm signal should set to low

uint16_t timer = 0;

//Timer 0 overflow vector - this runs every time timer0 overflows
ISR(TIM0_OVF_vect) {
	++pulseResetCounter;
}

void setup() {
	DDRB |= (1<<DDB4);

	TCCR0B = (1<<CS00); // clock frequency with no prescaling
	OCR0B = 0x00;  // Output compare
	TCNT0 = 0; // Set counter 0 to zero
	TIMSK0 = (1<<TOIE0); // Enable overflow interrupt

	sei();
}

int main(void) {

    setup();

    while (1) {
		if (pulseResetCounter >= PULSE_RESET_VAL) {
			pulseResetCounter = 0;
			PORTB |= (1<<PB4);
		}

		if (pulseResetCounter >= setOff) {
			PORTB &= ~(1<<PB4);
		}

		++timer;
		if (timer == 20000) {
			timer = 0;
			++setLow;
		}

		if (setLow >= MAX_ROTARY_VAL) {
			setLow = MIN_ROTARY_VAL;
		}
    }

    return 0;
}
