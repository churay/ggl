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

    template <class T>
    struct surface {
        surface( const T& pEvalFxn );
        ggl::real intersect( const ggl::geom::ray<3>& pRay ) const;

        T mEvalFxn;
    };

    auto plane( const ggl::vectorf<3>& pOrigin, const ggl::vectorf<3>& pNormal );
    auto sphere( const ggl::vectorf<3>& pOrigin, const ggl::real& pRadius );
    // TODO(JRC): Implement the following functions:
    // surface box( const ggl::vectorf<3>& pMin, const ggl::vectorf<3>& pMax );
    // surface triangle( const std::array<ggl::vectorf<3>, 3>& pVertices );
}

}

#include "geom.cpp"

#endif
