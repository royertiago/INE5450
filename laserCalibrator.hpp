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

class LaserCalibrator{
    
public:
    LaserCalibrator(double angle, Stepper* verticalStepper, Stepper* horizontalStepper, int capID, char* cameraParamsPath, double markerSize);

    cv::Mat points;
    double angle;

    bool start();

private:

    Stepper* verticalStepper;
    Stepper* horizontalStepper;

    cv::VideoCapture capture;
    aruco::CameraParameters camParams;
    double markerSize;

    void getPoint(aruco::Marker& marker, cv::Mat& out);
};

#endif
