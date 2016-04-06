#include <array>
#include <cstring>
#include <iostream>
#include <vector>
#include <utility>

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

    const ggl::vector<GLfloat, 3> red{ 1.0f, 0.0f, 0.0f },
        green{ 0.0f, 1.0f, 0.0f }, black{ 0.0f, 0.0f, 0.0f };
    const ggl::vectorf<3> lightPos{ 5.0f, 100.0f, -5.0f };

    ggl::geom::sphere sphereT{ ggl::vectorf<3>{5.0f, 8.0f, -5.0f}, 2.0f };
    ggl::geom::sphere sphereB{ ggl::vectorf<3>{5.0f, 3.0f, -5.0f}, 3.0f };
    std::vector<ggl::geom::surface*> surfaces{ &sphereT, &sphereB };

    const unsigned sceneDim = 500;
    const ggl::real sceneDimf = static_cast<ggl::real>( sceneDim - 1 );
    GLfloat scenePixels[3 * sceneDim * sceneDim];
    for( size_t sy = 0; sy < sceneDim; ++sy ) {
        for( size_t sx = 0; sx < sceneDim; ++sx ) {
            const ggl::real syf = 10.0f * (sy / sceneDimf), sxf = 10.0f * (sx / sceneDimf);
            const ggl::geom::ray<3> sxyRay = {
                ggl::vectorf<3>{ sxf, syf, +1.0f },
                ggl::vectorf<3>{ 0.0f, 0.0f, -1.0f }
            };

            ggl::geom::surface* sxyClosest = ggl::geom::findClosest( sxyRay, surfaces );
            GLfloat* sxyPixel = &scenePixels[3 * (sy * sceneDim + sx)];
            const ggl::vector<GLfloat, 3> sxyBaseColor = ( sxyClosest == &sphereT ) ?
                red : ( (sxyClosest == &sphereB) ? green : black );

            ggl::real sxyLightScale = 1.0f;
            if( sxyClosest != nullptr ) {
                ggl::real sxyRayT = sxyClosest->intersect( sxyRay ).min();
                ggl::vectorf<3> sxySurfPos = sxyRay.at( sxyRayT );

                ggl::vectorf<3> sxySurfNorm = sxyClosest->normalAt( sxySurfPos );
                ggl::vectorf<3> sxySurfLDir = ( lightPos - sxySurfPos ).normalize();
                sxyLightScale = std::max( 0.05f, sxySurfNorm.dot(sxySurfLDir) );

                const ggl::geom::ray<3> sxyShadowRay = { sxySurfPos, sxySurfLDir };
                std::vector<ggl::geom::surface*> sxyShadowSurfaces;
                for( ggl::geom::surface* surface : surfaces )
                    if( surface != sxyClosest )
                        sxyShadowSurfaces.push_back( surface );

                ggl::geom::surface* sxyShadowClosest = ggl::geom::findClosest(
                    sxyShadowRay, sxyShadowSurfaces );
                sxyLightScale = ( sxyShadowClosest != nullptr ) ? 0.05f : sxyLightScale;
            }

            ggl::vector<GLfloat, 3> sxyColor = sxyLightScale * sxyBaseColor;
            std::memcpy( sxyPixel, sxyColor.data(), 3 * sizeof(GLfloat) );
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
