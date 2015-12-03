#include "laserCalibrator.hpp"

LaserCalibrator::LaserCalibrator(double angle, Stepper* verticalStepper, Stepper* horizontalStepper, int capID, char* cameraParamsPath, double markerSize)
    :   points(3, 4, CV_64FC1), angle(angle), verticalStepper(verticalStepper), horizontalStepper(horizontalStepper),capture(capID), camParams(), markerSize(markerSize)
{
    camParams.readFromXMLFile(cameraParamsPath);
}

bool LaserCalibrator::start(){

    cv::Mat frame;

    aruco::MarkerDetector detector;
    std::vector<aruco::Marker> markers;

    int cont = 0;

    std::cout << "press s to take a shot" << std::endl;

    while(true){

        capture >> frame;

        if(!frame.data){
            std::cout << "null frame. Capture not open?" << std::endl;
            return false;
        }

        char key = cv::waitKey(5);

        if(key == 's'){
            
            detector.detect(frame, markers, camParams, markerSize);

            if(markers.size()){

                assert(markers.size() == 1 && "ONE MARKER ONLY");
                
                cv::Mat p;

                getPoint(markers[0], p);
               
                points.at<double>(0, cont) = p.at<double>(0, 0);
                points.at<double>(1, cont) = p.at<double>(1, 0);
                points.at<double>(2, cont) = p.at<double>(2, 0);

                cont++;

                if(cont == 4){
                    std::cout << "calibration points extracted" << std::endl;
                    return true;
                }
                else if(cont == 2){
                    //girar o motor vertical(?) em X graus
                    verticalStepper->step(angle / (verticalStepper->DEGREES_PER_STEP));
                    std::cout << "press s to take a shot" << std::endl;
                }
                else if(cont == 3){
                    //girar o motor vertical(?) em X graus
                    horizontalStepper->step(angle / (horizontalStepper->DEGREES_PER_STEP));
                    std::cout << "press s to take a shot" << std::endl;
                }


            }
            else{
                std::cout << "markers not detected!!" << std::endl;
            }

        }
        else if(key == 'q'){
            std::cout << "abort!" << std::endl;
            return false;
        }


    }

}

void LaserCalibrator::getPoint(aruco::Marker& marker, cv::Mat& out){

    cv::Mat rMat;
    cv::Rodrigues(marker.Rvec, rMat);
    cv::Mat tVec = marker.Tvec;
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

    out = poseMat * points;

    cout << "point = " << out << endl;


}

