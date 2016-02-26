#ifndef GGL_UTIL_H
#define GGL_UTIL_H

#include <array>
#include <utility>

namespace ggl {

namespace util {
    constexpr unsigned factorial( const unsigned n );

    template <unsigned N> auto permutations();
    template <> auto permutations<1>();
}

}

#include "util.cpp"

#endif
