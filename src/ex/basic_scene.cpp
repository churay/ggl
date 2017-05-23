#include "matrix.hpp"
#include "geom.hpp"
#include "consts.hpp"

#include "ex/basic_scene.h"

namespace ggl {

basic_scene::basic_scene() : scene( sDim, sDim ) {
    mSphere = { ggl::vectorf<3>{ 0.5f, 0.5f, -0.5f }, 0.5f };
    mTriangle = {
        ggl::vectorf<3>{ 0.0f, 0.0f, -0.10f },
        ggl::vectorf<3>{ 1.0f, 0.0f, -0.10f },
        ggl::vectorf<3>{ 0.5f, 1.0f, -0.10f }
    };

    mSurfaces.push_back( &mSphere );
    mSurfaces.push_back( &mTriangle );
}


void basic_scene::update( ggl::real pDelta ) {
    
}


void basic_scene::render() {
    const ggl::real sDimf = static_cast<ggl::real>( sDim - 1 );

    for( size_t sy = 0; sy < sDim; ++sy ) {
        for( size_t sx = 0; sx < sDim; ++sx ) {
            const ggl::real syf = sy / sDimf, sxf = sx / sDimf;
            const ggl::geom::ray<3> sxyRay = {
                ggl::vectorf<3>{ sxf, syf, +1.0f },
                ggl::vectorf<3>{ 0.0f, 0.0f, -1.0f }
            };

            ggl::geom::surface* sxyClosest = ggl::geom::findClosest( sxyRay, mSurfaces );
            GLfloat& sxyPixel = mPixels[sy * sDim + sx];
            if( sxyClosest == &mTriangle ) { sxyPixel = grey; }
            else if( sxyClosest == &mSphere ) { sxyPixel = white; }
            else { sxyPixel = black; }
        }
    }
}

}
