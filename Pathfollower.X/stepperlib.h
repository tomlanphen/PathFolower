#ifndef Stepper_h
#define Stepper_h

#include <xc.h>

// constructors:
void Stepper(int number_of_steps);

// mover method:
void step(int number_of_steps);

void stepMotor(int this_step);

int direction; // Direction of rotation
int number_of_steps; // total number of steps this motor can take
int step_number; // which step the motor is on

#endif	/* Stepper_h */

