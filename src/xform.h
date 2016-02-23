#ifndef GGL_XFORM_H
#define GGL_XFORM_H

#include <utility>
#include "matrix.h"

namespace ggl {

namespace xform {
    int test();

    /*
    template <class... Ts>
    matrix<Ts, sizeof...(Ts), sizeof...(Ts)> scale( Ts&&... pValues );
    */
}

}

#include "xform.cpp"

#endif
