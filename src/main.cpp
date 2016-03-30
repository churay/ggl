#include <array>
#include <iostream>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "matrix.hpp"
#include "geom.hpp"
#include "xform.hpp"
#include "consts.hpp"

void handleInputs( GLFWwindow* window, int key, int scode, int action, int mod ) {
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
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

    /*
    const uint32_t black = 0x000000ff, grey = 0xaaaaaaff, white = 0xffffffff;

    const ggl::geom::surface<ggl::geom::sphere> sphere = { ggl::geom::sphere{
        ggl::vectorf<3>{ -1.0f, -1.0f, 0.0f },
        2.0f
    } };

    const ggl::geom::surface<ggl::geom::triangle> triangle = { ggl::geom::triangle{
        ggl::vectorf<3>{ -1.0f, -1.0f, -0.5f },
        ggl::vectorf<3>{ +1.0f, -1.0f, -0.5f },
        ggl::vectorf<3>{ +0.5f, +1.0f, -0.5f }
    } };

    uint32_t scene[2000 * 2000];
    for( size_t sy = 0; sy < 2000; ++sy ) {
        for( size_t sx = 0; sx < 2000; ++sx ) {
            // TODO(JRC): Update this code so that each of the pixels in the
            // "scene" array is filled with the color of the surface that
            // was hit first by a ray coming from the viewing plane.
            scene[sy * 2000 + sx] = 0xffffffff;
        }
    }
    */

    /// Update and Render ///

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

        // TODO(JRC): Render the pixels in the "scene" array by rendering it as
        // a texture map that lies at the near viewing plane.

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    /// Uninitialize GLFW Window ///

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}
