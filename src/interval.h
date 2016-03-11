#ifndef GGL_INTERVAL_H
#define GGL_INTERVAL_H

#include "consts.hpp"

namespace ggl {

class interval {
    public:
    
    /// Constructors ///

    interval();
    interval( const ggl::real& pMin, const ggl::real& pMax );

    /// Class Functions ///

    ggl::real clamp( const ggl::real& pValue );
    ggl::real interpolate( const ggl::real& pValue );
    interval intersect( const interval& pOther );

    bool contains( const ggl::real& pValue );
    bool overlaps( const interval& pOther );

    bool empty();

    private:

    /// Class Fields ///

    ggl::real mMin, mMax;
};

}

#endif
