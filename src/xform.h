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
    auto rotate( const T& pRadians );

    template <class T>
    auto rotate( const T& pRadians, const vector<T, 3>& pAxis );
}

}

#include "xform.cpp"

#endif
