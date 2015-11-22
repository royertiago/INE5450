#ifndef BALLISTICS_HPP
#define BALLISTICS_HPP

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
};

#endif // BALLISTICS_HPP
