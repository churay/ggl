#include <cmath>
#include <utility>

namespace ggl {

template <class T, unsigned E>
vector<T, E>::vector() : matrix<T, E, 1>() {
    /* empty */
}


template <class T, unsigned E> template <class... Ts>
vector<T, E>::vector( Ts&&... pEntries ) : matrix<T, E, 1>( pEntries... ) {
    /* empty */
}


template <class T, unsigned E>
T vector<T, E>::dot( const vector& pOther ) const {
    EntryType result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx )
        result += (*this)( eIdx ) * pOther( eIdx );

    return result;
}


template <class T, unsigned E>
vector<T, E> vector<T, E>::cross( const vector& pOther ) const {
    vector<T, E> result;
    for( unsigned eIdx = 0; eIdx < sNumEnts; ++eIdx ) {
        unsigned eIdx1 = ( eIdx + 1 ) % sNumEnts;
        unsigned eIdx2 = ( eIdx + 2 ) % sNumEnts;
        result( eIdx ) = (*this)( eIdx1 ) * pOther( eIdx2 ) -
            (*this)( eIdx2 ) * pOther( eIdx1 );
    }

    return result;
}

}
