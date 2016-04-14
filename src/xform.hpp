#ifndef GGL_XFORM_H
#define GGL_XFORM_H

#include <array>
#include <utility>

#include "matrix.hpp"

namespace ggl {

namespace xform {
    template <class T, class LT>
    auto basis( const vector<T, 3, LT>& pWVector );

    template <class... Ts>
    auto scale( Ts&&... pValues );

    template <class... Ts>
    auto translate( Ts&&... pValues );

    template <class T, class LT=std::less<T>>
    auto rotate( const T& pRadians );

    template <class T, class LT>
    auto rotate( const T& pRadians, const vector<T, 3, LT>& pAxis );

    template <class T, class LT>
    auto rotate( const std::array<vector<T, 3, LT>, 3>& pAxes );
}

}

#include "xform.cpp"

#endif
