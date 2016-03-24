#ifndef GGL_CONST_H
#define GGL_CONST_H

#ifndef M_PI_R
#define M_PI_R (static_cast<ggl::real>(M_PI))
#endif

namespace ggl {

    using real = float;

    constexpr real gZero{ static_cast<ggl::real>(0) };
    constexpr real gOne{ static_cast<ggl::real>(1) };

}

#endif
