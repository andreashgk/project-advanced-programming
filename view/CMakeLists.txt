SET(SRC
        main.cpp
        SFMLFactory.cpp
        Game.cpp

        assets/AssetServer.cpp
        assets/Texture.cpp
        assets/Tileset.cpp
        assets/Sprite.cpp
        assets/Font.cpp

        viewer/GenericViewer.cpp
        viewer/ColliderViewer.cpp
        viewer/ColliderViewer.cpp
        viewer/TilemapViewer.cpp
        viewer/PlayerViewer.cpp
        viewer/AnimatedViewer.cpp
        viewer/viewUtils.hpp

        state/Manager.cpp
        state/State.h
        state/GameState.cpp
        state/PausedState.cpp
        state/MenuState.cpp

        particle/Particle.cpp
        particle/Particles.cpp

        ui/Element.cpp
        ui/Label.cpp
        ui/List.cpp
        ui/Margin.cpp
        ui/utils.cpp

        level/TomlLevel.cpp
        level/TomlLevel.h assets/Level.cpp assets/Level.h level/Loader.cpp level/Loader.h)
ADD_EXECUTABLE(game ${SRC})
