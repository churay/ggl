#ifndef GGL_GEOM_H
#define GGL_GEOM_H

#include <vector>

#include "matrix.hpp"
#include "interval.h"
#include "consts.hpp"

namespace ggl {

namespace geom {
    /// Namespace Types ///

    template <size_t D>
    struct ray {
        ggl::vectorf<D> at( const ggl::real& pParam ) const;
        ggl::vectorf<D> mOrigin, mVector;
    };

    struct surface {
        public:
        bool contains( const ggl::vectorf<3>& pPos ) const;
        ggl::interval intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> normalAt( const ggl::vectorf<3>& pPos ) const;

        private:
        virtual ggl::interval _intersect( const ggl::geom::ray<3>& pRay ) const = 0;
        virtual ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const = 0;
    };

    struct plane : public surface {
        plane( ggl::vectorf<3> pOrigin, ggl::vectorf<3> pNormal );
        ggl::vectorf<3> mOrigin, mNormal;

        private:
        ggl::interval _intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const;
    };

    struct sphere : public surface {
        sphere( ggl::vectorf<3> pOrigin, ggl::real pRadius );
        ggl::vectorf<3> mOrigin; ggl::real mRadius;

        private:
        ggl::interval _intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const;
    };

    struct box : public surface {
        box( ggl::vectorf<3> pMin, ggl::vectorf<3> pMax );
        ggl::vectorf<3> mMin, mMax;

        private:
        ggl::interval _intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const;
    };

    struct triangle : public surface {
        triangle( ggl::vectorf<3> pV0, ggl::vectorf<3> pV1, ggl::vectorf<3> pV2 );
        ggl::vectorf<3> mV0, mV1, mV2;

        private:
        ggl::interval _intersect( const ggl::geom::ray<3>& pRay ) const;
        ggl::vectorf<3> _normalAt( const ggl::vectorf<3>& pPos ) const;
    };

    /// Namespace Functions ///

    ggl::geom::surface* findClosest( const ggl::geom::ray<3>& pRay,
        const std::vector<ggl::geom::surface*>& pSurfaces );
}

}

#include "geom.cpp"

#endif
