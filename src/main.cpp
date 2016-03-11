#include <array>
#include <iostream>

#include "matrix.h"
#include "geom.h"
#include "consts.hpp"

int main() {
    ggl::matrixf<3, 3> mat1{ 1.0f };

    ggl::matrixf<3, 3> mat2{
        0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f
    };

    ggl::matrixf<3, 3> mat3{ -1.0f };

    std::cout << mat1 << std::endl;
    std::cout << mat2 << std::endl;
    std::cout << mat3 << std::endl;

    ggl::matrixf<3, 3> mat = 2.0f * ( mat1 - mat3 ) * 4.0f;
    std::cout << mat << std::endl;

    ggl::vectorf<3> vec1{ 1.0f, 0.0f, 0.0f };
    ggl::vectorf<3> vec2{ 0.0f, 1.0f, 0.0f };

    ggl::vectorf<3> vec3 = vec1.cross( vec2 );
    std::cout << vec3 << std::endl;

    std::cout << vec1.tensor( vec2 ) << std::endl;

    std::cout << mat << " * " << vec3 << " = " << mat * vec3 << std::endl;

    std::array<ggl::vectorf<3>, 3> basisVecs = ggl::geom::basis( vec3 );
    std::cout << "Basis Vectors:" << std::endl;
    for( const auto& basisVec : basisVecs )
        std::cout << "  " << basisVec << std::endl;

    std::cout << ggl::geom::scale( 1.0f, 2.0f ) << std::endl;
    std::cout << ggl::geom::translate( 1.0f, 2.0f, 3.0f ) << std::endl;
    std::cout << ggl::geom::rotate( M_PI_R / 2.0f ) << std::endl;
    std::cout << ggl::geom::rotate( M_PI_R / 2.0f, vec3 ) << std::endl;

    std::cout << mat1.determinant() << std::endl;
    std::cout << mat2.determinant() << std::endl;
    std::cout << mat3.determinant() << std::endl;

    std::cout << ( mat1 | mat1 ) << std::endl;

    std::cout << mat2.inverse() << std::endl;
    std::cout << mat2 * mat2.inverse() << std::endl;

    std::cout << vec1.angleTo( vec2 ) << std::endl;
    std::cout << vec2.angleTo( vec1 ) << std::endl;
    std::cout << vec3.angleTo( vec1 ) << std::endl;
    std::cout << vec1.projectOnto( vec2 ) << std::endl;
}
