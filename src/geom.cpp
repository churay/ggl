#include <array>
#include <cmath>
#include <utility>

#include "matrix.hpp"
#include "interval.h"
#include "util.h"
#include "consts.hpp"

namespace ggl {

/// Ray Functions ///

template <size_t D>
ggl::vectorf<D> geom::ray<D>::at( const ggl::real& pParam ) const {
    return mOrigin + pParam * mVector;
}

/// Plane Functions ///

geom::plane::plane( ggl::vectorf<3> pOrigin, ggl::vectorf<3> pNormal ) :
        mOrigin( std::move(pOrigin) ), mNormal( std::move(pNormal) ) {
}

ggl::interval geom::plane::intersect( const ggl::geom::ray<3>& pRay ) const {
    ggl::real rayIntx = mNormal.dot( mOrigin - pRay.mOrigin ) / mNormal.dot( pRay.mVector );
    return ggl::interval( rayIntx );
}

/// Sphere Functions ///

geom::sphere::sphere( ggl::vectorf<3> pOrigin, ggl::real pRadius ) :
        mOrigin( std::move(pOrigin) ), mRadius( std::move(pRadius) ) {
}

ggl::interval geom::sphere::intersect( const ggl::geom::ray<3>& pRay ) const {
    const ggl::vectorf<3> rayToDir = pRay.mVector, origToDir = pRay.mOrigin - mOrigin;
    std::pair<ggl::real, ggl::real> rayIntxs = ggl::util::solveQuadratic(
        rayToDir.dot( rayToDir ),
        2 * rayToDir.dot( origToDir ),
        origToDir.dot( origToDir ) - mRadius * mRadius
    );

    return ggl::interval( rayIntxs.first, rayIntxs.second );
}

/// Box Functions ///

geom::box::box( ggl::vectorf<3> pMin, ggl::vectorf<3> pMax ) :
        mMin( std::move(pMin) ), mMax( std::move(pMax) ) {
}

ggl::interval geom::box::intersect( const ggl::geom::ray<3>& pRay ) const {
    std::array<ggl::interval, 3> rayAxisSpans;
    for( size_t axisIdx = 0; axisIdx < 3; ++axisIdx )
        rayAxisSpans[axisIdx] = ggl::interval(
            ( mMin[axisIdx] - pRay.mOrigin[axisIdx] ) / pRay.mVector[axisIdx],
            ( mMax[axisIdx] - pRay.mOrigin[axisIdx] ) / pRay.mVector[axisIdx]
        );

    return rayAxisSpans[0].intersect( rayAxisSpans[1] ).intersect( rayAxisSpans[2] );
}

/// Triangle Functions ///

geom::triangle::triangle( ggl::vectorf<3> pV0, ggl::vectorf<3> pV1, ggl::vectorf<3> pV2 ) :
        mV0( std::move(pV0) ), mV1( std::move(pV1) ), mV2( std::move(pV2) ) {
}

ggl::interval geom::triangle::intersect( const ggl::geom::ray<3>& pRay ) const {
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
    const bool triIntxValid = triIntxWV1 >= ggl::zero() && triIntxWV2 >= ggl::zero() &&
        triIntxWV1 + triIntxWV2 <= ggl::one();

    ggl::real rayIntx = triIntxValid ? triIntxRayT : ggl::nan();
    return ggl::interval( rayIntx );
}

}
