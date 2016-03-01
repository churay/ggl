#ifndef GGL_UTIL_H
#define GGL_UTIL_H

#include <array>
#include <utility>

namespace ggl {

namespace util {
    constexpr unsigned factorial( const unsigned n );

    template <size_t N> auto permutations();
    template <> auto permutations<1>();

    template <class T, size_t N>
    unsigned inversions( const std::array<T, N>& array );
}

}

#include "util.cpp"

#endif
