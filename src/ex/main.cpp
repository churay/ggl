#include <array>
#include <iostream>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "matrix.hpp"
#include "geom.hpp"
#include "xform.hpp"
#include "util/timer.h"
#include "consts.hpp"

#ifndef GGL_SCENE
#define GGL_SCENE_NAME basic
#else
#define GGL_SCENE_NAME GGL_SCENE
#endif

#define GGL_SCENE_CLASS GGL_SCENE_NAME ## _scene

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

    ggl::scene* scene = new GGL_SCENE_CLASS();
    const ggl::vector<GLuint>& scenePixels = scene->pixels();

    /// Update and Render ///

    glEnable( GL_TEXTURE_2D );
    glDisable( GL_LIGHTING );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    ggl::timer glfwTimer( 60 );
    bool glfwDoRender = false;

    while( !glfwWindowShouldClose(window) ) {
        glfwTimer.split();

        scene->input( window );
        glfwDoRender = scene->update( glfwTimer.delta() );

        /// Bind Scene Rendering to a Texture ///
        if( glfwDoRender ) {
            scene->render();

            GLuint sceneTID = 0;
            glGenTextures( 1, &sceneTID );
            glBindTexture( GL_TEXTURE_2D, sceneTID );

            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, scene->width(), scene->height(),
                0, GL_RGBA, GL_UNSIGNED_BYTE, scenePixels );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
        }

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
