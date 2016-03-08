#ifndef GGL_MATRIX_H
#define GGL_MATRIX_H

#include <array>
#include <functional>
#include <type_traits>
#include <utility>

#include "tutil.h"

namespace ggl {

template <class T, size_t R, size_t C, class LT=std::less<T>>
class matrix {
    public:

    /// Class Attributes ///

    using EntryType = T;
    using Compare = LT;
    constexpr static size_t sNumRows{ R };
    constexpr static size_t sNumCols{ C };
    constexpr static size_t sNumEnts{ R * C };

    constexpr static EntryType sZeroValue{ 0 };
    constexpr static LT sLessThan{};

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

    matrix<T, R, C, LT> operator+( const matrix& pOther ) const;
    matrix<T, R, C, LT> operator-( const matrix& pOther ) const;
    matrix<T, R, C, LT> operator*( const EntryType& pValue ) const;
    template <size_t C2>
    matrix<T, R, C2, LT> operator*( const matrix<T, C, C2, LT>& pOther ) const;
    template <size_t C2>
    matrix<T, R, C+C2, LT> operator|( const matrix<T, R, C2, LT>& pOther ) const;

    /// Class Functions ///

    template <size_t SR0, size_t SC0, size_t SR, size_t SC>
    matrix<T, SR, SC, LT> submatrix() const;

    EntryType normal() const;
    matrix<T, R, C, LT> normalize() const;
    matrix<T, C, R, LT> transpose() const;

    EntryType determinant() const;
    matrix<T, R, C, LT> inverse() const;

    EntryType dot( const matrix& pOther ) const;
    matrix<T, R, C, LT> cross( const matrix& pOther ) const;

    const EntryType* data() const;

    private:

    /// Class Setup ///

    template <class TT, size_t RR, size_t CC, class LTLT> friend class matrix;

    static_assert( sNumEnts > 0,
        "'ggl::matrix' must have a positive number of entries." );
    static_assert( std::is_arithmetic<EntryType>::value,
        "'ggl::matrix' must have an arithmetic entry type." );

    /// Private Functions ///

    bool _areEqual( const EntryType& pValue1, const EntryType& pValue2 ) const;

    void _swapRows( size_t pSrcRow, size_t pDstRow );
    void _scaleRows( size_t pSrcRow, const EntryType& pScale );
    void _addRows( size_t pSrcRow, size_t pDstRow, const EntryType& pScale );

    /// Class Fields ///

    std::array<EntryType, sNumEnts> mEntries;
};

template <class T, size_t R, class LT=std::less<T>> using vector = matrix<T, R, 1, LT>;

template <size_t R, size_t C> using matrixi = matrix<int, R, C>;
template <size_t R> using vectori = vector<int, R>;

template <size_t R, size_t C> using matrixf = matrix<float, R, C, ggl::fless<float>>;
template <size_t R> using vectorf = vector<float, R, ggl::fless<float>>;

}

#include "matrix.cpp"

#endif
