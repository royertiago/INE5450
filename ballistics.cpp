#include <cmath>
#include "ballistics.hpp"

Ballistics::Ballistics(cv::Mat p1, cv::Mat p2, double alpha, cv::Mat q) {
    _front = p2 - p1;
    _front /= cv::norm(_front);

    /* Find the intersection between the line p1-p2 and the line l,
     * that passes through q and intersects p1-p2 with an angle of alpha.
     * First, shift the world so that p1-p2 is on the origin.
     */
    cv::Mat qq = q - p1;
    // Now, project qq on the line p1-p2.
    cv::Mat pqq = qq.dot(_front) * _front;

    /* We now have a triangle, formed by the points _center, qq and pqq.
     * The side c-pqq lies on the line spanned by _front.
     * Due to the projection, this triangle is right on pqq
     * and we have the size of pqq-qq, so using trigonomotry
     * we can compute the distance c-pqq, thus obtaining c.
     */
    double oppositeCathetus = cv::norm(pqq-qq);
    double adjacentCathetus = oppositeCathetus / std::atan(alpha);
    _center = pqq - adjacentCathetus * _front;
    // Finally, shift back
    _center += p1;

    /* To compute the main rotation axis,
     * which is orthogonal both to the former _front
     * and the current _front (which is q - _center),
     * we will compute the cross product between these two vectors.
     */
    _up = _front.cross(q - _center);
}
