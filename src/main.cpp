#include <iostream>
#include "matrix.h"

int main() {
    ggl::matrix<3, 3> mat;

    for( unsigned diagIdx = 0; diagIdx < 3; ++diagIdx )
        mat( diagIdx, diagIdx ) = 1.0f;

    std::cout << "Matrix Contents:" << std::endl;
    for( unsigned rowIdx = 0; rowIdx < 3; ++rowIdx ) {
        std::cout << "[ ";
        for( unsigned colIdx = 0; colIdx < 3; ++colIdx )
            std::cout << mat( rowIdx, colIdx ) << " ";
        std::cout << "]" << std::endl;;
    }
}
