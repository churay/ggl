#include "catch.hpp"
#include "src/interval.h"

SCENARIO( "ggl::interval is correctly constructed", "[interval]" ) {
    GIVEN( "no parameters (default constructor)" ) {
        WHEN( "a default interval is created" ) {
            const ggl::interval empty;
            THEN( "the bounds are both zero (an empty interval)" ) {
                std::pair<ggl::real, ggl::real> expected = std::make_pair( 0.0, 0.0 );
                REQUIRE( empty.getBounds() == expected );
            }
        }
    }

    GIVEN( "an explicit minimum and maximum" ) {
        const ggl::real min = 313.9999;
        const ggl::real max = 314.0;
        WHEN( "the given maximum is lower than the minimum" ) {
            const ggl::interval empty( max, min );
            THEN( "the result interval is an empty interval centered at minimum" ) {
                std::pair<ggl::real, ggl::real> expected = std::make_pair( max, max );
                REQUIRE( empty.getBounds() == expected );
            }
        }

        WHEN( "the given maximum is higher than the minimum" ) {
            const ggl::interval nonempty( min, max );
            THEN( "the result interval is bounded from minimum to maximum" ) {
                std::pair<ggl::real, ggl::real> expected = std::make_pair( min, max );
                REQUIRE( nonempty.getBounds() == expected );
            }
        }
    }
}

SCENARIO( "ggl::interval clamp operation works", "[interval]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::interval interpolate operation works", "[interval]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
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

SCENARIO( "ggl::interval empty test works", "[interval]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}
