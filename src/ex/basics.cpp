#include <array>
#include <vector>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "matrix.hpp"
#include "geom.hpp"
#include "xform.hpp"
#include "util/timer.h"
#include "consts.hpp"

void handleInputs( GLFWwindow* window, int key, int scode, int action, int mod ) {
    if( (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS )
        glfwSetWindowShouldClose( window, GL_TRUE );
}

int main() {
    /// Initialize GLFW Window ///

    if( !glfwInit() )
        return 1;

    GLFWwindow* window = glfwCreateWindow( 500, 500, "ggl", NULL, NULL );
    if( window == NULL )
        return 1;

    glfwMakeContextCurrent( window );
    glfwSetKeyCallback( window, handleInputs );
    glfwSwapInterval( 1 );

    /// Initialize Scene Geometry ///

    const GLuint black = 0xff000000, grey = 0xffaaaaaa, white = 0xffffffff;

    ggl::geom::sphere sphere{ ggl::vectorf<3>{ 0.5f, 0.5f, -0.5f }, 0.5f };
    ggl::geom::triangle triangle{
        ggl::vectorf<3>{ 0.0f, 0.0f, -0.10f },
        ggl::vectorf<3>{ 1.0f, 0.0f, -0.10f },
        ggl::vectorf<3>{ 0.5f, 1.0f, -0.10f }
    };
    std::vector<ggl::geom::surface*> surfaces{ &sphere, &triangle };

    const unsigned sceneDim = 500;
    const ggl::real sceneDimf = static_cast<ggl::real>( sceneDim - 1 );
    GLuint scenePixels[sceneDim * sceneDim];
    for( size_t sy = 0; sy < sceneDim; ++sy ) {
        for( size_t sx = 0; sx < sceneDim; ++sx ) {
            const ggl::real syf = sy / sceneDimf, sxf = sx / sceneDimf;
            const ggl::geom::ray<3> sxyRay = {
                ggl::vectorf<3>{ sxf, syf, +1.0f },
                ggl::vectorf<3>{ 0.0f, 0.0f, -1.0f }
            };

            ggl::geom::surface* sxyClosest = ggl::geom::findClosest( sxyRay, surfaces );
            GLuint& sxyPixel = scenePixels[sy * sceneDim + sx];
            if( sxyClosest == &triangle ) { sxyPixel = grey; }
            else if( sxyClosest == &sphere ) { sxyPixel = white; }
            else { sxyPixel = black; }
        }
    }

    /// Bind Scene Rendering to a Texture ///

    GLuint sceneTID = 0;

    glGenTextures( 1, &sceneTID );
    glBindTexture( GL_TEXTURE_2D, sceneTID );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, sceneDim, sceneDim, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, scenePixels );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

    /// Update and Render ///

    glEnable( GL_TEXTURE_2D );
    glDisable( GL_LIGHTING );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    ggl::timer glfwTimer;

    while( !glfwWindowShouldClose(window) ) {
        glfwTimer.split();

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
    }

    /// Uninitialize GLFW Window ///

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}
