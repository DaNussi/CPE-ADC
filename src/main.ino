#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// Datasheet
// Analog Comparator Block Diagram
// PD7 - AIN1 - Input
// PD6 - AIN0 - Input


void writeBytes(int data) {
    PORTC = (data & 0b11110000) >> 4;
    PORTB = (data & 0b00001111) << 0;

    // PORTB = data & 0b00111111;
    // PORTC &= 0b00001100;
    // PORTC |= (data & 0b11000000) >> 4;

    //PORTD=data;

}

int main(void) {

    // SREG – AVR Status Register
    cli(); // Clear bit 7 of SREG to disable global interrupts



    // ADCSRA – ADC Control and Status Register A
    ADCSRA &= ~(1 << ADEN); // Disable the ADC ###

    // ADCSRB – ADC Control and Status Register B
    ADCSRB &= ~(1 << ACME);   // Disable Multiplexer
    // ADCSRB &= ~(1 << ADTS0); // Clear bit 0 of ADCSRB to ???
    // ADCSRB &= ~(1 << ADTS1); // Clear bit 1 of ADCSRB to ???
    // ADCSRB &= ~(1 << ADTS2); // Clear bit 2 of ADCSRB to ???


    // ACSR – Analog Comparator Control and Status Register
    ACSR &= ~(1 << ACD);  // Clear bit 7 of ACSR to enable the comparator
    ACSR &= ~(1 << ACBG); // Clear bit 6 of ACSR to disable the comparator band gap select
    // TODO ACO           // Read  bit 5 of ACSR to get the comparator output
    // ACSR &= ~(1 << ACI);  // Clear bit 4 of ACSR to disable the comparator interrupt flag
    // ACSR |= (1 << ACIE);  // Set   bit 3 of ACSR to enable the comparator interrupt enable
    // ACSR &= ~(1 << ACIC); // Clear bit 2 of ACSR to disable the comparator input capture
    // ACSR |= (1 << ACIS1); // Set   bit 1 of ACSR to set the interrupt mode to interrupt on rising output edge

    // ADMUX – ADC Multiplexer Selection Register
    ADMUX |= (1 << MUX0);
    ADMUX |= (1 << MUX1);

    // DIDR1 – Digital Input Disable Register 1
    DIDR1  |= (1 << AIN1D | 1 << AIN0D);   // Clear bit 1 of DIDR1 to disable the digital input buffer

    //ACSR |= (1 << ACIS0); // Set   bit 0 of ACSR to set the interrupt mode to interrupt on rising output edge



    /*
    DIDR1 |= (1 << AIN0D) | (1 << AIN1D);
    ACSR |= (1 << ACBG);
    ACSR |= (1 << ACIS1) | (1 << ACIS0);
    */

    // Set Output / Input
    // DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB3 | 1 << PB4 | 1 << PB5;
    // DDRC |= 1 << PC2 | 1 << PC3; // Set PC2 and PC3 as inputs
    // DDRB = 0b00001111;
    // DDRC = 0b11110000;
    DDRB = 0b00001111;
    DDRC = 0b00001111;
    DDRD &= ~((1 << PD6) | (1 << PD7)); // Set PC0 and PC1 as inputs
    PORTD &= ~((1 << PD6) | (1 << PD7));


    // SREG – AVR Status Register
    sei(); // Set bit 7 of SREG to enable global interrupts


    while (true) {

        /*
        if(ACSR & (1 << ACO)) {
            writeBytes(0xFF);
        } else {
            writeBytes(0x00);
        }
         */

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

    }
}
