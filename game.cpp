#include "game.hpp"

Grid createEmptyGrid()
{
  return Grid();
}

void printGrid(const Grid &grid)
{
  std::cout << "[";
  for (int i(0); i < 9; i++)
  {
    if (i != 0)
    {
      std::cout << " ";
    }
    std::cout << "[";
    for (int j(0); j < 9; j++)
    {
      if (j == 8)
      {
        std::cout << grid[i][j] << "]";
      }
      else
      {
        std::cout << grid[i][j] << ",";
      }
    }
    if (i == 8)
    {
      std::cout << "]" << std::endl;
    }
    else
    {
      std::cout << ",\n";
    }
  }
}

Grid generateValidGrid(int *nodeCount)
{

  Grid grid(createEmptyGrid());

  PossibilityNode rootNode(-1);

  std::vector<PossibilityNode *> nodesHistory;

  int availableValues[9] = {9, 9, 9, 9, 9, 9, 9, 9, 9};

  PossibilityNode *currentNode = &rootNode;

  int nodesReached(0);

  for (int k(0); k < 81; k++)
  {

    //If currentNode is not locked and not valid, this means that this is a new possibility, not an already visited node, so we have to generate next possibilities.
    if (!currentNode->isLocked() && !currentNode->isValid())
    {

      std::vector<int> availableMoves = getCaseValues(grid, k / 9, k % 9);

      //Generate possibilities for current node
      //TODO: Prefilter possibilities by removing impossible moves
      for (int i(1); i <= 9; i++)
      {
        if (availableValues[i - 1] != 0 && std::find(availableMoves.begin(), availableMoves.end(), i) != availableMoves.end())
        {
          currentNode->addPossibility(i);
        }
      }
    }

    PossibilityNode *nextNode = __null;

    std::vector<PossibilityNode *> randomizedPossibilites(currentNode->getNextPossibilities().begin(), currentNode->getNextPossibilities().end());

    std::random_shuffle(randomizedPossibilites.begin(), randomizedPossibilites.end());

    for (auto it = randomizedPossibilites.begin(); it != randomizedPossibilites.end(); it++)
    {
      nodesReached++;
      PossibilityNode *currentPossibility = *it;
      grid[k / 9][k % 9] = currentPossibility->getValue();

      if (verifyLine(grid, k / 9) && verifyColumn(grid, k % 9) && verifySquare(grid, (int)((k / 9) / 3) * 3 + ((k % 9) / 3)))
      {
        nextNode = currentPossibility;
        break;
      }

      currentNode->removePossibility(currentPossibility->getValue());
    }

    //If there is no next possibilities, we revert one step back in node history
    if (!currentNode->isValid())
    {
      int possibilityValueToRemove = currentNode->getValue();
      currentNode = nodesHistory[nodesHistory.size() - 1];
      currentNode->removePossibility(possibilityValueToRemove);
      availableValues[possibilityValueToRemove - 1] += 1;
      nodesHistory.pop_back();
      grid[k / 9][k % 9] = 0;
      k -= 2;
      continue;
    }

    if (k < 80)
    {
      int currentValue = grid[k / 9][k % 9];
      availableValues[currentValue - 1] -= 1;
      nodesHistory.push_back(currentNode);
      currentNode = nextNode;
    }
  }
  if (nodeCount != __null)
    *nodeCount = nodesReached;
  std::cout << "Grid generated after reaching: " << nodesReached << " nodes" << std::endl;
  //printGrid(grid);
  return grid;
}

float averageGridNodeCount(const int n)
{
  std::vector<int> values;
  float totalNodeCount(0);
  int currentNodeCount(0);
  int timeout = SDL_GetTicks();
  for (int i(0); i < n; i++)
  {
    generateValidGrid(&currentNodeCount);
    // std::cout << currentNodeCount << std::endl;
    values.push_back(currentNodeCount);
    totalNodeCount += currentNodeCount;
  }
  float average(totalNodeCount / n);
  float sigma(0);
  for (int i(0); i < (int)values.size(); i++)
  {
    sigma += std::pow(values[i] - average, 2);
  }
  sigma /= n;
  sigma = std::sqrt(sigma);
  float confiance = 2 * sigma / (std::sqrt(n));
  float xmin(average - confiance), xmax(average + confiance);
  std::cout << "Simulation took: " << (SDL_GetTicks() - timeout) / 1000.0 << " seconds" << std::endl;
  std::cout << "Confidence interval: [" << xmin << ";" << xmax << "]" << std::endl;
  return average;
}

