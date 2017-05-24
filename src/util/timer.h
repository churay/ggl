#ifndef GGL_TIMER_H
#define GGL_TIMER_H

#include <chrono>
#include <ratio>

#include "consts.hpp"

namespace ggl {

class timer {
    public:

    /// Constructors ///

    timer( size_t pFPS = 60 );

    /// Class Functions ///

    void split();
    void wait();

    ggl::real fps( size_t pNumFrames = 1 ) const;
    ggl::real dt( size_t pNumFrames = 1 ) const;

    private:

    /// Class Setup ///

    using Clock = std::chrono::high_resolution_clock;
    using ClockPoint = decltype( Clock::now() );
    using ClockDuration = decltype( Clock::now() - Clock::now() );
    using SecDuration = std::chrono::duration<long double, std::ratio<1>>;

    /// Class Fields ///

    ClockDuration mFrameDuration;
    ClockPoint mSplitTime, mWaitTime;
};

}

#endif
