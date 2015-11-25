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
