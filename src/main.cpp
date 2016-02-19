#include <iostream>
#include "matrix.h"

int main() {
    ggl::matrix<float, 3, 3> mat1{{{
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    }}};

    ggl::matrix<float, 3, 3> mat2{{{
        0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f
    }}};

    ggl::matrix<float, 3, 3> mat3{{{
        -1.0f, +0.0f, +0.0f,
        +0.0f, -1.0f, +0.0f,
        +0.0f, +0.0f, -1.0f
    }}};

    // ggl::vectorf<3> vec1{ 1.0f, 1.0f, 1.0f };

    ggl::matrix<float, 3, 3> mat = 2.0f * ( mat1 - mat3 ) * 4.0f;
    // ggl::matrix<float, 3, 3> mat = mat1 + mat2;

    std::cout << "Matrix Contents:" << std::endl;
    for( unsigned rowIdx = 0; rowIdx < 3; ++rowIdx ) {
        std::cout << "[ ";
        for( unsigned colIdx = 0; colIdx < 3; ++colIdx )
            std::cout << mat( rowIdx, colIdx ) << " ";
        std::cout << "]" << std::endl;;
    }
}
