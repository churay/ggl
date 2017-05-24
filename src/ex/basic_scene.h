#ifndef GGL_STATIC_SCENE_H
#define GGL_STATIC_SCENE_H

#include <vector>

#include "ex/scene.h"
#include "consts.hpp"

namespace ggl {

class basic_scene : scene {
    public:

    /// Class Attributes ///

    static const size_t sDim = 500;

    /// Constructors ///

    basic_scene();

    /// Class Functions ///

    virtual void input( GLFWwindow* pWindow );
    virtual bool update( ggl::real pDelta );
    virtual void render();

    private:

    /// Class Fields ///

    std::vector<ggl::geom::surface*> mSurfaces;
    ggl::geom::sphere mSphere;
    ggl::geom::triangle mTriangle;
};

}

#endif
