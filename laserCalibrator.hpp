#ifndef _LASER_CALIBRATOR_HPP_
#define _LASER_CALIBRATOR_HPP_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <aruco/marker.h>
#include <aruco/markerdetector.h>
#include <aruco/cameraparameters.h>

#include <iostream>
#include <vector>

#include "stepper.hpp"
#include "targetDetector.hpp"

class LaserCalibrator{
    
public:
    LaserCalibrator(double angle, Stepper* verticalStepper, Stepper* horizontalStepper, TargetDetector* detector);

    cv::Mat point1;
    cv::Mat point2;
    cv::Mat point3;
    cv::Mat point4;
    double angle;

    bool start();

private:

    Stepper* verticalStepper;
    Stepper* horizontalStepper;

    TargetDetector* detector;

    void getPoint(aruco::Marker& marker, cv::Mat& out);
};

#endif
