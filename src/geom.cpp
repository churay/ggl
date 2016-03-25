#include <array>
#include <cmath>

#include "matrix.hpp"
#include "interval.h"
#include "consts.hpp"

namespace ggl {

template <size_t D>
ggl::vectorf<D> geom::ray<D>::at( const ggl::real& pParam ) const {
    return mOrigin + pParam * mVector;
}


template <class T>
ggl::real geom::surface<T>::intersect( const ggl::geom::ray<3>& pRay ) const {
    return mSurface.intersect( pRay );
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


ggl::real geom::box::intersect( const ggl::geom::ray<3>& pRay ) const {
    std::array<ggl::interval, 3> rayAxisSpans;
    for( size_t axisIdx = 0; axisIdx < 3; ++axisIdx )
        rayAxisSpans[axisIdx] = ggl::interval(
            ( mMin[axisIdx] - pRay.mOrigin[axisIdx] ) / pRay.mVector[axisIdx],
            ( mMax[axisIdx] - pRay.mOrigin[axisIdx] ) / pRay.mVector[axisIdx]
        );

    ggl::interval rayBoxSpan =
        rayAxisSpans[0].intersect( rayAxisSpans[1] ).intersect( rayAxisSpans[2] );
    return rayBoxSpan.min();
}


ggl::real geom::triangle::intersect( const ggl::geom::ray<3>& pRay ) const {
    ggl::matrixf<3, 4> rayIntxEqs;
    for( size_t axisIdx = 0; axisIdx < 3; ++axisIdx ) {
        rayIntxEqs(axisIdx, 0) = mV0[axisIdx] - mV1[axisIdx];
        rayIntxEqs(axisIdx, 1) = mV0[axisIdx] - mV2[axisIdx];
        rayIntxEqs(axisIdx, 2) = pRay.mVector[axisIdx];
        rayIntxEqs(axisIdx, 3) = mV0[axisIdx] - pRay.mOrigin[axisIdx];
    }

    ggl::matrixf<3, 4> rayIntxSolns = rayIntxEqs.rreform();
    const ggl::real& triIntxWV1 = rayIntxSolns(0, 3);
    const ggl::real& triIntxWV2 = rayIntxSolns(1, 3);
    const ggl::real& triIntxRayT = rayIntxSolns(2, 3);

    return ( triIntxWV1 + triIntxWV2 < ggl::one() ) ? triIntxRayT : ggl::zero();
}

}
