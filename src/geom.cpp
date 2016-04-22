#include <array>
#include <cmath>
#include <vector>
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

/// Surface Functions ///

bool geom::surface::contains( const ggl::vectorf<3>& pPos ) const {
    // TODO(JRC): Consider moving this functionality to a standalone function
    // so that the canonical basis vectors can be produced and used freely.
    const std::array<ggl::vectorf<3>, 6> basisVectors = geom::basis();

    for( const ggl::vectorf<3>& basisVector : basisVectors ) {
        const ggl::geom::ray<3> basisRay = { pPos, basisVector };
        const ggl::interval rayIntx = (*this).intersect( basisRay );

        if( rayIntx.valid() && ggl::util::feq(rayIntx.min(), ggl::zero()) )
            return true;
    }

    return false;
}

ggl::interval geom::surface::intersect( const ggl::geom::ray<3>& pRay ) const {
    ggl::interval raySurfIntx = (*this)._intersect( pRay );

    if( ggl::util::flt(raySurfIntx.max(), ggl::zero()) ) {
        return ggl::interval( ggl::nan() );
    } else {
        return ggl::interval(
            std::max( raySurfIntx.min(), ggl::zero() ),
            std::max( raySurfIntx.max(), ggl::zero() )
        );
    }
}

ggl::vectorf<3> geom::surface::normalAt( const ggl::vectorf<3>& pPos ) const {
    return (*this).contains( pPos ) ? (*this)._normalAt( pPos ).normalize() :
        ggl::vectorf<3>{ ggl::nan(), ggl::nan(), ggl::nan() };
}

/// Plane Functions ///

geom::plane::plane( ggl::vectorf<3> pOrigin, ggl::vectorf<3> pNormal ) :
        mOrigin( std::move(pOrigin) ), mNormal( std::move(pNormal) ) {
}

ggl::interval geom::plane::_intersect( const ggl::geom::ray<3>& pRay ) const {
    ggl::real rayIntx = mNormal.dot( mOrigin - pRay.mOrigin ) / mNormal.dot( pRay.mVector );
    return ggl::interval( rayIntx );
}

ggl::vectorf<3> geom::plane::_normalAt( const ggl::vectorf<3>& pPos ) const {
    return mNormal;
}

/// Sphere Functions ///

geom::sphere::sphere( ggl::vectorf<3> pOrigin, ggl::real pRadius ) :
        mOrigin( std::move(pOrigin) ), mRadius( std::move(pRadius) ) {
}

ggl::interval geom::sphere::_intersect( const ggl::geom::ray<3>& pRay ) const {
    const ggl::vectorf<3> rayToDir = pRay.mVector, origToDir = pRay.mOrigin - mOrigin;
    std::pair<ggl::real, ggl::real> rayIntxs = ggl::util::solveQuadratic(
        rayToDir.dot( rayToDir ),
        2 * rayToDir.dot( origToDir ),
        origToDir.dot( origToDir ) - mRadius * mRadius
    );

    return ggl::interval( rayIntxs.first, rayIntxs.second );
}

ggl::vectorf<3> geom::sphere::_normalAt( const ggl::vectorf<3>& pPos ) const {
    return pPos - mOrigin;
}

/// Box Functions ///

geom::box::box( ggl::vectorf<3> pMin, ggl::vectorf<3> pMax ) :
        mMin( std::move(pMin) ), mMax( std::move(pMax) ) {
}

ggl::interval geom::box::_intersect( const ggl::geom::ray<3>& pRay ) const {
    std::array<ggl::interval, 3> rayAxisSpans;
    for( size_t axisIdx = 0; axisIdx < 3; ++axisIdx )
        rayAxisSpans[axisIdx] = ggl::interval(
            ( mMin[axisIdx] - pRay.mOrigin[axisIdx] ) / pRay.mVector[axisIdx],
            ( mMax[axisIdx] - pRay.mOrigin[axisIdx] ) / pRay.mVector[axisIdx]
        );

    return rayAxisSpans[0].intersect( rayAxisSpans[1] ).intersect( rayAxisSpans[2] );
}

ggl::vectorf<3> geom::box::_normalAt( const ggl::vectorf<3>& pPos ) const {
    ggl::vectorf<3> normal{ ggl::zero() };

    const std::array<ggl::vectorf<3>, 6> basisVectors = geom::basis();
    for( size_t axisIdx = 0; axisIdx < 3; ++axisIdx ) {
        if( ggl::util::feq(pPos[axisIdx], mMin[axisIdx]) )
            normal = normal + basisVectors[2 * axisIdx + 0];
        else if( ggl::util::feq(pPos[axisIdx], mMax[axisIdx]) )
            normal = normal + basisVectors[2 * axisIdx + 1];
    }

    return normal.normalize();
}

/// Triangle Functions ///

geom::triangle::triangle( ggl::vectorf<3> pV0, ggl::vectorf<3> pV1, ggl::vectorf<3> pV2 ) :
        mV0( std::move(pV0) ), mV1( std::move(pV1) ), mV2( std::move(pV2) ) {
}

ggl::interval geom::triangle::_intersect( const ggl::geom::ray<3>& pRay ) const {
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

ggl::vectorf<3> geom::triangle::_normalAt( const ggl::vectorf<3>& pPos ) const {
    return ( mV1 - mV0 ).cross( mV2 - mV0 );
}

/// Namespace Functions ///

std::array<ggl::vectorf<3>, 6> geom::basis() {
    std::array<ggl::vectorf<3>, 6> basisVectors;
    for( size_t basisIdx = 0; basisIdx < 3; ++basisIdx ) {
        ggl::vectorf<3> basisVector;
        for( size_t dimIdx = 0; dimIdx < 3; ++dimIdx )
            basisVector[dimIdx] = ( basisIdx == dimIdx ) ? ggl::one() : ggl::zero();
        for( size_t signIdx = 0; signIdx < 2; ++signIdx ) {
            ggl::real basisSign = ( signIdx == 0 ) ? -ggl::one() : +ggl::one();
            basisVectors[2 * basisIdx + signIdx] = basisSign * basisVector;
        }
    }

    return basisVectors;
}

ggl::geom::surface* geom::findClosest( const ggl::geom::ray<3>& pRay,
        const std::vector<ggl::geom::surface*>& pSurfaces ) {
    ggl::geom::surface* closestSurf( nullptr );
    ggl::interval closestIntx( ggl::nan() );

    for( ggl::geom::surface* surf : pSurfaces ) {
        ggl::interval surfIntx = surf->intersect( pRay );
        if( surfIntx.valid() && (!closestIntx.valid() || surfIntx.min() < closestIntx.min()) ) {
            closestSurf = surf;
            closestIntx = surfIntx;
        }
    }

    return closestSurf;
}

}
