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
std::vector<int> generateValues(std::vector<int> validValues = std::vector<int>());
Grid generateValidGrid(int n);

class Case{
private:
    Sprite *m_sprite = 0;
    TextSprite *m_texte = 0;
    static SDL_Texture* g_texture_default;
    static SDL_Texture* g_texture_selected;
    static Font *g_font;
    int m_value = 0;
    bool m_isLocked = false;
public:
    Case(Pos pos, int i);
    void draw();
    static void loadTexture(const SpriteManager &manager);
    static void destroyTexture();
    int getValue();
    void setValue(int value);
    ~Case();
    void lock();
    void unlock();
    bool isLocked();
    void setIsSelected(bool isSelected);
};

bool verifyLine(const Grid &grid, int i);
bool verifyColumn(const Grid &grid, int j);
bool verifySquare(const Grid &grid, int n);
bool verifyGrid(const Grid &grid);

typedef std::array<std::array<Case, 9>, 9> CaseGrid;

template<int N>
class Branch{
private:
    std::array<Branch*, N> m_nodes;
    bool m_isValid = true;
    int m_count;
public:
    Branch(bool isValid, int count);
    ~Branch();
    void addNode(Branch *node, int index);
    Branch* operator[](int index);
    bool isValid();
    void setValidity(bool validity);
    int getCount();
};

#endif //GAME_H_INCLUDED