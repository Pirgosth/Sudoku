#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game.hpp"

typedef std::array<std::array<Case *, 9>, 9> GraphicGrid;

Vector2i getCurrentCase(const GraphicGrid &grid, const Vector2i &pos);
bool isNumberKey(int key);
void refreshCurrent(Grid &grid, GraphicGrid &graphicGrid, Vector2i &current, int key, bool notesMode);
void refreshGraphicGrid(const GraphicGrid &graphicGrid, const Grid &grid);
void getGraphicGrid(std::shared_ptr<SDL_Renderer> renderer, TexturesManager &texturesManager, FontsManager &fontsManager, const Grid &grid, GraphicGrid &graphicGrid);
void destroyGraphicGrid(GraphicGrid &graphicGrid);
void compareGrids(const GraphicGrid &graphicGrid, const Grid &game, const Grid &solution);

void drawGrid(const GraphicGrid &grid);

int main(int argc, char *argv[])
{
  initRand();

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
 
  SDL_Window *window(0);
  int windowWidth = 11 * CASE_LENGTH + 10 * CASE_SPACE;
  int windowHeight = 9 * CASE_LENGTH + 10 * CASE_SPACE;
  window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 11 * CASE_LENGTH + 10 * CASE_SPACE, 9 * CASE_LENGTH + 10 * CASE_SPACE, SDL_WINDOW_SHOWN);

  std::shared_ptr<SDL_Renderer> renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC), &SDL_DestroyRenderer);

  std::shared_ptr<SDL_Texture> bgTexture(IMG_LoadTexture(renderer.get(), "./sources/grid.png"), &SDL_DestroyTexture);
  Sprite gridBg(renderer, bgTexture, {1 * CASE_LENGTH, 0}, {9 * CASE_LENGTH + 10 * CASE_SPACE, 9 * CASE_LENGTH + 10 * CASE_SPACE});
  
  std::shared_ptr<SDL_Texture> penTexture(IMG_LoadTexture(renderer.get(), "./sources/pen.png"), &SDL_DestroyTexture);
  Sprite notesSprite(renderer, penTexture, {windowWidth - CASE_LENGTH / 2 - 16, CASE_LENGTH / 2 + CASE_SPACE - 16}, {32, 32});
  // gridBg.setColorFilter({200, 200, 200, 0});

  // int nodeAverage = averageGridNodeCount(10000);

  // std::cout << "Nodes encountered average: " << nodeAverage << std::endl;

  std::pair<Grid, Grid> grid(generatePlayableGrid(81));
  Grid grille(grid.first);

  TexturesManager texturesManager;
  FontsManager fontsManager;

  for (auto const &[key, path]: std::map<std::string, std::string>({
    {"default", "./sources/default.png"},
    {"selected", "./sources/selected.png"},
    {"valid", "./sources/valid.png"},
    {"invalid", "./sources/invalid.png"},
  }))
  {
    texturesManager.addTexture(key, IMG_LoadTexture(renderer.get(), path.c_str()));
  }

  fontsManager.addFont("default", TTF_OpenFont("./sources/DejaVuSans.ttf", 30));
  fontsManager.addFont("small", TTF_OpenFont("./sources/DejaVuSans.ttf", 10));
  
  GraphicGrid test;

  getGraphicGrid(renderer, texturesManager, fontsManager, grille, test);

  Vector2i current({-1, -1});
  bool notesMode = false;

  bool quit(false);
  SDL_Event event;

  while (!quit)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_MOUSEBUTTONUP:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
        {
          Vector2i tmp;
          SDL_GetMouseState(&tmp.x, &tmp.y);
          tmp.x -= 1 * CASE_LENGTH;
          if (current.x != -1)
          {
            if (grille[current.x][current.y] != 0)
            {
              test[current.x][current.y]->removeState(Case::Selected);
            }
            else
            {
              test[current.x][current.y]->setState(Case::Default);
            }
          }
          current = getCurrentCase(test, tmp);
          if (current.x != -1)
          {
            test[current.x][current.y]->addState(Case::Selected);
          }
        }
          std::cout << current.x << std::endl;
          break;
        default:
          break;
        }
        break;
      case SDL_KEYUP:
        if (isNumberKey(event.key.keysym.sym))
        {
          std::cout << "Number Key pressed" << std::endl;
          refreshCurrent(grille, test, current, event.key.keysym.sym, notesMode);
        }
        switch (event.key.keysym.sym)
        {
        case SDLK_r:
          if (current.x != -1)
          {
            test[current.x][current.y]->setState(Case::Default);
          }
          current.x = -1;
          grid = generatePlayableGrid(81);
          std::cout << "Grille régénérée !" << std::endl;
          grille = grid.first;
          refreshGraphicGrid(test, grille);
          //printGrid(grille);
          //compareGrids(test, grille, grid.second);
          break;
        case SDLK_v:
          compareGrids(test, grille, grid.second);
          break;
        case SDLK_e:
          notesMode = !notesMode;
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());

    gridBg.draw();
    drawGrid(test);

    if (notesMode)
      notesSprite.draw();

    SDL_RenderPresent(renderer.get());
  }

  fontsManager.clear();
  texturesManager.clear();

  destroyGraphicGrid(test);

  SDL_DestroyWindow(window);

  TTF_Quit();
  SDL_Quit();

  destroyRand();

  return 0;
}

