#include "catch.hpp"
#include "src/xform.hpp"

#include <iostream>

SCENARIO( "ggl::xform basis operation works", "[xform]" ) {
    GIVEN( "an arbitrary basis vector" ) {
        WHEN( "the basis vector is the +z vector" ) {
            THEN( "the output basis is the canonical basis" ) {
                const ggl::vectorf<3> zvec{ ggl::zero(), ggl::zero(), ggl::one() };
                const std::array<ggl::vectorf<3>, 3> zbasis = ggl::xform::basis( zvec );
                const std::array<ggl::vectorf<3>, 3> expected{{
                    ggl::vectorf<3>{ ggl::one(), ggl::zero(), ggl::zero() },
                    ggl::vectorf<3>{ ggl::zero(), ggl::one(), ggl::zero() },
                    zvec
                }};

                REQUIRE( zbasis == expected );
            }
        }

        WHEN( "the basis vector is a vector that isn't a canonical basis vector" ) {
            const ggl::vectorf<3> bvec{ ggl::pi(), ggl::pi(), ggl::pi() };
            const std::array<ggl::vectorf<3>, 3> basis = ggl::xform::basis( bvec );

            THEN( "the output basis has the normalized basis vector input as its +Z" ) {
                REQUIRE( basis[2] == bvec.normalize() );
            } THEN( "the output basis is a valid basis" ) {
                for( size_t vIdx = 0; vIdx < 3; ++vIdx ) {
                    size_t nvIdx = ( vIdx + 1 ) % 3;
                    REQUIRE( basis[vIdx].dot(basis[nvIdx]) == Approx(ggl::zero()) );
                }
            } THEN( "the output basis is properly oriented to the canonical up vector" ) {
                const ggl::vectorf<3> upvec{ ggl::zero(), ggl::one(), ggl::zero() };
                REQUIRE( basis[0] == upvec.cross(bvec).normalize() );
                REQUIRE( basis[1] == bvec.cross(upvec.cross(bvec)).normalize() );
            }
        }
    }
}

SCENARIO( "ggl::xform scale operation works", "[xform]" ) {
    const int zero = ggl::zero<int>(), one = ggl::one<int>();
    const ggl::matrixi<3, 3> identity2d{ one };
    const ggl::matrixi<4, 4> identity3d{ one };

    GIVEN( "an arbitrary scale" ) {
        WHEN( "the scale amount is uniformly one" ) {
            THEN( "the scale transform output is the identity matrix" ) {
                REQUIRE( ggl::xform::scale(one, one) == identity2d );
                REQUIRE( ggl::xform::scale(one, one, one) == identity3d );
            }
        }

        WHEN( "the scale amount is nonuniform" ) {
            THEN( "the scale transform output appropriately scales along each dimension" ) {
                const int xScale = 5, yScale = -3, zScale = 1;
                const ggl::matrixi<4, 4> expected{
                    xScale, zero, zero, zero,
                    zero, yScale, zero, zero,
                    zero, zero, zScale, zero,
                    zero, zero, zero, one
                };

                REQUIRE( ggl::xform::scale(xScale, yScale, zScale) == expected );
            }
        }
    }
}

SCENARIO( "ggl::xform translate operation works", "[xform]" ) {
    const int zero = ggl::zero<int>(), one = ggl::one<int>();
    const ggl::matrixi<3, 3> identity2d{ one };
    const ggl::matrixi<4, 4> identity3d{ one };

    GIVEN( "an arbitrary translation" ) {
        WHEN( "the translation amount is uniformly zero" ) {
            THEN( "the translation transform output is the identity matrix" ) {
                REQUIRE( ggl::xform::translate(zero, zero) == identity2d );
                REQUIRE( ggl::xform::translate(zero, zero, zero) == identity3d );
            }
        }

        WHEN( "the translation amount is nonuniform and nonzero" ) {
            THEN( "the translation transform output appropriately transforms along each dimension" ) {
                const int xTrans = 5, yTrans = -3, zTrans = 1;
                const ggl::matrixi<4, 4> expected{
                    one, zero, zero, xTrans,
                    zero, one, zero, yTrans,
                    zero, zero, one, zTrans,
                    zero, zero, zero, one
                };

                REQUIRE( ggl::xform::translate(xTrans, yTrans, zTrans) == expected );
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
            THEN( "the rotate transform output transforms the space by the given angle CCW" ) {
                const ggl::vectorf<3> xAxis{ ggl::one(), ggl::zero(), ggl::zero() };
                const ggl::vectorf<3> yAxis{ ggl::zero(), ggl::one(), ggl::zero() };
                const ggl::vectorf<3> nilVec{ ggl::zero(), ggl::zero(), ggl::one() };

                const ggl::real rotAmt90{ (1.0f * ggl::pi()) / 2.0f };
                const ggl::matrixf<3, 3> rotMat90 = ggl::xform::rotate<F, FLT>( rotAmt90 );
                REQUIRE( (rotMat90.template submatrix<0, 0, 3, 1>()) == +yAxis );
                REQUIRE( (rotMat90.template submatrix<0, 1, 3, 1>()) == -xAxis );
                REQUIRE( (rotMat90.template submatrix<0, 2, 3, 1>()) == nilVec );

                const ggl::real rotAmt180{ (2.0f * ggl::pi()) / 2.0f };
                const ggl::matrixf<3, 3> rotMat180 = ggl::xform::rotate<F, FLT>( rotAmt180 );
                REQUIRE( (rotMat180.template submatrix<0, 0, 3, 1>()) == -xAxis );
                REQUIRE( (rotMat180.template submatrix<0, 1, 3, 1>()) == -yAxis );
                REQUIRE( (rotMat180.template submatrix<0, 2, 3, 1>()) == nilVec );
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
