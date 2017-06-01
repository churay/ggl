#include <array>

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

/*
template <size_t N, size_t I>
struct basis_value {
    static constexpr ggl::real value = N == I ? ggl::one() : ggl::zero();
};

template <size_t I, size_t N, size_t D, size_t... Args>
struct basis_impl {
    static constexpr std::initializer_list<ggl::real> value = basis_impl<I-1, N, D, Args..., I>::value;
};

template <size_t N, size_t D, size_t... Args>
struct basis_impl<0, N, D, Args...> {
    static constexpr std::initializer_list<ggl::real> value{ basis_value<N, Args>::value... };
};

template <size_t D, size_t N>
constexpr ggl::vectorf<D> geom::basis<D, N>::value = ggl::vectorf<D>{{ basis_impl<D, N, D>::value }};
*/

}
