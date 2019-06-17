#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define CASE_LENGTH 50
#define CASE_SPACE 5

#include <Stone/Stone.h>

#include <random>
#include <iostream>
#include <vector>
#include <array>

typedef std::array<std::array<int, 9>, 9> Grid;

Grid createEmptyGrid();
void printGrid(const Grid &grid);
Grid generateValidGrid(int n);

class Case: public Sprite, public TextSprite{
private:
    static SDL_Texture* g_texture;
    static Font *g_font;
    int m_value = 0;
    bool m_isLocked = false;
public:
    Case(Pos pos, int i);
    void draw();
    static void loadTexture();
    static void destroyTexture();
    int getValue();
    void setValue(int value);
    virtual ~Case();
    void lock();
    void unlock();
    bool isLocked();
};

bool verifyLine(const Grid &grid, int i);
bool verifyColumn(const Grid &grid, int j);
bool verifySquare(const Grid &grid, int n);
bool verifyGrid(const Grid &grid);

typedef std::array<std::array<Case, 9>, 9> CaseGrid;

#endif //GAME_H_INCLUDED