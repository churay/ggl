#ifndef GGL_UTIL_H
#define GGL_UTIL_H

#include <utility>
#include "consts.hpp"

namespace ggl {

namespace util {
    constexpr unsigned factorial( const unsigned pVal );
    std::pair<ggl::real, ggl::real> solveQuadratic( const ggl::real& pA,
        const ggl::real& pB, const ggl::real& pC );
}

}

#endif
