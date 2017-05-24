#ifndef GGL_COLOR_H
#define GGL_COLOR_H

#include <GL/gl.h>

namespace ggl {

namespace color {
    template <class T=GLuint> constexpr T black() { return static_cast<T>(0xff000000); };
    template <class T=GLuint> constexpr T white() { return static_cast<T>(0xffffffff); };
    template <class T=GLuint> constexpr T gray()  { return static_cast<T>(0xffaaaaaa); };

    template <class T=GLuint> constexpr T red()   { return static_cast<T>(0xff0000ff); };
    template <class T=GLuint> constexpr T green() { return static_cast<T>(0xff00ff00); };
    template <class T=GLuint> constexpr T blue()  { return static_cast<T>(0xffff0000); };
}

}

#endif
