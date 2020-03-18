project(Sudoku)

#set (CMAKE_CXX_STANDARD 11)

add_executable(Sudoku main.cpp game.h game.cpp random.h random.cpp)
target_link_libraries(Sudoku SDL2 SDL2_image SDL2_ttf Sprite)
