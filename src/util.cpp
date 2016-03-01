#include <vector>

namespace ggl {

constexpr unsigned util::factorial( const unsigned n ) {
    return ( n != 0 ) ? n * factorial( n - 1 ) : 1;
}


std::vector<std::vector<unsigned>> util::permutations( const unsigned n ) {
    std::vector<std::vector<unsigned>> permutes( factorial(n), std::vector<unsigned>(n) );

    if( n <= 1 ) {
        permutes[0][0] = 1;
        return permutes;
    }

    const std::vector<std::vector<unsigned>> prevPermutes = permutations( n-1 );
    for( size_t permIdx = 0; permIdx < factorial(n-1); ++permIdx ) {
        const auto& prevPermute = prevPermutes[permIdx];
        for( size_t insIdx = 0; insIdx < n; ++insIdx ) {
            auto& currPermute = permutes[n * permIdx + insIdx];
            for( size_t iterIdx = 0; iterIdx < n; ++iterIdx ) {
                auto& currValue = currPermute[iterIdx];
                if( iterIdx < insIdx ) { currValue = prevPermute[iterIdx]; }
                else if( iterIdx > insIdx ) { currValue = prevPermute[iterIdx-1]; }
                else { currValue = n; }
            }
        }
    }

    return permutes;
}


unsigned util::inversions( const std::vector<unsigned>& permutation ) {
    return 0;
}

}
