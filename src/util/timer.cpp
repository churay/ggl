#include <chrono>
#include <ratio>
#include <thread>

#include "consts.hpp"

#include "util/timer.h"

namespace ggl {

timer::timer( size_t pFPS ) {
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


ggl::real timer::fps( size_t pNumFrames ) const {
    // TODO(JRC): Implement this function so that it properly outputs the average
    // FPS for the past "pNumFrames" frames.
    SecDuration prevFrameTime = std::chrono::duration_cast<SecDuration>( mWaitTime - mSplitTime );
    return static_cast<ggl::real>( 1.0 / prevFrameTime.count() );
}


ggl::real timer::dt( size_t pNumFrames ) const {
    // TODO(JRC): Implement this function so that it properly outputs the average
    // time delta for the past "pNumFrames" frames.
    SecDuration prevFrameTime = std::chrono::duration_cast<SecDuration>( mWaitTime - mSplitTime );
    return static_cast<ggl::real>( prevFrameTime.count() );
}

}
