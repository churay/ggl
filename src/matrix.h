#ifndef GGL_MATRIX_H
#define GGL_MATRIX_H

#include <array>
#include <utility>
#include <type_traits>

namespace ggl {

template <class T, size_t R, size_t C>
class matrix {
    public:

    /// Class Attributes ///

    using EntryType = T;
    constexpr static size_t sNumRows{ R };
    constexpr static size_t sNumCols{ C };
    constexpr static size_t sNumEnts{ R * C };

    /// Constructors ///

    matrix();
    template <class... Ts> explicit matrix( Ts&&... pEntries );

    /// Operator Overloads ///

    EntryType& operator[]( size_t pEntry );
    const EntryType& operator[]( size_t pEntry ) const;
    EntryType& operator()( size_t pRow, size_t pCol );
    const EntryType& operator()( size_t pRow, size_t pCol ) const;

    bool operator==( const matrix& pOther ) const;
    bool operator!=( const matrix& pOther ) const;

    matrix<T, R, C> operator+( const matrix& pOther ) const;
    matrix<T, R, C> operator-( const matrix& pOther ) const;
    matrix<T, R, C> operator*( const EntryType& pValue ) const;
    template <size_t C2>
    matrix<T, R, C2> operator*( const matrix<T, C, C2>& pOther ) const;
    template <size_t C2>
    matrix<T, R, C+C2> operator|( const matrix<T, R, C2>& pOther ) const;

    /// Class Functions ///

    template <size_t SR0, size_t SC0, size_t SR, size_t SC>
    matrix<T, SR, SC> submatrix() const;

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

    template <class TT, size_t RR, size_t CC> friend class matrix;

    static_assert( sNumEnts > 0,
        "'ggl::matrix' must have a positive number of entries." );
    static_assert( std::is_arithmetic<EntryType>::value,
        "'ggl::matrix' must have an arithmetic entry type." );

    /// Private Functions ///

    void _swapRows( size_t pSrcRow, size_t pDstRow );
    void _scaleRows( size_t pSrcRow, const EntryType& pScale );
    void _addRows( size_t pSrcRow, size_t pDstRow, const EntryType& pScale );

    /// Class Fields ///

    std::array<EntryType, sNumEnts> mEntries;
};

template <class T, size_t R> using vector = matrix<T, R, 1>;

template <size_t R, size_t C> using matrixf = matrix<float, R, C>;
template <size_t R> using vectorf = vector<float, R>;

}

#include "matrix.cpp"

#endif
