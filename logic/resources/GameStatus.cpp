#include "GameStatus.h"

namespace resources {

GameStatus::GameStatus(GameStatus::Status value) : value(value) {}

GameStatus::Status GameStatus::get() const { return value; }

} // namespace resources