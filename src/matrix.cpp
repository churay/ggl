#include <algorithm>
#include <cmath>
#include <sstream>
#include <utility>

#include "util.h"

namespace ggl {

template <class T, size_t R, size_t C>
matrix<T, R, C>::matrix() {
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        (*this)( eIdx ) = EntryType( 0 );
}


// NOTE(JRC): Recipe for argument constructor from the following location:
// http://stackoverflow.com/a/7725611
template <class T, size_t R, size_t C> template <class... Ts>
matrix<T, R, C>::matrix( Ts&&... pEntries ) : mEntries{{ std::forward<Ts>(pEntries)... }} {
    static_assert( sizeof...(Ts) == sNumEnts,
        "'ggl::matrix' value constructor must be given exactly R*C entries." );
}


template <class T, size_t R, size_t C>
T& matrix<T, R, C>::operator()( size_t pEntry ) {
    return mEntries[pEntry];
}


template <class T, size_t R, size_t C>
const T& matrix<T, R, C>::operator()( size_t pEntry ) const {
    return mEntries[pEntry];
}


template <class T, size_t R, size_t C>
T& matrix<T, R, C>::operator()( size_t pRow, size_t pCol ) {
    return mEntries[pRow * sNumCols + pCol];
}


template <class T, size_t R, size_t C>
const T& matrix<T, R, C>::operator()( size_t pRow, size_t pCol ) const {
    return mEntries[pRow * sNumCols + pCol];
}


template <class T, size_t R, size_t C>
bool matrix<T, R, C>::operator==( const matrix& pOther ) const {
    for( size_t rIdx = 0; rIdx < sNumRows; ++rIdx )
        for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx )
            if( (*this)( rIdx, cIdx ) != pOther( rIdx, cIdx ) )
                return false;

    return true;
}


template <class T, size_t R, size_t C>
bool matrix<T, R, C>::operator!=( const matrix& pOther ) const {
    return !( *this == pOther );
}


template <class T, size_t R, size_t C>
matrix<T, R, C> matrix<T, R, C>::operator+( const matrix& pOther ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result( eIdx ) = (*this)( eIdx ) + pOther( eIdx );

    return result;
}


template <class T, size_t R, size_t C>
matrix<T, R, C> matrix<T, R, C>::operator-( const matrix& pOther ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result( eIdx ) = (*this)( eIdx ) - pOther( eIdx );

    return result;
}


template <class T, size_t R, size_t C>
matrix<T, R, C> matrix<T, R, C>::operator*( const T& pValue ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result( eIdx ) = (*this)( eIdx ) * pValue;

    return result;
}


template <class T, size_t R, size_t C>
matrix<T, R, C> operator*( const T& pValue, const matrix<T, R, C>& pMatrix ) {
    matrix<T, R, C> result;
    for( size_t eIdx = 0; eIdx < result.sNumEnts; ++eIdx )
        result( eIdx ) = pValue * pMatrix( eIdx );

    return result;
}


template <class T, size_t R, size_t C> template <size_t C2>
matrix<T, R, C2> matrix<T, R, C>::operator*( const matrix<T, C, C2>& pOther ) const {
    matrix<EntryType, R, C2> result;
    for( size_t rIdx = 0; rIdx < result.sNumRows; ++rIdx )
        for( size_t cIdx = 0; cIdx < result.sNumCols; ++cIdx )
            for( size_t iIdx = 0; iIdx < this->sNumCols; ++iIdx )
                result( rIdx, cIdx ) += (*this)( rIdx, iIdx ) * pOther( iIdx, cIdx );

    return result;
}


template <class T, size_t R, size_t C>
T matrix<T, R, C>::normal() const {
    EntryType normal;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        normal += (*this)( eIdx ) * (*this)( eIdx );

    return std::sqrt( normal );
}


template <class T, size_t R, size_t C>
matrix<T, R, C> matrix<T, R, C>::normalize() const {
    const EntryType normal = this->normal();

    matrix<EntryType, sNumRows, sNumCols> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result( eIdx ) = (*this)( eIdx ) / normal;

    return result;
}


template <class T, size_t R, size_t C>
matrix<T, C, R> matrix<T, R, C>::transpose() const {
    matrix<EntryType, sNumCols, sNumRows> result;
    for( size_t rIdx = 0; rIdx < sNumRows; ++rIdx )
        for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx )
            result( cIdx, rIdx ) = (*this)( rIdx, cIdx );

    return result;
}


