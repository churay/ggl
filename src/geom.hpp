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

    struct surface {
        virtual ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const = 0;
    };

    struct plane : public surface {
        plane( ggl::vectorf<3> pOrigin, ggl::vectorf<3> pNormal );
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mOrigin, mNormal;
    };

    struct sphere : public surface {
        sphere( ggl::vectorf<3> pOrigin, ggl::real pRadius );
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mOrigin; ggl::real mRadius;
    };

    struct box : public surface {
        box( ggl::vectorf<3> pMin, ggl::vectorf<3> pMax );
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mMin, mMax;
    };

    struct triangle : public surface {
        triangle( ggl::vectorf<3> pV0, ggl::vectorf<3> pV1, ggl::vectorf<3> pV2 );
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> mV0, mV1, mV2;
    };
}

}

#include "geom.cpp"

#endif
