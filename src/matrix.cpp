#include <algorithm>
#include <cmath>
#include <sstream>
#include <utility>

#include "util.h"

namespace ggl {

// NOTE(JRC): This is weird, but necessary according to this Stack Overflow response:
// http://stackoverflow.com/q/8016780/837221
template <class T, size_t R, size_t C, class LT>
constexpr T matrix<T, R, C, LT>::sZeroValue;
template <class T, size_t R, size_t C, class LT>
constexpr LT matrix<T, R, C, LT>::sLessThan;


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT>::matrix() {
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        (*this)[eIdx] = EntryType( 0 );
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
                (*this)( rIdx, cIdx ) = ( rIdx == cIdx ) ?
                    (*this)( 0, 0 ) : EntryType( 0 );
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
            for( size_t iIdx = 0; iIdx < this->sNumCols; ++iIdx )
                result( rIdx, cIdx ) += (*this)( rIdx, iIdx ) * pOther( iIdx, cIdx );

    return result;
}


template <class T, size_t R, size_t C, class LT> template <size_t C2>
matrix<T, R, C+C2, LT> matrix<T, R, C, LT>::operator|( const matrix<T, R, C2, LT>& pOther ) const {
    matrix<EntryType, R, C+C2, Compare> result;

    for( size_t rIdx = 0; rIdx < sNumRows; ++rIdx )
        for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx )
            result( rIdx, cIdx ) = (*this)( rIdx, cIdx );

    for( size_t rIdx = 0; rIdx < pOther.sNumRows; ++rIdx )
        for( size_t cIdx = 0; cIdx < pOther.sNumCols; ++cIdx )
            result( rIdx, cIdx + sNumCols ) = pOther( rIdx, cIdx );

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
            result( rIdx, cIdx ) = (*this)( rIdx + SR0, cIdx + SC0 );

    return result;
}


template <class T, size_t R, size_t C, class LT>
T matrix<T, R, C, LT>::normal() const {
    EntryType normal = sZeroValue;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        normal += (*this)[eIdx] * (*this)[eIdx];

    return std::sqrt( normal );
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::normalize() const {
    const EntryType normal = this->normal();

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
            result( cIdx, rIdx ) = (*this)( rIdx, cIdx );

    return result;
}


template <class T, size_t R, size_t C, class LT>
T matrix<T, R, C, LT>::determinant() const {
    static_assert( sNumRows == sNumCols,
        "'ggl::matrix' determinant operation is only valid on square matrices." );

    EntryType result = EntryType( 0 );
    for( const auto& ePermute : ggl::util::permutations(sNumRows) ) {
        EntryType eResult = ( ggl::util::inversions(ePermute) % 2 == 0 ) ? 1 : -1;
        for( size_t dIdx = 0; dIdx < sNumRows; ++dIdx )
            eResult *= (*this)( ePermute[dIdx], dIdx );
        result += eResult;
    }

    return result;
}


template <class T, size_t R, size_t C, class LT>
matrix<T, R, C, LT> matrix<T, R, C, LT>::inverse() const {
    static_assert( sNumRows == sNumCols,
        "'ggl::matrix' inverse operation is only valid on square matrices." );

    matrix<EntryType, sNumRows, sNumCols, Compare> identity{ EntryType(1) };
    matrix<EntryType, sNumRows, sNumCols+sNumCols, Compare> elim = *this | identity;
    for( size_t rIdx = 0; rIdx < sNumRows; ++rIdx ) {
        size_t pivotIdx = rIdx;
        for( size_t krIdx = rIdx + 1; krIdx < sNumRows; ++krIdx )
            if( std::abs(elim(pivotIdx, rIdx)) < std::abs(elim(krIdx, rIdx)) )
                pivotIdx = krIdx;
        elim._swapRows( pivotIdx, rIdx );

        if( _areEqual(elim(rIdx, rIdx), sZeroValue) )
            return matrix<EntryType, sNumRows, sNumCols, Compare>();

        for( size_t krIdx = 0; krIdx < sNumRows; ++krIdx ) {
            if( krIdx != rIdx ) {
                EntryType krScale = -1 * elim( krIdx, rIdx ) / elim( rIdx, rIdx );
                elim._addRows( rIdx, krIdx, krScale );
                elim( krIdx, rIdx ) = sZeroValue;
            }
        }

        EntryType rScale = 1 / elim( rIdx, rIdx );
        elim._scaleRows( rIdx, rScale );
    }

    return elim.submatrix<0, sNumCols, sNumRows, sNumCols>();
}


template <class T, size_t R, size_t C, class LT>
T matrix<T, R, C, LT>::dot( const matrix& pOther ) const {
    static_assert( sNumRows == 1 || sNumCols == 1,
        "'ggl::matrix' dot operation is only valid on vectors." );

    EntryType result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result += (*this)[eIdx] * pOther[eIdx];

    return result;
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
const T* matrix<T, R, C, LT>::data() const {
    return mEntries.data();
}


template <class T, size_t R, size_t C, class LT>
bool matrix<T, R, C, LT>::_areEqual( const T& pValue1, const T& pValue2 ) const {
    return !sLessThan( pValue1, pValue2 ) && !sLessThan( pValue2, pValue1 );
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
void matrix<T, R, C, LT>::_scaleRows( size_t pSrcRow, const T& pScale ) {
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
std::ostream& operator<<( std::ostream& os, const ggl::matrix<T, R, C, LT>& m ) {
    os << "[ ";
    for( size_t rIdx = 0; rIdx < m.sNumRows; ++rIdx ) {
        os << "| ";
        for( size_t cIdx = 0; cIdx < m.sNumCols; ++cIdx ) {
            os << m( rIdx, cIdx ) << ( cIdx < m.sNumCols - 1 ? ", " : " " );
        }
        os << ( rIdx < m.sNumRows - 1 ? "|, " : "| " );
    }
    os << "]";

    return os;
}
