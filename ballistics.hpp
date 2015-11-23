#ifndef BALLISTICS_HPP
#define BALLISTICS_HPP

#include <opencv2/core/core.hpp>

/* Class responsible for computing how much each stepper motor must rotate.
 *
 * The class constructor receives some calibration parameters,
 * from which the object deduces the laser positioning,
 * rotation axes and current angles.
 *
 * Each call to operator()() returns how much each stepper must rotate
 * in order to point the laser to the newly given point,
 * in relation to the point given before.
 *
 * This class' main focus is on ("ballistic") computations
 * and it is mostly oblivious about the pipeline of the remainder of the program.
 */

class Ballistics {
public:
    /* Constructs the ballistic calculator with the given callibration data.
     * Parameters:
     * p1, p2: Two points in the viewing line of the laser,
     *  that were detected without rotating the laser.
     *  p1 is closer to the laser than p2.
     *
     * alpha, q: Rotate the main axis by alpha radians;
     *  q is the point the laser focus now.
     */
    Ballistics(cv::Mat p1, cv::Mat p2, double alpha, cv::Mat q);

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

private:
    cv::Mat _front;
    cv::Mat _up;
    cv::Mat _center;
};

#endif // BALLISTICS_HPP
