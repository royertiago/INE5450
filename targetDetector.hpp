#ifndef TARGETDETECTOR_H_126234213265
#define TARGETDETECTOR_H_126234213265

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <aruco/cameraparameters.h>
#include <aruco/marker.h>
#include <aruco/markerdetector.h>
#include <aruco/cvdrawingutils.h>
#include <vector>

class TargetDetector{
public:

    TargetDetector(int camID, char* camParamsPath, float markerSize);
    virtual ~TargetDetector();

    bool detect();

private:
    cv::VideoCapture vCap;
    cv::Mat image;
    aruco::MarkerDetector mDetector;
    aruco::CameraParameters camParams;
    std::vector<aruco::Marker> markers;
    float markerSize;
};




#endif
