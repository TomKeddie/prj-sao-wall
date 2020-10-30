#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Pins
// A_PWM   OC0A PB7 (Arduino D11)
// B_PWM   OC3A PC6 (Arduino D5)
// C_PWM   OC1A PB5 (Arduino D9)
// D_GPIO2 OC1B PB6 (Arduino D10)
// D_GPIO1 OC4D PD7 (Arduino D6)

#define A_GPIO1 PF0
#define A_GPIO2 PF1
#define C_GPIO1 PF4
#define C_GPIO2 PF5

static volatile uint8_t m_level0 = 4+0*256/5;
static volatile uint8_t m_level1 = 4+1*256/5;
static volatile uint8_t m_level2 = 4+2*256/5;
static volatile uint8_t m_level3 = 4+3*256/5;
static volatile uint8_t m_level4 = 4+4*256/5;


static void update_ocr(void) {
    OCR0A = m_level0;
    OCR3A = m_level1;
    OCR1A = m_level2;
    OCR1B = m_level3;
    OCR4D = m_level4;
}    

void main(void) {
    DDRB = (1 << DDB5) | (1 << DDB6) | (1 << DDB7);
    DDRC = (1 << DDC6);
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
    static uint8_t increment0 = 1;
    static uint8_t increment1 = 1;
    static uint8_t increment2 = 1;
    static uint8_t increment3 = 1;
    static uint8_t increment4 = 1;
    if (++counter == 8) {
        counter = 0;
        if (increment0) m_level0++; else m_level0--;
        if (increment1) m_level1++; else m_level1--;
        if (increment2) m_level2++; else m_level2--;
        if (increment3) m_level3++; else m_level3--;
        if (increment4) m_level4++; else m_level4--;
        if (m_level0 == 0xff) increment0 = 0; if (m_level0 == 4) increment0 = 1;
        if (m_level1 == 0xff) increment1 = 0; if (m_level1 == 4) increment1 = 1;
        if (m_level2 == 0xff) increment2 = 0; if (m_level2 == 4) increment2 = 1;
        if (m_level3 == 0xff) increment3 = 0; if (m_level3 == 4) increment3 = 1;
        if (m_level4 == 0xff) increment4 = 0; if (m_level4 == 4) increment4 = 1;
        update_ocr();
    }
}

