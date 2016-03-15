#include "catch.hpp"
#include "src/interval.h"

SCENARIO( "ggl::interval is correctly constructed", "[interval]" ) {
    GIVEN( "no parameters (default constructor)" ) {
        WHEN( "a default interval is created" ) {
            const ggl::interval empty;
            THEN( "the bounds are both zero (an empty interval)" ) {
                std::pair<ggl::real, ggl::real> expected = std::make_pair( 0.0f, 0.0f );
                REQUIRE( empty.bounds() == expected );
            }
        }
    }

    GIVEN( "an explicit minimum and maximum" ) {
        const ggl::real min = 313.9999f, max = 314.0f;
        WHEN( "the given maximum is lower than the minimum" ) {
            const ggl::interval empty( max, min );
            THEN( "the result interval is an empty interval centered at minimum" ) {
                std::pair<ggl::real, ggl::real> expected = std::make_pair( max, max );
                REQUIRE( empty.bounds() == expected );
            }
        }

        WHEN( "the given maximum is higher than the minimum" ) {
            const ggl::interval nonempty( min, max );
            THEN( "the result interval is bounded from minimum to maximum" ) {
                std::pair<ggl::real, ggl::real> expected = std::make_pair( min, max );
                REQUIRE( nonempty.bounds() == expected );
            }
        }
    }
}

SCENARIO( "ggl::interval clamp operation works", "[interval]" ) {
    GIVEN( "an empty interval" ) {
        const ggl::interval empty;
        WHEN( "any value is clamped to this interval" ) {
            THEN( "the result is always the root of the empty interval" ) {
                REQUIRE( empty.clamp(0.0f) == 0.0f );
                REQUIRE( empty.clamp(10.0f) == 0.0f );
                REQUIRE( empty.clamp(-3.0f) == 0.0f );
            }
        }
    }

    GIVEN( "a nonempty interval" ) {
        const ggl::real min = 10.0f, max = 80.0f;
        const ggl::interval nonempty( min, max );
        WHEN( "a value in the interval is clamped to the interval" ) {
            THEN( "the result is always the same as the input value" ) {
                REQUIRE( nonempty.clamp(min) == min );
                REQUIRE( nonempty.clamp(max) == max );

                REQUIRE( nonempty.clamp(min + 1.0f) == (min + 1.0f) );
                REQUIRE( nonempty.clamp(max - 3.0f) == (max - 3.0f) );
            }
        }

        WHEN( "a value below the interval is clamped to the interval" ) {
            THEN( "the result is always the minimum interval value" ) {
                REQUIRE( nonempty.clamp(min - 5.0f) == min );
                REQUIRE( nonempty.clamp(min - 1.0f) == min );
                REQUIRE( nonempty.clamp(min - 0.01f) == min );
            }
        }

        WHEN( "a value above the interval is clamped to the interval" ) {
            THEN( "the result is always the maximum interval value" ) {
                REQUIRE( nonempty.clamp(max + 5.0f) == max );
                REQUIRE( nonempty.clamp(max + 1.0f) == max );
                REQUIRE( nonempty.clamp(max + 0.01f) == max );
            }
        }
    }
}

SCENARIO( "ggl::interval lerp operation works", "[interval]" ) {
    GIVEN( "an arbitrary interval" ) {
        const ggl::real min = -10.0f, max = 10.0f;
        const ggl::interval test( min, max );

        WHEN( "the given interpolation factor is in the range [0, 1]" ) {
            THEN( "the returned value will be the interval lerp of the factor" ) {
                REQUIRE( test.lerp(0.0f) == min );
                REQUIRE( test.lerp(1.0f) == max );

                REQUIRE( test.lerp(0.25f) == Approx(-5.0f) );
                REQUIRE( test.lerp(0.50f) == Approx(+0.0f) );
                REQUIRE( test.lerp(0.75f) == Approx(+5.0f) );
                
            }
        }

        WHEN( "the given interpolation factor is in the range (1, +inf)" ) {
            THEN( "the returned value will be the maximum of the interval" ) {
                REQUIRE( test.lerp(-5.0f) == min );
                REQUIRE( test.lerp(-30.0f) == min );
            }
        }

        WHEN( "the given interpolation factor is in the range (-inf, 0)" ) {
            THEN( "the returned value will be the minimum of the interval" ) {
                REQUIRE( test.lerp(+5.0f) == max );
                REQUIRE( test.lerp(+30.0f) == max );
            }
        }
    }
}

SCENARIO( "ggl::interval intersect operation works", "[interval]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::interval contains test works", "[interval]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::interval overlaps test works", "[interval]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}
