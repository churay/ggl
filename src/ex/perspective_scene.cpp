#include "geom.h"
#include "camera.h"
#include "tgeom.hpp"
#include "xform.hpp"
#include "matrix.hpp"
#include "colors.hpp"
#include "consts.hpp"

#include "ex/perspective_scene.h"

namespace ggl {

perspective_scene::perspective_scene() : scene( sDim, sDim ),
        mBox{ ggl::vectorf<3>(-1.0f), ggl::vectorf<3>(+1.0f) } {
    mSurfaces.push_back( &mBox );

    mDoRender = true;
}


perspective_scene::~perspective_scene() {
    
}


void perspective_scene::input( GLFWwindow* pWindow ) {
    scene::input( pWindow );

    int wKeyAction = glfwGetKey( pWindow, GLFW_KEY_W );
    if( wKeyAction == GLFW_PRESS ) mCamera.tilt( false, true );
    int sKeyAction = glfwGetKey( pWindow, GLFW_KEY_S );
    if( sKeyAction == GLFW_PRESS ) mCamera.tilt( false, false );

    int aKeyAction = glfwGetKey( pWindow, GLFW_KEY_A );
    if( aKeyAction == GLFW_PRESS ) mCamera.tilt( true, false );
    int dKeyAction = glfwGetKey( pWindow, GLFW_KEY_D );
    if( dKeyAction == GLFW_PRESS ) mCamera.tilt( true, true );

    int rKeyAction = glfwGetKey( pWindow, GLFW_KEY_R );
    if( rKeyAction == GLFW_PRESS ) mCamera.zoom( true );
    int fKeyAction = glfwGetKey( pWindow, GLFW_KEY_F );
    if( fKeyAction == GLFW_PRESS ) mCamera.zoom( false );

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

    for( size_t sj = 0; sj < sDim; ++sj ) {
        for( size_t si = 0; si < sDim; ++si ) {
            const ggl::real su = ( si + (ggl::one() / 2) ) / sDim;
            const ggl::real sv = ( sj + (ggl::one() / 2) ) / sDim;
            const ggl::geom::ray sijRay = mCamera.calcViewRay( su, sv );

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
