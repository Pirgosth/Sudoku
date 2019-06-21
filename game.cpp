#include "game.h"

Grid createEmptyGrid(){
    return Grid();
}

void printGrid(const Grid &grid){
    std::cout << "[";
    for(int i(0); i<9; i++){
        if(i != 0){
            std::cout << " ";
        }
        std::cout << "[";
        for(int j(0); j<9; j++){
            if(j == 8){
                std::cout << grid[i][j] << "]";
            }
            else{
                std::cout << grid[i][j] << ",";
            }
        }
        if(i == 8){
            std::cout <<  "]" << std::endl;
        }
        else{
            std::cout << ",\n";
        }
    }
}

Grid generateValidGrid(int n, int *nodeCount){

    Grid grid(createEmptyGrid());

    Branch<9> tree(true, 1);
    for(int i(0); i<9; i++){
        tree.addNode(new Branch<9>(true, 9), i);
    }

    std::vector<Branch<9>*> nodesHistory;
    std::vector<int> valuesHistory;

    Branch<9>* currentNode(&tree);

    std::random_device rd;
    std::mt19937 gen(rd());

    int nodesReached(0);

    for(int k(0); k<n; k++){
        //Generate vector of validValues for the k-th element
        std::vector<std::pair<int, int>> validValues;
        if(currentNode!=0){
            for(int i(0); i<9; i++){
                if((*currentNode)[i] != 0 && (*currentNode)[i]->isValid()){
                    validValues.push_back(std::pair<int, int>(i+1, (*currentNode)[i]->getCount()));
                    // std::cout << "(" << i+1 << "," << (*currentNode)[i]->getCount() << ")" << std::endl;
                }
            }
        }

        // std::cout << "There's: " << validValues.size() << " validValues" << std::endl;

        int currentValue(-1);
        bool isNodeValid(true);

        do{
            nodesReached++;
            //If there is no more validValues, node is invalid
            if(validValues.size() == 0){
                isNodeValid = false;
                // std::cout << "No more validValues in list, abort current Node" << std::endl;
                break;
            }

            //Create uniform distribution to get random value from validValues
            std::uniform_int_distribution<> dis(0, validValues.size()-1);
            int rv(dis(gen));
            grid[k/9][k%9] = validValues[rv].first;

            //We decrease the value selected
            validValues[rv].second--;

            //If there's no more count of a value, we remove it from the list of validValues
            if(validValues[rv].second <= 0){
                validValues.erase(validValues.begin()+rv);
            }

        }while(!(verifyLine(grid, k/9) && verifyColumn(grid, k%9) && verifySquare(grid, (int)((k/9)/3)*3+((k%9)/3))));
        // printGrid(grid);
        // getLineValues(grid, k/9);
        // getColumnValues(grid, k%9);
        // getSquareValues(grid, (int)((k/9)/3)*3+((k%9)/3));
        // std::cout << "Carre: " << (int)((k/9)/3)*3+((k%9)/3) << "\tligne: " << k/9 << "\tcolonne: " << k%9 << std::endl;
        // std::system("read");
        // }while(!verifyGrid(grid));
        if(!isNodeValid){
            // printGrid(grid);
            currentNode->setValidity(false);
            currentNode = nodesHistory[nodesHistory.size()-1];
            nodesHistory.pop_back();
            valuesHistory.pop_back();
            grid[k/9][k%9] = 0;
            k-=2;
            continue;
        }
        if(k < 80){
            currentValue = grid[k/9][k%9];
            std::array<int, 9> lv(getLineValues(grid, (k+1)/9)), cv(getColumnValues(grid, (k+1)%9)), sv(getSquareValues(grid, (int)(((k+1)/9)/3)*3+(((k+1)%9)/3)));

            for(int i(0); i<9; i++){
                bool validity(true);
                validity = lv[i] == 1 && cv[i] == 1 && sv[i] == 1;
                (*currentNode)[currentValue-1]->addNode(new Branch<9>(validity, i == currentValue-1 ? (*currentNode)[i]->getCount()-1: (*currentNode)[i]->getCount()), i);
            }

            valuesHistory.push_back(currentValue);
            nodesHistory.push_back(currentNode);
            currentNode = (*currentNode)[currentValue-1];
        }
    }
    if(nodeCount != 0)
      *nodeCount = nodesReached;
    // std::cout << "Grid generated after reaching: " << nodesReached << " nodes" << std::endl;
    return grid;
}

