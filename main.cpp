#include "stepper.hpp"
#include "laserCalibrator.hpp"
#include "ballistics.hpp"
#include "laserAimer.hpp"
#include "targetDetector.hpp"

#include <wiringPi.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void* run(void*);

Ballistics* eniac;
Stepper* ve, *ho;

TargetDetector detector(0, "param_camera_philips_quadradinha.yml", 0.145);

void func(){
    pthread_t _this;
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
    ret = pthread_create(&_this, &custom_sched_attr1, run, 0);
    if(ret){
        std::cout << "pthread_create(&_this, &custom_sched_attr1, run, 0)" << std::endl;
        throw std::runtime_error("pthread_create(&_this, &custom_sched_attr1, run, 0)");
    }
    pthread_join(_this, 0);
}

void* run(void*){

    LaserAimer aimer(ve, ho);

    while(1){
    
        if(detector.detect()){
        
            auto angles = eniac->align(detector.targetPos);

            std::cout << "main angle = " << angles.main << " seccond angle = " << angles.secondary << std::endl;


            double mainDegre = angles.main * 180.0 / M_PI;
            double secondDegre = angles.secondary * 180.0 / M_PI;

            aimer.updateVertical(secondDegre);
            aimer.updateHorizontal(mainDegre);

        
        }


    }

}


int main(void) {
    unsigned char pins_v[4] = { 4, 17, 27, 22 };
    unsigned char pins_h[4] = { 14, 15, 18, 23 };

    Stepper v(pins_v,2048);
    Stepper h(pins_h,2048);

    LaserCalibrator* cal = new LaserCalibrator(10, &v, &h, &detector);

    if (!cal->start() ) {
        cout << "Calibrator not working well...\nClosing the program\n";
        return 1;
    }

    cout << "Fazendo o calculo de balistica\n" ;
    Ballistics bal(cal->point1,cal->point2,
            10.0/(180.0*M_PI),
            cal->point3,cal->point4);
    
    eniac = &bal;
    ve = &v;
    ho = &h;

       
    //delete cal;

    cout << "CTRL+C para parar o programa" << endl;

    func();

    return 0;
}
