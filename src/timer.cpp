#include <chrono>
#include <thread>

#include "timer.h"

namespace ggl {

timer::timer( const size_t pRefRate ) {
    // TODO(JRC): Improve the implementation here so that conversions won't
    // be needed between the refresh rate (in hertz) and the split time units.
    mRefRate = pRefRate;
}


void timer::split() {
    // TODO(JRC): Implement this function using "std::chrono" timers.
    mSplit = std::chrono::high_resolution_clock::now();
}


void timer::wait() {
    // TODO(JRC): Implement this function using "std::this_thread::sleep_for".
}

}
