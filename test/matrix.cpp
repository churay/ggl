#include "catch.hpp"
#include "matrix.hpp"

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

        WHEN( "the matrices are subtracted" ) {
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

SCENARIO( "ggl::matrix negation works", "[matrix]" ) {
    GIVEN( "a matrix with nontrivial dimensions" ) {
        const ggl::matrixi<2, 2> m1{ 1, 2, 3, 4 };
        const ggl::matrixi<2, 2> m2{ 5, 6, 7, 8 };

        WHEN( "the matrix is negated" ) {
            THEN( "the result matrix is the entry negation of the operands" ) {
                const ggl::matrixi<2, 2> e1{ -1, -2, -3, -4 };
                REQUIRE( -m1 == e1 );

                const ggl::matrixi<2, 2> e2{ -5, -6, -7, -8 };
                REQUIRE( -m2 == e2 );
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

        WHEN( "both of the matrices are nontrivial" ) {
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

SCENARIO( "ggl::matrix augment operation works", "[matrix]" ) {
    GIVEN( "two matrices with the same number of rows" ) {
        const ggl::matrixi<2, 1> m1{ 1, 2 };
        const ggl::matrixi<2, 3> m2{ 3, 4, 5, 6, 7, 8 };

        WHEN( "a matrix augmentation is performed" ) {
            THEN( "the result is a matrix containing the values [ LHS | RHS ]" ) {
                const ggl::matrixi<2, 4> e1{ 1, 3, 4, 5, 2, 6, 7, 8 };
                REQUIRE( (m1 | m2) == e1 );

                const ggl::matrixi<2, 4> e2{ 3, 4, 5, 1, 6, 7, 8, 2 };
                REQUIRE( (m2 | m1) == e2 );
            }
        }
    }
}

SCENARIO( "ggl::matrix submatrix operation works", "[matrix]" ) {
    GIVEN( "a matrix with nontrivial dimensions" ) {
        WHEN( "a trivial submatrix is queried" ) {
            const ggl::matrixi<2, 2> m{ 1, 2, 3, 4 };
            THEN( "the result is the entry of the source matrix at the submatrix coordinates" ) {
                REQUIRE( (m.template submatrix<0, 0, 1, 1>())(0, 0) == m(0, 0) );
                REQUIRE( (m.template submatrix<0, 1, 1, 1>())(0, 0) == m(0, 1) );
                REQUIRE( (m.template submatrix<1, 0, 1, 1>())(0, 0) == m(1, 0) );
                REQUIRE( (m.template submatrix<1, 1, 1, 1>())(0, 0) == m(1, 1) );
            }
        }

        WHEN( "a nontrivial submatrix is queried" ) {
            const ggl::matrixi<3, 3> m{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            THEN( "the result is the submatrix at the given subset of the source matrix" ) {
                const ggl::matrixi<2, 2> r1 = m.template submatrix<0, 0, 2, 2>();
                const ggl::matrixi<2, 2> expected1{ 1, 2, 4, 5 };
                REQUIRE( r1 == expected1 );

                const ggl::matrixi<2, 3> r2 = m.template submatrix<1, 0, 2, 3>();
                const ggl::matrixi<2, 3> expected2{ 4, 5, 6, 7, 8, 9 };
                REQUIRE( r2 == expected2 );

                const ggl::matrixi<1, 2> r3 = m.template submatrix<2, 1, 1, 2>();
                const ggl::matrixi<1, 2> expected3{ 8, 9 };
                REQUIRE( r3 == expected3 );
            }
        }
    }
}

SCENARIO( "ggl::matrix embed operation works", "[matrix]" ) {
    GIVEN( "a matrix with nontrivial dimensions" ) {
        WHEN( "a trivial submatrix is embedded" ) {
            ggl::matrixi<2, 2> m{ 1, 2, 3, 4 };
            THEN( "the target matrix has the source entry at the embedding coordinates" ) {
                m.template embed<0, 0>( ggl::matrixi<1, 1>(5) );
                m.template embed<1, 1>( ggl::matrixi<1, 1>(10) );

                const ggl::matrixi<2, 2> expected{ 5, 2, 3, 10 };
                REQUIRE( m == expected );
            }
        }

        WHEN( "a nontrivial submatrix is embedded" ) {
            ggl::matrixi<3, 3> m{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            THEN( "the target matrix has the subset values replaced by those in the source" ) {
                m.template embed<1, 1>( ggl::matrixi<2, 1>(20, 30) );

                const ggl::matrixi<3, 3> expected1{ 1, 2, 3, 4, 20, 6, 7, 30, 9 };
                REQUIRE( m == expected1 );

                m.template embed<2, 0>( ggl::matrixi<1, 3>(100, 200, 300) );

                const ggl::matrixi<3, 3> expected2{ 1, 2, 3, 4, 20, 6, 100, 200, 300 };
                REQUIRE( m == expected2 );
            }
        }
    }
}

SCENARIO( "ggl::matrix normal operation works", "[matrix]" ) {
    GIVEN( "a matrix with nontrivial dimensions" ) {
        WHEN( "the matrix is a vector" ) {
            THEN( "the normal is the euclidean length of the vector" ) {
                const ggl::vectori<2> v1{ 3, 4 };
                REQUIRE( v1.normal() == 5 );

                const ggl::vectori<2> v2{ 28, 45 };
                REQUIRE( v2.normal() == 53 );
            }
        }

        WHEN( "the matrix isn't a vector" ) {
            THEN( "the normal is the square root of the product of the entry squares" ) {
                const ggl::matrixf<2, 2> m1{ 1.0f, 1.0f, 1.0f, 1.0f };
                REQUIRE( m1.normal() == Approx(2.0f) );

                const ggl::matrixf<2, 2> m2{ 2.0f, 2.0f, 2.0f, 2.0f };
                REQUIRE( m2.normal() == Approx(4.0f) );
            }
        }
    }
}

SCENARIO( "ggl::matrix normalize operation works", "[matrix]" ) {
    GIVEN( "a matrix with nontrivial dimensions" ) {
        WHEN( "the matrix is a vector" ) {
            THEN( "the normalized vector is an equivalent vector of unit length" ) {
                const ggl::vectorf<2> testvec{ 3.0f, 4.0f };
                const ggl::real testvecNorm = testvec.normal();
                const ggl::vectorf<2> ntestvec{ testvec.normalize() };

                REQUIRE( ntestvec.normal() == Approx(1.0f) );
                REQUIRE( testvecNorm * ntestvec == testvec );
            }
        }

        WHEN( "the matrix isn't a vector" ) {
            THEN( "the normalized matrix is an equivalent matrix normalized by the 2-norm" ) {
                const ggl::matrixf<2, 2> testmat{ 2.0f, 2.0f, 2.0f, 2.0f };
                const ggl::real testmatNorm = testmat.normal();
                const ggl::matrixf<2, 2> ntestmat{ testmat.normalize() };

                REQUIRE( ntestmat.normal() == Approx(1.0f) );
                REQUIRE( testmatNorm * ntestmat == testmat );
            }
        }
    }
}

SCENARIO( "ggl::matrix transpose operation works", "[matrix]" ) {
    GIVEN( "a non-square m by n matrix" ) {
        const ggl::matrixi<2, 3> m{ 1, 2, 3, 4, 5, 6 };

        WHEN( "the matrix transpose operation is invoked" ) {
            THEN( "the result matrix is an n by m matrix with the input rows as columns" ) {
                const ggl::matrixi<3, 2> expected{ 1, 4, 2, 5, 3, 6 };
                REQUIRE( m.transpose() == expected );
            }
        }
    }
}

SCENARIO( "ggl::matrix rreform operation works", "[matrix][stub]" ) {
    GIVEN( "a square n by n matrix" ) {
        WHEN( "one or more of the matrix's rows are linearly dependent" ) {
            THEN( "the rreform is the identity for the number of linearly independent rows" ) {
                REQUIRE( 1 != 1 );
            }
        }

        WHEN( "all of the matrix's rows are linearly independent" ) {
            THEN( "the rreform is the identity matrix" ) {
                const ggl::matrixf<3, 3> identity{ 1.0f };
                REQUIRE( identity.rreform() == identity );

                const ggl::matrixf<3, 3> uptri{
                    10.0f, 8.0f, 3.0f,
                    0.0f, -3.0f, 2.0f,
                    0.0f, 0.0f, 3.0f
                };
                REQUIRE( uptri.rreform() == identity );

                const ggl::matrixf<3, 3> lotri{ uptri.transpose() };
                REQUIRE( lotri.rreform() == identity );

                const ggl::matrixf<3, 3> full{
                    10.0f, -10.0f, 3.0f,
                    33.0f, 55.0f, -23.0f,
                    1.0f, 5.0f, 9.0f
                };
                REQUIRE( full.rreform() == identity );
            }
        }
    }

    GIVEN( "a non-square m by n matrix" ) {
        WHEN( "" ) {
            THEN( "" ) {
                REQUIRE( 1 != 1 );
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
    GIVEN( "two nontrivial vectors with the same dimensions" ) {
        const ggl::vectorf<3> zerovec{ 0.0f, 0.0f, 0.0f };
        const ggl::vectorf<3> onevec{ 1.0f, 1.0f, 1.0f };

        const ggl::vectorf<3> testvec1{ 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f };
        const ggl::vectorf<3> testvec2{ 3.0f, 3.0f, 3.0f };

        WHEN( "the vectors are identical" ) {
            THEN( "the result of the dot operation is the square of the vector length" ) {
                REQUIRE( testvec1.dot(testvec1) == Approx(std::pow(testvec1.normal(), 2.0f)) );
                REQUIRE( testvec2.dot(testvec2) == Approx(std::pow(testvec2.normal(), 2.0f)) );
            }
        }

        WHEN( "the vectors are different" ) {
            THEN( "the result of the dot operation matches the matrix inner product" ) {
                REQUIRE( testvec1.dot(zerovec) == (testvec1.transpose() * zerovec)[0] );
                REQUIRE( testvec1.dot(onevec) == (testvec1.transpose() * onevec)[0] );
                REQUIRE( testvec1.dot(testvec2) == (testvec1.transpose() * testvec2)[0] );
            } THEN( "the dot operation is commutative" ) {
                REQUIRE( testvec1.dot(zerovec) == zerovec.dot(testvec1) );
                REQUIRE( testvec1.dot(onevec) == onevec.dot(testvec1) );
                REQUIRE( testvec1.dot(testvec2) == testvec2.dot(testvec1) );
            }
        }
    }
}

SCENARIO( "ggl::matrix tensor operation works", "[matrix]" ) {
    GIVEN( "two nontrivial vectors with the same dimensions" ) {
        const ggl::vectori<3> testvec1{ +1, +2, +3 };
        const ggl::vectori<3> testvec2{ +1, +0, -1 };
        const ggl::vectori<3> testvec3{ +5, +5, +5 };

        WHEN( "the operand vectors both have arbitrary values" ) {
            THEN( "the result of the tensor operation matches the matrix outer product" ) {
                REQUIRE( testvec1.tensor(testvec2) == testvec1 * testvec2.transpose() );
                REQUIRE( testvec1.tensor(testvec3) == testvec1 * testvec3.transpose() );
                REQUIRE( testvec2.tensor(testvec3) == testvec2 * testvec3.transpose() );
            }
        }
    }
}

SCENARIO( "ggl::matrix cross operation works", "[matrix]" ) {
    GIVEN( "two nontrivial vectors both with dimensionality three" ) {
        const ggl::vectori<3> zerovec{ 0, 0, 0 };
        const ggl::vectori<3> testvec1{ 1, 1, 1 };
        const ggl::vectori<3> testvec2{ 10, 0, -3 };

        WHEN( "the vectors are parallel or antiparallel" ) {
            THEN( "the result of the cross operation is the zero vector" ) {
                REQUIRE( testvec1.cross(+1*testvec1) == zerovec );
                REQUIRE( testvec1.cross(+5*testvec1) == zerovec );
                REQUIRE( testvec1.cross(-1*testvec1) == zerovec );

                REQUIRE( testvec2.cross(+1*testvec2) == zerovec );
                REQUIRE( testvec2.cross(+5*testvec2) == zerovec );
                REQUIRE( testvec2.cross(-1*testvec2) == zerovec );
            }
        }

        WHEN( "the vectors aren't parallel" ) {
            THEN( "the result of the cross operation is their combined entry matrix's determinant" ) {
                REQUIRE( testvec1.cross(zerovec) == zerovec );
                REQUIRE( testvec2.cross(zerovec) == zerovec );

                const ggl::vectori<3> expected{ -3, 13, -10 };
                REQUIRE( testvec1.cross(testvec2) == expected );
            } THEN( "the result of the cross operation is the negation of its commuted operation" ) {
                REQUIRE( testvec1.cross(testvec2) == -testvec2.cross(testvec1) );
            }
        }
    }
}

SCENARIO( "ggl::matrix angleTo operation works", "[matrix]" ) {
    GIVEN( "two nontrivial vectors with the same dimensions" ) {
        const ggl::vectorf<2> xvector{ 1.0f, 0.0f };
        const ggl::vectorf<2> yvector{ 0.0f, 1.0f };

        WHEN( "the vectors are identical" ) {
            THEN( "the angleTo operation returns 0" ) {
                REQUIRE( xvector.angleTo(xvector) == Approx(0.0f) );
                REQUIRE( yvector.angleTo(yvector) == Approx(0.0f) );
            }
        }

        WHEN( "the vectors are different" ) {
            const ggl::vectorf<2> nxvector{ -1.0f * xvector };
            const ggl::vectorf<2> nyvector{ -1.0f * yvector };

            THEN( "the angleTo operation returns the shortest angle between the vectors" ) {
                REQUIRE( xvector.angleTo(yvector) == Approx(ggl::pi() / 2.0f) );
                REQUIRE( xvector.angleTo(nxvector) == Approx(ggl::pi()) );
                REQUIRE( xvector.angleTo(nyvector) == Approx(ggl::pi() / 2.0f) );
            } THEN( "the angleTo operation is commutative" ) {
                REQUIRE( xvector.angleTo(yvector) == yvector.angleTo(xvector) );
                REQUIRE( xvector.angleTo(nxvector) == nxvector.angleTo(xvector) );
                REQUIRE( xvector.angleTo(nyvector) == nyvector.angleTo(xvector) );
            }
        }
    }
}

SCENARIO( "ggl::matrix projectOnto operation works", "[matrix]" ) {
    GIVEN( "two nontrivial vectors with the same dimensions" ) {
        const ggl::vectorf<2> xvector{ 1.0f, 0.0f };
        const ggl::vectorf<2> yvector{ 0.0f, 1.0f };

        WHEN( "the vectors are parallel" ) {
            THEN( "the result of the projection is the input vector" ) {
                REQUIRE( xvector.projectOnto(xvector) == xvector );
                REQUIRE( xvector.projectOnto(2.0f*xvector) == xvector );
                REQUIRE( xvector.projectOnto(0.5f*xvector) == xvector );
            }
        }

        WHEN( "the vectors are orthogonal" ) {
            THEN( "the result of the projection is the zero vector" ) {
                const ggl::vectorf<2> zerovector{ 0.0f, 0.0f };

                REQUIRE( xvector.projectOnto(yvector) == zerovector );
                REQUIRE( xvector.projectOnto(2.0f*yvector) == zerovector );
                REQUIRE( xvector.projectOnto(0.5f*yvector) == zerovector );
            }
        }

        WHEN( "the vectors are neither parallel nor orthogonal" ) {
            THEN( "the result of the projection is the projection of the LHS onto the RHS" ) {
                const ggl::vectorf<2> xvectorDelta{ 1.0f, 3.0f };
                REQUIRE( xvectorDelta.projectOnto(xvector) == xvector );

                const ggl::vectorf<2> yvectorDelta{ 0.25f, 1.0f };
                REQUIRE( yvectorDelta.projectOnto(yvector) == yvector );
            }
        }
    }
}
