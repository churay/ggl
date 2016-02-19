#include <memory>

namespace ggl {

template <class tEntryType, unsigned tNumRows, unsigned tNumCols>
matrix<tEntryType, tNumRows, tNumCols>::matrix() {
    for( unsigned eIdx = 0; eIdx < tNumEnts; ++eIdx )
        mEntries[eIdx] = tEntryType();
}


template <class tEntryType, unsigned tNumRows, unsigned tNumCols>
matrix<tEntryType, tNumRows, tNumCols>::matrix(
        std::array<tEntryType, tNumEnts> pEntries ) {
    mEntries = std::move( pEntries );
}


template <class tEntryType, unsigned tNumRows, unsigned tNumCols>
tEntryType& matrix<tEntryType, tNumRows, tNumCols>::operator()(
        unsigned pRow, unsigned pCol ) {
    return mEntries[pRow * tNumCols + pCol];
}


template <class tEntryType, unsigned tNumRows, unsigned tNumCols>
const tEntryType& matrix<tEntryType, tNumRows, tNumCols>::operator()(
        unsigned pRow, unsigned pCol ) const {
    return mEntries[pRow * tNumCols + pCol];
}


template <class tEntryType, unsigned tNumRows, unsigned tNumCols>
matrix<tEntryType, tNumRows, tNumCols>
matrix<tEntryType, tNumRows, tNumCols>::operator+( const matrix& pOther ) const {
    matrix<tEntryType, tNumRows, tNumCols> result;
    for( unsigned eIdx = 0; eIdx < tNumEnts; ++eIdx )
        result.mEntries[eIdx] = this->mEntries[eIdx] + pOther.mEntries[eIdx];

    return result;
}

}
