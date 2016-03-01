#include <cmath>
#include <memory>
#include <sstream>
#include <utility>

#include "util.h"

namespace ggl {

template <class T, size_t R, size_t C>
matrix<T, R, C>::matrix() {
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        (*this)( eIdx ) = EntryType();
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

    EntryType result;
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

    // TODO(JRC): Implement a check that asserts that the matrix determinant
    // is non-zero.

    matrix<T, R, C> result;
    return result;
}


// NOTE(JRC): The 'dot' and 'cross' operations are kept within the 'ggl::matrix'
// class because subclassing would require casting down if matrix operations
// produced a vector.
template <class T, size_t R, size_t C>
T matrix<T, R, C>::dot( const matrix& pOther ) const {
    static_assert( sNumRows == 1 || sNumCols == 1,
        "'ggl::matrix' dot operation is only valid on vectors." );

    EntryType result;
    for( size_t eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result += (*this)( eIdx ) * pOther( eIdx );

    return result;
}


// TODO(JRC): Implement this operation using the matrix determinant operation.
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

}

// TODO(JRC): Consider moving this to an auxiliary matrix function file
// (e.g. "matrix_aux.cpp").

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
