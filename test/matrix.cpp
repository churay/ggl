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
    using matrixs2x2 = ggl::matrix<std::string, 2, 2>;

    GIVEN( "two matrices with the same dimensions" ) {
        const matrixs2x2 m1{ "a", "b", "c", "d" };
        const matrixs2x2 m2{ "e", "f", "g", "h" };

        WHEN( "the matrices are added" ) {
            THEN( "the result matrix is the entry sum of the operands" ) {
                const matrixs2x2 expected{ "aa", "bb", "cc", "dd" };
                REQUIRE( (m1 + m1) == expected );
            } THEN( "the result matrix is the ordered sum of the operands" ) {
                const matrixs2x2 expected{ "ea", "fb", "gc", "hd" };
                REQUIRE( (m2 + m1) == expected );
            }
        }
    }
}
