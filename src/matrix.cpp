namespace ggl {

template <unsigned Rows, unsigned Cols, class EntryType>
matrix<Rows, Cols, EntryType>::matrix() {
    for( unsigned entidx = 0; entidx < Rows * Cols; ++entidx )
        m_entries[entidx] = EntryType();
}


template <unsigned Rows, unsigned Cols, class EntryType>
EntryType& matrix<Rows, Cols, EntryType>::operator()( unsigned _row, unsigned _col ) {
    return m_entries[_row * Cols + _col];
}


template <unsigned Rows, unsigned Cols, class EntryType>
const EntryType& matrix<Rows, Cols, EntryType>::operator()( unsigned _row, unsigned _col ) const {
    return m_entries[_row * Cols + _col];
}

}
