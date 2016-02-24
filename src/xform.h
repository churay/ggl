#ifndef GGL_XFORM_H
#define GGL_XFORM_H

#include <tuple>
#include <utility>
#include "matrix.h"

namespace ggl {

namespace xform {
    template <class... Ts>
    auto scale( Ts&&... pValues );

    template <class... Ts>
    auto translate( Ts&&... pValues );
}

}

#include "xform.cpp"

#endif
