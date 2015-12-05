#include "stepper.hpp"
#include "laserCalibrator.hpp"
#include "ballistics.hpp"
#include "laserAimer.hpp"
#include "targetDetector.hpp"
#include "aimer.hpp"

#include <wiringPi.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void* run(void*);

Ballistics* eniac;
LaserAimer* laserAim;
Stepper* veticalStepper;
Stepper* horizontalStepper;

TargetDetector detector(0, "param_camera_philips_quadradinha.yml", 0.145);

int main(void) {
    unsigned char pins_v[4] = { 4, 17, 27, 22 };
    unsigned char pins_h[4] = { 14, 15, 18, 23 };

    verticalStepper = new Stepper(pins_v, 2048);
    horizontalStepper = new Stepper(pins_h, 2048);

    LaserCalibrator* cal = new LaserCalibrator(10, verticalStepper, horizontalStepper, &detector);

    if (!cal->start() ) {
        cout << "Calibrator not working well...\nClosing the program" << endl;
        return -1;
    }

    cout << "Fazendo o calculo de balistica" << endl;

    eniac = new Ballistics(cal->point1, cal->point2,
            10.0/(180.0*M_PI),
            cal->point3, cal->point4);

    cout << "CTRL+C para parar o programa" << endl;

    Aimer aimer(&detector, laserAim, eniac);

    aimer.join();

    return 0;
}


