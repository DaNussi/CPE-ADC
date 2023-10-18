#include <HardwareSerial.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// Datasheet
// Analog Comparator Block Diagram
// PD7 - AIN1 - Input
// PD6 - AIN0 - Input

bool compare() {
    return (ACSR & (1<<ACO)) > 0; // Read  bit 5 of ACSR to get the comparator output
}

void writeBytes(int data) {
    PORTD &= (data & 0b11110000) >> 4 | 0b11000000;
    PORTB &= (data & 0b00001111) << 0 | 0b00000000;
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
    ACSR &= ~(1 << ACD);  // Clear bit 7 of ACSR to enable the comparator
    ACSR &= ~(1 << ACBG); // Clear bit 6 of ACSR to disable the comparator band gap select
    //TODO ACO            // Read  bit 5 of ACSR to get the comparator output
    ACSR &= ~(1 << ACI);  // Clear bit 4 of ACSR to disable the comparator interrupt flag
    ACSR |= (1 << ACIE);  // Set   bit 3 of ACSR to enable the comparator interrupt enable
    ACSR &= ~(1 << ACIC); // Clear bit 2 of ACSR to disable the comparator input capture
    ACSR |= (1 << ACIS1); // Set   bit 1 of ACSR to set the interrupt mode to interrupt on rising output edge
    ACSR |= (1 << ACIS0); // Set   bit 0 of ACSR to set the interrupt mode to interrupt on rising output edge

    /*
    // ADMUX – ADC Multiplexer Selection Register
    ADMUX |= (1 << MUX0);
    ADMUX |= (1 << MUX1);

    // DIDR1 – Digital Input Disable Register 1
    DIDR1  &= ~(1 << AIN1D);   // Clear bit 1 of DIDR1 to disable the digital input buffer
    DIDR1  &= ~(1 << AIN0D);   // Clear bit 0 of DIDR1 to disable the digital input buffer
    */

    // Set Output
    DDRD = 1 << PD0 & 1 << PD1 & 1 << PD2 & 1 << PD3;
    DDRB = 1 << PB0 & 1 << PB1 & 1 << PB2 & 1 << PB3;


    // SREG – AVR Status Register
    sei(); // Set bit 7 of SREG to enable global interrupts

    Serial.begin(9600);

    while (true) {
        //while (Serial.available() > 0) {
        //    Serial.write(Serial.read());
        //}
        bool val = compare();
        String text = "Comapre Value: ";
        if (val) text += "true";
        else text += "false";
        Serial.println(text);

        int data = 0;
        for (int i = 7; i >= 0; i--) {
            data |= 1 << i;
            writeBytes(data);
            _delay_ms(20);

            if(compare()) {
                data &= ~(1 << i);
            }

        }

        data = 255 - data;
        float value = 5 * (data / (float) 255);
        Serial.println("Value: " + String(value) + " " + String(data, BIN));
    }
}
