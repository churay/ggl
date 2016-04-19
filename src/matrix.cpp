#include <algorithm>
#include <cmath>
#include <sstream>
#include <utility>

#include "consts.hpp"

namespace ggl {

// NOTE(JRC): This is weird, but necessary according to this Stack Overflow response:
// http://stackoverflow.com/q/8016780/837221
template <class T, size_t R, size_t C, class LT>
constexpr size_t matrix<T, R, C, LT>::sNumRows;
template <class T, size_t R, size_t C, class LT>
constexpr size_t matrix<T, R, C, LT>::sNumCols;
template <class T, size_t R, size_t C, class LT>
constexpr size_t matrix<T, R, C, LT>::sNumEnts;
template <class T, size_t R, size_t C, class LT>
constexpr LT matrix<T, R, C, LT>::sLessThan;


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT>::matrix() {
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        (*this)[eIdx] = ggl::zero<T>();
}


// TODO(JRC): Optimize this function by using template specialization.
// NOTE(JRC): Recipe for argument constructor from the following location:
// http://stackoverflow.com/a/7725611
template <class T, size_t R, size_t C, class LT> template <class... Ts>
matrix<T, R, C, LT>::matrix( Ts&&... pEntries ) : mEntries{{ std::forward<Ts>(pEntries)... }} {
    static_assert( sizeof...(Ts) == sNumEnts || sizeof...(Ts) == 1,
        "'ggl::matrix' value constructor must be given exactly R*C|1 entries" );

    if( sizeof...(Ts) == 1 )
        for( size_t rIdx = 0; rIdx < sNumRows; ++rIdx )
            for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx )
                (*this)(rIdx, cIdx) = ( rIdx == cIdx ) ? (*this)(0, 0) : ggl::zero<T>();
}


template <class T, size_t R, size_t C, class LT>
T& matrix<T, R, C, LT>::operator[]( size_t pEntry ) {
    return mEntries[pEntry];
}


template <class T, size_t R, size_t C, class LT>
const T& matrix<T, R, C, LT>::operator[]( size_t pEntry ) const {
    return mEntries[pEntry];
}


template <class T, size_t R, size_t C, class LT>
T& matrix<T, R, C, LT>::operator()( size_t pRow, size_t pCol ) {
    return mEntries[pRow * sNumCols + pCol];
}


template <class T, size_t R, size_t C, class LT>
const T& matrix<T, R, C, LT>::operator()( size_t pRow, size_t pCol ) const {
    return mEntries[pRow * sNumCols + pCol];
}


template <class T, size_t R, size_t C, class LT>
bool matrix<T, R, C, LT>::operator==( const matrix& pOther ) const {
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        if( !_areEqual((*this)[eIdx], pOther[eIdx]) )
            return false;

    return true;
}


