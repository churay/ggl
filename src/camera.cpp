#include "xform.hpp"
#include "geom.h"
#include "matrix.hpp"
#include "consts.hpp"

#include "camera.h"

namespace ggl {

camera::camera( const ggl::vectorf<3>& pOrigin, const ggl::vectorf<3>& pDirection,
        const ggl::real& pViewWidth,
        const ggl::real& pViewHeight,
        const ggl::real& pViewDepth ) {
    mOrigin = pOrigin;
    mDirection = pDirection;

    mViewWidth = pViewWidth;
    mViewHeight = pViewHeight;
    mViewDepth = pViewDepth;
}

ggl::geom::ray camera::calcViewRay( const ggl::real& pI, const ggl::real& pJ ) const {
    /*
    const std::array<ggl::vectorf<3>, 3> xyzBasis = ggl::geom::basis();
    const ggl::vectorf<3>& xDir = xyzBasis[0], & yDir = xyzBasis[1], & zDir = xyzBasis[2];

    ggl::matrixf<3, 3> viewPosHXform =
        ggl::xform::rotate( mViewAngleH, yDir ).template submatrix<0, 0, 3, 3>();
    ggl::matrixf<3, 3> viewPosVXform =
        ggl::xform::rotate( mViewAngleV, viewPosHXform * zDir ).template submatrix<0, 0, 3, 3>();

    ggl::vectorf<3> viewPos = mViewRadius * ( viewPosVXform * viewPosHXform * xDir );
    ggl::vectorf<3> viewDir = -viewPos.normalize();
    std::array<ggl::vectorf<3>, 3> viewBasis = ggl::xform::basis( -viewDir );

    for( size_t sj = 0; sj < sDim; ++sj ) {
        for( size_t si = 0; si < sDim; ++si ) {
            const ggl::real su = ggl::util::lerp( (si + 0.5f) / sDim,
                mViewRectMin[0], mViewRectMax[0] );
            const ggl::real sv = ggl::util::lerp( (sj + 0.5f) / sDim,
                mViewRectMin[1], mViewRectMax[1] );
            const ggl::geom::ray sijRay = { viewPos,
                su*viewBasis[0] + sv*viewBasis[1] + mViewRectMin[2]*viewBasis[2] };

            mPixels[sj * sDim + si] = (*this)._calcRayLight( sijRay );
        }
    }
    */
    return ggl::geom::ray();
}

}
