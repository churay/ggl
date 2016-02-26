#ifndef GGL_MATRIX_H
#define GGL_MATRIX_H

// NOTE(JRC): LibStdC++ defines a "minor" macro, which needs to be unbound in
// order to define a "minor" function.
#undef minor

#include <array>
#include <utility>
#include <type_traits>

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
    template <class... Ts> explicit matrix( Ts&&... pEntries );

    /// Operator Overloads ///

    EntryType& operator()( unsigned pEntry );
    const EntryType& operator()( unsigned pEntry ) const;
    EntryType& operator()( unsigned pRow, unsigned pCol );
    const EntryType& operator()( unsigned pRow, unsigned pCol ) const;

    bool operator==( const matrix& pOther ) const;
    bool operator!=( const matrix& pOther ) const;

    matrix<T, R, C> operator+( const matrix& pOther ) const;
    matrix<T, R, C> operator-( const matrix& pOther ) const;
    matrix<T, R, C> operator*( const EntryType& pValue ) const;
    template <unsigned C2>
    matrix<T, R, C2> operator*( const matrix<T, C, C2>& pOther ) const;

    /// Class Functions ///

    EntryType normal() const;
    matrix<T, R, C> normalize() const;
    matrix<T, C, R> transpose() const;

    EntryType determinant() const;
    matrix<T, R, C> inverse() const;

    EntryType dot( const matrix& pOther ) const;
    matrix<T, R, C> cross( const matrix& pOther ) const;

    const EntryType* data() const;

    private:

    /// Class Setup ///

    template <class TT, unsigned RR, unsigned CC> friend class matrix;

    static_assert( sNumEnts > 0,
        "'ggl::matrix' must have a positive number of entries." );
    static_assert( std::is_arithmetic<EntryType>::value,
        "'ggl::matrix' must have an arithmetic entry type." );

    /// Class Fields ///

    std::array<EntryType, sNumEnts> mEntries;
};

template <class T, unsigned R> using vector = matrix<T, R, 1>;

template <unsigned R, unsigned C> using matrixf = matrix<float, R, C>;
template <unsigned R> using vectorf = vector<float, R>;

}

#include "matrix.cpp"

#endif
