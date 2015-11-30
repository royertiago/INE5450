#include "laserAimer.hpp"

LaserAimer::LaserAimer(Stepper* verticalStepper, Stepper* horizontalStepper)
    : stop(false), angleV(0), angleH(0), horizontalStepper(verticalStepper), verticalStepper(horizontalStepper)
{
    pthread_mutex_init( &mutexV, NULL );
    pthread_mutex_init( &mutexH, NULL );

    pthread_attr_t custom_sched_attr1;
    sched_param fifo_param1;
    fifo_param1.__sched_priority = 10;

    int ret = pthread_attr_init(&custom_sched_attr1);
    if(ret){
        std::cout << "attr_init(&custom_sched_attr1)" << std::endl;
        throw std::runtime_error("attr_init(&custom_sched_attr1)");
    }
    ret = pthread_attr_setinheritsched(&custom_sched_attr1, PTHREAD_EXPLICIT_SCHED);
    if(ret){
        std::cout << "attr_setinheritsched(&custom_sched_attr1, PTHREAD_EXPLICIT_SCHED)" << std::endl;
        throw std::runtime_error("attr_setinheritsched(&custom_sched_attr1, PTHREAD_EXPLICIT_SCHED)");
    }
    ret = pthread_attr_setschedpolicy(&custom_sched_attr1, SCHED_FIFO);
    if(ret){
        std::cout << "attr_setschedpolicy(&custom_sched_attr1, SCHED_FIFO)" << std::endl;
        throw std::runtime_error("attr_setschedpolicy(&custom_sched_attr1, SCHED_FIFO)");
    }
    ret = pthread_attr_setschedparam(&custom_sched_attr1, &fifo_param1);
    if(ret){
        std::cout << "attr_setschedparam(&custom_sched_attr1, &fifo_param1)" << std::endl;
        throw std::runtime_error("attr_setschedparam(&custom_sched_attr1, &fifo_param1)");
    }
    ret = pthread_create(&_this, &custom_sched_attr1, run, 0);
    if(ret){
        std::cout << "pthread_create(&_this, &custom_sched_attr1, run, 0)" << std::endl;
        throw std::runtime_error("pthread_create(&_this, &custom_sched_attr1, run, 0)");
    }

}

LaserAimer::~LaserAimer(){
    stop = true;
    pthread_join(_this, 0);
    pthread_mutex_destroy( &mutexV );
    pthread_mutex_destroy( &mutexH );
}

void* LaserAimer::run(void* param){
    static_cast<LaserAimer *>(param)->operator()();
    return 0;
}

void LaserAimer::operator()() {
    while( !stop ) {
        if( angleV > verticalStepper->DEGREES_PER_STEP ) {
            updateVertical( - (verticalStepper->DEGREES_PER_STEP));
            verticalStepper->stepBackward();
        }
        else if( -angleV > verticalStepper->DEGREES_PER_STEP ) {
            updateVertical(verticalStepper->DEGREES_PER_STEP);
            verticalStepper->stepForward();
        }

        if( angleH > horizontalStepper->DEGREES_PER_STEP ) {
            updateHorizontal( - (horizontalStepper->DEGREES_PER_STEP));
            horizontalStepper->stepBackward();
        }
        else if( -angleH > horizontalStepper->DEGREES_PER_STEP ) {
            updateHorizontal(horizontalStepper->DEGREES_PER_STEP);
            horizontalStepper->stepForward();
        }
//        sleep( 2 );
    }
}

void LaserAimer::updateVertical(float alfa){
    pthread_mutex_lock(&mutexV);
    angleV += alfa;
    pthread_mutex_unlock(&mutexV);
}

void LaserAimer::updateHorizontal(float alfa){
    pthread_mutex_lock(&mutexH);
    angleH += alfa;
    pthread_mutex_unlock(&mutexH);
}
