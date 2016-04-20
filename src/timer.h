#ifndef GGL_TIMER_H
#define GGL_TIMER_H

#include <chrono>

namespace ggl {

class timer {
    public:
    
    /// Constructors ///

    timer( const size_t pRefRate = 60 );

    /// Class Functions ///

    void split();
    void wait();

    private:

    /// Class Fields ///

    size_t mRefRate;
    decltype( std::chrono::high_resolution_clock::now() ) mSplit;
};

}

#endif
