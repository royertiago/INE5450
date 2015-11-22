#include "ballistics.hpp"

Ballistics::Ballistics(cv::Mat p1, cv::Mat p2) {
    _laserFront = p2 - p1;
    _laserFront /= cv::norm(_laserFront);
}
