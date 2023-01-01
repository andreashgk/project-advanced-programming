#include "AssetServer.h"

#include "Font.h"
#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "Tileset.h"

namespace assets {

AssetServer& AssetServer::get() {
    static AssetServer server;
    return server;
}

AssetServer::AssetServer() {
    addType<Texture>();
    addType<Tileset>();
    addType<Sprite>();
    addType<Font>();
    addType<Level>();
}

} // namespace assets