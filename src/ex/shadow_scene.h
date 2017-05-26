#ifndef GGL_SHADOW_SCENE_H
#define GGL_SHADOW_SCENE_H

#include <vector>

#include "geom.h"
#include "consts.hpp"

#include "ex/scene.h"

namespace ggl {

class shadow_scene : public scene {
    public:

    /// Class Attributes ///

    static const size_t sDim = 500;

    /// Constructors ///

    shadow_scene();
    virtual ~shadow_scene();

    /// Class Functions ///

    virtual void input( GLFWwindow* pWindow );
    virtual bool update( ggl::real pDelta );
    virtual void render();

    private:

    /// Class Fields ///

    std::vector<ggl::geom::surface*> mSurfaces;
    ggl::geom::sphere mTopSphere, mBotSphere;
};

}

#endif
