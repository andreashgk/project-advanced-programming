#include "Sprite.h"

#include <filesystem>
#include <iostream>
#include <yaml-cpp/yaml.h>

namespace assets {

std::string Sprite::getName() { return "Sprite"; }

std::map<std::string, Sprite> Sprite::findAll() {
    std::map<std::string, Sprite> map;
    for (const auto& entry : std::filesystem::directory_iterator("./assets/sprites")) {
        if (entry.is_directory())
            continue;

        Sprite sprite{};
        try {
            YAML::Node tilesetCfg = YAML::LoadFile(entry.path().string());
            int size = tilesetCfg["size"].as<int>();
            if (size <= 0) {
                throw std::domain_error("Size must be greater than 0");
            }

            for (const auto& state : tilesetCfg["states"]) {
                auto name = state.first.as<std::string>();
                int duration = state.second["duration"].as<int>();
                int frame_delta = state.second["frame_delta"].as<int>();

                sf::Texture tx;

                std::string fullpath = entry.path().string();
                std::string path2 = fullpath.substr(0, fullpath.find(entry.path().extension().string()));
                if (!tx.loadFromFile(path2.append("/").append(name).append(".png"))) {
                    throw std::runtime_error("Could not load corresponding image");
                }

                sprite.states.insert({name, State(tx, frame_delta, size, duration)});
            }
        } catch (const YAML::Exception& e) {
            std::cerr << "Could not parse \"" << entry.path().string() << "\": " << e.what() << std::endl;
            continue;
        } catch (const std::exception& e) {
            std::cerr << "Could not process \"" << entry.path().string() << "\": " << e.what() << std::endl;
            continue;
        }
        map.insert({entry.path().filename().string(), std::move(sprite)});
    }
    return map;
}

const Sprite::State& Sprite::getState(const std::string& name) const {
    auto it = states.find(name);
    if (it == states.end())
        throw std::out_of_range("Sprite state `" + name + "` not found");
    return it->second;
}

Sprite::State::State(const sf::Texture& tx, int frame_delta, int size, int duration)
    : tx(tx), frame_delta(frame_delta), size(size), frame_count(int(tx.getSize().x) / size), duration(duration) {}

int Sprite::State::getDuration() const { return duration; }

const sf::Texture& Sprite::State::getTexture() const { return tx; }

sf::IntRect Sprite::State::getFrame(int timeSinceStart, bool mirror) const {
    int frame = (timeSinceStart / frame_delta) % frame_count;
    int s = size;
    if (mirror) {
        ++frame;
        s = -s;
    }
    return {frame * size, 0, s, size};
}

sf::IntRect Sprite::State::getFrame(float timeSinceStart, bool mirror) const {
    return getFrame(int(timeSinceStart * 1000.f), mirror);
}

} // namespace assets