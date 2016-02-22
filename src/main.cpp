#include <iostream>
#include "matrix.h"

int main() {
    ggl::matrixf<3, 3> mat1{
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    ggl::matrixf<3, 3> mat2{
        0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f
    };

    ggl::matrixf<3, 3> mat3{
        -1.0f, +0.0f, +0.0f,
        +0.0f, -1.0f, +0.0f,
        +0.0f, +0.0f, -1.0f
    };

    ggl::matrixf<3, 3> mat = 2.0f * ( mat1 - mat3 ) * 4.0f;
    std::cout << mat << std::endl;
}
