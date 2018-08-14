#include <avr/io.h>
#include <avr/interrupt.h>

// Timer 0 overflow vector - this runs every time timer0 overflows
// Frequency / Prescaling / Counter (9600000Hz / 1 / 256 = 37500Hz)
ISR(TIM0_OVF_vect) {
    // Do Some Stuff
}

void setup() {
	TCCR0B = (1<<CS00);     // clock frequency with no prescaling
	OCR0B = 0x00;           // Output compare
	TCNT0 = 0;              // Set counter 0 to zero (8Bit Couner = 256)
	TIMSK0 = (1<<TOIE0);    // Enable overflow interrupt

	sei(); // Enable interrupts
}

int main(void) {
    setup();

    while (1) {
    }

    return 0;
}
