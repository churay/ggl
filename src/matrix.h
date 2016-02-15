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

    private:

    /// Class Setup ///

    template <unsigned R, unsigned C> friend class Matrix;

    static_assert( Rows * Cols > 0, "Matrices must have positive dimensions!" );

    /// Helper Functions ///

    // TODO(JRC): Implement these functions.

    /// Class Fields ///

    std::unique_ptr<float[]> m_entries{ new float[Rows * Cols] };
};

}

#include "matrix.cpp"

#endif
