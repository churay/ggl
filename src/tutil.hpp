#ifndef GGL_TUTIL_H
#define GGL_TUTIL_H

#include "consts.hpp"

namespace ggl {

namespace util {
    template <class T>
    struct fless {
        bool operator()( const T& pV1, const T& pV2 ) const;
    };

    template <class T>
    T tlerp( const ggl::real& pVal, const T& pMin, const T& pMax );
}

}

#include "tutil.cpp"

#endif
