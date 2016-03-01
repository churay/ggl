#include <iostream>
#include "util.h"
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

    std::cout << ggl::xform::scale( 1.0f, 2.0f ) << std::endl;
    std::cout << ggl::xform::translate( 1.0f, 2.0f, 3.0f ) << std::endl;

    // std::cout << mat1.determinant() << std::endl;

    std::cout << ggl::util::factorial( 3 ) << std::endl;
    std::cout << ggl::util::factorial( 5 ) << std::endl;

    const auto& permutations = ggl::util::permutations( 3 );
    std::cout << "Permutations: [ ";
    for( const auto& permutation : permutations ) {
        std::cout << "[ ";
        for( const auto& permutationValue : permutation )
            std::cout << permutationValue << ", ";
        std::cout << "], ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Inversions in Permutation #3: " <<
        ggl::util::inversions( permutations[2] ) << std::endl;
}
