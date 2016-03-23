#ifndef GGL_GEOM_H
#define GGL_GEOM_H

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

namespace geom {
    template <size_t D>
    struct ray {
        ggl::vectorf<D> at( const ggl::real& pParam ) const;
        ggl::vectorf<D> mOrigin, mVector;
    };

    struct plane {
        ggl::real intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mOrigin, mNormal;
    };

    struct sphere {
        ggl::real intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mOrigin; ggl::real mRadius;
    };

    struct box {
        ggl::real intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mMin, mMax;
    };

    struct triangle {
        ggl::real intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mV1, mV2, mV3;
    };
}

}

#include "geom.cpp"

#endif
