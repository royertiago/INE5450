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

#endif // MATH_HPP