float averageGridNodeCount(const int n){
    std::vector<int> values;
    float totalNodeCount(0);
    int currentNodeCount(0);
    int timeout = SDL_GetTicks();
    for(int i(0); i<n; i++){
        generateValidGrid(81, &currentNodeCount);
        // std::cout << currentNodeCount << std::endl;
        values.push_back(currentNodeCount);
        totalNodeCount+=currentNodeCount;
    }
    float average(totalNodeCount/n);
    float sigma(0);
    for(int i(0); i<(int)values.size(); i++){
        sigma += std::pow(values[i]-average, 2);
    }
    sigma /= n;
    sigma = std::sqrt(sigma);
    float confiance = 2*sigma/(std::sqrt(n));
    float xmin(average-confiance), xmax(average+confiance);
    std::cout << "Simulation took: " << (SDL_GetTicks()-timeout)/1000.0 <<  " seconds" << std::endl;
    std::cout << "Confidence interval: [" << xmin << ";" << xmax << "]" << std::endl;
    return average;
}

SDL_Texture* Case::g_texture_default(0);
SDL_Texture* Case::g_texture_selected(0);
Font* Case::g_font(0);

void Case::loadTexture(const SpriteManager &manager){
    g_texture_default = _load_texture_from_file(manager.getContext().renderer, "./sources/default.png");
    g_texture_selected = _load_texture_from_file(manager.getContext().renderer, "./sources/selected.png");
    g_font = new Font("/usr/share/fonts/TTF/DejaVuSans.ttf");
}

void Case::destroyTexture(){
    delete g_font;
    SDL_DestroyTexture(g_texture_selected);
    SDL_DestroyTexture(g_texture_default);
}

Case::Case(Pos pos, int i){
    m_sprite = new Sprite(g_texture_default, pos, {CASE_LENGTH, CASE_LENGTH});
    m_texte = new TextSprite("a", pos, {}, *g_font, {255, 255, 255, 255}, 30, {0, 0, 0, 0});
    m_texte->pos({m_sprite->pos().x+(m_sprite->size().w-m_texte->size().w)/2, m_sprite->pos().y +(m_sprite->size().h-m_texte->size().h)/2});
    setValue(i);
}

void Case::draw(){
    m_sprite->draw();
    if(m_texte->isEnabled()){
        m_texte->draw();
    }
}

int Case::getValue(){
    return m_value;
}

void Case::setValue(int value){
    if(value == 0){
        m_texte->disable();
    }
    else{
        m_texte->enable();
    }
    m_value = value;
    m_texte->setText(std::to_string(m_value));
}

void Case::lock(){
    m_isLocked = true;
}

void Case::unlock(){
    m_isLocked = false;
}

bool Case::isLocked(){
    return m_isLocked;
}

Case::~Case(){
    delete m_texte;
    delete m_sprite;
}

void Case::setIsSelected(bool isSelected){
    if(isSelected){
        m_sprite->setTexture(g_texture_selected);
    }
    else{
        m_sprite->setTexture(g_texture_default);
    }
}

bool verifyLine(const Grid &grid, int i){
    std::array<int, 9> count;
    for(int j(0); j<9; j++){
        count[j] = 0;
    }
    for(int j(0); j<9; j++){
        if(grid[i][j] != 0){
            count[grid[i][j]-1] += 1;
            if(count[grid[i][j]-1] > 1){
                return false;
            }
        }
    }
    return true;
}

