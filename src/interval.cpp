#include <algorithm>

#include "interval.h"

namespace ggl {

interval::interval() {
    mMin = mMax = static_cast<ggl::real>( 0 );
}


interval::interval( const ggl::real& pMin, const ggl::real& pMax ) {
    mMin = pMin;
    mMax = std::max( pMin, pMax );
}


ggl::real interval::clamp( const ggl::real& pValue ) {
    return std::max( mMin, std::min(mMax, pValue) );
}


ggl::real interval::interpolate( const ggl::real& pValue ) {
    return (*this).clamp( mMin + pValue * (mMax - mMin) );
}


interval interval::intersect( const interval& pOther ) {
    if( !(*this).contains(pOther.mMin) && !(*this).contains(pOther.mMax) ) {
        return interval();
    } else {
        return interval(
            std::max( mMin, pOther.mMin ),
            std::min( mMax, pOther.mMax )
        );
    }
}


bool interval::contains( const ggl::real& pValue ) {
    return pValue >= mMin && pValue <= mMax;
}


bool interval::overlaps( const interval& pOther ) {
    interval overlapInterval = (*this).intersect( pOther );
    return !overlapInterval.empty();
}


bool interval::empty() {
    return mMin == mMax;
}

}
