#include "src/matrix.h"

SCENARIO( "ggl::matrix is correctly constructed", "[matrix]" ) {
    GIVEN( "no parameters (default constructor)" ) {
        // TODO(JRC): For some reason, using "m.sNumRows" in an assertion
        // causes an undefined reference error.
        WHEN( "the dimensions are trivial" ) {
            const ggl::matrixf<1, 1> m;
            const unsigned r = m.sNumRows, c = m.sNumCols, e = m.sNumEnts;
            THEN( "the dimensions are correct" ) {
                REQUIRE( r == 1 ); REQUIRE( c == 1 ); REQUIRE( e == 1 );
            } THEN( "the elements are all zero" ) {
                for( unsigned eIdx = 0; eIdx < e; ++eIdx )
                    REQUIRE( m[eIdx] == Approx(0.0f) );
            }
        }

        WHEN( "the dimensions are nontrivial" ) {
            const ggl::matrixf<2, 3> m;
            const unsigned r = m.sNumRows, c = m.sNumCols, e = m.sNumEnts;
            THEN( "the dimensions are correct" ) {
                REQUIRE( r == 2 ); REQUIRE( c == 3 ); REQUIRE( e == 6 );
            } THEN( "the elements are all zero" ) {
                for( unsigned eIdx = 0; eIdx < e; ++eIdx )
                    REQUIRE( m[eIdx] == Approx(0.0f) );
            }
        }
    }

    GIVEN( "explicit entries" ) {
        WHEN( "a single entry is given" ) {
            const ggl::matrixf<3, 3> m{ 1.0f };
            const unsigned r = m.sNumRows, c = m.sNumCols, e = m.sNumEnts;
            THEN( "the dimensions are correct" ) {
                REQUIRE( r == 3 ); REQUIRE( c == 3 ); REQUIRE( e == 9 );
            } THEN( "the matrix diagonal is initialized with the explicit value" ) {
                for( unsigned rIdx = 0; rIdx < r; ++rIdx )
                    for( unsigned cIdx = 0; cIdx < c; ++cIdx )
                        REQUIRE( m(rIdx, cIdx) == Approx(rIdx == cIdx) );
            }
        }

        WHEN( "the exact number of entries are given" ) {
            const ggl::matrixf<1, 3> m{ 1.0f, 2.0f, 3.0f };
            const unsigned r = m.sNumRows, c = m.sNumCols, e = m.sNumEnts;
            THEN( "the dimensions are correct" ) {
                REQUIRE( r == 1 ); REQUIRE( c == 3 ); REQUIRE( e == 3 );
            } THEN( "the entries are in the correct positions" ) {
                for( unsigned eIdx = 0; eIdx < e; ++eIdx )
                    REQUIRE( m[eIdx] == Approx(eIdx + 1.0f) );
            }
        }
    }
}

SCENARIO( "ggl::matrix equality works", "[matrix]" ) {
    GIVEN( "two matrices with the same dimensions" ) {
        const ggl::matrixi<2, 2> m1{ 1, 1, 1, 1 };
        const ggl::matrixi<2, 2> m2{ 1, 1, 1, 1 };

        WHEN( "the same matrix is used in the comparison" ) {
            THEN( "the equality operator returns true" ) {
                REQUIRE( m1 == m1 );
            }
        }

        WHEN( "matrices with the same entries are used in the comparison" ) {
            const ggl::matrixi<2, 2> m2{ 1, 1, 1, 1 };
            THEN( "the equality operator returns true" ) {
                REQUIRE( m1 == m2 );
            }
        }

        WHEN( "matrices with different entries are used in the comparison" ) {
            const ggl::matrixi<2, 2> m2{ 1, 1, 1, 2 };
            THEN( "the equality operator returns false" ) {
                REQUIRE( m1 != m2 );
            }
        }
    }
}

