#include "catch.hpp"
#include "interval.h"

SCENARIO( "ggl::interval is correctly constructed", "[interval]" ) {
    GIVEN( "no parameters (default constructor)" ) {
        WHEN( "a default interval is created" ) {
            const ggl::interval empty;
            THEN( "the bounds are both zero (an empty interval)" ) {
                REQUIRE( empty.bounds() == std::make_pair(0.0f, 0.0f) );
            }
        }
    }

    GIVEN( "an explicit minimum and maximum" ) {
        const ggl::real min = 313.9999f, max = 314.0f;
        WHEN( "the given maximum is lower than the minimum" ) {
            const ggl::interval test( max, min );
            THEN( "the result interval is rearranged to be from minimum to maximum" ) {
                REQUIRE( test.bounds() == std::make_pair(min, max) );
            }
        }

        WHEN( "the given maximum is higher than the minimum" ) {
            const ggl::interval test( min, max );
            THEN( "the result interval is bounded from minimum to maximum" ) {
                REQUIRE( test.bounds() == std::make_pair(min, max) );
            }
        }
    }
}

SCENARIO( "ggl::interval contains test works", "[interval]" ) {
    GIVEN( "a nonempty interval" ) {
        const ggl::real min = -5.0f, max = 5.0f;
        const ggl::interval intrvl( min, max );

        WHEN( "the given value in the interval range" ) {
            THEN( "the contains operation returns true" ) {
                REQUIRE( intrvl.contains( min ) );
                REQUIRE( intrvl.contains( max ) );
                REQUIRE( intrvl.contains( (min + max) / 2.0f ) );
            }
        }

        WHEN( "the given value is below the interval range" ) {
            THEN( "the contains operation returns false" ) {
                REQUIRE( !intrvl.contains( min - 1.0f ) );
                REQUIRE( !intrvl.contains( min - 0.01f ) );
            }
        }

        WHEN( "the given value is above the interval range" ) {
            THEN( "the contains operation returns false" ) {
                REQUIRE( !intrvl.contains( max + 1.0f ) );
                REQUIRE( !intrvl.contains( max + 0.01f ) );
            }
        }
    }
}

SCENARIO( "ggl::interval intersect operation works", "[interval]" ) {
    GIVEN( "two nonempty intervals" ) {
        const ggl::real min1 = -10.0f, max1 = 10.0f;
        const ggl::interval int1( min1, max1 );

        WHEN( "the intervals are disjoint" ) {
            const ggl::interval int2( min1 - 2.0f, min1 - 1.0f );

            THEN( "the result of the intersection is empty" ) {
                const ggl::interval result1 = int1.intersect( int2 );
                REQUIRE( result1.empty() );

                const ggl::interval result2 = int2.intersect( int1 );
                REQUIRE( result2.empty() );
            }

            THEN( "the result of the intersection is invalid" ) {
                const ggl::interval result1 = int1.intersect( int2 );
                REQUIRE( !result1.valid() );

                const ggl::interval result2 = int2.intersect( int1 );
                REQUIRE( !result2.valid() );
            }
        }

        WHEN( "the intervals are partially intersecting" ) {
            const ggl::real min2 = 0.0f, max2 = 20.0f;
            const ggl::interval int2( min2, max2 );

            THEN( "the result of the intersection is the overlapping interval" ) {
                const ggl::interval result1 = int1.intersect( int2 );
                REQUIRE( !result1.empty() );
                REQUIRE( result1.bounds() == std::make_pair(min2, max1) );

                const ggl::interval result2 = int2.intersect( int1 );
                REQUIRE( !result2.empty() );
                REQUIRE( result2.bounds() == std::make_pair(min2, max1) );
            }
        }

        WHEN( "the intervals are fully intersecting" ) {
            const ggl::real min2 = -9.0f, max2 = 9.0f;
            const ggl::interval int2( min2, max2 );

            THEN( "the result of the intersection is the contained interval" ) {
                const ggl::interval result1 = int1.intersect( int2 );
                REQUIRE( !result1.empty() );
                REQUIRE( result1.bounds() == std::make_pair(min2, max2) );

                const ggl::interval result2 = int2.intersect( int1 );
                REQUIRE( !result2.empty() );
                REQUIRE( result2.bounds() == std::make_pair(min2, max2) );
            }
        }
    }
}
