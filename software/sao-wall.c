#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

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

void main(void) {
#if 0     
    // Power Reduction Register 0
    PRR0    =   (0<<PRTWI)  |   // Bit 7 - PRTWI: TWI
                (0<<PRTIM2) |   // Bit 6 - Res: Reserved bit
                (0<<PRTIM0) |   // Bit 5 - PRTIM0: Timer/Counter0
                (0<<PRTIM1) |   // Bit 3 - PRTIM1: Timer/Counter1
                (0<<PRSPI)  |   // Bit 2 - PRSPI: SPI
                (0<<PRADC);     // Bit 0 - PRADC: PADC

    // Power Reduction Register 1
    PRR1    =   (0<<PRUSB)  |   // Bit 7 - PRUSB: USB
                (0<<PRTIM4) |   // Bit 4- PRTIM4: Timer/Counter4
                (0<<PRTIM3) |   // Bit 3 - PRTIM3: Timer/Counter3
                (0<<PRUSART1);  // Bit 0 - PRUSART1: USART1
#endif    
    
    DDRB = (1 << DDB5) | (1 << DDB6) | (1 << DDB7);
    DDRC = (1 << DDC6) | (1 << DDC7);
    DDRD = (1 << DDD7) | (1 << DDD1);
    DDRF = (1 << DDF5) | (1 << DDF4) | (1 << DDF1) | (1 << DDF0);

    // use timer 0 to drive the level change logic
    TIFR0  = 0;
    TIMSK0 = (1 << TOIE0);
    
    TCCR0A = (1 << WGM00) | (1 << COM0A0) | (1 << COM0A1); // 8 bit, non inverting on A
    TCCR0B = (1 << CS02); // Set up timer as /256 (16000000 / 256) = 62500 Hz / 512 = 122 Hz
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0); // 8 bit, non-inverting on A and B
    TCCR1B = (1 << CS12); // Set up timer as /256 (16000000 / 256) = 62500 Hz / 512 = 122 Hz
    TCCR3A = (1 << WGM30) | (1 << COM3A0) | (1 << COM3A1); // 8 bit, non inverting on A
    TCCR3B = (1 << CS32); // Set up timer as /256 (16000000 / 256) = 62500 Hz / 512 = 122 Hz
    TCCR4A = (1 << PWM4A) | (1 << COM4A0) | (1 << COM4A1); // 8 bit, non inverting on A
    TCCR4B = (1 << CS43) | (1 << CS40); // Set up timer as /256 (16000000 / 256) = 62500 Hz / 512 = 122 Hz
    TCCR4C = (1 << COM4A1S) | (1 << COM4A0S) | (1 << PWM4D) | (1 << COM4D0) | (1 << COM4D1); // 8 bit, non inverting on D
    TCCR4D = (1 << WGM40); // phase/freq correct

    sei();

    PORTF |= 1;
    PORTF &= !1;

    PORTF |= 1;
    PORTF &= !1;

    OCR0A = 40;
    OCR1A = 80;
    OCR1B = 120;
    OCR3A = 160;
    OCR4A = 200;
    OCR4D = 240;

    while(1) {} ;
}

static volatile uint8_t counter = 0;
ISR(TIMER0_OVF_vect) {
    if (++counter == 121) {
        counter = 0;
        PORTD |= 2;
        PORTD &= !2;
    }
}

