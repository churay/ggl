#include <iostream>
#include "matrix.h"
#include "xform.h"

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

    ggl::vectorf<3> vec1{ 1.0f, 0.0f, 0.0f };
    ggl::vectorf<3> vec2{ 0.0f, 1.0f, 0.0f };

    ggl::vectorf<3> vec = vec1.cross( vec2 );
    std::cout << vec << std::endl;

    std::cout << mat << " * " << vec << " = " << mat * vec << std::endl;

    std::cout << ggl::xform::scale( 1.0f, 1.0f ) << std::endl;
}
