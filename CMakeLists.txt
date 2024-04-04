cmake_minimum_required(VERSION 3.9)

project(Sudoku)
set (CMAKE_CXX_STANDARD 17)

find_package(SDL2)
find_package(SDL2_image)
find_package(SDL2_ttf)

# Allow for relative headers import
include_directories(${CMAKE_SOURCE_DIR})

add_executable(Sudoku main.cpp game.hpp game.cpp random.hpp random.cpp possibility_node.hpp possibility_node.cpp)

if(WIN32)
    target_link_libraries(Sudoku mingw32 SDL2main
    )
endif()
target_link_libraries(Sudoku SDL2 SDL2_image SDL2_ttf)

add_subdirectory(Graphics)
