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
     */
    Ballistics(cv::Mat p1, cv::Mat p2);

// Getters

    /* Vector that points to the front of the laser,
     * from the point of view of the laser center.
     */
    cv::Mat laserFront() const {
        return _laserFront;
    }

private:

    cv::Mat _laserFront;
};

#endif // BALLISTICS_HPP
