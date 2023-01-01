SET(SRC
        math/Vector2.cpp
        math/Vector2.h
        math/Transform.cpp
        math/Transform.h
        math/AABB.cpp
        math/Collider.cpp

        data/Graph.h

        resources/Manager.h
        resources/Input.cpp
        resources/Score.cpp

        world/Entity.cpp
        world/Entity.h
        world/Viewer.h
        world/World.cpp
        world/Camera.cpp
        world/EntityFactory.h
        world/entities/Player.cpp
        world/entities/Tilemap.cpp
        world/entities/Melon.cpp

        utils/Stopwatch.cpp world/entities/Finish.cpp world/entities/Finish.h resources/GameStatus.cpp resources/GameStatus.h)
ADD_LIBRARY(logic ${SRC})