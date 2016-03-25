#ifndef GGL_CONST_H
#define GGL_CONST_H

#include <limits>

namespace ggl {

    using real = float;

    template <class T> constexpr T gZero{ static_cast<T>(0) };
    template <class T> constexpr T gOne{ static_cast<T>(1) };
    template <class T> constexpr T gPi{ static_cast<T>(3.14159265358979323846) };
    template <class T> constexpr T gNaN{ std::numeric_limits<T>::quiet_NaN() };

    constexpr real gZerof{ gZero<real> };
    constexpr real gOnef{ gOne<real> };
    constexpr real gPif{ gPi<real> };
    constexpr real gNaNf{ gNaN<real> };
}

#endif
