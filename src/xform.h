#ifndef GGL_XFORM_H
#define GGL_XFORM_H

#include <tuple>
#include <utility>
#include "matrix.h"

namespace ggl {

namespace xform {
    template <class... Ts>
    auto scale( Ts&&... pValues );

    template <class... Ts>
    auto translate( Ts&&... pValues );

    template <class T>
    auto rotate2d( const T& pRadians );

    template <class T, class LT>
    auto rotate3d( const T& pRadians, const vector<T, 3, LT>& pAxis );
}

}

#include "xform.cpp"

#endif
