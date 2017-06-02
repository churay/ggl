#ifndef GGL_MATERIAL_SCENE_H
#define GGL_MATERIAL_SCENE_H

#include <vector>

#include "geom.h"
#include "camera.h"
#include "colors.hpp"
#include "consts.hpp"

#include "ex/scene.h"

namespace ggl {

enum material { diffuse, reflective, dielectric, polished };

class material_scene : public scene {
    public:

    /// Class Attributes ///

    static const size_t sDim = 100;

    /// Constructors ///

    material_scene();
    virtual ~material_scene();

    /// Class Functions ///

    virtual void input( GLFWwindow* pWindow );
    virtual bool update( ggl::real pDelta );
    virtual void render();

    private:

    /// Private Functions ///

    ggl::vectorc<3> _calcRayLight( const ggl::geom::ray& pRay,
        const ggl::geom::surface* pPrevSurface = nullptr,
        const ggl::vectorf<3> pPrevPtIntx = ggl::vectorf<3>(),
        const size_t pCasts = 0 );

    /// Class Fields ///

    std::vector<ggl::geom::surface*> mSurfaces;
    std::vector<ggl::material> mMaterials;
    ggl::geom::box mMatBox, mEnvBox;

    ggl::camera mCamera;
    bool mDoRender;
};

}

#endif
