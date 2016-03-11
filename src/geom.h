#ifndef GGL_GEOM_H
#define GGL_GEOM_H

#include <tuple>
#include <utility>
#include "matrix.h"

namespace ggl {

namespace geom {
    template <class T, class LT>
    auto basis( const vector<T, 3, LT>& pWVector );

    template <class... Ts>
    auto scale( Ts&&... pValues );

    template <class... Ts>
    auto translate( Ts&&... pValues );

    template <class T>
    auto rotate( const T& pRadians );

    template <class T, class LT>
    auto rotate( const T& pRadians, const vector<T, 3, LT>& pAxis );
}

}

#include "geom.cpp"

#endif
