#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Pins
// A_PWM   OC0A PB7 (Arduino D11)
// B_GPIO1 OC3A PC6 (Arduino D5)
// B_GPIO2 OC4A PC7 (Arduino D13)
// C_PWM   OC1A PB5 (Arduino D9)
// D_GPIO2 OC1B PB6 (Arduino D10)
// D_GPIO1 OC4D PD7 (Arduino D6)

#define A_GPIO1 PF0
#define A_GPIO2 PF1
#define C_GPIO1 PF4
#define C_GPIO2 PF5

static volatile uint8_t m_level = 0;

static const uint8_t m_level_to_count[] = {15, 1*16+15, 2*16+15, 3*16+15, 4*16+15, 5*16+15, 6*16+15, 7*16+15, 8*16+15, 9*16+15, 10*16+15, 11*16+15, 12*16+15, 13*16+15, 14*16+15, 15*16+15};

static void update_ocr(void) {
    OCR0A = m_level_to_count[(m_level +  0) % sizeof(m_level_to_count)];
    OCR3A = m_level_to_count[(m_level +  2) % sizeof(m_level_to_count)];
    OCR4A = m_level_to_count[(m_level +  5) % sizeof(m_level_to_count)];
    OCR1A = m_level_to_count[(m_level +  8) % sizeof(m_level_to_count)];
    OCR1B = m_level_to_count[(m_level + 11) % sizeof(m_level_to_count)];
    OCR4D = m_level_to_count[(m_level + 14) % sizeof(m_level_to_count)];
}    

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

    update_ocr();

    while(1) {} ;
}

ISR(TIMER0_OVF_vect) {
    static uint8_t counter = 0;
    static uint8_t increment = 1;
    if (++counter == 122) {
        counter = 0;
        if (increment) m_level++; else m_level--;
        if (m_level == 15) {
            increment = 0;
        } else if (m_level == 0) {
            increment = 1;
        }
        update_ocr();
    }
}

