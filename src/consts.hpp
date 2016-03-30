#ifndef GGL_CONST_H
#define GGL_CONST_H

#include <limits>

namespace ggl {

    using real = float;

    template <class T=real> constexpr T zero() { return static_cast<T>(0); };
    template <class T=real> constexpr T one() { return static_cast<T>(1); };
    template <class T=real> constexpr T pi() { return static_cast<T>(3.14159265358979323846); };
    template <class T=real> constexpr T nan() { return std::numeric_limits<T>::quiet_NaN(); };
    template <class T=real> constexpr T epsilon() { return static_cast<T>(1.0e-6); };
}

#endif
