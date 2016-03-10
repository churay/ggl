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
        result(eIdx, eIdx) = pValueArray[eIdx];

    return result;
}


template <class... Ts>
auto xform::translate( Ts&&... pValues ) {
    using T = typename std::tuple_element<0, std::tuple<Ts...>>::type;
    constexpr unsigned D = sizeof...( Ts );
    const std::array<T, D> pValueArray{{ std::forward<Ts>(pValues)... }};

    ggl::matrix<T, D+1, D+1> result{ T(1) };
    for( unsigned eIdx = 0; eIdx < D; ++eIdx )
        result(eIdx, D) = pValueArray[eIdx];

    return result;
}


template <class T>
auto xform::rotate( const T& pRadians ) {
    static const T sZeroValue{ 0 }, sOneValue{ 1 };

    ggl::matrix<T, 3, 3> result{ 
        +std::cos( pRadians ), -std::sin( pRadians ), sZeroValue,
        +std::sin( pRadians ), +std::cos( pRadians ), sZeroValue,
        sZeroValue, sZeroValue, sOneValue
    };

    return result;
}


template <class T, class LT>
auto xform::rotate( const T& pRadians, const vector<T, 3, LT>& pAxis ) {
    static const T sZeroValue{ 0 }, sOneValue{ 1 };

    // NOTE(JRC): This succinct 3D rotation formula was taken from here:
    // https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
    auto cosMatrix = std::cos( pRadians ) * ggl::matrix<T, 3, 3, LT>( sOneValue );
    auto sinMatrix = std::sin( pRadians ) * ggl::matrix<T, 3, 3, LT>{
        sZeroValue, -pAxis[2], +pAxis[1],
        +pAxis[2], sZeroValue, -pAxis[0],
        -pAxis[1], +pAxis[0], sZeroValue
    };
    auto icosMatrix = ( sOneValue - std::cos(pRadians) ) * pAxis.tensor( pAxis );

    ggl::matrix<T, 4, 4, LT> result{ sOneValue };
    result.template embed<0, 0>( cosMatrix + sinMatrix + icosMatrix );
    return result;
}

}
