#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// Datasheet
// Analog Comparator Block Diagram
// PD7 - AIN1 - Input
// PD6 - AIN0 - Input

bool compare() {
    return (ACSR & (1 << ACO)) > 0; // Read  bit 5 of ACSR to get the comparator output
}

void writeBytes(int data) {
    // PORTD &= (data & 0b11110000) >> 4 | 0b11000000;
    // PORTB &= (data & 0b00001111) << 0 | 0b00000000;

    PORTB = data;
    //PORTB = data & 0b00111111;
    //PORTC = data >> 6;
}

int main(void) {

    // SREG – AVR Status Register
    cli(); // Clear bit 7 of SREG to disable global interrupts

    DDRD &= ~(1 << PD7); // Clear bit 7 of DDRD to make it an input
    DDRD &= ~(1 << PD6); // Clear bit 6 of DDRD to make it an input

    PORTD &= ~(1 << PD7); // Clear bit 7 of PORTD to disable the pull-up resistor
    PORTD &= ~(1 << PD6); // Clear bit 6 of PORTD to disable the pull-up resistor

    /*
    // ADCSRA – ADC Control and Status Register A
    ADCSRA &= ~(1 << ADC); // Clear bit 7 of ADCSRA to disable the ADC

    // ADCSRB – ADC Control and Status Register B
    ADCSRB |= (1 << ACME);   // Clear bit 6 of ADCSRB to disable the comparator multiplexer
    ADCSRB &= ~(1 << ADTS0); // Clear bit 0 of ADCSRB to ???
    ADCSRB &= ~(1 << ADTS1); // Clear bit 1 of ADCSRB to ???
    ADCSRB &= ~(1 << ADTS2); // Clear bit 2 of ADCSRB to ???
     */

    // ACSR – Analog Comparator Control and Status Register
    // ACSR &= ~(1 << ACD);  // Clear bit 7 of ACSR to enable the comparator
    // ACSR &= ~(1 << ACBG); // Clear bit 6 of ACSR to disable the comparator band gap select
    // TODO ACO            // Read  bit 5 of ACSR to get the comparator output
    // ACSR &= ~(1 << ACI);  // Clear bit 4 of ACSR to disable the comparator interrupt flag
    // ACSR |= (1 << ACIE);  // Set   bit 3 of ACSR to enable the comparator interrupt enable
    // ACSR &= ~(1 << ACIC); // Clear bit 2 of ACSR to disable the comparator input capture
    // ACSR |= (1 << ACIS1); // Set   bit 1 of ACSR to set the interrupt mode to interrupt on rising output edge
    // ACSR |= (1 << ACIS0); // Set   bit 0 of ACSR to set the interrupt mode to interrupt on rising output edge

    // Configure the comparator to use PC0 (ACBG) and PC1 (ACBG) as inputs in continuous mode
    ACSR |= (1 << ACD);    // Disable the digital input buffer on the comparator input pins
    ACSR |= (1 << ACBG);   // Select the PC0 (AIN0) as the negative input
    ACSR |= (1 << ACIS1);  // Set the ACIS bits to select the rising edge of the comparator output
    ACSR |= (1 << ACIS0);

    /*
    // ADMUX – ADC Multiplexer Selection Register
    ADMUX |= (1 << MUX0);
    ADMUX |= (1 << MUX1);

    // DIDR1 – Digital Input Disable Register 1
    DIDR1  &= ~(1 << AIN1D);   // Clear bit 1 of DIDR1 to disable the digital input buffer
    DIDR1  &= ~(1 << AIN0D);   // Clear bit 0 of DIDR1 to disable the digital input buffer
    */

    // Set Output
    DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB3 | 1 << PB4 | 1 << PB5 | 1 << PB6 | 1 << PB7;
    //DDRC &= ~((1 << PC0) | (1 << PC1)); // Set PC0 and PC1 as inputs


    // SREG – AVR Status Register
    sei(); // Set bit 7 of SREG to enable global interrupts


    while (true) {

        int data = 0;
        int last = 0;
        writeBytes(data);
        _delay_ms(100);
        for (int i = 0; i < 8; i++) {
            int index = 7-i;

            last = data;
            data |= 1 << index;
            writeBytes(data);
            _delay_ms(100);

            if(compare()) {
                data = last;
            }
        }
        _delay_ms(1000);
    }
}
