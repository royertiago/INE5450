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

TEST_CASE( "Construction of an origin-centered Ballistics object", "[ballistics]" ) {
    Ballistics eniac(
            (cv::Mat_<double>(3, 1) << std::sqrt(2)/2, std::sqrt(2)/2, 1),
            (cv::Mat_<double>(3, 1) << std::sqrt(2), std::sqrt(2), 2),
            3.14159/4,
            (cv::Mat_<double>(3, 1) << 1, 0, 1),
            (cv::Mat_<double>(3, 1) << 2, 0, 2)
        );
    assert_near( eniac.center(), vec( 0, 0, 0 ) );
    assert_near( eniac.up(), vec( 0, 0, 1 ) );
    assert_near( eniac.front(), vec( std::sqrt(2)/2, 0, std::sqrt(2)/2) );
    assert_near( eniac.left(), vec( 0, 1, 0 ) );
}
