#include "util.h"
#include "consts.hpp"

namespace ggl {

template <class T>
bool util::fless<T>::operator()( const T& pV1, const T& pV2 ) const {
    return ggl::util::flt( pV1, pV2 );
}


template <class T>
T util::tlerp( const ggl::real& pVal, const T& pMin, const T& pMax ) {
    return pMin + ggl::util::clamp( pVal, ggl::zero(), ggl::one() ) * ( pMax - pMin );
}

}