SCENARIO( "ggl::matrix addition works", "[matrix]" ) {
    GIVEN( "two matrices with the same dimensions" ) {
        const ggl::matrixi<2, 2> m1{ 1, 2, 3, 4 };
        const ggl::matrixi<2, 2> m2{ 5, 6, 7, 8 };

        WHEN( "the matrices are added" ) {
            THEN( "the result matrix is the entry sum of the operands" ) {
                const ggl::matrixi<2, 2> expected{ 2, 4, 6, 8 };
                REQUIRE( (m1 + m1) == expected );
            } THEN( "the result matrix is the ordered sum of the operands" ) {
                const ggl::matrixi<2, 2> expected{ 6, 8, 10, 12 };
                REQUIRE( (m2 + m1) == expected );
            }
        }
    }
}

SCENARIO( "ggl::matrix subtraction works", "[matrix]" ) {
    GIVEN( "two matrices with the same dimensions" ) {
        const ggl::matrixi<2, 2> m1{ 1, 2, 3, 4 };
        const ggl::matrixi<2, 2> m2{ 8, 7, 6, 5 };

        WHEN( "the matrices are added" ) {
            THEN( "the result matrix is the entry subtraction of the operands" ) {
                const ggl::matrixi<2, 2> expected{ 0, 0, 0, 0 };
                REQUIRE( (m1 - m1) == expected );
            } THEN( "the result matrix is the right-hand matrix subtracted from the left-hand matrix " ) {
                const ggl::matrixi<2, 2> expected1{ 7, 5, 3, 1 };
                REQUIRE( (m2 - m1) == expected1 );

                const ggl::matrixi<2, 2> expected2{ -7, -5, -3, -1 };
                REQUIRE( (m1 - m2) == expected2 );
            }
        }
    }
}

SCENARIO( "ggl::matrix matrix multiplication works", "[matrix]" ) {
    GIVEN( "two matrices with the same dimensions" ) {
        const ggl::matrixi<2, 2> m1{ 1, 2, 3, 4 };

        WHEN( "one of the matrices is the identity matrix" ) {
            const ggl::matrixi<2, 2> mi{ 1, 0, 0, 1 };
            THEN( "the result matrix is the same as the non-identity matrix" ) {
                REQUIRE( (mi * m1) == m1 );
                REQUIRE( (m1 * mi) == m1 );
            }
        }

        WHEN( "both of the matrices are non-trivial" ) {
            THEN( "the result matrix is the product of the operands" ) {
                const ggl::matrixi<2, 2> expected{ 7, 10, 15, 22 };
                REQUIRE( (m1 * m1) == expected );
            } THEN( "the result matrix is the ordered product of the operands" ) {
                const ggl::matrixi<2, 2> o1{ 0, 1, 0, 0 };
                const ggl::matrixi<2, 2> o2{ 1, 0, 0, 0 };

                const ggl::matrixi<2, 2> expected1{ 0, 0, 0, 0 };
                REQUIRE( (o1 * o2) == expected1 );

                const ggl::matrixi<2, 2> expected2{ 0, 1, 0, 0 };
                REQUIRE( (o2 * o1) == expected2 );
            }
        }
    }
}

SCENARIO( "ggl::matrix scalar multiplication works", "[matrix]" ) {
    GIVEN( "a matrix and a scalar value" ) {
        const ggl::matrixi<2, 2> m{ 1, 2, 3, 4 };
        const int s = 100;

        WHEN( "the matrix is multiplied by the scalar on the left-hand side" ) {
            THEN( "the result matrix is the input entry-scaled by the scalar" ) {
                const ggl::matrixi<2, 2> expected{ 100, 200, 300, 400 };
                REQUIRE( (s * m) == expected );
            }
        }

        WHEN( "the matrix is multiplied by the scalar on the right-hand side" ) {
            THEN( "the result matrix is the input entry-scaled by the scalar" ) {
                const ggl::matrixi<2, 2> expected{ 100, 200, 300, 400 };
                REQUIRE( (m * s) == expected );
            }
        }
    }
}

