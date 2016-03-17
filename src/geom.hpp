#ifndef GGL_GEOM_H
#define GGL_GEOM_H

#include "matrix.hpp"

namespace ggl {

namespace geom {
    template <size_t D>
    struct ray {
        ray( const ggl::vectorf<D>& pOrigin, const ggl::vectorf<D>& pVector );
        ggl::vectorf<D> at( const ggl::real& pParam ) const;

        ggl::vectorf<D> mOrigin, mVector;
    };

}

}

#include "geom.cpp"

#endif
