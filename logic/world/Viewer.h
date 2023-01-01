#pragma once

namespace world {

class World;
class Entity;
class Camera;

/**
 * A viewer is an abstract class that can be implemented to 'view' an entity. Every frame, the Viewer::view() function
 * will be called by the world so the entity can be rendered in some way.
 */
class Viewer {
public:
    Viewer() = default;
    Viewer(Viewer&&) = default;
    virtual ~Viewer() = default;

    virtual void view(World& w, const Entity& entity) = 0;
};

} // namespace world
