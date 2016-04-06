#include <algorithm>
#include <cmath>
#include <utility>

#include "util.h"
#include "consts.hpp"

namespace ggl {

bool util::feq( const ggl::real& pV1, const ggl::real& pV2 ) {
    return std::fabs( pV1 - pV2 ) <= ggl::epsilon();
}


bool util::flt( const ggl::real& pV1, const ggl::real& pV2 ) {
    return ( pV1 < pV2 ) && !ggl::util::feq( pV1, pV2 );
}


bool util::fle( const ggl::real& pV1, const ggl::real& pV2 ) {
    return ggl::util::flt( pV1, pV2 ) || ggl::util::feq( pV1, pV2 );
}


ggl::real util::clamp( const ggl::real& pVal, const ggl::real& pMin, const ggl::real& pMax ) {
    return std::max( pMin, std::min(pMax, pVal) );
}


ggl::real util::lerp( const ggl::real& pVal, const ggl::real& pMin, const ggl::real& pMax ) {
    return pMin + ggl::util::clamp( pVal, ggl::zero(), ggl::one() ) * ( pMax - pMin );
}


constexpr unsigned util::factorial( const unsigned& pVal ) {
    return ( pVal != 0 ) ? pVal * factorial( pVal - 1 ) : 1;
}


std::pair<ggl::real, ggl::real> util::solveQuadratic( const ggl::real& pA,
        const ggl::real& pB, const ggl::real& pC ) {
    std::pair<ggl::real, ggl::real> results = std::make_pair( ggl::nan(), ggl::nan() );

    ggl::real discriminant = pB * pB - 4 * pA * pC;
    if( ggl::util::feq(discriminant, ggl::zero()) ) {
        results.first = ( -pB ) / ( 2 * pA );
    } else if( discriminant > ggl::zero() ) {
        results.first = ( -pB - std::sqrt(discriminant) ) / ( 2 * pA );
        results.second = ( -pB + std::sqrt(discriminant) ) / ( 2 * pA );
    }

    return results;
}

}