template <class T, size_t R, size_t C, class LT>
bool matrix<T, R, C, LT>::operator!=( const matrix& pOther ) const {
    return !( *this == pOther );
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::operator+() const {
    return (*this);
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::operator-() const {
    matrix<EntryType, sNumRows, sNumCols, Compare> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result[eIdx] = -ggl::one<T>() * (*this)[eIdx];

    return result;
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::operator+( const matrix& pOther ) const {
    matrix<EntryType, sNumRows, sNumCols, Compare> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result[eIdx] = (*this)[eIdx] + pOther[eIdx];

    return result;
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::operator-( const matrix& pOther ) const {
    matrix<EntryType, sNumRows, sNumCols, Compare> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result[eIdx] = (*this)[eIdx] - pOther[eIdx];

    return result;
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::operator*( const T& pValue ) const {
    matrix<EntryType, sNumRows, sNumCols, Compare> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result[eIdx] = (*this)[eIdx] * pValue;

    return result;
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> operator*( const T& pValue, const matrix<T, R, C, LT>& pMatrix ) {
    matrix<T, R, C, LT> result;
    for( size_t eIdx = 0; eIdx < result.sNumEnts; ++eIdx )
        result[eIdx] = pValue * pMatrix[eIdx];

    return result;
}


template <class T, size_t R, size_t C, class LT> template <size_t C2>
matrix<T, R, C2, LT> matrix<T, R, C, LT>::operator*( const matrix<T, C, C2, LT>& pOther ) const {
    matrix<EntryType, R, C2, Compare> result;
    for( size_t rIdx = 0; rIdx < result.sNumRows; ++rIdx )
        for( size_t cIdx = 0; cIdx < result.sNumCols; ++cIdx )
            for( size_t iIdx = 0; iIdx < (*this).sNumCols; ++iIdx )
                result(rIdx, cIdx) += (*this)(rIdx, iIdx) * pOther(iIdx, cIdx);

    return result;
}


template <class T, size_t R, size_t C, class LT> template <size_t C2>
matrix<T, R, C+C2, LT> matrix<T, R, C, LT>::operator|( const matrix<T, R, C2, LT>& pOther ) const {
    matrix<EntryType, R, C+C2, Compare> result;
    result.template embed<0, 0>( *this );
    result.template embed<0, C>( pOther );

    return result;
}


template <class T, size_t R, size_t C, class LT>
template <size_t SR0, size_t SC0, size_t SR, size_t SC>
matrix<T, SR, SC, LT> matrix<T, R, C, LT>::submatrix() const {
    static_assert( SR0 + SR <= sNumRows && SC0 + SC <= sNumCols,
        "'ggl::matrix' submatrix operation has invalid parameters." );

    matrix<EntryType, SR, SC, Compare> result;
    for( size_t rIdx = 0; rIdx < SR; ++rIdx )
        for( size_t cIdx = 0; cIdx < SC; ++cIdx )
            result(rIdx, cIdx) = (*this)(rIdx + SR0, cIdx + SC0);

    return result;
}


template <class T, size_t R, size_t C, class LT>
template <size_t SR0, size_t SC0, size_t SR, size_t SC>
void matrix<T, R, C, LT>::embed( const matrix<T, SR, SC, LT>& pMatrix ) {
    static_assert( SR0 + SR <= sNumRows && SC0 + SC <= sNumCols,
        "'ggl::matrix' embed operation has invalid parameters." );

    for( size_t rIdx = 0; rIdx < SR; ++rIdx )
        for( size_t cIdx = 0; cIdx < SC; ++cIdx )
            (*this)(rIdx + SR0, cIdx + SC0) = pMatrix(rIdx, cIdx);
}


template <class T, size_t R, size_t C, class LT>
T matrix<T, R, C, LT>::normal() const {
    EntryType normal = ggl::zero<T>();
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        normal += (*this)[eIdx] * (*this)[eIdx];

    return std::sqrt( normal );
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::normalize() const {
    const EntryType normal = (*this).normal();

    matrix<EntryType, sNumRows, sNumCols, Compare> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result[eIdx] = (*this)[eIdx] / normal;

    return result;
}


template <class T, size_t R, size_t C, class LT>
matrix<T, C, R, LT> matrix<T, R, C, LT>::transpose() const {
    matrix<EntryType, sNumCols, sNumRows, Compare> result;
    for( size_t rIdx = 0; rIdx < sNumRows; ++rIdx )
        for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx )
            result(cIdx, rIdx) = (*this)(rIdx, cIdx);

    return result;
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::rreform() const {
    matrix<EntryType, sNumRows, sNumCols, Compare> result{ *this };
    result._reduceRows();
    return result;
}


template <class T, size_t R, size_t C, class LT>
T matrix<T, R, C, LT>::determinant() const {
    static_assert( sNumRows == sNumCols,
        "'ggl::matrix' determinant operation is only valid on square matrices." );

    matrix<EntryType, sNumRows, sNumCols, Compare> rre{ *this };
    EntryType rreDelta = ggl::one<T>();
    rre._reduceRows( rreDelta );

    if( _areEqual(rreDelta, ggl::zero<T>()) ) {
        return ggl::zero<T>();
    } else {
        EntryType result = ggl::one<T>() / rreDelta;
        for( size_t dIdx = 0; dIdx < std::min(sNumRows, sNumCols); ++dIdx )
            result *= rre(dIdx, dIdx);
        return result;
    }
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::inverse() const {
    static_assert( sNumRows == sNumCols,
        "'ggl::matrix' inverse operation is only valid on square matrices." );

    matrix<EntryType, sNumRows, sNumCols, Compare> identity{ EntryType(1) };
    matrix<EntryType, sNumRows, sNumCols+sNumCols, Compare> elim{ *this | identity };
    matrix<EntryType, sNumRows, sNumCols+sNumCols, Compare> rreElim{ elim.rreform() };

    return rreElim.template submatrix<0, sNumCols, sNumRows, sNumCols>();
}


template <class T, size_t R, size_t C, class LT>
T matrix<T, R, C, LT>::dot( const matrix& pOther ) const {
    static_assert( sNumRows == 1 || sNumCols == 1,
        "'ggl::matrix' dot operation is only valid on vectors." );

    EntryType result = ggl::zero<T>();
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result += (*this)[eIdx] * pOther[eIdx];

    return result;
}


template <class T, size_t R, size_t C, class LT> template <size_t R2>
matrix<T, R, R2, LT> matrix<T, R, C, LT>::tensor( const matrix<T, R2, C, LT>& pOther ) const {
    static_assert( sNumCols == 1,
        "'ggl::matrix' tensor operation is only valid on column vectors." );

    return (*this) * pOther.transpose();
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::cross( const matrix& pOther ) const {
    static_assert( (sNumRows == 1 || sNumCols == 1) && sNumEnts == 3,
        "'ggl::matrix' cross operation is only valid on 3-vectors." );

    matrix<T, R, C, LT> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx ) {
        size_t eIdx1 = ( eIdx + 1 ) % sNumEnts;
        size_t eIdx2 = ( eIdx + 2 ) % sNumEnts;
        result[eIdx] = (*this)[eIdx1] * pOther[eIdx2] - (*this)[eIdx2] * pOther[eIdx1];
    }
    return result;
}


template <class T, size_t R, size_t C, class LT>
T matrix<T, R, C, LT>::angleTo( const matrix& pOther ) const {
    static_assert( sNumRows == 1 || sNumCols == 1,
        "'ggl::matrix' angleTo operation is only valid on vectors." );

    // NOTE(JRC): a . b == |a||b|cos(t) => t == acos( (a . b) / (|a||b|) )
    return std::acos( (*this).dot(pOther) / ((*this).normal() * pOther.normal()) );
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::projectOnto( const matrix& pOther ) const {
    static_assert( sNumRows == 1 || sNumCols == 1,
        "'ggl::matrix' projectOnto operation is only valid on vectors." );

    // NOTE(JRC): proj_d(s) == ( (s . d) / (d . d) ) * d
    return ( (*this).dot(pOther) / pOther.dot(pOther) ) * pOther;
}


template <class T, size_t R, size_t C, class LT>
const T* matrix<T, R, C, LT>::data() const {
    return mEntries.data();
}


template <class T, size_t R, size_t C, class LT>
bool matrix<T, R, C, LT>::_areEqual( const T& pValue1, const T& pValue2 ) const {
    return !sLessThan( pValue1, pValue2 ) && !sLessThan( pValue2, pValue1 );
}


template <class T, size_t R, size_t C, class LT>
void matrix<T, R, C, LT>::_reduceRows() {
    EntryType unusedValue = ggl::one<T>();
    (*this)._reduceRows( unusedValue );
}


template <class T, size_t R, size_t C, class LT>
void matrix<T, R, C, LT>::_reduceRows( T& oMatrixDelta ) {
    oMatrixDelta = ggl::one<T>();

    for( size_t rIdx = 0; rIdx < sNumRows; ++rIdx ) {
        size_t pivotIdx = rIdx;
        for( size_t krIdx = rIdx + 1; krIdx < sNumRows; ++krIdx )
            if( std::abs((*this)(pivotIdx, rIdx)) < std::abs((*this)(krIdx, rIdx)) )
                pivotIdx = krIdx;
        (*this)._swapRows( pivotIdx, rIdx );

        if( _areEqual((*this)(rIdx, rIdx), ggl::zero<T>()) ) {
            *this = matrix<T, R, C, LT>();
            oMatrixDelta = ggl::zero<T>();
            return;
        }

        for( size_t krIdx = 0; krIdx < sNumRows; ++krIdx ) {
            if( krIdx != rIdx ) {
                EntryType krScale = -1 * (*this)(krIdx, rIdx) / (*this)(rIdx, rIdx);
                (*this)._addRows( rIdx, krIdx, krScale );
                (*this)(krIdx, rIdx) = ggl::zero<T>();
            }
        }

        EntryType rScale = 1 / (*this)(rIdx, rIdx);
        (*this)._scaleRow( rIdx, rScale );

        oMatrixDelta *= rScale * ( (pivotIdx == rIdx) ? 1 : -1 );
    }
}


template <class T, size_t R, size_t C, class LT>
void matrix<T, R, C, LT>::_swapRows( size_t pSrcRow, size_t pDstRow ) {
    std::array<EntryType, sNumCols> temp;

    const auto srcRowStart = mEntries.begin() + sNumCols * pSrcRow;
    const auto dstRowStart = mEntries.begin() + sNumCols * pDstRow;

    if( pSrcRow != pDstRow ) {
        std::copy( srcRowStart, srcRowStart + sNumCols, temp.begin() );
        std::copy( dstRowStart, dstRowStart + sNumCols, srcRowStart );
        std::copy( temp.begin(), temp.end(), dstRowStart );
    }
}


template <class T, size_t R, size_t C, class LT>
void matrix<T, R, C, LT>::_scaleRow( size_t pSrcRow, const T& pScale ) {
    auto srcRowIter = mEntries.begin() + sNumCols * pSrcRow;
    for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx, ++srcRowIter )
        (*srcRowIter) *= pScale;
}


template <class T, size_t R, size_t C, class LT>
void matrix<T, R, C, LT>::_addRows( size_t pSrcRow, size_t pDstRow, const T& pScale ) {
    auto srcRowIter = mEntries.begin() + sNumCols * pSrcRow;
    auto dstRowIter = mEntries.begin() + sNumCols * pDstRow;

    for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx, ++srcRowIter, ++dstRowIter )
        (*dstRowIter) += pScale * (*srcRowIter);
}

}

template <class T, size_t R, size_t C, class LT>
std::ostream& operator<<( std::ostream& pOS, const ggl::matrix<T, R, C, LT>& pMat ) {
    pOS << "[ ";
    for( size_t rIdx = 0; rIdx < pMat.sNumRows; ++rIdx ) {
        pOS << "| ";
        for( size_t cIdx = 0; cIdx < pMat.sNumCols; ++cIdx ) {
            pOS << pMat(rIdx, cIdx) << ( cIdx < pMat.sNumCols - 1 ? ", " : " " );
        }
        pOS << ( rIdx < pMat.sNumRows - 1 ? "|, " : "| " );
    }
    pOS << "]";

    return pOS;
}
