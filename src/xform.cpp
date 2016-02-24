#include <cmath>
#include <tuple>
#include <utility>
#include "matrix.h"

namespace ggl {

template <class... Ts>
auto xform::scale( Ts&&... pValues ) {
    using T = typename std::tuple_element<0, std::tuple<Ts...>>::type;
    constexpr unsigned D = sizeof...( Ts );

    ggl::matrix<T, D, D> result;
    return result;
}

}
