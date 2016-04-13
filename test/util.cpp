#include "catch.hpp"
#include "src/util.h"
#include "src/tutil.hpp"

SCENARIO( "ggl::util clamp operation works", "[util]" ) {
    GIVEN( "an empty clamp interval" ) {
        const ggl::real center = 3.5f;
        WHEN( "any value is clamped to this interval" ) {
            THEN( "the result is always the center of the empty interval" ) {
                REQUIRE( ggl::util::clamp(0.0f, center, center) == center );
                REQUIRE( ggl::util::clamp(10.0f, center, center) == center );
                REQUIRE( ggl::util::clamp(-3.0f, center, center) == center );
            }
        }
    }

    GIVEN( "a nonempty clamp interval" ) {
        const ggl::real min = 10.0f, max = 80.0f;
        WHEN( "a value in the interval is clamped to the interval" ) {
            THEN( "the result is always the same as the input value" ) {
                REQUIRE( ggl::util::clamp(min, min, max) == min );
                REQUIRE( ggl::util::clamp(max, min, max) == max );

                REQUIRE( ggl::util::clamp(min + 1.0f, min, max) == (min + 1.0f) );
                REQUIRE( ggl::util::clamp(max - 3.0f, min, max) == (max - 3.0f) );
            }
        }

        WHEN( "a value below the interval is clamped to the interval" ) {
            THEN( "the result is always the minimum interval value" ) {
                REQUIRE( ggl::util::clamp(min - 5.0f, min, max) == min );
                REQUIRE( ggl::util::clamp(min - 1.0f, min, max) == min );
                REQUIRE( ggl::util::clamp(min - 0.01f, min, max) == min );
            }
        }

        WHEN( "a value above the interval is clamped to the interval" ) {
            THEN( "the result is always the maximum interval value" ) {
                REQUIRE( ggl::util::clamp(max + 5.0f, min, max) == max );
                REQUIRE( ggl::util::clamp(max + 1.0f, min, max) == max );
                REQUIRE( ggl::util::clamp(max + 0.01f, min, max) == max );
            }
        }
    }
}

SCENARIO( "ggl::util lerp operation works", "[util]" ) {
    GIVEN( "a nonempty interval" ) {
        const ggl::real min = -10.0f, max = 10.0f;
        WHEN( "the given interpolation factor is in the range [0, 1]" ) {
            THEN( "the returned value will be the interval lerp of the factor" ) {
                REQUIRE( ggl::util::lerp(0.0f, min, max) == min );
                REQUIRE( ggl::util::lerp(1.0f, min, max) == max );

                REQUIRE( ggl::util::lerp(0.25f, min, max) == Approx(-5.0f) );
                REQUIRE( ggl::util::lerp(0.50f, min, max) == Approx(+0.0f) );
                REQUIRE( ggl::util::lerp(0.75f, min, max) == Approx(+5.0f) );
                
            }
        }

        WHEN( "the given interpolation factor is in the range (1, +inf)" ) {
            THEN( "the returned value will be the maximum of the interval" ) {
                REQUIRE( ggl::util::lerp(-5.0f, min, max) == min );
                REQUIRE( ggl::util::lerp(-30.0f, min, max) == min );
            }
        }

        WHEN( "the given interpolation factor is in the range (-inf, 0)" ) {
            THEN( "the returned value will be the minimum of the interval" ) {
                REQUIRE( ggl::util::lerp(+5.0f, min, max) == max );
                REQUIRE( ggl::util::lerp(+30.0f, min, max) == max );
            }
        }
    }
}

