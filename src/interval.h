#ifndef GGL_INTERVAL_H
#define GGL_INTERVAL_H

#include <utility>

#include "consts.hpp"

namespace ggl {

class interval {
    public:
    
    /// Constructors ///

    interval( const ggl::real pCenter = ggl::zero() );
    interval( const ggl::real& pMin, const ggl::real& pMax );

    /// Class Functions ///

    ggl::real clamp( const ggl::real& pValue ) const;
    ggl::real lerp( const ggl::real& pValue ) const;

    bool contains( const ggl::real& pValue ) const;
    bool overlaps( const interval& pOther ) const;
    interval intersect( const interval& pOther ) const;

    std::pair<ggl::real, ggl::real> bounds() const;
    bool empty() const;
    bool valid() const;

    const ggl::real& min() const;
    const ggl::real& max() const;

    private:

    /// Class Fields ///

    ggl::real mMin, mMax;
};

}

#endif
