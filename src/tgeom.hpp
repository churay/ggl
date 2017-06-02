#ifndef GGL_TGEOM_H
#define GGL_TGEOM_H

#include <array>

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

namespace geom {
    template <size_t D, size_t N>
    struct axis {
        static_assert( N != 0, "'ggl::geom::axis' must have positive index." );
        static_assert( N <= D, "'ggl::geom::axis' must have index less than dimension." );

        static const ggl::vectorf<D> value;
    };

    template <size_t D>
    struct basis {
        static_assert( D != 0, "'ggl::geom::basis' must have positive dimension." );

        static const std::array<ggl::vectorf<D>, D> value;
    };
}

}

#include "tgeom.cpp"

#endif
