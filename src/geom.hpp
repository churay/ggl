#ifndef GGL_GEOM_H
#define GGL_GEOM_H

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

namespace geom {
    template <size_t D>
    struct ray {
        ray( const ggl::vectorf<D>& pOrigin, const ggl::vectorf<D>& pVector );
        ggl::vectorf<D> at( const ggl::real& pParam ) const;

        ggl::vectorf<D> mOrigin, mVector;
    };

    struct plane {
        plane( const ggl::vectorf<3>& pOrigin, const ggl::vectorf<3>& pNormal );
        ggl::real intersect( const ggl::geom::ray<3>& pRay ) const;

        ggl::vectorf<3> mOrigin, mNormal;
    };

    struct sphere {
        sphere( const ggl::vectorf<3>& pOrigin, const ggl::real& pRadius );
        ggl::real intersect( const ggl::geom::ray<3>& pRay );

        ggl::vectorf<3> mOrigin, mRadius;
    };
}

}

#include "geom.cpp"

#endif