SDL_Texture *Case::g_texture_default(0);
SDL_Texture *Case::g_texture_selected(0);
SDL_Texture *Case::g_texture_valid(0);
SDL_Texture *Case::g_texture_invalid(0);
Font *Case::g_font(0);

void Case::loadTexture(const SpriteManager &manager)
{
  g_texture_default = _load_texture_from_file(manager.getContext().renderer, "./sources/default.png");
  g_texture_selected = _load_texture_from_file(manager.getContext().renderer, "./sources/selected.png");
  g_texture_valid = _load_texture_from_file(manager.getContext().renderer, "./sources/valid.png");
  g_texture_invalid = _load_texture_from_file(manager.getContext().renderer, "./sources/invalid.png");
  g_font = new Font("./sources/DejaVuSans.ttf");
}

void Case::destroyTexture()
{
  SDL_DestroyTexture(g_texture_selected);
  SDL_DestroyTexture(g_texture_default);
}

Case::Case(Pos pos, int i)
{
  m_sprite = new Sprite(g_texture_default, pos, {CASE_LENGTH, CASE_LENGTH});
  m_texte = new TextSprite("a", pos, *g_font, {255, 255, 255, 255}, 30, {0, 0, 0, 0});
  m_texte->pos({m_sprite->pos().x + (m_sprite->size().w - m_texte->size().w) / 2, m_sprite->pos().y + (m_sprite->size().h - m_texte->size().h) / 2});
  setValue(i);
}

void Case::draw()
{
  m_sprite->draw();
  if (m_texte->isEnabled())
  {
    m_texte->draw();
  }
}

int Case::getValue()
{
  return m_value;
}

void Case::setValue(int value)
{
  if (value == 0)
  {
    m_texte->disable();
  }
  else
  {
    m_texte->enable();
  }
  m_value = value;
  m_texte->setText(std::to_string(m_value));
}

void Case::lock()
{
  m_isLocked = true;
}

void Case::unlock()
{
  m_isLocked = false;
}

bool Case::isLocked()
{
  return m_isLocked;
}

Case::~Case()
{
  delete m_texte;
  delete m_sprite;
}

bool Case::isValid()
{
  return (this->m_state & Valid) != 0;
}

bool Case::isVerified()
{
  return (this->m_state & Default) != 0;
}

void Case::setState(const State &state)
{
  m_state = state;
  if (m_state == Default)
  {
    m_sprite->setTexture(g_texture_default);
  }
  else
  {
    if (this->hasState(Selected))
    {
      m_sprite->setTexture(g_texture_selected);
    }
    else if (this->hasState(Verified))
    {
      if (this->hasState(Valid))
      {
        m_sprite->setTexture(g_texture_valid);
      }
      else
      {
        m_sprite->setTexture(g_texture_invalid);
      }
    }
  }
}

void Case::addState(const State &state)
{
  setState(m_state | state);
}

void Case::removeState(const State &state)
{
  setState(m_state & ~state);
}

bool Case::hasState(const State &state)
{
  return (this->m_state & state) != 0;
}

const Case::State &Case::getState() const
{
  return this->m_state;
}

bool verifyLine(const Grid &grid, int i)
{
  std::array<int, 9> count;
  for (int j(0); j < 9; j++)
  {
    count[j] = 0;
  }
  for (int j(0); j < 9; j++)
  {
    if (grid[i][j] != 0)
    {
      count[grid[i][j] - 1] += 1;
      if (count[grid[i][j] - 1] > 1)
      {
        return false;
      }
    }
  }
  return true;
}

bool verifyColumn(const Grid &grid, int j)
{
  std::array<int, 9> count;
  for (int i(0); i < 9; i++)
  {
    count[i] = 0;
  }
  for (int i(0); i < 9; i++)
  {
    if (grid[i][j] != 0)
    {
      count[grid[i][j] - 1] += 1;
      if (count[grid[i][j] - 1] > 1)
      {
        return false;
      }
    }
  }
  return true;
}

bool verifySquare(const Grid &grid, int n)
{
  // std::cout << "n = " << n << std::endl;
  std::array<int, 9> count;
  for (int i(0); i < 9; i++)
  {
    count[i] = 0;
  }

  for (int i(0); i < 3; i++)
  {
    for (int j(0); j < 3; j++)
    {
      int p((int)(n / 3) * 3 + i), q((n % 3) * 3 + j);
      // std::cout << "(" << p << "," << q << "): " << grid[p][q] << ",";
      if (grid[p][q] != 0)
      {
        count[grid[p][q] - 1] += 1;
        if (count[grid[p][q] - 1] > 1)
        {
          // std::cout << std::endl;
          return false;
        }
      }
    }
  }
  // std::cout << std::endl;
  return true;
}

