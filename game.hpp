#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define CASE_LENGTH 50
#define CASE_SPACE 6

#include <Stone/Sprite/sprite.h>

#include <iostream>
#include <vector>

#include "possibility_node.hpp"
#include "random.hpp"

typedef std::array<std::array<int, 9>, 9> Grid;

Grid createEmptyGrid();
void printGrid(const Grid &grid);
Grid generateValidGrid(int *nodeCount = 0);
float averageGridNodeCount(const int n);

class Case
{
public:
  enum State
  {
    Default = 0x0,
    Selected = 0x1,
    Verified = 0x2,
    Valid = 0x4,
  };
  typedef enum State State;

private:
  Sprite *m_sprite = 0;
  TextSprite *m_texte = 0;
  static SDL_Texture *g_texture_default;
  static SDL_Texture *g_texture_selected;
  static SDL_Texture *g_texture_valid;
  static SDL_Texture *g_texture_invalid;
  static Font *g_font;
  int m_value = 0;
  bool m_isLocked = false;
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
  bool isVerified();
  void setState(const State &state);
  void addState(const State &state);
  void removeState(const State &state);
  bool hasState(const State &state);
  const State &getState() const;
};

constexpr inline Case::State operator~(Case::State a) { return static_cast<Case::State>(~static_cast<std::underlying_type<Case::State>::type>(a)); }
constexpr inline Case::State operator|(Case::State a, Case::State b) { return static_cast<Case::State>(static_cast<std::underlying_type<Case::State>::type>(a) | static_cast<std::underlying_type<Case::State>::type>(b)); }
constexpr inline Case::State operator&(Case::State a, Case::State b) { return static_cast<Case::State>(static_cast<std::underlying_type<Case::State>::type>(a) & static_cast<std::underlying_type<Case::State>::type>(b)); }
constexpr inline Case::State operator^(Case::State a, Case::State b) { return static_cast<Case::State>(static_cast<std::underlying_type<Case::State>::type>(a) ^ static_cast<std::underlying_type<Case::State>::type>(b)); }
constexpr inline Case::State &operator|=(Case::State &a, Case::State b) { return reinterpret_cast<Case::State &>(reinterpret_cast<std::underlying_type<Case::State>::type &>(a) |= static_cast<std::underlying_type<Case::State>::type>(b)); }
constexpr inline Case::State &operator&=(Case::State &a, Case::State b) { return reinterpret_cast<Case::State &>(reinterpret_cast<std::underlying_type<Case::State>::type &>(a) &= static_cast<std::underlying_type<Case::State>::type>(b)); }
constexpr inline Case::State &operator^=(Case::State &a, Case::State b) { return reinterpret_cast<Case::State &>(reinterpret_cast<std::underlying_type<Case::State>::type &>(a) ^= static_cast<std::underlying_type<Case::State>::type>(b)); }

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

#endif //GAME_H_INCLUDED
