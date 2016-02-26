#include <array>
#include <utility>

namespace ggl {

constexpr unsigned util::factorial( const unsigned n ) {
    return ( n != 0 ) ? n * factorial( n - 1 ) : 1;
}


template <unsigned N>
auto util::permutations() {
    std::array<std::array<unsigned, N>, factorial(N)> perms{};

    const auto prevPermutations = permutations<N-1>();
    for( unsigned permIdx = 0; permIdx < factorial(N-1); ++permIdx ) {
        const auto& prevPermutation = prevPermutations[permIdx];
        for( unsigned insIdx = 0; insIdx < N; ++insIdx ) {
            auto& currPermutation = perms[N * permIdx + insIdx];
            for( unsigned iterIdx = 0; iterIdx < N; ++iterIdx ) {
                auto& currValue = currPermutation[iterIdx];
                if( iterIdx < insIdx ) { currValue = prevPermutation[iterIdx]; }
                else if( iterIdx > insIdx ) { currValue = prevPermutation[iterIdx-1]; }
                else { currValue = N; }
            }
        }
    }

    return perms;
}
template <>
auto util::permutations<1>() {
    return std::array<std::array<unsigned, 1>, factorial(1)>{{{{ 1 }}}};
}

}
