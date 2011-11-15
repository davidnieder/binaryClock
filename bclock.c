#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define FALSE 0
#define TRUE 1

/* variables for the actual time */
volatile unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;

/* variables for the decimal digits */
unsigned char seconds_0;
unsigned char seconds_1;
unsigned char minutes_0;
unsigned char minutes_1;
unsigned char hours_0;
unsigned char hours_1;

void display(void);
void split(void);

int main(void)  {
    /* define push button-pins as input pins */
    DDRB |= (0<<PB0) | (0<<PB1);
    /* set pins to high */
    PORTB |= (1<<PB0) | (1<<PB1);

    /* define LED-pins as output-pins */
    DDRA = 0b01111111;
    DDRC = 0b01111111;
    DDRD = 0b00111111;
    /* set all pins to low */
    PORTA = 0b00000000;
    PORTC = 0b00000000;
    PORTD = 0b00000000;


    /* prescale 256, clear timer on compare */
    TCCR1B |= (1<<CS12) | (1<<WGM12);
    /* compare register a */
    OCR1A = 31249;
    /* enable output compare match a interrupt */
    TIMSK  |= (1<<OCIE1A);

    /* enable interrupts */
    sei();

    while(TRUE) {
        split();
        display();

        /* check if button 0 was pressed */
        if( !(PINB & (1<<PB0))) {
            hours++;
            seconds = 0;
            _delay_ms(500);
        }
        /* check if button 1 was pressed */
        if( !(PINB & (1<<PB1))) {
            minutes++;
            seconds = 0;
            _delay_ms(500);
        }
    }

}

/* compare match interrupt handler, called every second */
ISR(TIMER1_COMPA_vect)   {
    seconds++;

    if( seconds >= 60 ) {
        seconds = 0;
        minutes++;
    }
    if( minutes >= 60 ) {
        minutes = 0;
        hours++;
    }
    if( hours >= 24 )   {
        hours = 0;
    }
}

/* display the time */
void display(void)  {
    PORTC = (seconds_1 << 4) | seconds_0;
    PORTA = (minutes_1 << 4) | minutes_0;
    PORTD = (hours_1   << 4) | hours_0;
}

/* split the decimal digits */
void split(void) {
    seconds_0 = seconds % 10;
    seconds_1 = seconds / 10;

    minutes_0 = minutes % 10;
    minutes_1 = minutes / 10;

    hours_0   = hours % 10;
    hours_1   = hours / 10;
}
