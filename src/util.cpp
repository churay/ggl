#include <cmath>
#include <utility>

#include "util.h"
#include "consts.hpp"

namespace ggl {

constexpr unsigned util::factorial( const unsigned pVal ) {
    return ( pVal != 0 ) ? pVal * factorial( pVal - 1 ) : 1;
}


std::vector<ggl::real> qsolve( const ggl::real& pA, const ggl::real& pB, const ggl::real& pC ) {
    std::vector<ggl::real> results;
    return results;
}

}