SCENARIO( "ggl::matrix normal operation works", "[matrix]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::matrix normalize operation works", "[matrix]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::matrix transpose operation works", "[matrix]" ) {
    GIVEN( "a non-square m by n matrix" ) {
        const ggl::matrixi<2, 3> m{ 1, 2, 3, 4, 5, 6 };

        WHEN( "the matrix transpose operation is invoked" ) {
            THEN( "the result matrix is an n by m matrix with the input rows as columns" ) {
                const ggl::matrixi<3, 2> e{ 1, 4, 2, 5, 3, 6 };
                REQUIRE( m.transpose() == e );
            }
        }
    }
}

SCENARIO( "ggl::matrix determinant operation works", "[matrix]" ) {
    GIVEN( "a square matrix" ) {
        WHEN( "the matrix is the identity matrix" ) {
            THEN( "the determinant result is 1" ) {
                ggl::matrixi<2, 2> m2x2{ 1 };
                REQUIRE( m2x2.determinant() == 1 );

                ggl::matrixi<4, 4> m4x4{ 1 };
                REQUIRE( m4x4.determinant() == 1 );
            }
        }

        WHEN( "the matrix is a non-singular matrix that isn't the identity" ) {
            THEN( "the determinant is the signed area of the rectangle defined by the matrix columns" ) {
                ggl::matrixf<2, 2> m2x2{
                    3.0f, 8.0f,
                    4.0f, 6.0f
                };
                REQUIRE( m2x2.determinant() == Approx(-14.0f) );

                ggl::matrixf<3, 3> m3x3{
                    4.0f, -1.0f, 1.0f,
                    4.0f, 5.0f, 3.0f,
                    -2.0f, 0.0f, 0.0f
                };
                REQUIRE( m3x3.determinant() == Approx(16.0f) );
            }
        }

        WHEN( "the matrix is a singular matrix that isn't the identity" ) {
            THEN( "the determinant result is 0" ) {
                ggl::matrixf<2, 2> m2x2{
                    0.0f, 1.0f,
                    0.0f, 0.0f
                };
                REQUIRE( m2x2.determinant() == Approx(0.0f) );

                ggl::matrixf<3, 3> m3x3{
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f
                };
                REQUIRE( m3x3.determinant() == Approx(0.0f) );
            }
        }
    }
}

SCENARIO( "ggl::matrix inverse operation works", "[matrix]" ) {
    GIVEN( "a square matrix" ) {
        WHEN( "the matrix is the identity matrix" ) {
            THEN( "the inverse of the matrix is also the identity matrix" ) {
                ggl::matrixi<2, 2> m2x2{ 1 };
                REQUIRE( m2x2.inverse() == m2x2 );

                ggl::matrixi<4, 4> m4x4{ 1 };
                REQUIRE( m4x4.inverse() == m4x4 );
            }
        }

        WHEN( "the matrix is not the identity matrix" ) {
            THEN( "the inverse is correct" ) {
                ggl::matrixf<2, 2> m{ 2.0f, 0.0f, 0.0f, 4.0f };
                ggl::matrixf<2, 2> expected{ 0.5f, 0.0f, 0.0f, 0.25f };

                REQUIRE( m.inverse() == expected );
            } THEN( "the product of the matrix and its inverse is the identity matrix" ) {
                ggl::matrixf<2, 2> m{ 1.0f, 2.0f, 3.0f, 4.0f };
                ggl::matrixf<2, 2> identity{ 1.0f };

                REQUIRE( m * m.inverse() == identity );
                REQUIRE( m.inverse() * m == identity );
            }
        }
    }
}

SCENARIO( "ggl::matrix dot operation works", "[matrix]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}

SCENARIO( "ggl::matrix cross operation works", "[matrix]" ) {
    GIVEN( "" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
            }
        }
    }
}
