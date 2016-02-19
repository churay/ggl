#include <memory>

namespace ggl {

template <class T, unsigned R, unsigned C>
matrix<T, R, C>::matrix() {
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        mEntries[eIdx] = EntryType();
}


template <class T, unsigned R, unsigned C>
matrix<T, R, C>::matrix( std::array<EntryType, sNumEnts> pEntries ) {
    mEntries = std::move( pEntries );
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

}
