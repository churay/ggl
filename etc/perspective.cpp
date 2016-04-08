#include <array>
#include <cmath>
#include <vector>
#include <utility>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "matrix.hpp"
#include "geom.hpp"
#include "xform.hpp"
#include "util.h"
#include "consts.hpp"

void handleInputs( GLFWwindow* window, int key, int scode, int action, int mod ) {
    if( (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS )
        glfwSetWindowShouldClose( window, GL_TRUE );
}

int main() {
    /// Initialize GLFW Window ///

    if( !glfwInit() )
        return 1;

    GLFWwindow* window = glfwCreateWindow( 100, 100, "ggl", NULL, NULL );
    if( window == NULL )
        return 1;

    glfwMakeContextCurrent( window );
    glfwSetKeyCallback( window, handleInputs );
    glfwSwapInterval( 1 );

    /// Initialize Scene Geometry ///

    const ggl::vectorf<3> viewPos{ 0.0f, 0.0f, +2.0f };
    const ggl::vectorf<3> viewDir{ 0.0f, 0.0f, -1.0f };

    const ggl::real viewRectW{ -2.0f };
    const ggl::vectorf<3> viewRectMin{ -2.0f, -2.0f, viewRectW };
    const ggl::vectorf<3> viewRectMax{ +2.0f, +2.0f, viewRectW };

    const std::array<ggl::vectorf<3>, 3> viewBasis = ggl::xform::basis( -viewDir );

    const ggl::vector<GLfloat, 3> white{ 1.0f, 1.0f, 1.0f }, black{ 0.0f, 0.0f, 0.0f };
    const ggl::vectorf<3> lightDir{ 0.0f, 1.0f, 0.0f };

    ggl::geom::sphere sphere{ ggl::vectorf<3>{0.0f, 0.0f, 0.0f}, std::sqrt(2.0f) };
    std::vector<ggl::geom::surface*> surfaces{ &sphere };

    /// Create Scene Rendering ///

    const unsigned sceneDim = 100;
    const ggl::real sceneDimf = static_cast<ggl::real>( sceneDim - 1 );
    GLfloat scenePixels[3 * sceneDim * sceneDim];
    for( size_t sj = 0; sj < sceneDim; ++sj ) {
        for( size_t si = 0; si < sceneDim; ++si ) {
            const ggl::real su = ggl::util::lerp( si / sceneDimf, viewRectMin[0], viewRectMax[0] );
            const ggl::real sv = ggl::util::lerp( sj / sceneDimf, viewRectMin[1], viewRectMax[1] );
            const ggl::geom::ray<3> sijRay = {
                su * viewBasis[0] + sv * viewBasis[1] + viewRectW * viewBasis[2],
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

    /// Bind Scene Rendering to a Texture ///

    GLuint sceneTID = 0;

    glGenTextures( 1, &sceneTID );
    glBindTexture( GL_TEXTURE_2D, sceneTID );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, sceneDim, sceneDim, 0,
        GL_RGB, GL_FLOAT, scenePixels );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

    /// Update and Render ///

    glEnable( GL_TEXTURE_2D );
    glDisable( GL_LIGHTING );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    while( !glfwWindowShouldClose(window) ) {
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
