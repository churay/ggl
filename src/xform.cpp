#include <cmath>
#include <tuple>
#include <utility>
#include "matrix.h"

namespace ggl {

// TODO(JRC): Improve the implementation of the following two functions so
// that there isn't as much duplication.
template <class... Ts>
auto xform::scale( Ts&&... pValues ) {
    using T = typename std::tuple_element<0, std::tuple<Ts...>>::type;
    constexpr unsigned D = sizeof...( Ts );
    const std::array<T, D> pValueArray{{ std::forward<Ts>(pValues)... }};

    ggl::matrix<T, D+1, D+1> result{ T(1) };
    for( unsigned eIdx = 0; eIdx < D; ++eIdx )
        result( eIdx, eIdx ) = pValueArray[eIdx];

    return result;
}


template <class... Ts>
auto xform::translate( Ts&&... pValues ) {
    using T = typename std::tuple_element<0, std::tuple<Ts...>>::type;
    constexpr unsigned D = sizeof...( Ts );
    const std::array<T, D> pValueArray{{ std::forward<Ts>(pValues)... }};

    ggl::matrix<T, D+1, D+1> result{ T(1) };
    for( unsigned eIdx = 0; eIdx < D; ++eIdx )
        result( eIdx, D ) = pValueArray[eIdx];

    return result;
}


template <class T>
auto xform::rotate2d( const T& pRadians ) {
    static const T sZeroValue{ 0 }, sOneValue{ 1 };

    ggl::matrix<T, 3, 3> result{ 
        +std::cos( pRadians ), -std::sin( pRadians ), sZeroValue,
        +std::sin( pRadians ), +std::cos( pRadians ), sZeroValue,
        sZeroValue, sZeroValue, sOneValue
    };

    return result;
}


template <class T, class LT>
auto xform::rotate3d( const T& pRadians, const vector<T, 3, LT>& pAxis ) {
    static const T sZeroValue{ 0 }, sOneValue{ 1 };

    const T cosValue = std::cos( pRadians );
    const T cosValueInv = sOneValue - cosValue;
    const T sinValue = std::sin( pRadians );

    // TODO(JRC): Make this more concise using the representation found here:
    // https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
    ggl::matrix<T, 4, 4, LT> result{
        cosValue + cosValueInv*pAxis[0]*pAxis[0],
        cosValueInv*pAxis[0]*pAxis[1] - sinValue*pAxis[2],
        cosValueInv*pAxis[0]*pAxis[2] + sinValue*pAxis[1],
        sZeroValue,
        cosValueInv*pAxis[0]*pAxis[1] + sinValue*pAxis[2],
        cosValue + cosValueInv*pAxis[1]*pAxis[1],
        cosValueInv*pAxis[1]*pAxis[2] - sinValue*pAxis[0],
        sZeroValue,
        cosValueInv*pAxis[0]*pAxis[2] - sinValue*pAxis[1],
        cosValueInv*pAxis[1]*pAxis[2] + sinValue*pAxis[0],
        cosValue + cosValueInv*pAxis[2]*pAxis[2],
        sZeroValue,
        sZeroValue, sZeroValue, sZeroValue, sOneValue
    };

    return result;
}

}
