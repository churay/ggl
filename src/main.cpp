#include <array>
#include <iostream>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "matrix.hpp"
#include "geom.hpp"
#include "consts.hpp"

void handleInputs( GLFWwindow* window, int key, int scode, int action, int mod ) {
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose( window, GL_TRUE );
}

int main() {
    if( !glfwInit() )
        return 1;

    GLFWwindow* window = glfwCreateWindow( 640, 480, "ggl", NULL, NULL );
    if( window == NULL )
        return 1;

    glfwMakeContextCurrent( window );
    glfwSetKeyCallback( window, handleInputs );
    glfwSwapInterval( 1 );

    while( !glfwWindowShouldClose(window) ) {
        int windowWidth = 0, windowHeight = 0;
        glfwGetFramebufferSize( window, &windowWidth, &windowHeight );
        ggl::real windowRatio = static_cast<ggl::real>( windowWidth ) / windowHeight;

        glViewport( 0, 0, windowWidth, windowHeight );
        glClear( GL_COLOR_BUFFER_BIT );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( -windowRatio, +windowRatio, -1.0f, +1.0f, +1.0f, -1.0f );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glRotatef( 50.0f * glfwGetTime(), 0.0f, 0.0f, 1.0f );

        glBegin( GL_TRIANGLES );
        glColor3f( +1.0f, +0.0f, +0.0f );
        glVertex3f( -0.6f, -0.4f, +0.0f );
        glColor3f( +0.0f, +1.0f, +0.0f );
        glVertex3f( +0.6f, -0.4f, +0.0f );
        glColor3f( +0.0f, +0.0f, +1.0f );
        glVertex3f( +0.0f, +0.6f, 0.0f );
        glEnd();

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}
