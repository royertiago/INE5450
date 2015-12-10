#include <cmath>
#include "math.hpp"
#include "ballistics.hpp"

Ballistics::Ballistics(cv::Mat p1, cv::Mat p2, double alpha, cv::Mat q1, cv::Mat q2) {
    _center = line_line_intersection(p1, p2, q1, q2);

    /* old_front is the orthonormal vector
     * that pointed to the front view of the laser,
     * in the first two measurements.
     *
     * Mathematically speaking, there are two possible ways of computing old_front:
     * normalizing p1 - _center, and normalizing p2 - _center.
     * In real life, p1 and p2 almost certainly will be given to us with errors.
     * We hope to minimize the error by picking the largest of
     * (p1 - _center) and (p2 - _center) to normalize.
     * (That is, the error relative to _center will probably be smaller
     * if we pick the vector that is more distant.)
     */
    cv::Mat old_front = p1 - _center;
    if( cv::norm(p2 - _center) > cv::norm(old_front) )
        old_front = p2 - _center;
    // And now normalize.
    old_front /= cv::norm(old_front);

    // Repeating the same trick for computing _front
    _front = q1 - _center;
    if( cv::norm(q2 - _center) > cv::norm(_front) )
        _front = q2 - _center;
    _front /= cv::norm(_front);

    double actual_angle = angle_between( old_front, _front );
    double projection_angle = angle_of_projection( actual_angle, alpha );

    if( alpha > 0 ) {
        /* There was a positive rotation from old_front to _front,
         * and it is, by exigence, less than pi.
         * So, this satisfies the assumptions of compute_rotation_axis.
         */
        _up = compute_rotation_axis( old_front, _front, projection_angle );
    }
    else {
        /* There was a negative rotation from old_front to _front.
         * This violetes the assumptions made by compute_rotation_axis,
         * so we will pretend that there was a positive rotation
         * from _front to old_front during the computation of _up.
         */
        _up = compute_rotation_axis( _front, old_front, projection_angle );
    }

    _left = _up.cross(_front);

    _up /= cv::norm(_up);
    _left /= cv::norm(_left);
}

Ballistics::rotation_data Ballistics::align(cv::Mat p) {
    p -= _center;
    rotation_data ret;

    // First, compute the rotation in the main axis
    {
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
            ret.main = angle_between( pp, fp );

            /* Note the value returned by angle_between is "unsigned",
             * and we need the angle to be "signed"
             * (that is, positive is counterclockwise and negative is clockwise
             * if the rotation axis points towards you.)
             */
            if( cv::norm( pp - rotate_around_axis(_up, fp, -ret.main) )
                < cv::norm( pp - rotate_around_axis(_up, fp, ret.main) ) )
                ret.main = -ret.main;
            // TODO: There must be a smarter way of doing this.
        }
    } // Rotation in the main axis computed.

    /* Now, to compute the rotation in the secondary axis,
     * we will first rotate everything through the primary axis.
     * We cannot use projections directly because the rotation plane itself
     * will be rotated around the primary axis.
     *
     * Note p remains still; what rotates is _front and _left.
     * (_front will be adjusted again later; _left is on its final form.)
     */
    _front = rotate_around_axis( _up, _front, ret.main );
    _left = rotate_around_axis( _up, _left, ret.main );

    // Compute the rotation around the secondary axis
    {
        /* This one is easier since both p and _front are on the rotation plane.
         */
        ret.secondary = angle_between( p, _front );

        // Again compute the sign of the rotation
        if( cv::norm( p - rotate_around_axis(_left, _front, -ret.secondary) )
            < cv::norm( p - rotate_around_axis(_left, _front, ret.secondary) ) )
            ret.secondary = -ret.secondary;
    } // Rotation in the secondary axis computed.

    // Finally, "rotate" _front in the secondary rotation plane.
    _front = p / cv::norm(p);

    return ret;
}
