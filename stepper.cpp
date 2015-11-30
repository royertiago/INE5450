#include "stepper.hpp"

unsigned char Stepper::states[4][4] = {{1, 1, 0, 0},
                                       {0, 1, 1, 0},
                                       {0, 0, 1, 1},
                                       {1, 0, 0, 1}};

Stepper::Stepper(unsigned char* pins, unsigned int STEPS_PER_REVOLUTION)
    : state(0), PIN0(pins[0]), PIN1(pins[1]), PIN2(pins[2]), PIN3(pins[3]), STEPS_PER_REVOLUTION(STEPS_PER_REVOLUTION),
    DEGREES_PER_STEP(360.0/STEPS_PER_REVOLUTION)
{
//    digitalWrite(PIN0, 1);
//    digitalWrite(PIN1, 1);
//    digitalWrite(PIN2, 0);
//    digitalWrite(PIN3, 0);
}

Stepper::~Stepper()
{
}

void Stepper::stepForward()
{
    state = (state+1)%4;
//    digitalWrite(PIN0, states[state][0]);
//    digitalWrite(PIN1, states[state][1]);
//    digitalWrite(PIN2, states[state][2]);
//    digitalWrite(PIN3, states[state][3]);
}

void Stepper::stepBackward()
{
    state = (state+3)%4;
//    digitalWrite(PIN0, states[state][0]);
//    digitalWrite(PIN1, states[state][1]);
//    digitalWrite(PIN2, states[state][2]);
//    digitalWrite(PIN3, states[state][3]);
}

void Stepper::step(int n)
{
    unsigned char increment = n > 0 ? 1 : 3;
    if(n < 0)
        n *= -1;

    while(n > 0){

        state = (state+increment)%4;
//        digitalWrite(PIN0, states[state][0]);
//        digitalWrite(PIN1, states[state][1]);
//        digitalWrite(PIN2, states[state][2]);
//        digitalWrite(PIN3, states[state][3]);

        n--;
//        sleep(2);
    }
}







