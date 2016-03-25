#include <array>
#include <cmath>
#include <tuple>
#include <utility>

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

template <class T, class LT>
auto xform::basis( const vector<T, 3, LT>& pWVector ) {
    const vector<T, 3, LT> xVector{ ggl::one<T>(), ggl::zero<T>(), ggl::zero<T>() };
    const vector<T, 3, LT> yVector{ ggl::zero<T>(), ggl::one<T>(), ggl::zero<T>() };

    const vector<T, 3, LT> vVector = ( pWVector != yVector ) ? yVector : xVector;

    std::array<vector<T, 3, LT>, 3> result;
    result[2] = pWVector.normalize();
    result[0] = vVector.cross( pWVector ).normalize();
    result[1] = result[2].cross( result[0] ).normalize();

    return result;
}


// TODO(JRC): Improve the implementation of the following two functions so
// that there isn't as much duplication.
template <class... Ts>
auto xform::scale( Ts&&... pValues ) {
    using T = typename std::tuple_element<0, std::tuple<Ts...>>::type;
    constexpr unsigned D = sizeof...( Ts );
    const std::array<T, D> pValueArray{{ std::forward<Ts>(pValues)... }};

    ggl::matrix<T, D+1, D+1> result{ T(1) };
    for( unsigned eIdx = 0; eIdx < D; ++eIdx )
        result(eIdx, eIdx) = pValueArray[eIdx];

    return result;
}


template <class... Ts>
auto xform::translate( Ts&&... pValues ) {
    using T = typename std::tuple_element<0, std::tuple<Ts...>>::type;
    constexpr unsigned D = sizeof...( Ts );
    const std::array<T, D> pValueArray{{ std::forward<Ts>(pValues)... }};

    ggl::matrix<T, D+1, D+1> result{ T(1) };
    for( unsigned eIdx = 0; eIdx < D; ++eIdx )
        result(eIdx, D) = pValueArray[eIdx];

    return result;
}


template <class T>
auto xform::rotate( const T& pRadians ) {
    ggl::matrix<T, 3, 3> result{ 
        +std::cos( pRadians ), -std::sin( pRadians ), ggl::zero<T>(),
        +std::sin( pRadians ), +std::cos( pRadians ), ggl::zero<T>(),
        ggl::zero<T>(), ggl::zero<T>(), ggl::one<T>()
    };

    return result;
}


template <class T, class LT>
auto xform::rotate( const T& pRadians, const vector<T, 3, LT>& pAxis ) {
    // NOTE(JRC): This succinct 3D rotation formula was taken from here:
    // https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
    auto cosMatrix = std::cos( pRadians ) * ggl::matrix<T, 3, 3, LT>( ggl::one<T>() );
    auto sinMatrix = std::sin( pRadians ) * ggl::matrix<T, 3, 3, LT>{
        ggl::zero<T>(), -pAxis[2], +pAxis[1],
        +pAxis[2], ggl::zero<T>(), -pAxis[0],
        -pAxis[1], +pAxis[0], ggl::zero<T>()
    };
    auto icosMatrix = ( ggl::one<T>() - std::cos(pRadians) ) * pAxis.tensor( pAxis );

    ggl::matrix<T, 4, 4, LT> result{ ggl::one<T>() };
    result.template embed<0, 0>( cosMatrix + sinMatrix + icosMatrix );
    return result;
}


template <class T, class LT>
auto xform::rotate( const std::array<vector<T, 3, LT>, 3>& pAxes ) {
    ggl::matrix<T, 4, 4, LT> result{ static_cast<T>(1) };

    result.template embed<0, 0>( pAxes[0] );
    result.template embed<0, 1>( pAxes[1] );
    result.template embed<0, 2>( pAxes[2] );

    return result.transpose();
}

}
