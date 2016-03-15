#ifndef GGL_INTERVAL_H
#define GGL_INTERVAL_H

#include <utility>

#include "consts.hpp"

namespace ggl {

class interval {
    public:
    
    /// Constructors ///

    interval();
    interval( const ggl::real& pMin, const ggl::real& pMax );

    /// Class Functions ///

    ggl::real clamp( const ggl::real& pValue ) const;
    ggl::real lerp( const ggl::real& pValue ) const;

    interval intersect( const interval& pOther ) const;
    bool contains( const ggl::real& pValue ) const;
    bool overlaps( const interval& pOther ) const;

    std::pair<ggl::real, ggl::real> bounds() const;
    bool empty() const;

    private:

    /// Class Fields ///

    ggl::real mMin, mMax;
};

}

#endif
