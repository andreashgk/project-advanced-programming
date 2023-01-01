#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include "Manager.h"

namespace resources {

/**
 * A resource that is used to communicate the current input with the game world.
 */
class Input : public Resource {
public:
    Input(float xAxis, bool jump);

    /**
     * Returns which direction the player is trying to move their character. Zero means the payer is not moving, ]0,1]
     * means they are moving to the right and [-1,0[ means they are moving to the left.
     *
     * This is a decimal instead of an enum, as input methods such as a controller would allow for different movement
     * speeds.
     */
    float getHorizontalAxis() const;

    /**
     * Returns if the player is trying to jump.
     */
    bool isJumping() const;

#ifndef NDEBUG
    /**
     * Returns if the player has debug mode activated. This value is not present in release mode.
     */
    bool isDebugMode() const;
    void setDebugMode(bool debug);
#endif

private:
    float horizontalAxis;
    bool jump;
#ifndef NDEBUG
    bool debug;
#endif
};

} // namespace resources

#endif // GAME_INPUT_H
