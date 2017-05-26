#include <vector>

#include "consts.hpp"

#include "ex/scene.h"

namespace ggl {

scene::scene( const size_t pWidth, const size_t pHeight ) {
    mWidth = pWidth;
    mHeight = pHeight;
    mPixels.resize( mWidth * mHeight );

    mFrameCount = 0;
    mFrameTime = ggl::zero();
}


scene::~scene() {
    
}


void scene::input( GLFWwindow* pWindow ) {
    int qKeyAction = glfwGetKey( pWindow, GLFW_KEY_Q );
    int escKeyAction = glfwGetKey( pWindow, GLFW_KEY_ESCAPE );
    if( qKeyAction == GLFW_PRESS || escKeyAction == GLFW_PRESS )
        glfwSetWindowShouldClose( pWindow, GL_TRUE );
}


bool scene::update( ggl::real pDelta ) {
    mFrameCount += 1;
    mFrameTime += pDelta;
    return true;
}


const std::vector<ggl::vectorc<3>>& scene::pixels() {
    return mPixels;
}


size_t scene::width() {
    return mWidth;
}


size_t scene::height() {
    return mHeight;
}

}
