#include "matrix.hpp"
#include "geom.h"
#include "colors.hpp"
#include "consts.hpp"

#include "ex/shadow_scene.h"

namespace ggl {

shadow_scene::shadow_scene() : scene( sDim, sDim ),
        mTopSphere{ ggl::vectorf<3>{5.0f, 8.0f, -5.0f}, 2.0f },
        mBotSphere{ ggl::vectorf<3>{5.0f, 3.0f, -5.0f}, 3.0f } {
    mSurfaces.push_back( &mTopSphere );
    mSurfaces.push_back( &mBotSphere );
}


shadow_scene::~shadow_scene() {
    
}


void shadow_scene::input( GLFWwindow* pWindow ) {
    scene::input( pWindow );
}


bool shadow_scene::update( ggl::real pDelta ) {
    scene::update( pDelta );
    return mFrameCount == 1;
}


void shadow_scene::render() {
    const ggl::real dimf = static_cast<ggl::real>( sDim - 1 );
    const ggl::vectorf<3> lightPos{ 5.0f, 100.0f, -5.0f };

    for( size_t sy = 0; sy < sDim; ++sy ) {
        for( size_t sx = 0; sx < sDim; ++sx ) {
            const ggl::real syf = 10.0f * (sy / dimf), sxf = 10.0f * (sx / dimf);
            const ggl::geom::ray sxyRay = {
                ggl::vectorf<3>{ sxf, syf, +1.0f },
                ggl::vectorf<3>{ 0.0f, 0.0f, -1.0f }
            };
            ggl::geom::surface* sxyClosest = ggl::geom::findClosest( sxyRay, mSurfaces );

            ggl::vectorc<3> sxyBaseColor;
            if( sxyClosest == &mTopSphere ) { sxyBaseColor = ggl::color::red<3>(); }
            else if( sxyClosest == &mBotSphere ) { sxyBaseColor = ggl::color::green<3>(); }
            else { sxyBaseColor = ggl::color::black<3>(); }

            ggl::real sxyLightScale = 1.0f;
            if( sxyClosest != nullptr ) {
                ggl::real sxyRayT = sxyClosest->intersect( sxyRay ).min();
                ggl::vectorf<3> sxySurfPos = sxyRay.at( sxyRayT );

                ggl::vectorf<3> sxySurfNorm = sxyClosest->normalAt( sxySurfPos );
                ggl::vectorf<3> sxySurfLDir = ( lightPos - sxySurfPos ).normalize();
                sxyLightScale = std::max( 0.05f, sxySurfNorm.dot(sxySurfLDir) );

                const ggl::geom::ray sxyShadowRay = { sxySurfPos, sxySurfLDir };
                std::vector<ggl::geom::surface*> sxyShadowSurfaces;
                for( ggl::geom::surface* surface : mSurfaces )
                    if( surface != sxyClosest )
                        sxyShadowSurfaces.push_back( surface );

                ggl::geom::surface* sxyShadowClosest = ggl::geom::findClosest(
                    sxyShadowRay, sxyShadowSurfaces );
                sxyLightScale = ( sxyShadowClosest != nullptr ) ? 0.05f : sxyLightScale;
            }

            ggl::vectorc<3>& sxyPixel = mPixels[sy * sDim + sx];
            sxyPixel = sxyLightScale * sxyBaseColor;
        }
    }
}

}
