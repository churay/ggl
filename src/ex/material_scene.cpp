#include "geom.h"
#include "xform.hpp"
#include "interval.h"
#include "matrix.hpp"
#include "colors.hpp"
#include "consts.hpp"

#include "util/util.h"
#include "util/tutil.hpp"

#include "ex/material_scene.h"

namespace ggl {

material_scene::material_scene() : scene( sDim, sDim ),
        mMatBox{ ggl::vectorf<3>(-1.0f), ggl::vectorf<3>(+1.0f) },
        mEnvBox{ ggl::vectorf<3>(-10.0f), ggl::vectorf<3>(+10.0f) } {
    mSurfaces.push_back( &mMatBox );
    mMaterials.push_back( ggl::material::dielectric );

    mSurfaces.push_back( &mEnvBox );
    mMaterials.push_back( ggl::material::diffuse );

    mViewRectMin = ggl::vectorf<3>{ -2.0f, -2.0f, -2.0f };
    mViewRectMax = ggl::vectorf<3>{ +2.0f, +2.0f, -2.0f };

    mViewRadius = 5.0f;
    mViewAngleH = -ggl::pi() / 4.0f;
    mViewAngleV = +ggl::pi() / 4.0f;

    mDoRender = true;
}


material_scene::~material_scene() {
    
}


void material_scene::input( GLFWwindow* pWindow ) {
    const ggl::real viewAngleIncr{ ggl::pi() / 32.0f };
    const ggl::real viewRadiusIncr{ 0.05f };

    auto angleWrap = [] ( const ggl::real& pAngle ) {
        return ggl::util::wrap( pAngle, 0.0f, 2.0f * ggl::pi() ); };
    auto radiusClamp = [] ( const ggl::real& pRadius ) {
        return ggl::util::clamp( pRadius, 3.0f, 20.0f ); };

    scene::input( pWindow );

    int wKeyAction = glfwGetKey( pWindow, GLFW_KEY_W );
    if( wKeyAction == GLFW_PRESS )
        mViewAngleV = angleWrap( mViewAngleV + viewAngleIncr );
    int sKeyAction = glfwGetKey( pWindow, GLFW_KEY_S );
    if( sKeyAction == GLFW_PRESS )
        mViewAngleV = angleWrap( mViewAngleV - viewAngleIncr );

    int aKeyAction = glfwGetKey( pWindow, GLFW_KEY_A );
    if( aKeyAction == GLFW_PRESS )
        mViewAngleH = angleWrap( mViewAngleH - viewAngleIncr );
    int dKeyAction = glfwGetKey( pWindow, GLFW_KEY_D );
    if( dKeyAction == GLFW_PRESS )
        mViewAngleH = angleWrap( mViewAngleH + viewAngleIncr );

    int rKeyAction = glfwGetKey( pWindow, GLFW_KEY_R );
    if( rKeyAction == GLFW_PRESS )
        mViewRadius = radiusClamp( mViewRadius - viewRadiusIncr );
    int fKeyAction = glfwGetKey( pWindow, GLFW_KEY_F );
    if( fKeyAction == GLFW_PRESS )
        mViewRadius = radiusClamp( mViewRadius + viewRadiusIncr );

    mDoRender = wKeyAction == GLFW_PRESS || sKeyAction == GLFW_PRESS ||
        aKeyAction == GLFW_PRESS || dKeyAction == GLFW_PRESS ||
        rKeyAction == GLFW_PRESS || fKeyAction == GLFW_PRESS;
}


bool material_scene::update( ggl::real pDelta ) {
    scene::update( pDelta );
    return ( mFrameCount == 1 ) || mDoRender;
}


void material_scene::render() {
    const std::array<ggl::vectorf<3>, 3> xyzBasis = ggl::geom::basis();
    const ggl::vectorf<3>& xDir = xyzBasis[0], & yDir = xyzBasis[1], & zDir = xyzBasis[2];

    ggl::matrixf<3, 3> viewPosHXform =
        ggl::xform::rotate( mViewAngleH, yDir ).template submatrix<0, 0, 3, 3>();
    ggl::matrixf<3, 3> viewPosVXform =
        ggl::xform::rotate( mViewAngleV, viewPosHXform * zDir ).template submatrix<0, 0, 3, 3>();

    ggl::vectorf<3> viewPos = mViewRadius * ( viewPosVXform * viewPosHXform * xDir );
    ggl::vectorf<3> viewDir = -viewPos.normalize();
    std::array<ggl::vectorf<3>, 3> viewBasis = ggl::xform::basis( -viewDir );

    for( size_t sj = 0; sj < sDim; ++sj ) {
        for( size_t si = 0; si < sDim; ++si ) {
            const ggl::real su = ggl::util::lerp( (si + 0.5f) / sDim,
                mViewRectMin[0], mViewRectMax[0] );
            const ggl::real sv = ggl::util::lerp( (sj + 0.5f) / sDim,
                mViewRectMin[1], mViewRectMax[1] );
            const ggl::geom::ray sijRay = { viewPos,
                su*viewBasis[0] + sv*viewBasis[1] + mViewRectMin[2]*viewBasis[2] };

            mPixels[sj * sDim + si] = (*this)._calcRayLight( sijRay );
        }
    }
}


ggl::vectorc<3> material_scene::_calcRayLight(
        const ggl::geom::ray& pRay,
        const ggl::geom::surface* pPrevSurface,
        const ggl::vectorf<3> pPrevPtIntx,
        const size_t pCasts ) {
    const static ggl::vectorf<3> sLightPos{ 0.0f, 5.0f, 0.0f };

    const static ggl::real sMatRefrIdx{ 1.330f };
    const static ggl::real sMatAtten{ 0.75f };
    const static ggl::real sMatReflBase{ 0.70f };
    // NOTE(JRC): This formula is another result of "Shlick's Approximation"
    // that uses refraction indices to determine the amount of light reflected
    // when light enters one medium from another.
    const static ggl::real sMatReflRefr{
        std::pow( sMatRefrIdx - 1.0f, 2.0f ) / std::pow( sMatRefrIdx + 1.0f, 2.0f )
    };

    const static auto sCalcReflectFactor = [] (
            const ggl::vectorf<3>& pIncident,
            const ggl::vectorf<3>& pNormal,
            const ggl::real& pBaseFactor ) {
        ggl::real reflectCos = ( -pIncident ).dot( pNormal );
        ggl::real absorbedAmount = std::pow( 1.0f - reflectCos, 5.0f );
        return ggl::util::lerp( pBaseFactor, absorbedAmount, 1.0f );
    };
    const static auto sReflectRay = [] ( 
            const ggl::vectorf<3>& pIncident,
            const ggl::vectorf<3>& pNormal ) {
        ggl::vectorf<3> reflect = pIncident - 2.0f * pIncident.projectOnto( pNormal );
        return reflect.normalize();
    };
    const static auto sRefractRay = [] (
            const ggl::vectorf<3>& pIncident,
            const ggl::vectorf<3>& pNormal,
            const ggl::real& pRefrIdx ) {
        // NOTE(JRC): See "Realistic Ray Tracing" (p. 45) for the derivations of
        // these formulas.
        ggl::real reflectCos = ( -pIncident ).dot( pNormal );
        ggl::real invRefrIdx = 1.0f / pRefrIdx;

        ggl::vectorf<3> refrOrtho = invRefrIdx * ( pIncident - reflectCos * pNormal );
        ggl::vectorf<3> refrNormal = pNormal * 
            -std::sqrt( 1.0f - std::pow(invRefrIdx, 2.0f) *
            (1.0f - std::pow(reflectCos, 2.0f)) );

        return ( refrOrtho + refrNormal ).normalize();
    };

    ggl::geom::surface* surface = ggl::geom::findClosest( pRay, mSurfaces );
    if( pCasts > 5 ) { return ggl::color::white<3>(); }
    if( surface == nullptr ) { return ggl::color::black<3>(); }

    // NOTE(JRC): Surfaces can still self-intersect, but they cannot reintersect
    // at zero; such vectors are always surface reflection/refraction vectors.
    ggl::interval surfRayIntx = surface->intersect( pRay );
    ggl::vectorf<3> surfPtIntx = pRay.at( surfRayIntx.min() );
    if( surface == pPrevSurface &&
            surface->normalAt(surfPtIntx) == surface->normalAt(pPrevPtIntx) ) {
        if( surfRayIntx.empty() ) {
            std::vector<ggl::geom::surface*> surfs = mSurfaces;
            surfs.erase( std::find(surfs.begin(), surfs.end(), pPrevSurface) );

            surface = ggl::geom::findClosest( pRay, surfs );
            if( surface == nullptr ) { return ggl::color::black<3>(); }
            surfRayIntx = surface->intersect( pRay );
        } else {
            surfRayIntx = ggl::interval( surfRayIntx.max() );
        }
    }

    size_t surfIdx = std::find( mSurfaces.begin(), mSurfaces.end(), surface ) - mSurfaces.begin();
    ggl::material surfMat = mMaterials[surfIdx];

    ggl::vectorf<3> rayVec = pRay.mVector.normalize();
    ggl::real rayT = surfRayIntx.min();
    ggl::vectorf<3> surfPos = pRay.at( rayT );
    ggl::vectorf<3> surfNorm = ( surfIdx ? -1.0f : 1.0f ) * surface->normalAt( surfPos );

    ggl::vectorf<3> surfToLight = ( sLightPos - surfPos ).normalize();
    ggl::vectorf<3> surfReflLight = sReflectRay( rayVec, surfNorm );

    if( surfMat == ggl::material::diffuse ) {
        const std::array<ggl::vectorf<3>, 3> xyz = ggl::geom::basis();

        // NOTE(JRC): This ugly little piece of code is responsible for giving
        // a color to each of the separate faces of the environment cube.
        ggl::vectorc<3> faceColor;
        if( surfNorm == -xyz[0] ) { faceColor = ggl::color::red<3>(); }
        else if( surfNorm == +xyz[0] ) { faceColor = ggl::color::green<3>(); }
        else if( surfNorm == -xyz[1] ) { faceColor = ggl::color::blue<3>(); }
        else if( surfNorm == +xyz[1] ) { faceColor = ggl::color::yellow<3>(); }
        else if( surfNorm == -xyz[2] ) { faceColor = ggl::color::magenta<3>(); }
        else if( surfNorm == +xyz[2] ) { faceColor = ggl::color::cyan<3>(); }
        else { faceColor = ggl::color::black<3>(); }

        return std::max( 0.05f, surfNorm.dot(surfToLight) ) * faceColor;
    } else if( surfMat == ggl::material::reflective ) {
        ggl::geom::ray reflRay = { surfPos, surfReflLight };
        ggl::real surfReflFactor = sCalcReflectFactor( rayVec, surfNorm, sMatReflBase );

        return surfReflFactor * _calcRayLight( reflRay, surface, surfPos, pCasts+1 );
    } else if( surfMat == ggl::material::dielectric ) {
        ggl::vectorf<3> surfRefrLight;
        ggl::real surfReflFactor, surfAtten;

        bool isRayEntering = ggl::util::flt( rayVec.dot(surfNorm), 0.0f );
        if( isRayEntering ) {
            surfRefrLight = sRefractRay( rayVec, surfNorm, sMatRefrIdx );
            surfReflFactor = sCalcReflectFactor( rayVec, surfNorm, sMatReflRefr );
            surfAtten = 1.0f;
        } else {
            surfRefrLight = sRefractRay( rayVec, surfNorm, 1.0f / sMatRefrIdx );
            surfReflFactor = sCalcReflectFactor( -surfRefrLight, surfNorm, sMatReflRefr );
            surfAtten = std::pow( sMatAtten, (surfPos - pRay.mOrigin).normal() );
        }

        ggl::vectorc<3> reflLight = _calcRayLight( {surfPos, surfReflLight},
            surface, surfPos, pCasts+1 );
        ggl::vectorc<3> refrLight = _calcRayLight( {surfPos, surfRefrLight},
            surface, surfPos, pCasts+1 );

        return surfAtten * ggl::util::tlerp( surfReflFactor, refrLight, reflLight );
    // TODO(JRC): Finish implementing the lighting properties for polished surfaces.
    // The equation given for these surfaces in the book seems only half constructed
    // and without any explanations given for the meanings of variables.  That being
    // said, I'm going to skip out on implementing this technique for now.
    } else {
        return ggl::color::black<3>();
    }
}

}
