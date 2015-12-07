#include "ballistics.hpp"
#include <catch.hpp>

/* Catch's Approx macro has no support for OpenCV,
 * so we must "implement" it separately.
 *
 * This function is better than simply checking if the norm is below a limit
 * because it logs the matrices values before running the test,
 * thus producing better error messages.
 */
void assert_near( cv::Mat a, cv::Mat b, double epsilon = 1e-5 ) {
    INFO( a << "\n !=~ \n" << b << ", epsilon = " << epsilon );
    CHECK( cv::norm(a - b) < epsilon );
}

// Syntactic sugar.
cv::Mat vec( double a, double b, double c ) {
    return cv::Mat_<double>(3, 1) << a, b, c;
}

TEST_CASE( "assert_near self-test" ) {
    assert_near( vec(0, 0, 0), vec(0, 0, 1e-7), 1e-5 );
}
