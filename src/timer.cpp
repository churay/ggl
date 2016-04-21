#include <chrono>
#include <ratio>
#include <thread>

#include "timer.h"

namespace ggl {

timer::timer( const size_t pFPS ) {
    std::chrono::duration<long double, std::ratio<1>> frameDurationSecs( 1.0 / pFPS );

    mFrameDuration = std::chrono::duration_cast<TimeDuration>( frameDurationSecs );
    mSplitTime = Clock::now();
}


void timer::split() {
    mSplitTime = Clock::now();
}


void timer::wait() {
    auto currTime = Clock::now();
    auto elapsedTime = currTime - mSplitTime;
    auto remainingTime = mFrameDuration - elapsedTime;

    std::this_thread::sleep_for( remainingTime );
}

}
