#include "catch.hpp"
#include "src/xform.hpp"

SCENARIO( "ggl::xform basis operation works", "[xform][stub]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::xform scale operation works", "[xform][stub]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::xform translate operation works", "[xform][stub]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::xform rotate operation works", "[xform][stub]" ) {
    GIVEN( "a two-dimensional rotation" ) {
        using F = typename ggl::matrixf<3, 3>::EntryType;
        using FLT = typename ggl::matrixf<3, 3>::Compare;

        WHEN( "the rotation amount is zero" ) {
            THEN( "the rotate transform output is the identity matrix" ) {
                const ggl::real rotAmount{ ggl::zero() };
                const ggl::matrixf<3, 3> identity{ ggl::one() };
                REQUIRE( (ggl::xform::rotate<F, FLT>(rotAmount)) == identity );
            }
        }

        WHEN( "the rotation amount is nonzero" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }

    GIVEN( "a three-dimensional rotation about an arbitrary axis" ) {
        const ggl::vectorf<3> xAxis{ ggl::one(), ggl::zero(), ggl::zero() };
        const ggl::vectorf<3> yAxis{ ggl::zero(), ggl::one(), ggl::zero() };
        const ggl::vectorf<3> zAxis{ ggl::zero(), ggl::zero(), ggl::one() };
        const ggl::vectorf<3> xyzVec{ ggl::one(), ggl::one(), ggl::one() };

        WHEN( "the rotation amount is zero" ) {
            THEN( "the rotate transform output is the identity matrix" ) {
                const ggl::real rotAmount{ ggl::zero() };
                const ggl::matrixf<4, 4> identity{ ggl::one() };

                REQUIRE( ggl::xform::rotate(rotAmount, xAxis) == identity );
                REQUIRE( ggl::xform::rotate(rotAmount, yAxis) == identity );
                REQUIRE( ggl::xform::rotate(rotAmount, zAxis) == identity );
                REQUIRE( ggl::xform::rotate(rotAmount, xyzVec) == identity );
            }
        }

        WHEN( "the rotation amount is nonzero the axis is one of the canonical basis axes" ) {
            const ggl::vectorf<3> xAxis{ ggl::one(), ggl::zero(), ggl::zero() };
            const ggl::vectorf<3> yAxis{ ggl::zero(), ggl::one(), ggl::zero() };
            const ggl::vectorf<3> zAxis{ ggl::zero(), ggl::zero(), ggl::one() };

            std::array<ggl::real, 9> testAngles;
            for( size_t angleIdx = 0; angleIdx < 9; ++angleIdx )
                testAngles[angleIdx] = ( angleIdx * ggl::pi() ) / 4;

            // NOTE(JRC): The simplified formulae for axis rotatations can be
            // found at https://en.wikipedia.org/wiki/Rotation_matrix#Basic_rotations
            THEN( "the rotate transform output matches the matrices for the x-axis formula" ) {
                auto xAxisRotate = [ ] ( const ggl::real& pRadians ) {
                    return ggl::matrixf<3, 3>{
                        ggl::one(),  ggl::zero(), ggl::zero(),
                        ggl::zero(), +std::cos(pRadians), -std::sin(pRadians),
                        ggl::zero(), +std::sin(pRadians), +std::cos(pRadians)
                    };
                };

                for( const ggl::real& angle : testAngles ) {
                    const ggl::matrixf<4, 4> actual = ggl::xform::rotate( angle, xAxis );
                    const ggl::matrixf<3, 3> expected = xAxisRotate( angle );
                    REQUIRE( (actual.template submatrix<0, 0, 3, 3>()) == expected );
                }
            } THEN( "the rotate transform output matches the matrices for the y-axis formula" ) {
                auto yAxisRotate = [ ] ( const ggl::real& pRadians ) {
                    return ggl::matrixf<3, 3>{
                        +std::cos(pRadians), ggl::zero(), +std::sin(pRadians),
                        ggl::zero(), ggl::one(), ggl::zero(),
                        -std::sin(pRadians), ggl::zero(), +std::cos(pRadians),
                    };
                };

                for( const ggl::real& angle : testAngles ) {
                    const ggl::matrixf<4, 4> actual = ggl::xform::rotate( angle, yAxis );
                    const ggl::matrixf<3, 3> expected = yAxisRotate( angle );
                    REQUIRE( (actual.template submatrix<0, 0, 3, 3>()) == expected );
                }
            } THEN( "the rotate transform output matches the matrices for the z-axis formula" ) {
                auto zAxisRotate = [ ] ( const ggl::real& pRadians ) {
                    return ggl::matrixf<3, 3>{
                        +std::cos(pRadians), -std::sin(pRadians), ggl::zero(),
                        +std::sin(pRadians), +std::cos(pRadians), ggl::zero(),
                        ggl::zero(), ggl::zero(), ggl::one()
                    };
                };

                for( const ggl::real& angle : testAngles ) {
                    const ggl::matrixf<4, 4> actual = ggl::xform::rotate( angle, zAxis );
                    const ggl::matrixf<3, 3> expected = zAxisRotate( angle );
                    REQUIRE( (actual.template submatrix<0, 0, 3, 3>()) == expected );
                }
            }
        }

        WHEN( "the axis is not one of the canonical basis axes" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }

    GIVEN( "a three-dimensional rotation from an arbitrary basis" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}
