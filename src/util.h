#ifndef GGL_UTIL_H
#define GGL_UTIL_H

#include <vector>
#include "consts.hpp"

namespace ggl {

namespace util {
    constexpr unsigned factorial( const unsigned pVal );
    std::vector<ggl::real> qsolve( const ggl::real& pA, const ggl::real& pB, const ggl::real& pC );
}

}

#endif
