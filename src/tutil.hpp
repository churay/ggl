#ifndef GGL_TUTIL_H
#define GGL_TUTIL_H

namespace ggl {

namespace util {
    template <class T>
    struct fless {
        bool operator()( const T& pValue1, const T& pValue2 ) const;
    };
}

}

#include "tutil.cpp"

#endif
