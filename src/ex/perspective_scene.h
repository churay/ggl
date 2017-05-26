#ifndef GGL_PERSPECTIVE_SCENE_H
#define GGL_PERSPECTIVE_SCENE_H

#include <vector>

#include "geom.h"
#include "matrix.hpp"
#include "consts.hpp"

#include "ex/scene.h"

namespace ggl {

class perspective_scene : public scene {
    public:

    /// Class Attributes ///

    static const size_t sDim = 100;

    /// Constructors ///

    perspective_scene();
    virtual ~perspective_scene();

    /// Class Functions ///

    virtual void input( GLFWwindow* pWindow );
    virtual bool update( ggl::real pDelta );
    virtual void render();

    private:

    /// Class Fields ///

    std::vector<ggl::geom::surface*> mSurfaces;
    ggl::geom::box mBox;

    ggl::vectorf<3> mViewRectMin, mViewRectMax;
    ggl::real mViewRadius, mViewAngleH, mViewAngleV;

    bool mDoRender;
};

}

#endif
