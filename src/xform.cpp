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
auto xform::rotate( const T& pRadians ) {
    static const T sZeroValue{ 0 }, sOneValue{ 1 };

    ggl::matrix<T, 3, 3> result{ 
        +std::cos( pRadians ), -std::sin( pRadians ), sZeroValue,
        +std::sin( pRadians ), +std::cos( pRadians ), sZeroValue,
        sZeroValue, sZeroValue, sOneValue
    };

    return result;
}


template <class T>
auto xform::rotate( const T& pRadians, const vector<T, 3>& pAxis ) {
    ggl::matrix<T, 4, 4> result{ T(1) };
    return result;
}

}
