#ifndef GGL_SCENE_H
#define GGL_SCENE_H

#include <vector>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "colors.hpp"
#include "consts.hpp"

namespace ggl {

class scene {
    public:

    /// Constructors ///

    scene( const size_t pWidth, const size_t pHeight );
    virtual ~scene();

    /// Class Functions ///

    virtual void input( GLFWwindow* pWindow );
    virtual bool update( ggl::real pDelta );
    virtual void render() = 0;

    const std::vector<ggl::vectorc<3>>& pixels();
    size_t width();
    size_t height();

    protected:

    /// Class Fields ///

    std::vector<ggl::vectorc<3>> mPixels;
    size_t mWidth, mHeight;

    size_t mFrameCount;
    ggl::real mFrameTime;
};

}

#endif
