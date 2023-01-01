CMAKE_MINIMUM_REQUIRED(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wconversion")
PROJECT(Game)
IF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "default install path" FORCE)
ENDIF ()
IF (WIN32)
    INCLUDE_DIRECTORIES(extern/SFML/include)
    link_directories(extern/SFML/lib)
ELSE ()
    # Look for SFML >= 2.0, with the specific subcomponents listed below
    FIND_PACKAGE(SFML 2 COMPONENTS system window graphics)

    # Add include dirs for SFML
    INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})
ENDIF ()

INCLUDE_DIRECTORIES(extern/yaml-cpp/include)
INCLUDE_DIRECTORIES(extern/toml11)

add_subdirectory(extern/yaml-cpp)
add_subdirectory(extern/toml11)
add_subdirectory(view)
add_subdirectory(logic)

TARGET_LINK_LIBRARIES(game yaml-cpp)
# Link target to SFML libs
TARGET_LINK_LIBRARIES(game sfml-system sfml-graphics sfml-window)

TARGET_LINK_LIBRARIES(game logic)

# Install our executable to the CMAKE_INSTALL_PREFIX/bin
INSTALL(TARGETS game DESTINATION bin)
