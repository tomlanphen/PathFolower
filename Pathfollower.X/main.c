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
inline void button_interrupt(void);

void __interrupt() ISR(void) {
    if (IOCAF) {
        __delay_ms(5);
        if (PORTAbits.RA2 == 1) {
            step(stepvar);
        }
        IOCAF = 0;
    }
}

void main(void) {
    var = 1;
    stepvar = 2048;
    setup();
    Stepper(512);
    setup();
    while (1) {
    }

    return;
}

void setup(void){
    OSCCONbits.IRCF = 0b1011; //1MHz
    OSCCONbits.SCS = 0b00;
    OSCCONbits.SPLLEN = 0;
    
    TRISA2 = 1; // button input
    ANSELAbits.ANSA2 = 0;
    IOCAPbits.IOCAP2 = 1;
    IOCIE = 1; //interrupt on change aan
    
    GIE = 1;
    PEIE = 1;
}