bool verifyColumn(const Grid &grid, int j){
    std::array<int, 9> count;
    for(int i(0); i<9; i++){
        count[i] = 0;
    }
    for(int i(0); i<9; i++){
        if(grid[i][j] != 0){
            count[grid[i][j]-1] += 1;
            if(count[grid[i][j]-1] > 1){
                return false;
            }
        }
    }
    return true;
}

bool verifySquare(const Grid &grid, int n){
    // std::cout << "n = " << n << std::endl;
    std::array<int, 9> count;
    for(int i(0); i<9; i++){
        count[i] = 0;
    }

    for(int i(0); i<3; i++){
        for(int j(0); j<3; j++){
            int p((int)(n/3)*3+i), q((n%3)*3+j);
            // std::cout << "(" << p << "," << q << "): " << grid[p][q] << ",";
            if(grid[p][q] != 0){
                count[grid[p][q]-1] += 1;
                if(count[grid[p][q]-1]> 1){
                    // std::cout << std::endl;
                    return false;
                }
            }
        }
    }
    // std::cout << std::endl;    
    return true;
}

bool verifyGrid(const Grid &grid){
    for(int k(0); k<9; k++){
        if(!(verifyLine(grid, k) && verifyColumn(grid, k) && verifySquare(grid, k))){
            return false;
        }
    }
    return true;
}

std::array<int, 9> getLineValues(const Grid &grid, int i){
    std::array<int, 9> values;
    for(int j(0); j<9; j++){
        values[j] = 1;
    }
    for(int j(0); j<9; j++){
        if(grid[i][j] != 0){
            values[grid[i][j]-1] = 0;
        }
    }
    // for(int j(0); j<9; j++){
    //     std::cout << values[j] << ",";
    // }
    // std::cout << std::endl;
    return values;
}

std::array<int, 9> getColumnValues(const Grid &grid, int j){
    std::array<int, 9> values;
    for(int i(0); i<9; i++){
        values[i] = 1;
    }
    for(int i(0); i<9; i++){
        if(grid[i][j] != 0){
            values[grid[i][j]-1] = 0;
        }
    }
    // for(int i(0); i<9; i++){
    //     std::cout << values[i] << ",";
    // }
    // std::cout << std::endl;
    return values;
}

std::array<int, 9> getSquareValues(const Grid &grid, int n){
    std::array<int, 9> values;
    for(int j(0); j<9; j++){
        values[j] = 1;
    }
    for(int i(0); i<3; i++){
        for(int j(0); j<3; j++){
            int p((int)(n/3)*3+i), q((n%3)*3+j);
            if(grid[p][q] != 0){
                values[grid[p][q]-1] = 0;
            }
        }
    }
    // for(int i(0); i<9; i++){
    //     std::cout << values[i] << ",";
    // }
    // std::cout << std::endl;
    return values;
}

template<int N>
Branch<N>::Branch(bool isValid, int count){
    for(auto node = m_nodes.begin(); node!=m_nodes.end(); node++){
        *node = 0;
    }
    m_isValid = isValid;
    m_count = count;
}

template<int N>
Branch<N>::~Branch(){
    for(auto node = m_nodes.begin(); node!=m_nodes.end(); node++){
        Branch* child = *node;
        if(child != 0)
          delete child;
    }
}

template<int N>
void Branch<N>::addNode(Branch *node, int index){
    m_nodes[index] = node;
}
template<int N>
Branch<N>* Branch<N>::operator[](int index){
    return m_nodes[index];
}

template<int N>
bool Branch<N>::isValid(){
    return m_isValid;
}

template<int N>
void Branch<N>::setValidity(bool validity){
    m_isValid = validity;
    // if(!validity){
    //     for(auto node = m_nodes.begin(); node != m_nodes.end(); node++){
    //         delete (*node);
    //         (*node) = 0;
    //     }
    // }
}

template<int N>
int Branch<N>::getCount(){
    return m_count;
}