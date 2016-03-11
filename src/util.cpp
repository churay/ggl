#include <algorithm>
#include <cmath>
#include <vector>

#include "util.h"

namespace ggl {

constexpr unsigned util::factorial( const unsigned n ) {
    return ( n != 0 ) ? n * factorial( n - 1 ) : 1;
}


std::vector<std::vector<unsigned>> util::permutations( const unsigned n ) {
    std::vector<std::vector<unsigned>> permutes( factorial(n), std::vector<unsigned>(n, 0) );
    if( n <= 1 ) return permutes;

    const std::vector<std::vector<unsigned>> prevPermutes = permutations( n-1 );
    for( size_t permIdx = 0; permIdx < factorial(n-1); ++permIdx ) {
        const auto& prevPermute = prevPermutes[permIdx];
        for( size_t insIdx = 0; insIdx < n; ++insIdx ) {
            auto& currPermute = permutes[n * permIdx + insIdx];
            for( size_t iterIdx = 0; iterIdx < n; ++iterIdx ) {
                auto& currValue = currPermute[iterIdx];
                if( iterIdx < insIdx ) { currValue = prevPermute[iterIdx]; }
                else if( iterIdx > insIdx ) { currValue = prevPermute[iterIdx-1]; }
                else { currValue = n-1; }
            }
        }
    }

    return permutes;
}


namespace {
unsigned _merge( std::vector<unsigned>& permute,
        const std::vector<unsigned>& left, const std::vector<unsigned>& right ) {
    unsigned numInvs = 0;

    auto leftIter = left.begin(), rightIter = right.begin();
    auto permuteIter = permute.begin();
    while( leftIter != left.end() && rightIter != right.end() ) {
        if( *leftIter <= *rightIter ) {
            *(permuteIter++) = *(leftIter++);
        } else {
            *(permuteIter++) = *(rightIter++);
            numInvs += left.end() - leftIter;
        }
    }

    std::copy( leftIter, left.end(), permuteIter );
    std::copy( rightIter, right.end(), permuteIter );

    return numInvs;
}
unsigned _inversions( std::vector<unsigned>& permute ) {
    if( permute.size() <= 1 ) {
        return 0;
    } else {
        auto permuteMidIter = permute.begin() + ((permute.end() - permute.begin()) / 2);
        std::vector<unsigned> permuteLeft{ permute.begin(), permuteMidIter };
        std::vector<unsigned> permuteRight{ permuteMidIter, permute.end() };

        return _inversions( permuteLeft ) + _inversions( permuteRight ) +
            _merge( permute, permuteLeft, permuteRight );
    }
}
}
unsigned util::inversions( const std::vector<unsigned>& permute ) {
    std::vector<unsigned> permuteCopy( permute );
    return _inversions( permuteCopy );
}

}
