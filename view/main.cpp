#include <filesystem>
#include <iostream>

#include "Game.h"

int main() {
    if (!std::filesystem::is_directory("./assets")) {
        std::cerr << "Unable to start game: missing assets directory" << std::endl;
        return 1;
    }
    view::Game game;
    game.run();
    return 0;
}
