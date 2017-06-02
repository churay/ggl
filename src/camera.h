#ifndef GGL_CAMERA_H
#define GGL_CAMERA_H

#include "geom.h"
#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

// TODO(JRC): Update the implementation here so that the camera isn't always
// pointed toward the model space origin.

class camera {
    public:

    /// Constructors ///

    camera();

    void tilt( const bool& pLR, const bool& pDir );
    void zoom( const bool& pIn );

    ggl::geom::ray calcViewRay( const ggl::real& pI, const ggl::real& pJ );
    const std::array<ggl::vectorf<3>, 3>& calcViewBasis();

    private:

    /// Private Functions ///

    void _calcViewBasis();

    /// Class Fields ///

    ggl::real mViewAngleH, mViewAngleV, mViewRadius;
    ggl::real mViewWidth, mViewHeight, mViewDepth;

    std::array<ggl::vectorf<3>, 3> mViewBasis;
    ggl::vectorf<3> mViewPos;
    bool mUpdateBasis;
};

}

#endif
