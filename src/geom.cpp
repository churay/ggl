#include "matrix.hpp"

namespace ggl {

/// Definitions for `ggl::ray<size_t>` ///

template <size_t D>
geom::ray<D>::ray( const ggl::vectorf<D>& pOrigin, const ggl::vectorf<D>& pVector ) {
    mOrigin = pOrigin;
    mVector = pVector;
}


template <size_t D>
ggl::vectorf<D> geom::ray<D>::at( const ggl::real& pParam ) const {
    return mOrigin + pParam * mVector;
}

/// Function Definitions ///

}
