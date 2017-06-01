#ifndef GGL_CAMERA_H
#define GGL_CAMERA_H

#include "geom.h"
#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

// TODO(JRC): update over a period of time; give pathing (future)
// at start, just initialize the view and then return the view transform

class camera {
    public:

    /// Constructors ///

    camera( const ggl::vectorf<3>& pOrigin, const ggl::vectorf<3>& pDirection,
        const ggl::real& pViewWidth = 4 * ggl::one(),
        const ggl::real& pViewHeight = 4 * ggl::one(),
        const ggl::real& pViewDepth = -2 * ggl::one() );

    ggl::geom::ray calcViewRay( const ggl::real& pI, const ggl::real& pJ ) const;

    private:

    /// Private Functions ///

    // TODO

    /// Class Fields ///

    ggl::vectorf<3> mOrigin, mDirection;
    ggl::real mViewWidth, mViewHeight, mViewDepth;
};

}

#endif