template <class T, size_t R, size_t C>
T matrix<T, R, C>::determinant() const {
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


template <class T, size_t R, size_t C>
matrix<T, R, C> matrix<T, R, C>::inverse() const {
    static_assert( sNumRows == sNumCols,
        "'ggl::matrix' inverse operation is only valid on square matrices." );

    matrix<EntryType, sNumRows, sNumCols> elim( *this );
    matrix<EntryType, sNumRows, sNumCols> result;
    for( size_t dIdx = 0; dIdx < sNumRows; ++dIdx )
        result( dIdx, dIdx ) = EntryType( 1 );

    for( size_t rIdx = 0; rIdx < sNumRows; ++rIdx ) {
        size_t pivotIdx = rIdx;
        for( size_t krIdx = rIdx + 1; krIdx < sNumRows; ++krIdx )
            if( std::abs(elim(pivotIdx, rIdx)) < std::abs(elim(krIdx, rIdx)) )
                pivotIdx = krIdx;
        elim._swapRows( pivotIdx, rIdx );
        result._swapRows( pivotIdx, rIdx );

        // TODO(JRC): Implement this functionality once comparison is supported.
        /*
        if( elim(rIdx, rIdx) == 0 )
            throw std::runtime_error( "Matrix is singular and doesn't have an inverse!" );
        */

        for( size_t krIdx = 0; krIdx < sNumRows; ++krIdx ) {
            if( krIdx != rIdx ) {
                EntryType krScale = -1 * elim( krIdx, rIdx ) / elim( rIdx, rIdx );
                elim._addRows( rIdx, krIdx, krScale );
                result._addRows( rIdx, krIdx, krScale );
                elim( krIdx, rIdx ) = 0;
            }
        }

        EntryType rScale = 1 / elim( rIdx, rIdx );
        elim._scaleRows( rIdx, rScale );
        result._scaleRows( rIdx, rScale );
    }

    return result;
}


template <class T, size_t R, size_t C>
T matrix<T, R, C>::dot( const matrix& pOther ) const {
    static_assert( sNumRows == 1 || sNumCols == 1,
        "'ggl::matrix' dot operation is only valid on vectors." );

    EntryType result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result += (*this)( eIdx ) * pOther( eIdx );

    return result;
}


template <class T, size_t R, size_t C>
matrix<T, R, C> matrix<T, R, C>::cross( const matrix& pOther ) const {
    static_assert( (sNumRows == 1 || sNumCols == 1) && sNumEnts == 3,
        "'ggl::matrix' cross operation is only valid on 3-vectors." );

    matrix<T, R, C> result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx ) {
        size_t eIdx1 = ( eIdx + 1 ) % sNumEnts;
        size_t eIdx2 = ( eIdx + 2 ) % sNumEnts;
        result( eIdx ) = (*this)( eIdx1 ) * pOther( eIdx2 ) -
            (*this)( eIdx2 ) * pOther( eIdx1 );
    }
    return result;
}


template <class T, size_t R, size_t C>
const T* matrix<T, R, C>::data() const {
    return mEntries.data();
}


template <class T, size_t R, size_t C>
void matrix<T, R, C>::_swapRows( size_t pSrcRow, size_t pDstRow ) {
    std::array<EntryType, sNumCols> temp;

    const auto srcRowStart = mEntries.begin() + sNumCols * pSrcRow;
    const auto dstRowStart = mEntries.begin() + sNumCols * pDstRow;

    if( pSrcRow != pDstRow ) {
        std::copy( srcRowStart, srcRowStart + sNumCols, temp.begin() );
        std::copy( dstRowStart, dstRowStart + sNumCols, srcRowStart );
        std::copy( temp.begin(), temp.end(), dstRowStart );
    }
}


template <class T, size_t R, size_t C>
void matrix<T, R, C>::_scaleRows( size_t pSrcRow, const T& pScale ) {
    auto srcRowIter = mEntries.begin() + sNumCols * pSrcRow;
    for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx, ++srcRowIter )
        (*srcRowIter) *= pScale;
}


template <class T, size_t R, size_t C>
void matrix<T, R, C>::_addRows( size_t pSrcRow, size_t pDstRow, const T& pScale ) {
    auto srcRowIter = mEntries.begin() + sNumCols * pSrcRow;
    auto dstRowIter = mEntries.begin() + sNumCols * pDstRow;

    for( size_t cIdx = 0; cIdx < sNumCols; ++cIdx, ++srcRowIter, ++dstRowIter )
        (*dstRowIter) += pScale * (*srcRowIter);
}

}

template <class T, size_t R, size_t C>
std::ostream& operator<<( std::ostream& os, const ggl::matrix<T, R, C>& m ) {
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