SCENARIO( "ggl::util wrap operation works", "[util]" ) {
    GIVEN( "a nonempty interval" ) {
        const ggl::real min = -2.0f, max = 4.0f;
        WHEN( "the given value is inside the wrapping range (in [min, max])" ) {
            THEN( "the returned value is the same as the input value" ) {
                REQUIRE( ggl::util::wrap(-1.0f, min, max) == Approx(-1.0f) );
                REQUIRE( ggl::util::wrap(+1.0f, min, max) == Approx(+1.0f) );

                REQUIRE( ggl::util::wrap(min + 0.0001f, min, max) == Approx(min + 0.0001f) );
                REQUIRE( ggl::util::wrap(max - 0.0001f, min, max) == Approx(max - 0.0001f) );
            }
        }

        WHEN( "the given value is above the wrapping range (in (max, +inf)" ) {
            THEN( "the returned value is properly wrapped to the range" ) {
                REQUIRE( ggl::util::wrap(min - 0.1f, min, max) == Approx(max - 0.1f) );
                REQUIRE( ggl::util::wrap(min - 7.0f, min, max) == Approx(max - 1.0f) );
                REQUIRE( ggl::util::wrap(min - 14.0f, min, max) == Approx(max - 2.0f) );
            }
        }

        WHEN( "the given value is below the wrapping range (in (-inf, min)" ) {
            THEN( "the returned value is properly wrapped to the range" ) {
                REQUIRE( ggl::util::wrap(max + 0.1f, min, max) == Approx(min + 0.1f) );
                REQUIRE( ggl::util::wrap(max + 7.0f, min, max) == Approx(min + 1.0f) );
                REQUIRE( ggl::util::wrap(max + 14.0f, min, max) == Approx(min + 2.0f) );
            }
        }
    }
}

SCENARIO( "ggl::util factorial operation works", "[util]" ) {
    GIVEN( "any non-negative integer" ) {
        WHEN( "the non-negative integer is zero" ) {
            THEN( "the factorial of this integer is 1" ) {
                REQUIRE( ggl::util::factorial(0) == 1 );
            }
        }

        WHEN( "the non-negative integer is not zero" ) {
            THEN( "the factorial of this integer n is n*f(n-1)" ) {
                REQUIRE( ggl::util::factorial(1) == 1 );
                REQUIRE( ggl::util::factorial(2) == 2 );
                REQUIRE( ggl::util::factorial(3) == 6 );
                REQUIRE( ggl::util::factorial(4) == 24 );
                REQUIRE( ggl::util::factorial(5) == 120 );
            }
        }
    }
}

SCENARIO( "ggl::util quadratic operation works", "[util]" ) {
    GIVEN( "any set of real quadratic terms" ) {
        WHEN( "the determinant of the terms is negative" ) {
            THEN( "the quadratic operation returns a pair of nans" ) {
                const auto q1results = ggl::util::solveQuadratic( 1.0f, 1.0f, 1.0f );
                REQUIRE( std::isnan(q1results.first) );
                REQUIRE( std::isnan(q1results.second) );

                const auto q2results = ggl::util::solveQuadratic( 3.0f, 4.0f, 4.0f );
                REQUIRE( std::isnan(q2results.first) );
                REQUIRE( std::isnan(q2results.second) );
            }
        }

        WHEN( "the determinant of the terms is zero" ) {
            THEN( "the quadratic operation returns a pair with the one solution" ) {
                const auto q1solns = std::make_pair( -1.0f, -1.0f );
                REQUIRE( ggl::util::solveQuadratic(1.0f, 2.0f, 1.0f) == q1solns );

                const auto q2solns = std::make_pair( -2.0f, -2.0f );
                REQUIRE( ggl::util::solveQuadratic(1.0f, 4.0f, 4.0f) == q2solns );
            }
        }

        WHEN( "the determinant of the terms is positive" ) {
            THEN( "the quadratic operation returns a pair with the two solutions" ) {
                const auto q1solns = std::make_pair( -1.0f, 4.0f );
                REQUIRE( ggl::util::solveQuadratic(1.0f, -3.0f, -4.0f) == q1solns );

                const auto q2solns = std::make_pair( -2.0f, 2.0f );
                REQUIRE( ggl::util::solveQuadratic(1.0f, 0.0f, -4.0f) == q2solns );
            }
        }
    }
}
