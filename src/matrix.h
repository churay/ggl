#ifndef GGL_MATRIX_H
#define GGL_MATRIX_H

#include <array>
#include <utility>

namespace ggl {

template <class T, unsigned R, unsigned C>
class matrix {
    public:

    /// Class Attributes ///

    using EntryType = T;
    constexpr static unsigned sNumRows{ R };
    constexpr static unsigned sNumCols{ C };
    constexpr static unsigned sNumEnts{ R * C };

    /// Constructors ///

    matrix();
    matrix( std::array<EntryType, sNumEnts> pEntries );

    // TODO(JRC): Create a constructor that intakes an initialization list
    // with the appropriate number of entries (i.e. sNumEnts).

    /// Operator Overloads ///

    EntryType& operator()( unsigned pRow, unsigned pCol );
    const EntryType& operator()( unsigned pRow, unsigned pCol ) const;

    matrix<T, R, C> operator+( const matrix& pOther ) const;
    matrix<T, R, C> operator-( const matrix& pOther ) const;
    matrix<T, R, C> operator*( const EntryType& pValue ) const;
    template <unsigned C2>
    matrix<T, R, C2> operator*( const matrix<T, C, C2>& pOther ) const;

    matrix<T, C, R> transpose() const;

    /// Class Functions ///

    // TODO(JRC): Implement these functions.

    private:

    /// Class Setup ///

    static_assert( sNumEnts > 0, "'ggl::matrix' must have positive entries." );

    template <class TT, unsigned RR, unsigned CC> friend class matrix;

    /// Helper Functions ///

    // TODO(JRC): Implement these functions.

    /// Class Fields ///

    std::array<EntryType, sNumEnts> mEntries;
};

template <unsigned R, unsigned C> using matrixf = matrix<float, R, C>;
template <class T, unsigned R> using vectorf = matrix<T, R, 1>;

}

#include "matrix.cpp"

#endif
