#include "targetDetector.hpp"

#include <iostream>

using namespace std;


TargetDetector::TargetDetector(int camID, char* camParamsPath, float markerSize)
    : vCap(camID), image(), mDetector(), camParams(), markerSize(markerSize)
{
    camParams.readFromXMLFile(camParamsPath);
}

TargetDetector::~TargetDetector()
{
}

bool TargetDetector::detect()
{
    vCap >> image;

    mDetector.detect(image, markers, camParams, markerSize);

    if(markers.size()){
//        return false;


//    assert(markers.size() == 1); // if 2 or more markers apears in the image, which one should i use?



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


    cv::Mat points = cv::Mat::zeros(4, 1, CV_32F);

    points.at<float>(0, 0) = 0;
    points.at<float>(1, 0) = 0;
    points.at<float>(2, 0) = 0;
    points.at<float>(3, 0) = 1;

//    points.at<float>(0, 1) = 1;
//    points.at<float>(1, 1) = 0;
//    points.at<float>(2, 1) = 0;
//    points.at<float>(3, 1) = 1;

//    points.at<float>(0, 2) = 0;
//    points.at<float>(1, 2) = 1;
//    points.at<float>(2, 2) = 0;
//    points.at<float>(3, 2) = 1;

//    points.at<float>(0, 3) = 0;
//    points.at<float>(1, 3) = 0;
//    points.at<float>(2, 3) = 1;
//    points.at<float>(3, 3) = 1;

//    cv::Mat imagePoints = cv::Mat::zeros(3, 4, CV_32F);

//    imagePoints = camParams.CameraMatrix * poseMat * points;
    cv::Mat newPoint = poseMat * points;

//    imagePoints.at<float>(0, 0) = imagePoints.at<float>(0, 0)/imagePoints.at<float>(2, 0);
//    imagePoints.at<float>(1, 0) = imagePoints.at<float>(1, 0)/imagePoints.at<float>(2, 0);

//    imagePoints.at<float>(0, 1) = imagePoints.at<float>(0, 1)/imagePoints.at<float>(2, 1);
//    imagePoints.at<float>(1, 1) = imagePoints.at<float>(1, 1)/imagePoints.at<float>(2, 1);

//    imagePoints.at<float>(0, 2) = imagePoints.at<float>(0, 2)/imagePoints.at<float>(2, 2);
//    imagePoints.at<float>(1, 2) = imagePoints.at<float>(1, 2)/imagePoints.at<float>(2, 2);

//    imagePoints.at<float>(0, 3) = imagePoints.at<float>(0, 3)/imagePoints.at<float>(2, 3);
//    imagePoints.at<float>(1, 3) = imagePoints.at<float>(1, 3)/imagePoints.at<float>(2, 3);


//    cv::line(image, cv::Point2d(imagePoints.at<float>(0, 0), imagePoints.at<float>(1, 0)), cv::Point2d(imagePoints.at<float>(0, 1), imagePoints.at<float>(1, 1)), cv::Scalar(255, 0, 0), 2);
//    cv::line(image, cv::Point2d(imagePoints.at<float>(0, 0), imagePoints.at<float>(1, 0)), cv::Point2d(imagePoints.at<float>(0, 2), imagePoints.at<float>(1, 2)), cv::Scalar(255, 0, 0), 2);
//    cv::line(image, cv::Point2d(imagePoints.at<float>(0, 0), imagePoints.at<float>(1, 0)), cv::Point2d(imagePoints.at<float>(0, 3), imagePoints.at<float>(1, 3)), cv::Scalar(255, 0, 0), 2);


//    cout << "\n\n\n\n\n           " << imagePoints << "\n\n\n\n\n" << endl;


    float x = newPoint.at<float>(0, 0);
    float y = newPoint.at<float>(1, 0);
    float z = newPoint.at<float>(2, 0);

    cout << "dest = " << sqrt(x*x + y*y + z*z) << endl;

    aruco::CvDrawingUtils::draw3dCube(image, markers[0], camParams);

}
    cv::imshow("asd", image);
    cv::waitKey(1);





}
