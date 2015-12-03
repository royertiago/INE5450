#ifndef SERVO_H_123121412312312343
#define SERVO_H_123121412312312343

#include <wiringPi.h>

class Stepper{

public:
    Stepper(unsigned char* pins, unsigned int STEPS_PER_REVOLUTION);
    virtual ~Stepper();



    void stepForward();     // take one step forward
    void stepBackward();    // take one step backward
    void step(int n);       // n is how many steps the motor will take. The sign of n will tell the direction


    const unsigned int STEPS_PER_REVOLUTION;
    const float DEGREES_PER_STEP;

public://private:

    unsigned char state;

    static unsigned char states[4][4];

    const unsigned char PIN0;
    const unsigned char PIN1;
    const unsigned char PIN2;
    const unsigned char PIN3;
};



#endif
