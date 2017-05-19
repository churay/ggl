#include <chrono>
#include <ratio>
#include <thread>

#include "util/timer.h"

namespace ggl {

timer::timer( const size_t pFPS ) {
    SecDuration frameDuration( 1.0 / pFPS );

    mFrameDuration = std::chrono::duration_cast<ClockDuration>( frameDuration );
    mSplitTime = Clock::now();
    mWaitTime = Clock::now();
}


void timer::split() {
    mSplitTime = Clock::now();
}


void timer::wait() {
    mWaitTime = Clock::now();

    auto elapsedTime = mWaitTime - mSplitTime;
    auto remainingTime = mFrameDuration - elapsedTime;
    std::this_thread::sleep_for( remainingTime );
}


size_t timer::prevFrameFPS() const {
    SecDuration prevFrameTime = std::chrono::duration_cast<SecDuration>( mWaitTime - mSplitTime );
    return static_cast<size_t>( 1.0 / prevFrameTime.count() );
}


size_t timer::avgFrameFPS() const {
    // TODO(JRC): Implement this function so that it properly outputs the average
    // FPS for the past several frames.
    return (*this).prevFrameFPS();
}

}
