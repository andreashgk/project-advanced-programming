#ifndef GAME_ASSETSERVER_H
#define GAME_ASSETSERVER_H

#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeindex>

namespace assets {

class AssetServer {
public:
    static AssetServer& get();

    /**
     * Registers a new type of assets and loads all of the matching assets for this type.
     *
     * @tparam T The type of the asset to add.
     */
    template <class T>
    void addType();

    template <class T>
    const T& get(const std::string& path) const;

    template <class T>
    bool contains(const std::string& path) const;

    /**
     * Returns all loaded assets of a certain type.
     *
     * @tparam T The asset type to return.
     */
    template <class T>
    const std::map<std::string, T>& getAll() const;

private:
    AssetServer();

    struct IAssets {};

    template <class T>
    struct Assets : public IAssets {
        std::map<std::string, T> list{};
    };

    std::map<std::type_index, std::shared_ptr<IAssets>> assetTypes;
};

template <class T>
void AssetServer::addType() {
    std::type_index id = typeid(T);
    if (assetTypes.find(id) != assetTypes.end())
        throw std::domain_error("Trying to insert duplicate asset type");
    auto assets = std::make_shared<Assets<T>>(Assets<T>());
    assets->list = T::findAll();
    assetTypes.insert({id, std::move(assets)});
}

template <class T>
const T& AssetServer::get(const std::string& path) const {
    std::type_index id = typeid(T);
    if (assetTypes.find(id) == assetTypes.end())
        throw std::out_of_range(std::string("Unable to find asset type `").append(T::getName()).append("`"));

    std::shared_ptr<Assets<T>> m = std::static_pointer_cast<Assets<T>>(assetTypes.at(id));
    auto& list = m->list;
    if (list.find(path) == list.end())
        throw std::out_of_range("Unable to find asset `" + path + "` of type`" + T::getName() + "`");
    return list.at(path);
}

template <class T>
const std::map<std::string, T>& AssetServer::getAll() const {
    std::type_index id = typeid(T);
    if (assetTypes.find(id) == assetTypes.end())
        throw std::out_of_range(std::string("Unable to find asset type `").append(T::getName()).append("`"));

    std::shared_ptr<Assets<T>> m = std::static_pointer_cast<Assets<T>>(assetTypes.at(id));
    return m->list;
}

template <class T>
bool AssetServer::contains(const std::string& path) const {
    std::type_index id = typeid(T);
    if (assetTypes.find(id) == assetTypes.end())
        throw std::out_of_range(std::string("Unable to find asset type `").append(T::getName()).append("`"));

    std::shared_ptr<Assets<T>> m = std::static_pointer_cast<Assets<T>>(assetTypes.at(id));
    auto& list = m->list;
    return list.find(path) != list.end();
}

} // namespace assets

#endif // GAME_ASSETSERVER_H
