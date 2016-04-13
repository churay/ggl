#ifndef GGL_UTIL_H
#define GGL_UTIL_H

#include <utility>
#include "consts.hpp"

namespace ggl {

namespace util {
    bool feq( const ggl::real& pV1, const ggl::real& pV2 );
    bool flt( const ggl::real& pV1, const ggl::real& pV2 );
    bool fle( const ggl::real& pV1, const ggl::real& pV2 );

    ggl::real clamp( const ggl::real& pVal, const ggl::real& pMin, const ggl::real& pMax );
    ggl::real lerp( const ggl::real& pVal, const ggl::real& pMin, const ggl::real& pMax );
    ggl::real wrap( const ggl::real& pVal, const ggl::real& pMin, const ggl::real& pMax );

    std::pair<ggl::real, ggl::real> solveQuadratic( const ggl::real& pA,
        const ggl::real& pB, const ggl::real& pC );
}

}

#endif
