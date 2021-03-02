project(Sudoku)

#set (CMAKE_CXX_STANDARD 11)

add_executable(Sudoku main.cpp game.hpp game.cpp random.hpp random.cpp possibility_node.hpp possibility_node.cpp)
target_link_libraries(Sudoku SDL2 SDL2_image SDL2_ttf Stone_Sprite)
