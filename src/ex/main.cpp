#include <array>
#include <vector>
#include <iostream>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "tgeom.hpp"

#include "util/timer.h"
#include "colors.hpp"
#include "consts.hpp"

#include "ex/scene.h"
#include "ex/basic_scene.h"
#include "ex/shadow_scene.h"
#include "ex/perspective_scene.h"
#include "ex/material_scene.h"

#ifndef GGL_SCENE
#define GGL_SCENE_NAME basic_scene
#else
#define GGL_SCENE_NAME GGL_SCENE
#endif

// NOTE: This code is necessary due to how the preprocessor handles/expands
// variables when they're used in other variables.
// See this for more information: https://stackoverflow.com/a/1489985
#define PREPASTE(V1,V2) V1/**/V2
#define GGL_CLASS_NAME PREPASTE(ggl::,GGL_SCENE_NAME)

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

    ggl::scene* scene = new GGL_CLASS_NAME();
    const std::vector<ggl::vectorc<3>>& scenePixels = scene->pixels();

    /// Update and Render ///

    glEnable( GL_TEXTURE_2D );
    glDisable( GL_LIGHTING );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    ggl::timer glfwTimer( 60 );

    while( !glfwWindowShouldClose(window) ) {
        glfwTimer.split();

        scene->input( window );
        if( scene->update(glfwTimer.dt()) ) {
            scene->render();

            GLuint sceneTID = 0;
            glGenTextures( 1, &sceneTID );
            glBindTexture( GL_TEXTURE_2D, sceneTID );

            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, scene->width(), scene->height(),
                0, GL_RGB, GL_FLOAT, scenePixels.data() );
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
