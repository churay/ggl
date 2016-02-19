#ifndef GGL_MATRIX_H
#define GGL_MATRIX_H

#include <array>
#include <memory>
#include <utility>

namespace ggl {

template <class tEntryType, unsigned tNumRows, unsigned tNumCols>
class matrix {
    public:

    /// Class Attributes ///

    constexpr static unsigned tNumEnts{ tNumRows * tNumCols };

    /// Constructors ///

    matrix();
    matrix( const std::array<tEntryType, tNumEnts>& pEntries );

    /// Operator Overloads ///

    tEntryType& operator()( unsigned pRow, unsigned pCol );
    const tEntryType& operator()( unsigned pRow, unsigned pCol ) const;

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

}

#include "matrix.cpp"

#endif
