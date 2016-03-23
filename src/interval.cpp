#include <algorithm>
#include <utility>

#include "interval.h"

namespace ggl {

interval::interval() {
    mMin = mMax = static_cast<ggl::real>( 0 );
}


interval::interval( const ggl::real& pMin, const ggl::real& pMax ) {
    mMin = std::min( pMin, pMax );
    mMax = std::max( pMin, pMax );
}


ggl::real interval::clamp( const ggl::real& pValue ) const {
    return std::max( mMin, std::min(mMax, pValue) );
}


ggl::real interval::lerp( const ggl::real& pValue ) const {
    return (*this).clamp( mMin + pValue * (mMax - mMin) );
}


bool interval::contains( const ggl::real& pValue ) const {
    return pValue >= mMin && pValue <= mMax;
}


bool interval::overlaps( const interval& pOther ) const {
    interval overlapInterval = (*this).intersect( pOther );
    return !overlapInterval.empty();
}


interval interval::intersect( const interval& pOther ) const {
    if( !(*this).contains(pOther.mMin) && !(*this).contains(pOther.mMax) &&
            !pOther.contains(mMin) && !pOther.contains(mMax) ) {
        return interval();
    } else {
        return interval( std::max(mMin, pOther.mMin), std::min(mMax, pOther.mMax) );
    }
}


std::pair<ggl::real, ggl::real> interval::bounds() const {
    return std::make_pair( mMin, mMax );
}


bool interval::empty() const {
    return mMin == mMax;
}


const ggl::real& interval::min() const {
    return mMin;
}


const ggl::real& interval::max() const {
    return mMax;
}

}
