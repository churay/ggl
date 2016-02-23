#ifndef GGL_VECTOR_H
#define GGL_VECTOR_H

#include <utility>
#include "matrix.h"

namespace ggl {

template <class T, unsigned E>
class vector : public matrix<T, E, 1> {
    public:

    /// Class Attributes ///

    using EntryType = T;
    constexpr static unsigned sNumEnts{ E };

    /// Constructors ///

    vector();
    template <class... Ts>
    explicit vector( Ts&&... pEntries );

    /// Class Functions ///

    EntryType dot( const vector& pOther ) const;
    vector<T, E> cross( const vector& pOther ) const;
};

template <unsigned E> using vectorf = vector<float, E>;

}

#include "vector.cpp"

#endif
