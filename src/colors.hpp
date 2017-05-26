#ifndef GGL_COLOR_H
#define GGL_COLOR_H

#include <GL/gl.h>

#include "matrix.hpp"
#include "consts.hpp"

namespace ggl {

template <size_t R> using vectorc = vector<GLfloat, R>;

namespace color {
    template <size_t R> ggl::vectorc<R> black( ggl::real a = 0.0f ) { return ggl::vectorc<R>(0.0f); };
    template <size_t R> ggl::vectorc<R> white( ggl::real a = 0.0f ) { return ggl::vectorc<R>(1.0f); };
    template <size_t R> ggl::vectorc<R> gray( ggl::real a = 0.0f )  { return ggl::vectorc<R>(0.6f); };

    template <size_t R> ggl::vectorc<R> red( GLfloat a = 0.0f )   { ggl::vectorc<R> c(0.0f); c[0] = 1.0f; return c; };
    template <size_t R> ggl::vectorc<R> green( GLfloat a = 0.0f ) { ggl::vectorc<R> c(0.0f); c[1] = 1.0f; return c; };
    template <size_t R> ggl::vectorc<R> blue( GLfloat a = 0.0f )  { ggl::vectorc<R> c(0.0f); c[2] = 1.0f; return c; };
}

}

#endif
