#include <cmath>

namespace ggl {

template <class T>
bool util::fless<T>::operator()( const T& pValue1, const T& pValue2 ) const {
    return ( pValue1 < pValue2 ) && ( std::fabs(pValue1 - pValue2) >= 1.0e-6 );
}

}
