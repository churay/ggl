#include <cmath>
#include <utility>

#include "util.h"
#include "consts.hpp"

namespace ggl {

constexpr unsigned util::factorial( const unsigned pVal ) {
    return ( pVal != 0 ) ? pVal * factorial( pVal - 1 ) : 1;
}


std::pair<ggl::real, ggl::real> util::solveQuadratic( const ggl::real& pA,
        const ggl::real& pB, const ggl::real& pC ) {
    std::pair<ggl::real, ggl::real> results = std::make_pair( ggl::nan(), ggl::nan() );

    ggl::real discriminant = std::pow( pB, 2 ) - 4 * pA * pC;
    if( discriminant == ggl::zero() ) {
        results.first = ( -pB ) / ( 2 * pA );
    } else if( discriminant > ggl::zero() ) {
        results.first = ( -pB - std::sqrt(discriminant) ) / ( 2 * pA );
        results.second = ( -pB + std::sqrt(discriminant) ) / ( 2 * pA );
    }

    return results;
}

}
