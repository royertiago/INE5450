#include "targetDetector.hpp"

#include <iostream>

using namespace std;


TargetDetector::TargetDetector(int camID, char* camParamsPath, float markerSize)
    : vCap(camID), image(), mDetector(), camParams(), markerSize(markerSize)
{
    camParams.readFromXMLFile(camParamsPath);
    targetPos = cv::Mat::zeros(3, 1, CV_64FC1);
}

TargetDetector::~TargetDetector()
{
}

bool TargetDetector::detect()
{
    vCap >> image;

    mDetector.detect(image, markers, camParams, markerSize);

    if(markers.size())
        return false;


    if(markers.size() > 1){ // if 2 or more markers apears in the image, which one should i use?
        std::cout << "Warning!!    Two or more markers detected on the image!!!" << std::endl;
    }

    cv::Mat rMat;
    cv::Rodrigues(markers[0].Rvec, rMat);
    cv::Mat tVec = markers[0].Tvec;
    cv::Mat poseMat = cv::Mat::zeros(3, 4, CV_32FC1);

    poseMat.at<float>(0, 0) = rMat.at<float>(0, 0);
    poseMat.at<float>(0, 1) = rMat.at<float>(0, 1);
    poseMat.at<float>(0, 2) = rMat.at<float>(0, 2);
    poseMat.at<float>(0, 3) = tVec.at<float>(0, 0);

    poseMat.at<float>(1, 0) = rMat.at<float>(1, 0);
    poseMat.at<float>(1, 1) = rMat.at<float>(1, 1);
    poseMat.at<float>(1, 2) = rMat.at<float>(1, 2);
    poseMat.at<float>(1, 3) = tVec.at<float>(1, 0);

    poseMat.at<float>(2, 0) = rMat.at<float>(2, 0);
    poseMat.at<float>(2, 1) = rMat.at<float>(2, 1);
    poseMat.at<float>(2, 2) = rMat.at<float>(2, 2);
    poseMat.at<float>(2, 3) = tVec.at<float>(2, 0);


    cv::Mat markerPoint = cv::Mat::zeros(4, 1, CV_32F);

    markerPoint.at<float>(0, 0) = markerSize;
    markerPoint.at<float>(1, 0) = 0;
    markerPoint.at<float>(2, 0) = 0;
    markerPoint.at<float>(3, 0) = 1;


    targetPos = poseMat * markerPoint;

    return true;

}
