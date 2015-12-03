#ifndef MATH_HPP
#define MATH_HPP

/* Generic mathematical subroutines.
 */

#include <opencv2/core/core.hpp>

/* Returns the intersection point of the two 3D lines p1-p2 and q1-q2.
 * If these lines does not intersect,
 * the function returns the point in the line p1-p2 that is closest to q1-q2.
 * (That is, this function fails silently.)
 *
 * This function assumes the lines are not parallel.
 */
cv::Mat line_line_intersection(cv::Mat p1, cv::Mat p2, cv::Mat q1, cv::Mat q2);

/* Suppose that two vectors A and B are projected in a plane
 * such that the angle between A and the plane
 * is the same as the angle between B and the plane.
 * Given the angle between A and B ('actual_angle')
 * and the angle between the "projected version" of A and B ('projected_angle'),
 * this function computes the actual angle of projection.
 */
double angle_of_projection( double actual_angle, double projected_angle );

/* Rotate the vector v around the given axis
 * by theta radians.
 */
cv::Mat rotate_around_axis( cv::Mat axis, cv::Mat v, double theta );

cv::Mat compute_rotation_axis(cv::Mat a, cv::Mat b, double projection_angle );

/* Projects the vector 'vector' in the axis 'axis'.
 */
cv::Mat project_on_axis( cv::Mat vector, cv::Mat axis );

#endif // MATH_HPP
