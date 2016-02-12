#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE( "C++ Integer Equality Works", "Proves that C++ can compare integer values." ) {
    REQUIRE( 1 == 1 );
}
