#include "matrix.hpp"
#include "geom.h"
#include "colors.hpp"
#include "consts.hpp"

#include "ex/basic_scene.h"

namespace ggl {

basic_scene::basic_scene() : scene( sDim, sDim ),
        mSphere{ ggl::vectorf<3>{ 0.5f, 0.5f, -0.5f }, 0.5f },
        mTriangle{
            ggl::vectorf<3>{ 0.0f, 0.0f, -0.10f },
            ggl::vectorf<3>{ 1.0f, 0.0f, -0.10f },
            ggl::vectorf<3>{ 0.5f, 1.0f, -0.10f }
        } {
    mSurfaces.push_back( &mSphere );
    mSurfaces.push_back( &mTriangle );
}


basic_scene::~basic_scene() {
    
}


void basic_scene::input( GLFWwindow* pWindow ) {
    scene::input( pWindow );
}


bool basic_scene::update( ggl::real pDelta ) {
    scene::update( pDelta );
    return mFrameCount == 1;
}


void basic_scene::render() {
    const ggl::real dimf = static_cast<ggl::real>( sDim - 1 );

    for( size_t sy = 0; sy < sDim; ++sy ) {
        for( size_t sx = 0; sx < sDim; ++sx ) {
            const ggl::real syf = sy / dimf, sxf = sx / dimf;
            const ggl::geom::ray sxyRay = {
                 ggl::vectorf<3>{ sxf, syf, +1.0f },
                 ggl::vectorf<3>{ 0.0f, 0.0f, -1.0f }
             };
            ggl::geom::surface* sxyClosest = ggl::geom::findClosest( sxyRay, mSurfaces );

            ggl::vectorc<3>& sxyPixel = mPixels[sy * sDim + sx];
            if( sxyClosest == &mTriangle ) { sxyPixel = ggl::color::gray<3>(); }
            else if( sxyClosest == &mSphere ) { sxyPixel = ggl::color::white<3>(); }
            else { sxyPixel = ggl::color::black<3>(); }
        }
    }
}

}
