#include <cmath>
#include "math.hpp"
#include "ballistics.hpp"

Ballistics::Ballistics(cv::Mat p1, cv::Mat p2, double alpha, cv::Mat q1, cv::Mat q2) {
    _center = line_line_intersection(p1, p2, q1, q2);

    /* old_front is the orthonormal vector
     * that pointed to the front view of the laser,
     * in the first two measurements.
     */
    cv::Mat old_front = p1 - _center;
    old_front /= cv::norm(old_front);

    _front = q1 - _center;
    _front /= cv::norm(_front);

    double actual_angle = std::acos(old_front.dot(_front));
    double projection_angle = angle_of_projection( actual_angle, alpha );

    _up = compute_rotation_axis( old_front, _front, projection_angle );
    _left = _up.cross(_front);

    _up /= cv::norm(_up);
    _left /= cv::norm(_left);
}

Ballistics::rotation_data Ballistics::align(cv::Mat p) {
    p -= _center;
    rotation_data ret;

    // Projection on secondary rotation plane
    cv::Mat ps = project_on_axis( p, _up ) +
        project_on_axis( p, _up.cross(_left) );

    ret.secondary = std::acos( ps.dot(_front) / cv::norm(ps) );

    // Now, fix if is wrong
    if( cv::norm( ps - rotate_around_axis(_left, _front, -ret.secondary) )
        < cv::norm( ps - rotate_around_axis(_left, _front, ret.secondary) ) )
        ret.secondary = -ret.secondary;
    // TODO: There must be a smarter way of doing this.

    // Projection on primary rotation plane
    cv::Mat pp = project_on_axis( p, _left ) +
        project_on_axis( p, _up.cross(_left) );

    // Projection of front in the secondary rotation plane
    cv::Mat fp = project_on_axis( _front, _left ) +
        project_on_axis( _front, _up.cross(_left) );

    if( cv::norm(pp) < 1e-8 && cv::norm(fp) < 1e-8 ) {
        /* Arbitrarily chosen constants
         * This is to avoid division by zero.
         */
        ret.main = 0.0;
    }
    else {
        ret.main = std::acos( pp.dot(_front) / cv::norm(pp) );

        // Fix again if is wrong
        if( cv::norm( pp - rotate_around_axis(_up, fp, -ret.main) )
            < cv::norm( pp - rotate_around_axis(_up, fp, ret.main) ) )
            ret.main = -ret.main;
    }

    // Align axis _left and _front
    _front = p;
    _left = rotate_around_axis(_up, _left, ret.main);

    return ret;
}
