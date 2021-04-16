#include <xc.h>
#include "stepperlib.h"
#define _XTAL_FREQ 1000000

/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
void Stepper(int num_of_steps) {
    step_number = 0; // which step the motor is on
    direction = 0; // motor direction
    number_of_steps = num_of_steps; // total number of steps for this motor

    // setup the pins on the microcontroller:
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void step(int steps_to_move) {
    //TMR1 = 0;
    //T1CONbits.TMR1ON = 0;       // enable timer1

    int steps_left = abs(steps_to_move); // how many steps to take

    // determine direction based on whether steps_to_mode is + or -:
    if (steps_to_move > 0) {
        direction = 1;
    }
    if (steps_to_move < 0) {
        direction = 0;
    }


    // decrement the number of steps, moving one step each time:
    while (steps_left > 0) {
        __delay_ms(5);
        // increment or decrement the step number,
        // depending on direction:
        if (direction == 1) {
            step_number++;
            if (step_number == number_of_steps) {
                step_number = 0;
            }
        } else {
            if (step_number == 0) {
                step_number = number_of_steps;
            }
            step_number--;
        }
        // decrement the steps left:
        steps_left--;
        // step the motor to step number 0, 1, ..., {3 or 10}
        stepMotor(step_number % 4);
    }
    //}
}

/*
 * Moves the motor forward or backwards.
 */
void stepMotor(int thisStep) {
    switch (thisStep) {
        case 0: // 1010
            LATCbits.LATC0 = 1;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 1;
            LATCbits.LATC3 = 0;
            break;
        case 1: // 0110
            LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 1;
            LATCbits.LATC2 = 1;
            LATCbits.LATC3 = 0;
            break;
        case 2: //0101
            LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 1;
            LATCbits.LATC2 = 0;
            LATCbits.LATC3 = 1;
            break;
        case 3: //1001
            LATCbits.LATC0 = 1;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 0;
            LATCbits.LATC3 = 1;
            break;
    }
}
