#ifndef GGL_TUTIL_H
#define GGL_TUTIL_H

namespace ggl {

namespace util {
    template <class T>
    struct fless {
        bool operator()( const T& pV1, const T& pV2 ) const;
    };

    constexpr unsigned factorial( const unsigned& pVal );
}

}

#include "tutil.cpp"

#endif
