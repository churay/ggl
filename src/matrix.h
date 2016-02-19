#ifndef GGL_MATRIX_H
#define GGL_MATRIX_H

#include <array>
#include <utility>

// TODO(JRC): Figure out a way to alias the template parameters so that
// they're descriptive when used inside the class definitions but short and
// to the point when used outside (e.g. in the cpp file).

namespace ggl {

template <class tEntryType, unsigned tNumRows, unsigned tNumCols>
class matrix {
    public:

    /// Class Attributes ///

    constexpr static unsigned tNumEnts{ tNumRows * tNumCols };

    /// Constructors ///

    matrix();
    matrix( std::array<tEntryType, tNumEnts> pEntries );

    // TODO(JRC): Create a constructor that intakes an initialization list
    // with the appropriate number of entries (i.e. tNumEnts).

    /// Operator Overloads ///

    tEntryType& operator()( unsigned pRow, unsigned pCol );
    const tEntryType& operator()( unsigned pRow, unsigned pCol ) const;

    matrix<tEntryType, tNumRows, tNumCols> operator+( const matrix& pOther ) const;
    matrix<tEntryType, tNumRows, tNumCols> operator-( const matrix& pOther ) const;
    matrix<tEntryType, tNumRows, tNumCols> operator*( const tEntryType& pValue ) const;

    /// Class Functions ///

    // TODO(JRC): Implement these functions.

    private:

    /// Class Setup ///

    static_assert( tNumEnts > 0, "'ggl::matrix' must have positive entries." );

    template <class tET, unsigned tR, unsigned tC> friend class matrix;

    /// Helper Functions ///

    // TODO(JRC): Implement these functions.

    /// Class Fields ///

    std::array<tEntryType, tNumEnts> mEntries;
};

// TODO(JRC): Create a template typename for matricies that include floating
// point values called "matrixf" or equivalent.

// TODO(JRC): Create a template typename for vectors of both orientations
// (e.g. "rvectorf" and "cvectorf" or equivalent).

}

#include "matrix.cpp"

#endif
