#include "ballistics.hpp"
#include <catch.hpp>
#include <cmath>
#include <limits>

/* Catch's Approx macro has no support for OpenCV,
 * so we must "implement" it separately.
 *
 * This function is better than simply checking if the norm is below a limit
 * because it logs the matrices values before running the test,
 * thus producing better error messages.
 */
void assert_near( int line, cv::Mat a, cv::Mat b,
    double epsilon = std::numeric_limits<double>::epsilon() * 100
) {
    INFO( "line = " << line );
    INFO( "epsilon = " << epsilon );
    INFO( a << "\n !=~ \n" << b );
    CHECK( cv::norm(a - b) < epsilon );
}

// Syntactic sugar.
cv::Mat vec( double a, double b, double c ) {
    return cv::Mat_<double>(3, 1) << a, b, c;
}

// More syntatic sugar
#define ASSERT_NEAR( ... ) assert_near( __LINE__, __VA_ARGS__ )

TEST_CASE( "assert_near self-test" ) {
    ASSERT_NEAR( vec(0, 0, 0), vec(0, 0, 1e-7), 1e-5 );
}

TEST_CASE( "Construction of an origin-centered Ballistics object", "[ballistics]" ) {
    cv::Mat p1 = vec(1, 0, 1);
    cv::Mat p2 = vec(2, 0, 2);
    cv::Mat q1 = vec(std::sqrt(2)/2, std::sqrt(2)/2, 1);
    cv::Mat q2 = vec(std::sqrt(2), std::sqrt(2), 2);
    double angle = 3.141592653589793238462643383/4;

    auto check = []( Ballistics & eniac, int line ) {
        assert_near( line, eniac.center(), vec( 0, 0, 0 ) );
        assert_near( line, eniac.up(), vec( 0, 0, 1 ) );
        assert_near( line, eniac.front(), vec( 0.5, 0.5, std::sqrt(2)/2 ) );
        assert_near( line, eniac.left(), vec( -std::sqrt(2)/2, std::sqrt(2)/2, 0 ) );
    };

    Ballistics eniac( p1, p2, angle, q1, q2 );
    check( eniac, __LINE__ );

    eniac = Ballistics( p2, p1, angle, q1, q2 );
    check( eniac, __LINE__ );

    eniac = Ballistics( p1, p2, angle, q2, q1 );
    check( eniac, __LINE__ );

    eniac = Ballistics( p2, p1, angle, q2, q1 );
    check( eniac, __LINE__ );

    // Test with a negative angle
    eniac = Ballistics( q1, q2, -angle, p1, p2 );
    ASSERT_NEAR( eniac.center(), vec(0, 0, 0) );
    ASSERT_NEAR( eniac.up(), vec(0, 0, 1) );
    ASSERT_NEAR( eniac.front(), vec( std::sqrt(2)/2, 0, std::sqrt(2)/2) );
    ASSERT_NEAR( eniac.left(), vec( 0, 1, 0 ) );
}

TEST_CASE( "Ballistics aiming", "[ballistics]" ) {
    cv::Mat p1 = vec(1, 0, 1);
    cv::Mat p2 = vec(2, 0, 2);
    cv::Mat q1 = vec(std::sqrt(2)/2, std::sqrt(2)/2, 1);
    cv::Mat q2 = vec(std::sqrt(2), std::sqrt(2), 2);
    double angle = 3.141592653589793238462643383/4;
    Ballistics eniac( p1, p2, angle, q1, q2 );

    SECTION( "Aiming down first" ) {
        auto pair = eniac.align( vec(0.5, 0.5, 0) );
        CHECK( pair.main == Approx(0) );
        CHECK( pair.secondary == Approx(M_PI_4) );
        ASSERT_NEAR( eniac.front(), vec(std::sqrt(2)/2, std::sqrt(2)/2, 0) );
        ASSERT_NEAR( eniac.left(), vec( -std::sqrt(2)/2, std::sqrt(2)/2, 0 ) );
    }

    SECTION( "Aiming right first" ) {
        auto pair = eniac.align( vec(1, 0, 1) );
        CHECK( pair.main == Approx(-M_PI_4) );
        CHECK( pair.secondary == Approx(0) );
        ASSERT_NEAR( eniac.front(), vec(std::sqrt(2)/2, 0, std::sqrt(2)/2) );
        ASSERT_NEAR( eniac.left(), vec(0, 1, 0) );
    }

    SECTION( "Aiming left first" ) {
        auto pair = eniac.align( vec(0, 1, 1) );
        CHECK( pair.main == Approx(M_PI_4) );
        CHECK( pair.secondary == Approx(0) );
        ASSERT_NEAR( eniac.front(), vec(0, std::sqrt(2)/2, std::sqrt(2)/2) );
        ASSERT_NEAR( eniac.left(), vec(-1, 0, 0) );
    }

    SECTION( "Dual motion - right down" ) {
        auto pair = eniac.align( vec(1, 0, 0) );
        CHECK( pair.main == Approx(-M_PI_4) );
        CHECK( pair.secondary == Approx(M_PI_4) );
        ASSERT_NEAR( eniac.front(), vec(1, 0, 0) );
        ASSERT_NEAR( eniac.left(), vec(0, 1, 0) );
    }

    SECTION( "Dual motion - left down" ) {
        auto pair = eniac.align( vec(0, 1, 0) );
        CHECK( pair.main == Approx(M_PI_4) );
        CHECK( pair.secondary == Approx(M_PI_4) );
        ASSERT_NEAR( eniac.front(), vec(0, 1, 0) );
        ASSERT_NEAR( eniac.left(), vec(-1, 0, 0) );
    }
}
