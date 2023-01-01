#ifndef GAME_GAMESTATUS_H
#define GAME_GAMESTATUS_H

#include "Manager.h"

namespace resources {

/**
 * Returns what state the world is in. See GameStatus::Status for info on all possible states.
 */
class GameStatus : public Resource {
public:
    /**
     * The different states the world can be in.
     */
    enum Status {
        /**
         * The default state. This indicates that the player is playing as normal and has not yet completed the level.
         */
        Playing,
        /**
         * Indicates that the player has successfully completed the level.
         */
        Finished,
        /**
         * Indicates that the player has failed the level.
         */
        Failed,
    };
    GameStatus(Status value);

    /**
     * @return The value of the current status.
     */
    Status get() const;

private:
    Status value;
};

} // namespace resources

#endif // GAME_GAMESTATUS_H
