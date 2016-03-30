#include "util.h"

namespace ggl {

template <class T>
bool util::fless<T>::operator()( const T& pV1, const T& pV2 ) const {
    return ggl::util::flt( pV1, pV2 );
}

}
