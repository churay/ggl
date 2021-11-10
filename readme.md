# `ggl`: Game Graphics Library #

A linear algebra and graphics library written in C++ for the C++1y standard, targeting
rendering concepts and scenes presented in ["Realistic Ray Tracing"][ggl-book] by
P. Shirley and R.K. Morley. This library was developed with the intent of leveraging
C++ templates to implement ND linear algebra structures in a manner that's both
code-efficient (via templating for both matrices and vectors) and memory-efficient
(via static allocations per matrix/vector). This repository also includes interactive
demos of many of the example scenes described in the source literature.

This library has been entirely supplanted by [`glm`][ggl-glm], a superior implementation
of the same concept. `ggl` thus exists in its current state largely for archival purposes.

## Demos ##

### Perspective Scene ###

![`ggl` perspective demo](https://github.com/churay/ggl/raw/master/doc/perspective_scene.gif)

### Materials Scene ###

![`ggl` materials demo](https://github.com/churay/ggl/raw/master/doc/material_scene.gif)

## Build/Test Instructions ##

To build and install on Ubuntu 16.04+, run the following commands:

```
# Install third-party dependencies.
$ sudo apt-get install make clang-3.8 libglfw3 libglfw3-dev

# Build and run the test cases (requires catch.hpp).
$ make all
$ ./bin/ggl.test

# Build and run a rendering example (see src/ex/ directory).
$ make basic_scene.ex
$ ./bin/basic_scene.ex
```

### Dependencies ###

- [Clang](https://clang.llvm.org/)
- [GNU Make](https://www.gnu.org/software/make/)
- [GLFW3](https://www.glfw.org/)
- [(Optional) catch.hpp](https://github.com/catchorg/Catch2)

## License ##

This project is licensed under [the GPL v3 License][ggl-license].


[ggl-book]: https://www.goodreads.com/book/show/3258612-realistic-ray-tracing
[ggl-glm]: http://glm.g-truc.net/
[ggl-license]: https://raw.githubusercontent.com/churay/ggl/master/liscense.txt
