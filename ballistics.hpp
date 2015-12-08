#ifndef BALLISTICS_HPP
#define BALLISTICS_HPP

#include <opencv2/core/core.hpp>

/* Class responsible for computing how much each stepper motor must rotate.
 *
 * The class constructor receives some calibration parameters,
 * from which the object deduces the laser positioning,
 * rotation axes and current angles.
 *
 * Each call to align() returns how much each stepper must rotate
 * in order to point the laser to the newly given point,
 * in relation to the point given before.
 *
 * This class' main focus is on ("ballistic") computations
 * and it is mostly oblivious about the pipeline of the remainder of the program.
 *
 * Assumptions about the engine:
 *  - The engine follows the right-hand rule.
 *    That is, if the rotation axis is the z axis (0, 0, 1),
 *    then rotating the point (1, 0, 0) in the x axis by pi/2 radians
 *    yields the point (0, 1, 0) in the y axis.
 *  - The secondary rotation axis points "left" from the laser viewpoint.
 *    Together with the previous assumption, this meas that
 *    a positive rotation in the secondary axis lowers the laser,
 *    and a negative rotation raises the laser.
 *  - Both rotation axes actually cross in some point
 *  - The axes are orthogonal (their director vectors have a null dot product)
 *  - The front-view vector is always orthogonal to the secondary axis
 *  - The secondary axis gets rotated around the main axis,
 *    centered in the crossing between the axes.
 */

class Ballistics {
public:
    /* Constructs the ballistic calculator with the given callibration data.
     * The data is a series of measures, with some movement between
     * each measurement.
     *
     * Parameters:
     * p1, p2: Two points in the viewing line of the laser,
     *  that were detected without rotating the laser.
     *  The points must be different,
     *  and both must not be exactly in the center of the laser,
     *  but it makes no difference which of them are closer to the laser.
     *
     * alpha, q1, q2: Rotate the main axis by alpha radians;
     *  q1 and q2 are two new points in the viewing line of the laser.
     *  alpha must be in the interval (-pi, pi) and must be nonzero.
     */
    Ballistics(cv::Mat p1, cv::Mat p2, double alpha, cv::Mat q1, cv::Mat q2);

    /* Structure that describes how much each rotation axis must change
     * after a new point is measured.
     */
    struct rotation_data {
        double main;
        double secondary;
    };

    /* Alter the front-view vector to align the center to the point p
     * and return the rotation data to each of the laser's axis.
     */
    rotation_data align(cv::Mat p);

// Getters

    /* Vector that points to the front of the laser,
     * from the point of view of the laser center.
     */
    cv::Mat front() const {
        return _front;
    }

    /* Main rotation axis (make the laser rotate horizontally).
     * Altough is named "up", this is not the view-up vector,
     * because the front() vector might move up and down
     * withouth the main axis changing.
     * (The main axis never change.)
     */
    cv::Mat up() const {
        return _up;
    }

    /* Center of the laser; the intersection
     * of both rotation axis.
     * The center never change.
     */
    cv::Mat center() const {
        return _center;
    }

    /* Secondary rotation axis (make the laser rotate vertically).
     * Points to the left of the laser's current focus.
     */
    cv::Mat left() const {
        return _left;
    }

private:
    cv::Mat _front;
    cv::Mat _up;
    cv::Mat _center;
    cv::Mat _left;
};

#endif // BALLISTICS_HPP
