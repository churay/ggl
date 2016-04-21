#ifndef GGL_TIMER_H
#define GGL_TIMER_H

#include <chrono>

namespace ggl {

class timer {
    public:
    
    /// Constructors ///

    timer( const size_t pFPS = 60 );

    /// Class Functions ///

    void split();
    void wait();

    private:

    /// Class Setup ///

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = decltype( Clock::now() );
    using TimeDuration = decltype( Clock::now() - Clock::now() );

    /// Class Fields ///

    TimeDuration mFrameDuration;
    TimePoint mSplitTime;
};

}

#endif
