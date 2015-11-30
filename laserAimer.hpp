#ifndef LASER_H_35241254123154
#define LASER_H_35241254123154

#include <pthread.h>
#include <iostream>
#include "stepper.hpp"
#include <stdexcept>

class LaserAimer{

public:

    LaserAimer(Stepper* verticalStepper, Stepper* horizontalStepper);
    ~LaserAimer();

    void updateVertical(float alfa);
    void updateHorizontal(float alfa);

private:

    bool stop;
    pthread_t _this;
    pthread_mutex_t mutexV;
    pthread_mutex_t mutexH;

    static void* run(void*);
    void operator()();

    float angleV;
    float angleH;

    Stepper* verticalStepper;
    Stepper* horizontalStepper;

};


#endif
