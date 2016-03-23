#include <cmath>

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

/// Definitions for `ggl::geom::ray<size_t>` ///

template <size_t D>
geom::ray<D>::ray( const ggl::vectorf<D>& pOrigin, const ggl::vectorf<D>& pVector ) {
    mOrigin = pOrigin;
    mVector = pVector;
}


template <size_t D>
ggl::vectorf<D> geom::ray<D>::at( const ggl::real& pParam ) const {
    return mOrigin + pParam * mVector;
}

/// Definitions for `ggl::geom::surface<T>` ///

template <class T>
geom::surface<T>::surface( const T& pEvalFxn ) {
    mEvalFxn = pEvalFxn;
}


template <class T>
ggl::real geom::surface<T>::intersect( const ggl::geom::ray<3>& pRay ) const {
    return mEvalFxn( pRay );
}

/// Definitions for `ggl::geom::surface<T>` Functions ///

auto geom::plane( const ggl::vectorf<3>& pOrigin, const ggl::vectorf<3>& pNormal ) {
    auto planeFxn = [=] ( const ggl::geom::ray<3>& pRay ) {
        return pNormal.dot( pOrigin - pRay.mOrigin ) / pNormal.dot( pRay.mVector );
    };

    return surface<decltype(planeFxn)>( planeFxn );
}


auto geom::sphere( const ggl::vectorf<3>& pOrigin, const ggl::real& pRadius ) {
    auto sphereFxn = [=] ( const ggl::geom::ray<3>& pRay ) {
        const ggl::real quadA = pRay.mVector.dot( pRay.mVector );
        const ggl::real quadB = pRay.mVector.dot( pRay.mOrigin - pOrigin );
        const ggl::real quadC = std::pow((pRay.mOrigin - pOrigin).normal(), 2) - std::pow( pRadius, 2 );

        return ( -quadB + std::sqrt(std::pow(quadB, 2) - 4 * quadA * quadC) ) / ( 2 * quadC );
    };

    return surface<decltype(sphereFxn)>( sphereFxn );
}

}
