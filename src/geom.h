#ifndef GGL_GEOM_H
#define GGL_GEOM_H

#include <vector>

#include "matrix.hpp"
#include "interval.h"
#include "consts.hpp"

namespace ggl {

namespace geom {
    /// Namespace Types ///

    struct ray {
        ggl::vectorf<3> at( const ggl::real& pParam ) const;
        ggl::vectorf<3> mOrigin, mVector;
    };

    struct surface {
        bool contains( const ggl::vectorf<3>& pPos ) const;
        ggl::interval intersect( const ggl::geom::ray& pRay ) const;
        ggl::vectorf<3> normalAt( const ggl::vectorf<3>& pPos ) const;

        private:
        virtual ggl::vectorf<3> _center() const = 0;
        virtual ggl::interval _intersect( const ggl::geom::ray& pRay ) const = 0;
        virtual ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const = 0;
    };

    struct plane : public surface {
        plane( ggl::vectorf<3> pOrigin, ggl::vectorf<3> pNormal );
        ggl::vectorf<3> mOrigin, mNormal;

        private:
        ggl::vectorf<3> _center() const;
        ggl::interval _intersect( const ggl::geom::ray& pRay ) const;
        ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const;
    };

    struct sphere : public surface {
        sphere( ggl::vectorf<3> pOrigin, ggl::real pRadius );
        ggl::vectorf<3> mOrigin; ggl::real mRadius;

        private:
        ggl::vectorf<3> _center() const;
        ggl::interval _intersect( const ggl::geom::ray& pRay ) const;
        ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const;
    };

    struct box : public surface {
        box( ggl::vectorf<3> pMin, ggl::vectorf<3> pMax );
        ggl::vectorf<3> mMin, mMax;

        private:
        ggl::vectorf<3> _center() const;
        ggl::interval _intersect( const ggl::geom::ray& pRay ) const;
        ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const;
    };

    struct triangle : public surface {
        triangle( ggl::vectorf<3> pV0, ggl::vectorf<3> pV1, ggl::vectorf<3> pV2 );
        ggl::vectorf<3> mV0, mV1, mV2;

        private:
        ggl::vectorf<3> _center() const;
        ggl::interval _intersect( const ggl::geom::ray& pRay ) const;
        ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const;
    };

    /// Namespace Functions ///

    std::array<ggl::vectorf<3>, 6> basis();
    ggl::geom::surface* findClosest( const ggl::geom::ray& pRay,
        const std::vector<ggl::geom::surface*>& pSurfaces );
}

}

#endif
