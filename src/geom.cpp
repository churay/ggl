#include <cmath>

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

/// Definitions for `ggl::geom::ray<size_t>` ///

template <size_t D>
geom::ray<D>::ray( const ggl::vectorf<D>& pOrigin, const ggl::vectorf<D>& pVector ) {
    mOrigin = pOrigin;
    mVector = pVector;
}

template <size_t D>
ggl::vectorf<D> geom::ray<D>::at( const ggl::real& pParam ) const {
    return mOrigin + pParam * mVector;
}

/// Definitions for `ggl::geom::plane` ///

geom::plane::plane( const ggl::vectorf<3>& pOrigin, const ggl::vectorf<3>& pNormal ) {
    mOrigin = pOrigin;
    mNormal = pNormal;
}

ggl::real geom::plane::intersect( const ggl::geom::ray<3>& pRay ) const {
    return mNormal.dot( mOrigin - pRay.mOrigin ) / mNormal.dot( pRay.mVector );
}

/// Definitions for `ggl::geom::sphere` ///

geom::plane::sphere( const ggl::vectorf<3>& pOrigin, const ggl::real& pRadius ) {
    mOrigin = pOrigin;
    mRadius = pRadius;
}

ggl::real geom::sphere::intersect( const ggl::geom::ray<3>& pRay ) const {
    const ggl::real quadA = pRay.mVector.dot( pRay.mVector );
    const ggl::real quadB = pRay.mVector.dot( pRay.mOrigin - mOrigin );
    const ggl::real quadC = std::pow((pRay.mOrigin - mOrigin).normal(), 2) - std::pow( mRadius, 2 );

    return ( -quadB + std::sqrt(std::pow(quadB, 2) - 4 * quadA * quadC) ) / ( 2 * quadC );
}

}
