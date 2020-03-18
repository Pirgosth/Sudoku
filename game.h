#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define CASE_LENGTH 50
#define CASE_SPACE 6

#include <Stone/Sprite/sprite.h>

#include <iostream>
#include <vector>
#include <array>

#include "random.h"

typedef std::array<std::array<int, 9>, 9> Grid;

Grid createEmptyGrid();
void printGrid(const Grid &grid);
Grid generateValidGrid(int n, int *nodeCount = 0);
float averageGridNodeCount(const int n);

class Case{
public:
  enum State{
	     Default,
	     Selected,
	     Valid,
	     Invalid
  };typedef enum State State;
private:
  Sprite *m_sprite = 0;
  TextSprite *m_texte = 0;
  static SDL_Texture* g_texture_default;
  static SDL_Texture* g_texture_selected;
  static SDL_Texture* g_texture_valid;
  static SDL_Texture* g_texture_invalid;
  static Font *g_font;
  int m_value = 0;
  bool m_isLocked = false;
  bool m_isValid = true;
  State m_state = Default;
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
  bool isValid();
  void setState(const State &state);
};

bool verifyLine(const Grid &grid, int i);
bool verifyColumn(const Grid &grid, int j);
bool verifySquare(const Grid &grid, int n);
bool verifyGrid(const Grid &grid);

std::array<int, 9> getLineValues(const Grid &grid, int i);
std::array<int, 9> getColumnValues(const Grid &grid, int j);
std::array<int, 9> getSquareValues(const Grid &grid, int n);

std::vector<int> getCaseValues(const Grid &grid, int i, int j);
bool isGridSolvent(const Grid &grid);
std::pair<Grid, Grid> generatePlayableGrid(int n, int seed = 0);

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
