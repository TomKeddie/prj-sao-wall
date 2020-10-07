#include <stdlib.h>
#include <avr/io.h>

// Pins
// OC0A PB7 (Arduino D11)
// OC1A PB5 (Arduino D9)
// OC1B PB6 (Arduino D10)
// OC3A PC6 (Arduino D5)
// OC4A PC7 (Arduino D13)
// OC4D PD7 (Arduino D6)

#define A_PWM PB7
#define A_GPIO1 PF0
#define A_GPIO2 PF1

#define B_GPIO1 PC6
#define B_GPIO2 PC7

#define C_PWM   PB5
#define C_GPIO1 PF4
#define C_GPIO2 PF5

#define D_GPIO1 PD7
#define D_GPIO2 PB6


int main(void) {
    DDRB = (1 << DDB5) | (1 << DDB6) | (1 << DDB7);
    DDRC = (1 << DDC6) | (1 << DDC7);
    DDRD = (1 << DDD7);

    TCCR0A = (1 << WGM00) | (1 << COM0A0) | (1 << COM0A1); // 8 bit, non inverting on A
    TCCR0B = (1 << CS02); // Set up timer as /256 (16000000 / 256) = 62500 Hz / 512 = 122 Hz
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0); // 8 bit, non-inverting on A and B
    TCCR1B = (1 << CS12); // Set up timer as /256 (16000000 / 256) = 62500 Hz / 512 = 122 Hz
    TCCR3A = (1 << WGM30) | (1 << COM3A0) | (1 << COM3A1); // 8 bit, non inverting on A
    TCCR3B = (1 << CS32); // Set up timer as /256 (16000000 / 256) = 62500 Hz / 512 = 122 Hz
    TCCR4A = (1 << PWM4A) | (1 << COM4A0) | (1 << COM4A1); // 8 bit, non inverting on A
    TCCR4B = (1 << CS43) | (1 << CS40); // Set up timer as /256 (16000000 / 256) = 62500 Hz / 512 = 122 Hz
    TCCR4C = (1 << PWM4D) | (1 << COM4D0) | (1 << COM4D1); // 8 bit, non inverting on D
    TCCR4D = (1 << WGM40); // phase/freq correct

    OCR0A = 63;
    OCR1A = 63;
    OCR1B = 63;
    OCR3A = 63;
    OCR4A = 63;
    OCR4D = 63;
    
    for (;;);
    
	return 0;
}
