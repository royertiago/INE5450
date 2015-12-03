#include "laserCalibrator.hpp"

LaserCalibrator::LaserCalibrator(double angle, Stepper* verticalStepper, Stepper* horizontalStepper, TargetDetector* detector)
    :   point1(3, 1, CV_64FC1), point2(3, 1, CV_64FC1), point3(3, 1, CV_64FC1), point4(3, 1, CV_64FC1), 
    angle(angle), detector(detector), verticalStepper(verticalStepper), horizontalStepper(horizontalStepper)
{
}

bool LaserCalibrator::start(){




    int cont = 0;

    std::cout << "press s to take a shot" << std::endl;

    while(true){


        std::string input;
        std::getline(std::cin,input);

        if(!(detector->detect())){
            std::cout << "No markers detected!!" << std::endl;
            continue;
        }

        if(input == "s"){ 

            if(cont == 0){
                detector->targetPos.copyTo(point1);

                std::cout << "press s to take a shot" << std::endl;
            }
            else if(cont == 1){
                detector->targetPos.copyTo(point1);

                horizontalStepper->step(angle / (horizontalStepper->DEGREES_PER_STEP));

                std::cout << "press s to take a shot" << std::endl;
            }
            else if(cont == 2){
                detector->targetPos.copyTo(point1);

                std::cout << "press s to take a shot" << std::endl;
            }
            else if(cont == 3){
                detector->targetPos.copyTo(point1);

                std::cout << "calibration points extracted" << std::endl;

                return true;
            }

            cont++;
        }
        else if(input == "q"){
            std::cout << "abort!" << std::endl;
            return false;
        }


    }

}