Vector2i getCurrentCase(const GraphicGrid &grid, const Vector2i &pos)
{
  // float i1((float)(pos.x)/(float)CASE_SPACE), i2((float)(pos.x)/(float)(CASE_SPACE+CASE_LENGTH));
  int x(-1), y(-1);
  for (int k(1); k < 10; k++)
  {
    if (pos.x > (k * CASE_SPACE + (k - 1) * CASE_LENGTH) && pos.x < k * (CASE_SPACE + CASE_LENGTH))
    {
      x = k - 1;
    }
  }
  for (int k(1); k < 10; k++)
  {
    if (pos.y > (k * CASE_SPACE + (k - 1) * CASE_LENGTH) && pos.y < k * (CASE_SPACE + CASE_LENGTH))
    {
      y = k - 1;
    }
  }
  if (x == -1 || y == -1 || grid[y][x]->isLocked())
  {
    return {-1, -1};
  }
  return {y, x};
  // std::cout << "Mouse is in case " << x << ":" << y << std::endl;
}

bool isNumberKey(int key)
{
  std::array<int, 20> numberKeys = {{SDLK_KP_0, SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_7, SDLK_KP_8, SDLK_KP_9, SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9}};
  for (int i(0); i < 20; i++)
  {
    if (key == numberKeys[i])
    {
      return true;
    }
  }
  return false;
}

void refreshCurrent(Grid &grid, GraphicGrid &graphicGrid, Vector2i &current, int key, bool notesMode)
{
  std::array<int, 20> numberKeys = {{SDLK_KP_0, SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_7, SDLK_KP_8, SDLK_KP_9, SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9}};
  int index(-1);
  for (int i(0); i < 20; i++)
  {
    if (key == numberKeys[i])
    {
      index = i % 10;
    }
  }
  if (current.x != -1)
  {
    graphicGrid[current.x][current.y]->setState(Case::Selected);
    if (notesMode)
    {
      graphicGrid[current.x][current.y]->addOrRemoveNote(index);
    }
    else
    {
      graphicGrid[current.x][current.y]->setValue(index);
      grid[current.x][current.y] = index;
    }
  }
}

void refreshGraphicGrid(const GraphicGrid &graphicGrid, const Grid &grid)
{
  for (int i(0); i < 9; i++)
  {
    for (int j(0); j < 9; j++)
    {
      graphicGrid[i][j]->setState(Case::Default);
      graphicGrid[i][j]->setValue(grid[i][j]);
      if (grid[i][j] != 0)
      {
        graphicGrid[i][j]->lock();
      }
      else
      {
        graphicGrid[i][j]->unlock();
      }
    }
  }
}

void getGraphicGrid(std::shared_ptr<SDL_Renderer> renderer, TexturesManager &texturesManager, FontsManager &fontsManager, const Grid &grid, GraphicGrid &graphicGrid)
{
  for (int i(0); i < 9; i++)
  {
    for (int j(0); j < 9; j++)
    {
      graphicGrid[i][j] = new Case(renderer, texturesManager, fontsManager, {CASE_LENGTH + j * CASE_LENGTH + (j + 1) * CASE_SPACE, i * CASE_LENGTH + (i + 1) * CASE_SPACE}, grid[i][j]);
      if (grid[i][j] != 0)
      {
        graphicGrid[i][j]->lock();
      }
    }
  }
}

void destroyGraphicGrid(GraphicGrid &graphicGrid)
{
  for (int i(0); i < 9; i++)
  {
    for (int j(0); j < 9; j++)
    {
      delete graphicGrid[i][j];
      graphicGrid[i][j] = 0;
    }
  }
}

void compareGrids(const GraphicGrid &graphicGrid, const Grid &game, const Grid &solution)
{
  bool isValid(true);
  for (int i(0); i < 9; i++)
  {
    for (int j(0); j < 9; j++)
    {
      if (game[i][j] != 0 && !graphicGrid[i][j]->isLocked())
      {
        graphicGrid[i][j]->addState(Case::Verified);
        graphicGrid[i][j]->removeState(Case::Selected);
        if (game[i][j] == solution[i][j])
        {
          //set color green
          graphicGrid[i][j]->addState(Case::Valid);
        }
        else
        {
          //set color red
          graphicGrid[i][j]->removeState(Case::Valid);
          isValid = false;
        }
      }
      else
      {
        graphicGrid[i][j]->setState(Case::Default);
      }
    }
  }
  if (isValid)
  {
    std::cout << "Grille valide" << std::endl;
  }
  else
  {
    std::cout << "Grille invalide" << std::endl;
  }
}

void drawGrid(const GraphicGrid &grid)
{
  for (auto line: grid)
  {
    for (auto cell: line)
    {
      cell->draw();
    }
  }
}
