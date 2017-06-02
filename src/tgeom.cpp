#include <array>

#include "matrix.hpp"
#include "consts.hpp"

namespace {

template <size_t N, size_t I>
struct axis_value {
    static constexpr ggl::real value =
        N == I ? ggl::one() : ggl::zero();
};


template <size_t I, size_t N, size_t D, size_t... Args>
struct axis_impl {
    static constexpr std::array<ggl::real, D> value =
        axis_impl<I-1, N, D, I, Args...>::value;
};


template <size_t N, size_t D, size_t... Args>
struct axis_impl<0, N, D, Args...> {
    static constexpr std::array<ggl::real, D> value{{ axis_value<N, Args>::value... }};
};


template <size_t I, size_t N, size_t D, size_t... Args>
constexpr std::array<ggl::real, D> axis_impl<I, N, D, Args...>::value;


template <size_t I, size_t D, size_t... Args>
struct basis_impl {
    static const std::array<ggl::vectorf<D>, D> value;
};


template <size_t I, size_t D, size_t... Args>
const std::array<ggl::vectorf<D>, D> basis_impl<I, D, Args...>::value =
    basis_impl<I-1, D, I, Args...>::value;


template <size_t D, size_t... Args>
struct basis_impl<0, D, Args...> {
    static const std::array<ggl::vectorf<D>, D> value;
};


template <size_t D, size_t... Args>
const std::array<ggl::vectorf<D>, D> basis_impl<0, D, Args...>::value =
    std::array<ggl::vectorf<D>, D>{{ ggl::geom::axis<D, Args>::value... }};

}

namespace ggl {

template <size_t D, size_t N>
const ggl::vectorf<D> geom::axis<D, N>::value =
    ggl::vectorf<D>( axis_impl<D, N, D>::value );


template <size_t D>
const std::array<ggl::vectorf<D>, D> geom::basis<D>::value =
    basis_impl<D, D>::value;

}
