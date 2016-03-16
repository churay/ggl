#include <GLFW/glfw3.h>

#include <array>
#include <iostream>

#include "matrix.hpp"
#include "geom.hpp"
#include "consts.hpp"

int main() {
    if( !glfwInit() )
        return 1;

    GLFWwindow* window = glfwCreateWindow( 640, 480, "Hello World", NULL, NULL );
    if( window == NULL )
        return 1;

    glfwMakeContextCurrent( window );

    while( !glfwWindowShouldClose(window) )
    {
        // TODO(JRC): Implement the rendering code here.

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
