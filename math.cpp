#include "math.hpp"

/* Based on page 304 of book "Graphics Gems".
 */
cv::Mat line_line_intersection(cv::Mat p1, cv::Mat p2, cv::Mat q1, cv::Mat q2) {
    cv::Mat pd = p2 - p1; // p delta;
    cv::Mat qd = q2 - q1;
    cv::Mat cross = pd.cross(qd);
    double t = (q1 - p1).cross(qd).dot(cross) / cv::norm( cross, cv::NORM_L2SQR );
    return p1 + t * pd;
}

double angle_of_projection( double actual_angle, double projected_angle ) {
    return std::acos( std::sqrt(
                (1 - std::cos(actual_angle)) /
                (1 - std::cos(projected_angle))
            ));
}

cv::Mat rotate_around_axis( cv::Mat axis, cv::Mat v, double theta ) {
    axis /= cv::norm(axis);
    return v * std::cos(theta) + axis.cross(v) * std::sin(theta) +
        axis * axis.dot(v) * (1 - std::cos(theta));
}

cv::Mat compute_rotation_axis(cv::Mat a, cv::Mat b, double projection_angle ) {
    cv::Mat cross = a.cross(b);
    cv::Mat m = (a + b)/2;

    double d_angle = std::acos( std::sin(projection_angle)/cv::norm(m) );
    return rotate_around_axis( m.cross(cross), m, d_angle );
}

cv::Mat project_on_axis( cv::Mat vector, cv::Mat axis ) {
    axis /= cv::norm(axis);
    return axis * (vector.dot(axis));
}

double angle_between( cv::Mat a, cv::Mat b ) {
    double dot_product = a.dot(b) / (cv::norm(a) * cv::norm(b));
    /* Mathematically, the value dot_product is guaranteed to
     * never extrapolate 1. However, there might be rounding errors,
     * and the value of dot_product may be 1 + (machine epsilon), for instance.
     * In this case, dot_product > 1 and std::acos is required by the law
     * (C99 Standard, N1256, section 7.12.4.1) to produce a domain error,
     * rendering the return value unreliable (7.12.1, paragraph 2).
     *
     * Therefore, if dot_product > 1, it is due to rounding errors,
     * and we may force the angle to be 0.
     */
    if( dot_product > 1 )
        return 0;
    if( dot_product < -1 )
        return M_PI;
    return std::acos(dot_product);
}
