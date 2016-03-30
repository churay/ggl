#ifndef GGL_GEOM_H
#define GGL_GEOM_H

#include "matrix.hpp"
#include "interval.h"
#include "consts.hpp"

namespace ggl {

namespace geom {
    template <size_t D>
    struct ray {
        ggl::vectorf<D> at( const ggl::real& pParam ) const;
        ggl::vectorf<D> mOrigin, mVector;
    };

    template <class T>
    struct surface {
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        T mSurface;
    };

    struct plane {
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mOrigin, mNormal;
    };

    struct sphere {
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mOrigin; ggl::real mRadius;
    };

    struct box {
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mMin, mMax;
    };

    struct triangle {
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mV0, mV1, mV2;
    };
}

}

#include "geom.cpp"

#endif
