#include "Stopwatch.h"

namespace utils {

Stopwatch::Stopwatch() { prevFrame = std::chrono::high_resolution_clock::now(); }

Stopwatch& Stopwatch::get() {
    static Stopwatch instance;
    return instance;
}

float Stopwatch::getDelta() const { return delta; }

void Stopwatch::nextFrame() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = now - prevFrame;
    delta = float(dur.count()) * timeScale;
    prevFrame = now;
}

void Stopwatch::setTimeScale(float ts) { timeScale = ts; }

} // namespace utils