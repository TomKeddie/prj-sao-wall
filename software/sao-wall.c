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

static volatile uint8_t m_level0 = 0;
static volatile uint8_t m_level1 = 2;
static volatile uint8_t m_level2 = 5;
static volatile uint8_t m_level3 = 8;
static volatile uint8_t m_level4 = 11;
static volatile uint8_t m_level5 = 14;

static const uint8_t m_level_to_count[] = {15, 1*16+15, 2*16+15, 3*16+15, 4*16+15, 5*16+15, 6*16+15, 7*16+15, 8*16+15, 9*16+15, 10*16+15, 11*16+15, 12*16+15, 13*16+15, 14*16+15, 15*16+15};

static void update_ocr(void) {
    OCR0A = m_level_to_count[m_level0];
    OCR3A = m_level_to_count[m_level1];
    OCR4A = m_level_to_count[m_level2];
    OCR1A = m_level_to_count[m_level3];
    OCR1B = m_level_to_count[m_level4];
    OCR4D = m_level_to_count[m_level5];
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
    static uint8_t increment0 = 1;
    static uint8_t increment1 = 1;
    static uint8_t increment2 = 1;
    static uint8_t increment3 = 1;
    static uint8_t increment4 = 1;
    static uint8_t increment5 = 1;
    if (++counter == 122) {
        counter = 0;
        if (increment0) m_level0++; else m_level0--;
        if (increment1) m_level1++; else m_level1--;
        if (increment2) m_level2++; else m_level2--;
        if (increment3) m_level3++; else m_level3--;
        if (increment4) m_level4++; else m_level4--;
        if (increment5) m_level5++; else m_level5--;
        if (m_level0 == 15) increment0 = 0; if (m_level0 == 0) increment0 = 1;
        if (m_level1 == 15) increment1 = 0; if (m_level1 == 0) increment1 = 1;
        if (m_level2 == 15) increment2 = 0; if (m_level2 == 0) increment2 = 1;
        if (m_level3 == 15) increment3 = 0; if (m_level3 == 0) increment3 = 1;
        if (m_level4 == 15) increment4 = 0; if (m_level4 == 0) increment4 = 1;
        if (m_level5 == 15) increment5 = 0; if (m_level5 == 0) increment5 = 1;
        update_ocr();
    }
}

