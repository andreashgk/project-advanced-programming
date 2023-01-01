#ifndef GAME_UTILS_H
#define GAME_UTILS_H

/**
 * A utility function to scale an element relative to the window, regardless of how big the window is.
 *
 * @param windowHeight The height on the window in pixels.
 * @param scale The scale factor. Leave this as 1 to use the default scale. This will be around 65 for a 1080p window,
 * which is 2/33 times the height of the window.
 * @return The scaled size of the element in pixels.
 */
float scale(unsigned int windowHeight, float scale = 1.f);

#endif // GAME_UTILS_H
