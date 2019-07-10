project(Sudoku)

include_directories(C:/boost/include/boost-1_70)
link_directories(C:/boost/lib)

add_executable(Sudoku WIN32 main.cpp game.h game.cpp)
target_link_libraries(Sudoku mingw32 SDL2main SDL2 boost_random-mgw81-mt-x64-1_70 sprite)