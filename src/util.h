#ifndef GGL_UTIL_H
#define GGL_UTIL_H

#include <vector>

namespace ggl {

namespace util {
    constexpr unsigned factorial( const unsigned n );

    std::vector<std::vector<unsigned>> permutations( const unsigned n );
    unsigned inversions( const std::vector<unsigned>& permutation );
}

}

#include "util.cpp"

#endif
