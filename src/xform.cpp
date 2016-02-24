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

    ggl::matrix<T, D+1, D+1> result;
    for( unsigned eIdx = 0; eIdx < D; ++eIdx )
        result( eIdx, eIdx ) = pValueArray[eIdx];
    result( D, D ) = static_cast<T>( 1.0 );

    return result;
}


template <class... Ts>
auto xform::translate( Ts&&... pValues ) {
    using T = typename std::tuple_element<0, std::tuple<Ts...>>::type;
    constexpr unsigned D = sizeof...( Ts );
    const std::array<T, D> pValueArray{{ std::forward<Ts>(pValues)... }};

    ggl::matrix<T, D+1, D+1> result;
    for( unsigned eIdx = 0; eIdx < D; ++eIdx )
        result( eIdx, D ) = pValueArray[eIdx];
    result( D, D ) = static_cast<T>( 1.0 );
    return result;
}

}
