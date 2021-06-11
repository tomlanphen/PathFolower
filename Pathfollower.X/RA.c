#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = ON
#pragma config FCMEN = ON
#pragma config WRT = OFF
#pragma config PLLEN = ON
#pragma config STVREN = ON
#pragma config BORV = LO
#pragma config LVP = OFF

#include <xc.h>
#include <pic16f1829.h>
#include "resources.h"
#include "stepperlib.h"
#include "uartlib.h"
#define _XTAL_FREQ 1000000

void setup(void);
void empty_comb(void);
inline void button_interrupt(void);

void __interrupt() ISR(void) {
    uart_handle_rcv_int();
    uart_handle_tx_int();
}

void main(void) {
    //    stepvar = 2052; //volle ratoatie
    setup();
//    Stepper(512);
    Stepper(2052);
    setup();
    empty_comb();
    while (1) {
        char uart_char = uart_read_char();
        if (uart_char) {
            if (comb1 == 0x00) {
                comb1 = uart_char;
            } else if (comb2 == 0x00) {
                comb2 = uart_char;
            } else if (comb3 == 0x00) {
                comb3 = uart_char;
            } else if (comb4 == 0x00) {
                comb4 = uart_char;
            } else if (comb5 == 0x00) {
                comb5 = uart_char;
            } else if (comb6 == 0x00) {
                comb6 = uart_char;
            } else if (comb7 == 0x00) {
                comb7 = uart_char;
            }
        }

        // zet alleen de stepvar als deze pic aangeroepen wordt
        if (comb1 == 0x72 && comb2 == 0x61 && comb6 == 0x72 && comb7 == 0x61) { //ascii r && a
                        if (comb5 == 0x76) { //ascii v voor vooruit
                stepvar = comb3 * comb4;
            } else if (comb5 == 0x61) { //ascii a voor achteruit
                stepvar = comb3 * comb4 * -1;
            }
//            step(stepvar);
            empty_comb();
        }
        // als alles s is start het rijden
        if (comb1 == 0x73 && comb2 == 0x73 && comb3 == 0x73 && comb4 == 0x73 && comb5 == 0x73 && comb6 == 0x73 && comb7 == 0x73) {
            step(stepvar);
            empty_comb();
        }
        // leeg de variabele als de alle variabele vol zijn en de message is behandeld
        if (comb7 != 0x00) {
            empty_comb();
        }
        // leeg de variabele als de message niet deze pic aanspreekt
        if (comb1 != 0x72 && comb1 != 0x73 && comb1 != 0x00) { //ascii r, ascii s
            empty_comb();
        } else if (comb2 != 0x61 && comb2 != 0x73 && comb2 != 0x00) { //ascii a, ascii s
            empty_comb();
        }
    }

    return;
}

void empty_comb(void) {
    comb1 = 0x00;
    comb2 = 0x00;
    comb3 = 0x00;
    comb4 = 0x00;
    comb5 = 0x00;
    comb6 = 0x00;
    comb7 = 0x00;
}

void setup(void) {
    OSCCONbits.IRCF = 0b1011; //1MHz
    OSCCONbits.SCS = 0b00;
    OSCCONbits.SPLLEN = 0;
    TRISCbits.TRISC6 = 0; // setup RAC6 as output
    
    LATCbits.LATC6 = 0;

    GIE = 1;
    PEIE = 1;

    if (!uart_init(3, 1)) {
    }
}
