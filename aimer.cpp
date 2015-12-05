#include "aimer.hpp"

Aimer::Aimer(TargetDetector* detector, LaserAimer* laserAim, Ballistics* ballistics)
    : detector(detector), laserAim(laserAim), eniac(ballistics), stop(false)
{
    pthread_attr_t custom_sched_attr1;
    sched_param fifo_param1;
    fifo_param1.__sched_priority = 20;

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
    ret = pthread_create(&_this, &custom_sched_attr1, run, (void*)this);
    if(ret){
        std::cout << "pthread_create(&_this, &custom_sched_attr1, run, 0)" << std::endl;
        throw std::runtime_error("pthread_create(&_this, &custom_sched_attr1, run, 0)");
    }
}

Aimer::~Aimer(){
    stop = true;
    pthread_join(_this, 0);
}

void Aimer::join(){
    pthread_join(_this, 0);
}

void* Aimer::run(void* thisPointer){
    static_cast<Aimer*>(thisPointer)->operator()();
    return 0;
}

void Aimer::operator()(){

    while(!stop){

        if(detector.detect()){
            auto angles = eniac->align(detector->targetPos);

            std::cout << "main angle = " << angles.main << " seccond angle = " << angles.secondary << std::endl;

            double mainDegre = angles.main * 180.0 / M_PI;
            double secondDegre = angles.secondary * 180.0 / M_PI;

            aimer->updateVertical(secondDegre);
            aimer->updateHorizontal(mainDegre);
        }

    }  

}


