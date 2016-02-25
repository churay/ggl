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
                    REQUIRE( m(eIdx) == Approx(0.0f) );
            }
        }

        WHEN( "the dimensions are nontrivial" ) {
            const ggl::matrixf<2, 3> m;
            const unsigned r = m.sNumRows, c = m.sNumCols, e = m.sNumEnts;
            THEN( "the dimensions are correct" ) {
                REQUIRE( r == 2 ); REQUIRE( c == 3 ); REQUIRE( e == 6 );
            } THEN( "the elements are all zero" ) {
                for( unsigned eIdx = 0; eIdx < e; ++eIdx )
                    REQUIRE( m(eIdx) == Approx(0.0f) );
            }
        }
    }

    GIVEN( "explicit entries" ) {
        WHEN( "there are an appropriate number of entries" ) {
            const ggl::matrixf<1, 3> m{ 1.0f, 2.0f, 3.0f };
            const unsigned r = m.sNumRows, c = m.sNumCols, e = m.sNumEnts;
            THEN( "the dimensions are correct" ) {
                REQUIRE( r == 1 ); REQUIRE( c == 3 ); REQUIRE( e == 3 );
            } THEN( "the entries are in the correct positions" ) {
                for( unsigned eIdx = 0; eIdx < e; ++eIdx )
                    REQUIRE( m(eIdx) == Approx(eIdx + 1.0f) );
            }
        }
    }
}

SCENARIO( "ggl::matrix equality works", "[matrix]" ) {
    GIVEN( "two matrices with the same dimensions" ) {
        const ggl::matrix<int, 2, 2> m1{ 1, 1, 1, 1 };
        const ggl::matrix<int, 2, 2> m2{ 1, 1, 1, 1 };

        WHEN( "the same matrix is used in the comparison" ) {
            THEN( "the equality operator returns true" ) {
                REQUIRE( m1 == m1 );
            }
        }

        WHEN( "matrices with the same entries are used in the comparison" ) {
            const ggl::matrix<int, 2, 2> m2{ 1, 1, 1, 1 };
            THEN( "the equality operator returns true" ) {
                REQUIRE( m1 == m2 );
            }
        }

        WHEN( "matrices with different entries are used in the comparison" ) {
            const ggl::matrix<int, 2, 2> m2{ 1, 1, 1, 2 };
            THEN( "the equality operator returns false" ) {
                REQUIRE( m1 != m2 );
            }
        }
    }
}

SCENARIO( "ggl::matrix addition works", "[matrix]" ) {
    GIVEN( "two matrices with the same dimensions" ) {
        const ggl::matrix<int, 2, 2> m1{ 1, 2, 3, 4 };
        const ggl::matrix<int, 2, 2> m2{ 5, 6, 7, 8 };

        WHEN( "the matrices are added" ) {
            THEN( "the result matrix is the entry sum of the operands" ) {
                const ggl::matrix<int, 2, 2> expected{ 2, 4, 6, 8 };
                REQUIRE( (m1 + m1) == expected );
            } THEN( "the result matrix is the ordered sum of the operands" ) {
                const ggl::matrix<int, 2, 2> expected{ 6, 8, 10, 12 };
                REQUIRE( (m2 + m1) == expected );
            }
        }
    }
}