bool verifyGrid(const Grid &grid)
{
  for (int k(0); k < 9; k++)
  {
    if (!(verifyLine(grid, k) && verifyColumn(grid, k) && verifySquare(grid, k)))
    {
      return false;
    }
  }
  return true;
}

std::array<int, 9> getLineValues(const Grid &grid, int i)
{
  std::array<int, 9> values;
  for (int j(0); j < 9; j++)
  {
    values[j] = 1;
  }
  for (int j(0); j < 9; j++)
  {
    if (grid[i][j] != 0)
    {
      values[grid[i][j] - 1] = 0;
    }
  }
  // for(int j(0); j<9; j++){
  //     std::cout << values[j] << ",";
  // }
  // std::cout << std::endl;
  return values;
}

std::array<int, 9> getColumnValues(const Grid &grid, int j)
{
  std::array<int, 9> values;
  for (int i(0); i < 9; i++)
  {
    values[i] = 1;
  }
  for (int i(0); i < 9; i++)
  {
    if (grid[i][j] != 0)
    {
      values[grid[i][j] - 1] = 0;
    }
  }
  // for(int i(0); i<9; i++){
  //     std::cout << values[i] << ",";
  // }
  // std::cout << std::endl;
  return values;
}

std::array<int, 9> getSquareValues(const Grid &grid, int n)
{
  std::array<int, 9> values;
  for (int j(0); j < 9; j++)
  {
    values[j] = 1;
  }
  for (int i(0); i < 3; i++)
  {
    for (int j(0); j < 3; j++)
    {
      int p((int)(n / 3) * 3 + i), q((n % 3) * 3 + j);
      if (grid[p][q] != 0)
      {
        values[grid[p][q] - 1] = 0;
      }
    }
  }
  // for(int i(0); i<9; i++){
  //     std::cout << values[i] << ",";
  // }
  // std::cout << std::endl;
  return values;
}

std::vector<int> getCaseValues(const Grid &grid, int i, int j)
{
  auto lv(getLineValues(grid, i)), cv(getColumnValues(grid, j)), sv(getSquareValues(grid, (int)(i / 3) * 3 + (j / 3)));
  std::vector<int> values;
  for (int k(0); k < 9; k++)
  {
    if (lv[k] && cv[k] && sv[k])
    {
      values.push_back(k + 1);
    }
  }
  return values;
}

bool isGridSolvent(const Grid &grid)
{
  Grid tmpGrid;
  for (int i(0); i < 9; i++)
  {
    for (int j(0); j < 9; j++)
    {
      tmpGrid[i][j] = grid[i][j];
    }
  }
  // printGrid(tmpGrid);
  bool hasChanged(true);
  while (hasChanged)
  {
    hasChanged = false;
    for (int i(0); i < 9; i++)
    {
      for (int j(0); j < 9; j++)
      {
        auto values = getCaseValues(tmpGrid, i, j);
        if (values.size() == 1 && tmpGrid[i][j] == 0)
        {
          tmpGrid[i][j] = values[0];
          hasChanged = true;
        }
      }
    }
    // printGrid(tmpGrid);
    // std::system("read");
  }
  for (int i(0); i < 9; i++)
  {
    for (int j(0); j < 9; j++)
    {
      if (tmpGrid[i][j] == 0)
      {
        return false;
      }
    }
  }
  return true;
}

std::pair<Grid, Grid> generatePlayableGrid(int n, int seed)
{
  Grid solution(generateValidGrid());
  printGrid(solution);
  Grid grid;
  for (int i(0); i < 9; i++)
  {
    for (int j(0); j < 9; j++)
    {
      grid[i][j] = solution[i][j];
    }
  }
  std::vector<int> indexs;
  for (int i(0); i < 81; i++)
  {
    indexs.push_back(i + 1);
  }
  while (n != 0 && indexs.size() != 0)
  {
    int ri(randRange(0, indexs.size() - 1));
    int index(indexs[ri]);
    int tmpValue = grid[index / 9][index % 9];
    grid[index / 9][index % 9] = 0;
    // printGrid(grid);
    if (!isGridSolvent(grid))
    {
      grid[index / 9][index % 9] = tmpValue;
    }
    else
    {
      // std::cout << "Value removed !" << std::endl;
      n--;
    }
    indexs.erase(indexs.begin() + ri);
  }
  //std::cout << n << " values have not been removed from grid" << std::endl;
  //printGrid(grid);
  return std::pair<Grid, Grid>(grid, solution);
}