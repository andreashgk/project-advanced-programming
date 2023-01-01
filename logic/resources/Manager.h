#ifndef GAME_RESOURCEMANAGER_H
#define GAME_RESOURCEMANAGER_H

#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace resources {

/**
 * A resource is a piece of data which can be accessed from a world. It is used to insert specific data which is part
 * of the game itself and not the underlying 'engine'. It can be used to communicate to different parts of the code or
 * just store data needed in multiple places.
 */
class Resource {
public:
    virtual ~Resource() = default;
};

/**
 * Manages the resources of a world. For each type of resource, exactly one or zero resources of this type can be
 * present.
 */
class Manager {
public:
    Manager() = default;

    /**
     * Returns if the manager has a resource of the given type.
     *
     * @tparam T The type of resource to check.
     * @return True if there is a resource with this type, or false otherwise.
     */
    template <class T>
    bool exists();
    /**
     * Returns a mutable reference to the resource of the type provided as template parameter. This requires that the
     * resource is present within this manager. This reference is valid as long as the resource is not removed.
     *
     * @tparam T The type of resource to check.
     * @throws std::out_of_range if the resource does not exist.
     * @return A reference to the resource.
     */
    template <class T>
    std::shared_ptr<T> get();
    /**
     * Returns a constant reference to the resource of the type provided as template parameter. This requires that the
     * resource is present within this manager. This reference is valid as long as the resource is not removed.
     *
     * @tparam T The type of resource to check.
     * @throws std::out_of_range if the resource does not exist.
     * @return A const reference to the resource.
     */
    template <class T>
    std::shared_ptr<const T> get() const;

    /**
     * Adds a new resource of type T into the resource manager. If the a resource of this type already exists, it will
     * be overwritten. Existing references will stay valid.
     *
     * @tparam T The type of resource to insert.
     * @param resource The value of the resource to insert.
     */
    template <class T>
    void insert(T resource);

    /**
     * Removes a resource from the manager. This requires a resource of the provided type to exist in the manager.
     *
     * @tparam T The type of resource to remove.
     * @throws std::out_of_range if the resource was not found.
     */
    template <class T>
    void remove();

private:
    std::unordered_map<std::type_index, std::shared_ptr<Resource>> resources;
};

template <class T>
bool Manager::exists() {
    return resources.find(std::type_index(typeid(T))) != resources.end();
}

template <class T>
std::shared_ptr<T> Manager::get() {
    std::type_index id = typeid(T);
    auto it = resources.find(id);
    if (it == resources.end())
        throw std::out_of_range("Trying to retrieve a resource type not present in Resources");

    return std::static_pointer_cast<T>(it->second);
}

template <class T>
std::shared_ptr<const T> Manager::get() const {
    std::type_index id = typeid(T);
    auto it = resources.find(id);
    if (it == resources.end())
        throw std::out_of_range("Trying to retrieve a resource type not present in Resources");

    return std::static_pointer_cast<const T&>(it->second);
}

template <class T>
void Manager::insert(T resource) {
    std::type_index id = typeid(T);

    auto it = resources.find(id);
    if (it != resources.end()) {
        *std::static_pointer_cast<T>(it->second).get() = std::move(resource);
        return;
    }

    resources.insert({id, std::make_shared<T>(std::move(resource))});
}

template <class T>
void Manager::remove() {
    std::type_index id = typeid(T);
    auto it = resources.find(id);
    if (it == resources.end()) {
        throw std::out_of_range("Trying to remove a resource type not present in Resources");
    }
    resources.erase(it);
}

} // namespace resources

#endif // GAME_RESOURCEMANAGER_H
