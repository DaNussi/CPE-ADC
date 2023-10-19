#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void writeBytes(int data) {
    PORTC &= 0b00110000;
    PORTB &= 0b00000000;
    PORTC |= (data & 0b11110000) >> 4;
    PORTB |= (data & 0b00001111) << 0;
}

void setEOC() {
    PORTC |=  (1 << PC4);
}

void clearEOC() {
    PORTC &= ~(1 << PC4);
}

void setDataValid() {
    PORTC |=  (1 << PC5);
}

void clearDataValid() {
    PORTC &= ~(1 << PC5);
}

int startConvertion() {
    int data = 0;
    int last = 0;
    writeBytes(data);
    _delay_ms(100);
    for (int i = 0; i < 8; i++) {

        last = data;
        data |= 1 << i;
        writeBytes(data);
        _delay_ms(100);

        if(ACSR & (1 << ACO)) {
            data = last;
        }
    }
    _delay_ms(1000);
    return data;
}

void configureComperatorAndPins() {

    // ADCSRA – ADC Control and Status Register A
    ADCSRA &= ~(1 << ADEN); // Disable the ADC ###

    // ADCSRB – ADC Control and Status Register B
    ADCSRB &= ~(1 << ACME);   // Disable Multiplexer


    // ACSR – Analog Comparator Control and Status Register
    ACSR &= ~(1 << ACD);  // Clear bit 7 of ACSR to enable the comparator
    ACSR &= ~(1 << ACBG); // Clear bit 6 of ACSR to disable the comparator band gap select

    // ADMUX – ADC Multiplexer Selection Register
    ADMUX |= (1 << MUX0);
    ADMUX |= (1 << MUX1);

    // DIDR1 – Digital Input Disable Register 1
    DIDR1  |= (1 << AIN1D | 1 << AIN0D);   // Clear bit 1 of DIDR1 to disable the digital input buffer

    // Set Output / Input
    DDRB = 0b00001111;
    DDRC = 0b00111111;
    DDRD  &= ~((1 << PD6) | (1 << PD7)); // Set PC0 and PC1 as inputs
    PORTD &= ~((1 << PD6) | (1 << PD7)); // Disable Pull-Up Resistors on PC0 and PC1

}

int main(void) {

    // SREG – AVR Status Register
    cli(); // Clear bit 7 of SREG to disable global interrupts

    configureComperatorAndPins();

    // SREG – AVR Status Register
    sei(); // Set bit 7 of SREG to enable global interrupts


    while (true) {

        startConvertion();
    }
}
