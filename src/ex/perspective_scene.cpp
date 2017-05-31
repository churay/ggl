#include "geom.h"
#include "xform.hpp"
#include "matrix.hpp"
#include "colors.hpp"
#include "consts.hpp"

#include "ex/perspective_scene.h"

namespace ggl {

perspective_scene::perspective_scene() : scene( sDim, sDim ),
        mBox{ ggl::vectorf<3>(-1.0f), ggl::vectorf<3>(+1.0f) } {
    mSurfaces.push_back( &mBox );

    mViewRectMin = ggl::vectorf<3>{ -2.0f, -2.0f, -2.0f };
    mViewRectMax = ggl::vectorf<3>{ +2.0f, +2.0f, -2.0f };

    mViewRadius = 5.0f;
    mViewAngleH = -ggl::pi() / 4.0f;
    mViewAngleV = +ggl::pi() / 4.0f;

    mDoRender = true;
}


perspective_scene::~perspective_scene() {
    
}


void perspective_scene::input( GLFWwindow* pWindow ) {
    const ggl::real viewAngleIncr{ ggl::pi() / 32.0f };
    const ggl::real viewRadiusIncr{ 0.05f };

    auto angleWrap = [] ( const ggl::real& pAngle ) {
        return ggl::util::wrap( pAngle, 0.0f, 2.0f * ggl::pi() ); };
    auto radiusClamp = [] ( const ggl::real& pRadius ) {
        return ggl::util::clamp( pRadius, 3.0f, 20.0f ); };

    scene::input( pWindow );

    int wKeyAction = glfwGetKey( pWindow, GLFW_KEY_W );
    if( wKeyAction == GLFW_PRESS )
        mViewAngleV = angleWrap( mViewAngleV + viewAngleIncr );
    int sKeyAction = glfwGetKey( pWindow, GLFW_KEY_S );
    if( sKeyAction == GLFW_PRESS )
        mViewAngleV = angleWrap( mViewAngleV - viewAngleIncr );

    int aKeyAction = glfwGetKey( pWindow, GLFW_KEY_A );
    if( aKeyAction == GLFW_PRESS )
        mViewAngleH = angleWrap( mViewAngleH - viewAngleIncr );
    int dKeyAction = glfwGetKey( pWindow, GLFW_KEY_D );
    if( dKeyAction == GLFW_PRESS )
        mViewAngleH = angleWrap( mViewAngleH + viewAngleIncr );

    int rKeyAction = glfwGetKey( pWindow, GLFW_KEY_R );
    if( rKeyAction == GLFW_PRESS )
        mViewRadius = radiusClamp( mViewRadius - viewRadiusIncr );
    int fKeyAction = glfwGetKey( pWindow, GLFW_KEY_F );
    if( fKeyAction == GLFW_PRESS )
        mViewRadius = radiusClamp( mViewRadius + viewRadiusIncr );

    mDoRender = wKeyAction == GLFW_PRESS || sKeyAction == GLFW_PRESS ||
        aKeyAction == GLFW_PRESS || dKeyAction == GLFW_PRESS ||
        rKeyAction == GLFW_PRESS || fKeyAction == GLFW_PRESS;
}


bool perspective_scene::update( ggl::real pDelta ) {
    scene::update( pDelta );
    return ( mFrameCount == 1 ) || mDoRender;
}


void perspective_scene::render() {
    const ggl::vectorf<3> lightPos{ 1.5f, 1.5f, 1.5f };

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

            ggl::geom::surface* sijClosest = ggl::geom::findClosest( sijRay, mSurfaces );

            ggl::vectorc<3> sijColor;
            if( sijClosest == nullptr ) {
                sijColor = ggl::color::black<3>();
            } else {
                ggl::real sijRayT = sijClosest->intersect( sijRay ).min();
                ggl::vectorf<3> sijSurfPos = sijRay.at( sijRayT );

                ggl::vectorf<3> sijSurfNorm = sijClosest->normalAt( sijSurfPos );
                ggl::vectorf<3> sijLightDir = ( lightPos - sijSurfPos ).normalize();
                sijColor = std::max( 0.05f, sijSurfNorm.dot(sijLightDir) ) * ggl::color::white<3>();
            }

            mPixels[sj * sDim + si] = sijColor;
        }
    }
}

}
