#ifndef GGL_UTIL_H
#define GGL_UTIL_H

#include <cmath>
#include <vector>

#ifndef M_PIF
#define M_PIF (static_cast<float>(M_PI))
#endif

namespace ggl {

namespace util {
    constexpr unsigned factorial( const unsigned n );

    std::vector<std::vector<unsigned>> permutations( const unsigned n );
    unsigned inversions( const std::vector<unsigned>& permute );
}

}

#include "util.cpp"

#endif
