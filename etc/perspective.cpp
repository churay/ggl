#include <array>
#include <cmath>
#include <vector>
#include <utility>

#include <iostream>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "matrix.hpp"
#include "geom.hpp"
#include "xform.hpp"
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

    const ggl::vector<GLfloat, 3> white{ 1.0f, 1.0f, 1.0f }, black{ 0.0f, 0.0f, 0.0f };
    const ggl::vectorf<3> lightDir{ 0.0f, 1.0f, 0.0f };

    ggl::geom::sphere sphere{ ggl::vectorf<3>{0.0f, 0.0f, 0.0f}, 1.85f };
    std::vector<ggl::geom::surface*> surfaces{ &sphere };

    const ggl::real viewRectW{ -2.0f };
    const ggl::vectorf<3> viewRectMin{ -2.0f, -2.0f, viewRectW };
    const ggl::vectorf<3> viewRectMax{ +2.0f, +2.0f, viewRectW };

    ggl::vectorf<3> viewPos{ 0.0f, 0.0f, +2.0f };
    ggl::vectorf<3> viewDir{ 0.0f, 0.0f, -1.0f };

    const unsigned sceneDim = 100;
    const ggl::real sceneDimf = static_cast<ggl::real>( sceneDim - 1 );
    GLfloat scenePixels[3 * sceneDim * sceneDim];

    /// Create Scene Rendering Function ///

    auto renderScene = [ & ] ( ) {
        std::array<ggl::vectorf<3>, 3> viewBasis = ggl::xform::basis( -viewDir );

        for( size_t sj = 0; sj < sceneDim; ++sj ) {
            for( size_t si = 0; si < sceneDim; ++si ) {
                const ggl::real su = ggl::util::lerp( si / sceneDimf, viewRectMin[0], viewRectMax[0] );
                const ggl::real sv = ggl::util::lerp( sj / sceneDimf, viewRectMin[1], viewRectMax[1] );
                const ggl::geom::ray<3> sijRay = {
                    su*viewBasis[0] + sv*viewBasis[1] + viewRectW*viewBasis[2],
                    viewPos
                };

                ggl::geom::surface* sijClosest = ggl::geom::findClosest( sijRay, surfaces );
                GLfloat* sijPixel = &scenePixels[3 * (sj * sceneDim + si)];

                ggl::vector<GLfloat, 3> sijColor;
                if( sijClosest == nullptr ) {
                    sijColor = black;
                } else {
                    ggl::real sijRayT = sijClosest->intersect( sijRay ).min();
                    ggl::vectorf<3> sijSurfPos = sijRay.at( sijRayT );

                    ggl::vectorf<3> sijSurfNorm = sijClosest->normalAt( sijSurfPos );
                    sijColor = std::max( 0.05f, sijSurfNorm.dot(lightDir) ) * white;
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

    auto glfwHandleInputs = [ &renderScene, &viewPos, &viewDir ] ( GLFWwindow* window ) {
        // Quit Handling //

        int qKeyAction = glfwGetKey( window, GLFW_KEY_Q );
        int escKeyAction = glfwGetKey( window, GLFW_KEY_ESCAPE );
        if( qKeyAction == GLFW_PRESS || escKeyAction == GLFW_PRESS )
            glfwSetWindowShouldClose( window, GL_TRUE );

        // Scene Modification Handling //

        int wKeyAction = glfwGetKey( window, GLFW_KEY_W );
        if( wKeyAction == GLFW_PRESS )
            viewPos = viewPos - ggl::vectorf<3>{ 0.0f, 0.0f, 0.1f };

        int sKeyAction = glfwGetKey( window, GLFW_KEY_S );
        if( sKeyAction == GLFW_PRESS )
            viewPos = viewPos + ggl::vectorf<3>{ 0.0f, 0.0f, 0.1f };

        if( wKeyAction == GLFW_PRESS || sKeyAction == GLFW_PRESS ) {
            std::cout << "View Pos: " << viewPos << std::endl;
            renderScene();
        }
    };

    /// Update and Render ///

    glEnable( GL_TEXTURE_2D );
    glDisable( GL_LIGHTING );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    while( !glfwWindowShouldClose(window) ) {
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
    }

    /// Uninitialize GLFW Window ///

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}
