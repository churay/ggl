#ifndef GGL_MATRIX_H
#define GGL_MATRIX_H

#include <memory>
#include <utility>

namespace ggl {

template <unsigned Rows, unsigned Cols, class EntryType=float>
class matrix {
    public:

    /// Constructors ///

    matrix();

    /// Operator Overloads ///

    EntryType& operator()( unsigned _row, unsigned _col );
    const EntryType& operator()( unsigned _row, unsigned _col ) const;

    /// Class Functions ///

    // TODO(JRC): Implement these functions.

    /// Class Attributes ///

    constexpr static unsigned Entries{ Rows * Cols };

    private:

    /// Class Setup ///

    template <unsigned R, unsigned C> friend class matrix;

    static_assert( Entries > 0, "'ggl::matrix' must have positive entries." );

    /// Helper Functions ///

    // TODO(JRC): Implement these functions.

    /// Class Fields ///

    std::array<EntryType, Entries> m_entries;
};

}

#include "matrix.cpp"

#endif
