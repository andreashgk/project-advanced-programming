#ifndef GAME_STOPWATCH_H
#define GAME_STOPWATCH_H

#include <chrono>

namespace utils {

/**
 * A helper class that tracks time between frames. It is used to ensure the world will be simulated at the same speed
 * regardless of frame rate.
 */
class Stopwatch {
public:
    /**
     * Returns the stopwatch instance. It will always exist and always return a reference to the same stopwatch.
     */
    static Stopwatch& get();

    /**
     * Changes the speed at which time is perceived by the game world.
     *
     * @param ts The factor to speed up the time with. A value higher than one will make time appear to go faster, while
     * a value lower than one but higher than zero will slow everything down.
     */
    void setTimeScale(float ts);
    /**
     * Returns the time between the last frame and the current frame in seconds.
     */
    float getDelta() const;
    /**
     * Notifies the stopwatch that a new frame has begun and updates the delta accordingly.
     */
    void nextFrame();

private:
    Stopwatch();

    float delta = 0;
    float timeScale = 1;
    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> prevFrame;
};

} // namespace utils

#endif // GAME_STOPWATCH_H
