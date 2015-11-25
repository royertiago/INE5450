#include <cmath>
#include "math.hpp"
#include "ballistics.hpp"

Ballistics::Ballistics(cv::Mat p1, cv::Mat p2, double alpha, cv::Mat q1, cv::Mat q2) {
    _center = line_line_intersection(p1, p2, q1, q2);

    /* old_front is the orthonormal vector
     * that pointed to the front view of the laser,
     * in the first two measurements.
     */
    cv::Mat old_front = p2 - p1;
    old_front /= cv::norm(old_front);

    _front = q2 - q1;
    _front /= cv::norm(_front);

    double actual_angle = std::acos(old_front.dot(_front));
    double projection_angle = angle_of_projection( actual_angle, alpha );

    _up = compute_rotation_axis( old_front, _front, projection_angle );
    _left = _up.cross(_front);

    _up /= cv::norm(_up);
    _left /= cv::norm(_left);
}
