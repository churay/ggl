#include "xform.hpp"
#include "tgeom.hpp"
#include "geom.h"
#include "util/util.h"
#include "matrix.hpp"
#include "consts.hpp"

#include "camera.h"

namespace ggl {

camera::camera() {
    mViewAngleH = -ggl::pi() / 4;
    mViewAngleV = +ggl::pi() / 4;
    mViewRadius = 5 * ggl::one();

    mViewWidth = 4 * ggl::one();
    mViewHeight = 4 * ggl::one();
    mViewDepth = -2 * ggl::one();

    mUpdateBasis = true;
}


void camera::tilt( const bool& pLR, const bool& pDir ) {
    ggl::real& angleValue = pLR ? mViewAngleH : mViewAngleV;
    angleValue += ( pDir ? 1 : -1 ) * ( ggl::pi() / 32 );
    angleValue = ggl::util::wrap( angleValue, ggl::zero(), 2 * ggl::pi() );

    mUpdateBasis = true;
}


void camera::zoom( const bool& pIn ) {
    ggl::real& zoomValue = mViewRadius;
    zoomValue += ( pIn ? -1 : 1 ) * ( ggl::one() / 20 );
    zoomValue = ggl::util::wrap( zoomValue, 3 * ggl::one(), 20 * ggl::one() );

    mUpdateBasis = true;
}


ggl::geom::ray camera::calcViewRay( const ggl::real& pI, const ggl::real& pJ ) {
    (*this)._calcViewBasis();
    return ggl::geom::ray{ mViewPos,
        ggl::util::lerp(pI, -mViewWidth/2, +mViewWidth/2) * mViewBasis[0] +
        ggl::util::lerp(pJ, -mViewHeight/2, +mViewHeight/2) * mViewBasis[1] +
        mViewDepth*mViewBasis[2] };
}


const std::array<ggl::vectorf<3>, 3>& camera::calcViewBasis() {
    (*this)._calcViewBasis();
    return (*this).mViewBasis;
}


void camera::_calcViewBasis() {
    if( !mUpdateBasis ) return;

    const ggl::vectorf<3>& xDir = ggl::geom::axis<3, 1>::value;
    const ggl::vectorf<3>& yDir = ggl::geom::axis<3, 2>::value;
    const ggl::vectorf<3>& zDir = ggl::geom::axis<3, 3>::value;

    ggl::matrixf<3, 3> viewPosHXform = ggl::xform::rotate( mViewAngleH, yDir ).
        template submatrix<0, 0, 3, 3>();
    ggl::matrixf<3, 3> viewPosVXform = ggl::xform::rotate( mViewAngleV, viewPosHXform * zDir ).
        template submatrix<0, 0, 3, 3>();

    ggl::vectorf<3> viewPos = mViewRadius * ( viewPosVXform * viewPosHXform * xDir );
    ggl::vectorf<3> viewDir = -viewPos.normalize();

    mViewPos = viewPos;
    mViewBasis = ggl::xform::basis( -viewDir );
    mUpdateBasis = false;
}

}
