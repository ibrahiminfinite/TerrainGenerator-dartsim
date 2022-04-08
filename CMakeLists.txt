cmake_minimum_required(VERSION 3.20)
project(TerrainGenerator-Dart)

set(CMAKE_CXX_STANDARD 14)

set(required_components utils-urdf gui-osg collision-ode)
set(required_libraries dart dart-utils-urdf dart-gui-osg dart-collision-ode)

find_package(DART  REQUIRED COMPONENTS ${required_components} CONFIG)

include_directories(TerrainGenerator/include)
set(TERRAIN_GENERATOR_SOURCES
        TerrainGenerator/src/TerrainGenerator.cpp
        TerrainGenerator/src/PerlinNoise.cpp)

add_executable(Main main.cpp ${TERRAIN_GENERATOR_SOURCES})
target_link_libraries(Main PUBLIC ${required_libraries})
