namespace ggl {

template <class tEntryType, unsigned tNumRows, unsigned tNumCols>
matrix<tEntryType, tNumRows, tNumCols>::matrix() {
    for( unsigned entidx = 0; entidx < tNumEnts; ++entidx )
        mEntries[entidx] = tEntryType();
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

}
