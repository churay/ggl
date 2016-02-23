#include <cmath>
#include <memory>
#include <sstream>
#include <utility>

namespace ggl {

template <class T, unsigned R, unsigned C>
matrix<T, R, C>::matrix() {
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        (*this)( eIdx ) = EntryType();
}


// NOTE(JRC): Recipe for argument constructor from the following location:
// http://stackoverflow.com/a/7725611
template <class T, unsigned R, unsigned C> template <class... Ts>
matrix<T, R, C>::matrix( Ts&&... pEntries ) : mEntries{{ std::forward<Ts>(pEntries)... }} {
    static_assert( sizeof...(Ts) == sNumEnts,
        "'ggl::matrix' value constructor must be given exactly R*C entries." );
}


template <class T, unsigned R, unsigned C>
T& matrix<T, R, C>::operator()( unsigned pEntry ) {
    return mEntries[pEntry];
}


template <class T, unsigned R, unsigned C>
const T& matrix<T, R, C>::operator()( unsigned pEntry ) const {
    return mEntries[pEntry];
}


template <class T, unsigned R, unsigned C>
T& matrix<T, R, C>::operator()( unsigned pRow, unsigned pCol ) {
    return mEntries[pRow * sNumCols + pCol];
}


template <class T, unsigned R, unsigned C>
const T& matrix<T, R, C>::operator()( unsigned pRow, unsigned pCol ) const {
    return mEntries[pRow * sNumCols + pCol];
}


template <class T, unsigned R, unsigned C>
bool matrix<T, R, C>::operator==( const matrix& pOther ) const {
    for( unsigned rIdx = 0; rIdx < sNumRows; ++rIdx )
        for( unsigned cIdx = 0; cIdx < sNumCols; ++cIdx )
            if( (*this)( rIdx, cIdx ) != pOther( rIdx, cIdx ) )
                return false;

    return true;
}


template <class T, unsigned R, unsigned C>
bool matrix<T, R, C>::operator!=( const matrix& pOther ) const {
    return !( *this == pOther );
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> matrix<T, R, C>::operator+( const matrix& pOther ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result( eIdx ) = (*this)( eIdx ) + pOther( eIdx );

    return result;
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> matrix<T, R, C>::operator-( const matrix& pOther ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result( eIdx ) = (*this)( eIdx ) - pOther( eIdx );

    return result;
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> matrix<T, R, C>::operator*( const T& pValue ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result( eIdx ) = (*this)( eIdx ) * pValue;

    return result;
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> operator*( const T& pValue, const matrix<T, R, C>& pMatrix ) {
    matrix<T, R, C> result;
    for( unsigned eIdx = 0; eIdx < result.sNumEnts; ++eIdx )
        result( eIdx ) = pValue * pMatrix( eIdx );

    return result;
}


template <class T, unsigned R, unsigned C> template <unsigned C2>
matrix<T, R, C2> matrix<T, R, C>::operator*( const matrix<T, C, C2>& pOther ) const {
    matrix<EntryType, R, C2> result;
    for( unsigned rIdx = 0; rIdx < result.sNumRows; ++rIdx )
        for( unsigned cIdx = 0; cIdx < result.sNumCols; ++cIdx )
            for( unsigned iIdx = 0; iIdx < this->sNumCols; ++iIdx )
                result( rIdx, cIdx ) += (*this)( rIdx, iIdx ) * pOther( iIdx, cIdx );

    return result;
}


template <class T, unsigned R, unsigned C>
T matrix<T, R, C>::norm() const {
    EntryType norm;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        norm += (*this)( eIdx ) * (*this)( eIdx );

    return std::sqrt( norm );
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> matrix<T, R, C>::normal() const {
    const EntryType norm = this->norm();

    matrix<EntryType, sNumRows, sNumCols> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result( eIdx ) = (*this)( eIdx ) / norm;

    return result;
}


template <class T, unsigned R, unsigned C>
matrix<T, C, R> matrix<T, R, C>::transpose() const {
    matrix<EntryType, sNumCols, sNumRows> result;
    for( unsigned rIdx = 0; rIdx < sNumRows; ++rIdx )
        for( unsigned cIdx = 0; cIdx < sNumCols; ++cIdx )
            result( cIdx, rIdx ) = (*this)( rIdx, cIdx );

    return result;
}


template <class T, unsigned R, unsigned C>
const T* matrix<T, R, C>::data() const {
    return mEntries.data();
}

}

// TODO(JRC): Consider moving this to an auxiliary matrix function file
// (e.g. "matrix_aux.cpp").

template <class T, unsigned R, unsigned C>
std::ostream& operator<<( std::ostream& os, const ggl::matrix<T, R, C>& m ) {
    os << "[ ";
    for( unsigned rIdx = 0; rIdx < m.sNumRows; ++rIdx ) {
        os << "| ";
        for( unsigned cIdx = 0; cIdx < m.sNumCols; ++cIdx ) {
            os << m( rIdx, cIdx ) << ( cIdx < m.sNumCols - 1 ? ", " : " " );
        }
        os << ( rIdx < m.sNumRows - 1 ? "|, " : "| " );
    }
    os << "]";

    return os;
}
