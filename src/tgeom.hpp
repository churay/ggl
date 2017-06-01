#ifndef GGL_TGEOM_H
#define GGL_TGEOM_H

#include <array>

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

namespace geom {
    template <size_t N, size_t I>
    struct basis_value {
        static constexpr ggl::real value = N == I ? ggl::one() : ggl::zero();
    };

    template <size_t I, size_t N, size_t D, size_t... Args>
    struct basis_impl {
        static constexpr std::array<ggl::real, D> value = basis_impl<I-1, N, D, Args..., I>::value;
    };

    template <size_t N, size_t D, size_t... Args>
    struct basis_impl<0, N, D, Args...> {
        static constexpr std::array<ggl::real, D> value{{ basis_value<N, Args>::value... }};
    };

    template <size_t I, size_t N, size_t D, size_t... Args>
    constexpr std::array<ggl::real, D> basis_impl<I, N, D, Args...>::value;

    template <size_t D, size_t N>
    struct basis {
        static_assert( N != 0, "'ggl::geom::basis' must have positive index." );
        static_assert( N <= D, "'ggl::geom::basis' must have index less than dimension." );

        static const ggl::vectorf<D> value;
    };

    template <size_t D, size_t N>
    const ggl::vectorf<D> geom::basis<D, N>::value = ggl::vectorf<D>( basis_impl<D, N, D>::value );
}

}

#include "tgeom.cpp"

#endif
