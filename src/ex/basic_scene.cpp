#include "matrix.hpp"
#include "colors.hpp"
#include "consts.hpp"

#include "ex/basic_scene.h"

namespace ggl {

basic_scene::basic_scene() : scene( sDim, sDim ) {
    
}


basic_scene::~basic_scene() {
    
}


void basic_scene::input( GLFWwindow* pWindow ) {
    scene::input( pWindow );
}


bool basic_scene::update( ggl::real pDelta ) {
    return false;
}


void basic_scene::render() {
    // const ggl::real sDimf = static_cast<ggl::real>( sDim - 1 );

    for( size_t sy = 0; sy < sDim; ++sy ) {
        for( size_t sx = 0; sx < sDim; ++sx ) {
            /*
            const ggl::real syf = sy / sDimf, sxf = sx / sDimf;
            */

            GLuint& sxyPixel = mPixels[sy * sDim + sx];
            sxyPixel = ggl::color::black();
            /*
            if( sxyClosest == &mTriangle ) { sxyPixel = ggl::color::gray(); }
            else if( sxyClosest == &mSphere ) { sxyPixel = ggl::color::white(); }
            else { sxyPixel = ggl::color::black(); }
            */
        }
    }
}

}
