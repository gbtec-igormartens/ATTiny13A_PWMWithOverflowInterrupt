// expected F_CPU 9600000

#include <avr/io.h>
#include <avr/interrupt.h>

// Defines on which value the pwmCounter should be set to 0
#define PWM_COUNTER_RESET_VAL 100
#define PWM_HIGH_VALUE 49

// Values "pwmCounter <= PWM_HIGH_VALUE" should produce on output HIGH
// Values "pwmCounter > PWM_HIGH_VALUE" should produce on output LOW
volatile uint8_t pwmCounter =0;
volatile bool isHigh = true;

// Timer 0 overflow vector - this runs every time timer0 overflows
// Frequency / Prescaling / Counter (9600000Hz / 1 / 256 = 37500Hz)
// Interrupt Frequency / PWM_COUNTER_RESET_VAL (37500Hz / 100 = 375Hz)
ISR(TIM0_OVF_vect) {
    ++pwmCounter;

    // Reset pwmCounter if the maximum value is reached and set the output to HIGH
    if (pwmCounter >= PWM_COUNTER_RESET_VAL) {
        pwmCounter = 0;
        isHigh = true;

    }

    // Set the output to LOW if the PWM_HIGH_VALUE is exceeded
    else if (pwmCounter > PWM_HIGH_VALUE) {
        isHigh = false;
    }
}

void setup() {
    DDRB |= (1<<DDB4);      // Setup the output port for the PWM-Signal

	TCCR0B = (1<<CS00);     // clock frequency with no prescaling
	OCR0B = 0x00;           // Output compare
	TCNT0 = 0;              // Set counter 0 to zero (8Bit Couner = 256)
	TIMSK0 = (1<<TOIE0);    // Enable overflow interrupt

	sei(); // Enable interrupts
}

int main(void) {
    setup();

    while (1) {
        if (isHigh) {
            PORTB |= (1<<PB4);
        } else {
            PORTB &= ~(1<<PB4);
        }
    }

    return 0;
}
