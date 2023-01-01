#ifndef GAME_SCORE_H
#define GAME_SCORE_H

#include "Manager.h"

namespace resources {

/**
 * A resource which keeps track of the player's score.
 */
struct Score : public Resource {
    Score() = default;
    explicit Score(int score);

    int score = 0;
};

} // namespace resources

#endif // GAME_SCORE_H
