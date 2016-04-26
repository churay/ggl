#include <array>
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

    const ggl::vectorf<3> lightPos{ 0.0f, 100.0f, 0.0f };
    const ggl::vector<GLfloat, 3> white{ 1.0f, 1.0f, 1.0f };
    const ggl::vector<GLfloat, 3> black{ 0.0f, 0.0f, 0.0f };

    ggl::geom::sphere diffSphere{ ggl::vectorf<3>{0.0f, 0.0f, -3.0f}, 1.0f };
    ggl::geom::sphere specSphere{ ggl::vectorf<3>{0.0f, 0.0f, +3.0f}, 1.0f };
    std::vector<ggl::geom::surface*> surfaces{ &diffSphere, &specSphere };

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

                ggl::geom::surface* sijClosest = ggl::geom::findClosest( sijRay, surfaces );
                GLfloat* sijPixel = &scenePixels[3 * (sj * sceneDim + si)];

                ggl::vector<GLfloat, 3> sijColor;
                if( sijClosest == nullptr ) {
                    sijColor = black;
                } else {
                    ggl::real sijRayT = sijClosest->intersect( sijRay ).min();
                    ggl::vectorf<3> sijSurfPos = sijRay.at( sijRayT );

                    ggl::vectorf<3> sijSurfNorm = sijClosest->normalAt( sijSurfPos );
                    ggl::vectorf<3> sijLightDir = ( lightPos - sijSurfPos ).normalize();
                    sijColor = std::max( 0.05f, sijSurfNorm.dot(sijLightDir) ) * white;

                    // TODO(JRC): Implement the functionality for the different
                    // lighting schemes in the code below.
                    /*
                    if( sijClosest == &diffSphere ) {
                        sijColor = std::max( 0.05f, sijSurfNorm.dot(sijLightDir) ) * white;
                    } else { // sijClosest == &specSphere

                    }
                    */
                }

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
        return ggl::util::clamp( pRadius, 3.0f, 20.0f );
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
