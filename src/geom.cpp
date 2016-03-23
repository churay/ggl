#include <cmath>

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

template <size_t D>
ggl::vectorf<D> geom::ray<D>::at( const ggl::real& pParam ) const {
    return mOrigin + pParam * mVector;
}


ggl::real geom::plane::intersect( const ggl::geom::ray<3>& pRay ) const {
    return mNormal.dot( mOrigin - pRay.mOrigin ) / mNormal.dot( pRay.mVector );
}


ggl::real geom::sphere::intersect( const ggl::geom::ray<3>& pRay ) const {
    const ggl::real quadA = pRay.mVector.dot( pRay.mVector );
    const ggl::real quadB = pRay.mVector.dot( pRay.mOrigin - mOrigin );
    const ggl::real quadC = std::pow((pRay.mOrigin - mOrigin).normal(), 2) - std::pow( mRadius, 2 );

    return ( -quadB + std::sqrt(std::pow(quadB, 2) - 4 * quadA * quadC) ) / ( 2 * quadC );
}

}
