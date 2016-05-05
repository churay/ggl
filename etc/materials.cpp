#include <array>
#include <algorithm>
#include <cmath>
#include <vector>

#include <iostream>
#include <iomanip>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "matrix.hpp"
#include "geom.hpp"
#include "xform.hpp"
#include "timer.h"
#include "util.h"
#include "consts.hpp"

namespace ggl {
    template <size_t R, size_t C> using matrixgl = matrix<GLfloat, R, C, ggl::util::fless<GLfloat>>;
    template <size_t R> using vectorgl = vector<GLfloat, R, ggl::util::fless<GLfloat>>;

    enum material { diffuse, reflective, dielectric, polished };
};

ggl::real calcReflectionFactor(
        const ggl::vectorf<3>& pIncident,
        const ggl::vectorf<3>& pNormal,
        const ggl::real& pBaseFactor ) {
    // NOTE(JRC): The formula used here to calculate the amount of reflected light
    // is an approximation of the Fresnel factor called "Shlick's Approximation".
    ggl::real reflectCos = ( -pIncident ).dot( pNormal );
    ggl::real absorbedAmount = std::pow( 1.0f - reflectCos, 5.0f );
    return ggl::util::lerp( pBaseFactor, absorbedAmount, 1.0f );
}

ggl::vectorf<3> reflectLight(
        const ggl::vectorf<3>& pIncident,
        const ggl::vectorf<3>& pNormal ) {
    ggl::vectorf<3> reflect = pIncident - 2.0f * pIncident.projectOnto( pNormal );
    return reflect.normalize();
}

ggl::vectorf<3> refractLight(
        const ggl::vectorf<3>& pIncident,
        const ggl::vectorf<3>& pNormal,
        const ggl::real& pRefrIndex ) {
    // TODO(JRC): Implement the equation for refracted light here.
    return pIncident;
}

ggl::vectorgl<3> calcRayLight(
        const ggl::geom::ray<3>& pRay,
        const std::vector<ggl::geom::surface*>& pSurfaces,
        const std::vector<ggl::material>& pSurfMats,
        const ggl::geom::surface* pPrevSurface = nullptr,
        const size_t pCasts = 0 ) {
    /// Light Constants ///
    const static ggl::vectorf<3> sLightPos{ 0.0f, 5.0f, 0.0f };
    const static ggl::vectorgl<3> sWhite{ 1.0f, 1.0f, 1.0f };
    const static ggl::vectorgl<3> sBlack{ 0.0f, 0.0f, 0.0f };

    /// Material Property Constants ///
    const static ggl::real sMatRefl{ 0.70f };
    const static ggl::real sMatRefr{ 0.90f };
    const static ggl::real sMatAtten{ 0.75f };

    std::vector<ggl::geom::surface*> surfaces = pSurfaces;
    if( pPrevSurface != nullptr )
        surfaces.erase( std::find(surfaces.begin(), surfaces.end(), pPrevSurface) );

    ggl::geom::surface* surface = ggl::geom::findClosest( pRay, surfaces );
    size_t surfIdx = std::find( pSurfaces.begin(), pSurfaces.end(), surface ) - pSurfaces.begin();
    ggl::material surfmat = pSurfMats[surfIdx];

    if( surface == nullptr ) { return sBlack; }
    if( pCasts > 5 ) { return sWhite; }

    ggl::vectorf<3> rayVec = pRay.mVector.normalize();
    ggl::real rayT = surface->intersect( pRay ).min();
    ggl::vectorf<3> surfPos = pRay.at( rayT );
    ggl::vectorf<3> surfNorm = ( surfIdx ? -1.0f : 1.0f ) * surface->normalAt( surfPos );

    ggl::vectorf<3> surfToLight = ( sLightPos - surfPos ).normalize();
    ggl::vectorf<3> surfReflLight = reflectLight( rayVec, surfNorm );
    ggl::real surfReflFactor = calcReflectionFactor( rayVec, surfNorm, sMatRefl );

    if( surfmat == ggl::material::diffuse ) {
        const std::array<ggl::vectorf<3>, 6> basis = ggl::geom::basis();
        size_t faceIdx = std::find( basis.begin(), basis.end(), surfNorm ) - basis.begin();

        // NOTE(JRC): This ugly little piece of code is responsible for giving
        // a color to each of the separate faces of the environment cube.
        ggl::vectorgl<3> faceColor;
        if( faceIdx == 0 ) { faceColor = ggl::vectorgl<3>{1.0f, 0.0f, 0.0f}; }
        else if( faceIdx == 1 ) { faceColor = ggl::vectorgl<3>{0.0f, 1.0f, 0.0f}; }
        else if( faceIdx == 2 ) { faceColor = ggl::vectorgl<3>{0.0f, 0.0f, 1.0f}; }
        else if( faceIdx == 3 ) { faceColor = ggl::vectorgl<3>{1.0f, 1.0f, 0.0f}; }
        else if( faceIdx == 4 ) { faceColor = ggl::vectorgl<3>{1.0f, 0.0f, 1.0f}; }
        else if( faceIdx == 5 ) { faceColor = ggl::vectorgl<3>{0.0f, 1.0f, 1.0f}; }
        else { faceColor = ggl::vectorgl<3>{1.0f, 1.0f, 1.0f}; }

        return std::max( 0.05f, surfNorm.dot(surfToLight) ) * faceColor;
    } else if( surfmat == ggl::material::reflective ) {
        ggl::geom::ray<3> reflRay = { surfPos, surfReflLight };
        return surfReflFactor * calcRayLight( reflRay, pSurfaces, pSurfMats, surface, pCasts+1 );
    // TODO(JRC): Finish implementing the lighting properties for dielectric surfaces.
    } else if( surfmat == ggl::material::dielectric ) {
        ggl::vectorf<3> surfRefrLight;
        ggl::real surfRefrCos, surfAtten;

        bool isRayEntering = ggl::util::flt( rayVec.dot(surfNorm), 0.0f );
        if( isRayEntering ) {
            surfRefrLight = refractLight( rayVec, surfNorm, sMatRefr );
            surfRefrCos = ( -rayVec ).dot( surfNorm );
            surfAtten = 1.0f;
        } else {
            surfRefrLight = refractLight( rayVec, surfNorm, 1.0f / sMatRefr );
            surfRefrCos = surfRefrLight.dot( surfNorm );
            surfAtten = std::pow( sMatAtten, (surfPos - pRay.mOrigin).normal() );
        }

        return sMatRefr * sMatAtten * sBlack;
    // TODO(JRC): Finish implementing the lighting properties for polished surfaces.
    } else {
        return sBlack;
    }
}

