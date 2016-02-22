#include <cmath>
#include <memory>
#include <utility>

namespace ggl {

template <class T, unsigned R, unsigned C>
matrix<T, R, C>::matrix() {
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        mEntries[eIdx] = EntryType();
}


template <class T, unsigned R, unsigned C> template <class... Ts>
matrix<T, R, C>::matrix( Ts&&... pEntries ) : mEntries{{ std::forward<Ts>(pEntries)... }} {
    static_assert( sizeof...(Ts) == sNumEnts,
        "'ggl::matrix' value constructor must be given exactly R*C entries." );
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
matrix<T, R, C> matrix<T, R, C>::operator+( const matrix& pOther ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result.mEntries[eIdx] = this->mEntries[eIdx] + pOther.mEntries[eIdx];

    return result;
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> matrix<T, R, C>::operator-( const matrix& pOther ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result.mEntries[eIdx] = this->mEntries[eIdx] - pOther.mEntries[eIdx];

    return result;
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> matrix<T, R, C>::operator*( const T& pValue ) const {
    matrix<EntryType, sNumRows, sNumCols> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result.mEntries[eIdx] = pValue * this->mEntries[eIdx];

    return result;
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> operator*( const T& pValue, const matrix<T, R, C>& pMatrix ) {
    return pMatrix * pValue;
}


template <class T, unsigned R, unsigned C> template <unsigned C2>
matrix<T, R, C2> matrix<T, R, C>::operator*( const matrix<T, C, C2>& pOther ) const {
    matrix<EntryType, this->sNumRows, pOther.sNumCols> result;
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
        norm += this->mEntries[eIdx] * this->mEntries[eIdx];

    return std::sqrt( norm );
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C> matrix<T, R, C>::normal() const {
    const EntryType norm = this->norm();

    matrix<EntryType, sNumRows, sNumCols> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        this->mEntries = this->mEntries / norm;

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

}