int main() {
    /// Initialize GLFW Window ///

    if( !glfwInit() )
        return 1;

    GLFWwindow* window = glfwCreateWindow( 500, 500, "ggl", NULL, NULL );
    if( window == NULL )
        return 1;

    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 );

    /// Initialize Scene Geometry ///

    const ggl::vectorf<3> xDir{ 1.0f, 0.0f, 0.0f };
    const ggl::vectorf<3> yDir{ 0.0f, 1.0f, 0.0f };
    const ggl::vectorf<3> zDir{ 0.0f, 0.0f, 1.0f };

    ggl::geom::sphere sphere{ ggl::vectorf<3>{0.0f, 0.0f, 0.0f}, 2.0f };
    ggl::geom::box environment{
        ggl::vectorf<3>{ -10.0f, -10.0f, -10.0f },
        ggl::vectorf<3>{ +10.0f, +10.0f, +10.0f },
    };
    std::vector<ggl::geom::surface*> surfaces{ &sphere, &environment };
    std::vector<ggl::material> surfaceMats{ ggl::material::reflective, ggl::material::diffuse };

    const ggl::real viewRectW{ -2.0f };
    const ggl::vectorf<3> viewRectMin{ -2.0f, -2.0f, viewRectW };
    const ggl::vectorf<3> viewRectMax{ +2.0f, +2.0f, viewRectW };

    ggl::real viewPosRadius{ 5.0f };
    ggl::real viewPosAngleH{ 0.0f };
    ggl::real viewPosAngleV{ 0.0f };

    const unsigned sceneDim = 100;
    GLfloat scenePixels[3 * sceneDim * sceneDim];

    /// Create Scene Rendering Function ///

    auto renderScene = [ & ] ( ) {
        ggl::matrixf<3, 3> viewPosHXform =
            ggl::xform::rotate( viewPosAngleH, yDir ).template submatrix<0, 0, 3, 3>();
        ggl::matrixf<3, 3> viewPosVXform =
            ggl::xform::rotate( viewPosAngleV, viewPosHXform * zDir ).template submatrix<0, 0, 3, 3>();

        ggl::vectorf<3> viewPos = viewPosRadius * ( viewPosVXform * viewPosHXform * xDir );
        ggl::vectorf<3> viewDir = -viewPos.normalize();
        std::array<ggl::vectorf<3>, 3> viewBasis = ggl::xform::basis( -viewDir );

        for( size_t sj = 0; sj < sceneDim; ++sj ) {
            for( size_t si = 0; si < sceneDim; ++si ) {
                const ggl::real su = ggl::util::lerp( ( si + 0.5f ) / sceneDim,
                    viewRectMin[0], viewRectMax[0] );
                const ggl::real sv = ggl::util::lerp( ( sj + 0.5f ) / sceneDim,
                    viewRectMin[1], viewRectMax[1] );
                const ggl::geom::ray<3> sijRay = { viewPos,
                    su*viewBasis[0] + sv*viewBasis[1] + viewRectW*viewBasis[2] };

                GLfloat* sijPixel = &scenePixels[3 * (sj * sceneDim + si)];
                ggl::vectorgl<3> sijColor = calcRayLight( sijRay, surfaces, surfaceMats );

                std::memcpy( sijPixel, sijColor.data(), 3 * sizeof(GLfloat) );
            }
        }

        GLuint sceneTID = 0;
        glGenTextures( 1, &sceneTID );
        glBindTexture( GL_TEXTURE_2D, sceneTID );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, sceneDim, sceneDim, 0,
            GL_RGB, GL_FLOAT, scenePixels );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    };

    renderScene();

    /// Handle Inputs ///

    auto angleWrap = [] ( const ggl::real& pAngle ) {
        return ggl::util::wrap( pAngle, 0.0f, 2.0f * ggl::pi() );
    };

    auto radiusClamp = [] ( const ggl::real& pRadius ) {
        return ggl::util::clamp( pRadius, 3.0f, 9.0f );
    };

    auto glfwHandleInputs = [ &renderScene, &angleWrap, &radiusClamp,
            &viewPosRadius, &viewPosAngleH, &viewPosAngleV ] ( GLFWwindow* window ) {
        // Quit Handling //

        int qKeyAction = glfwGetKey( window, GLFW_KEY_Q );
        int escKeyAction = glfwGetKey( window, GLFW_KEY_ESCAPE );
        if( qKeyAction == GLFW_PRESS || escKeyAction == GLFW_PRESS )
            glfwSetWindowShouldClose( window, GL_TRUE );

        // Scene Modification Handling //

        const ggl::real viewAngleIncr{ ggl::pi() / 32.0f };
        const ggl::real viewRadiusIncr{ 0.05f };

        int wKeyAction = glfwGetKey( window, GLFW_KEY_W );
        if( wKeyAction == GLFW_PRESS )
            viewPosAngleV = angleWrap( viewPosAngleV + viewAngleIncr );
        int sKeyAction = glfwGetKey( window, GLFW_KEY_S );
        if( sKeyAction == GLFW_PRESS )
            viewPosAngleV = angleWrap( viewPosAngleV - viewAngleIncr );

        int aKeyAction = glfwGetKey( window, GLFW_KEY_A );
        if( aKeyAction == GLFW_PRESS )
            viewPosAngleH = angleWrap( viewPosAngleH - viewAngleIncr );
        int dKeyAction = glfwGetKey( window, GLFW_KEY_D );
        if( dKeyAction == GLFW_PRESS )
            viewPosAngleH = angleWrap( viewPosAngleH + viewAngleIncr );

        int rKeyAction = glfwGetKey( window, GLFW_KEY_R );
        if( rKeyAction == GLFW_PRESS )
            viewPosRadius = radiusClamp( viewPosRadius - viewRadiusIncr );
        int fKeyAction = glfwGetKey( window, GLFW_KEY_F );
        if( fKeyAction == GLFW_PRESS )
            viewPosRadius = radiusClamp( viewPosRadius + viewRadiusIncr );

        if( wKeyAction == GLFW_PRESS || sKeyAction == GLFW_PRESS ||
                aKeyAction == GLFW_PRESS || dKeyAction == GLFW_PRESS ||
                rKeyAction == GLFW_PRESS || fKeyAction == GLFW_PRESS )
            renderScene();
    };

    /// Update and Render ///

    glEnable( GL_TEXTURE_2D );
    glDisable( GL_LIGHTING );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    ggl::timer glfwTimer( 60 );

    while( !glfwWindowShouldClose(window) ) {
        glfwTimer.split();

        glfwHandleInputs( window );

        int windowWidth = 0, windowHeight = 0;
        glfwGetFramebufferSize( window, &windowWidth, &windowHeight );

        glViewport( 0, 0, windowWidth, windowHeight );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( -1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();

        glBegin( GL_QUADS );
        glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
        glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, +1.0f, 0.0f );
        glTexCoord2f( 1.0f, 1.0f ); glVertex3f( +1.0f, +1.0f, 0.0f );
        glTexCoord2f( 1.0f, 0.0f ); glVertex3f( +1.0f, -1.0f, 0.0f );
        glEnd();

        glfwSwapBuffers( window );
        glfwPollEvents();

        glfwTimer.wait();

        // Report Simulation Statistics for the Previous Frame //
        std::cout << '\r' << "Frame Rate: " << std::setw( 5 ) <<
            glfwTimer.prevFrameFPS() << std::flush;
    }

    std::cout << std::endl;

    /// Uninitialize GLFW Window ///

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}
