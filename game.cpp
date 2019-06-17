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

Grid generateValidGrid(int n){
    static int timeout(SDL_GetTicks());
    std::random_device rd;
    std::mt19937 gen(rd());

    Grid grille(createEmptyGrid());

    std::vector<int> indexs(81, 0);
    for(int i(0); i<81; i++){
        indexs[i] = i;
    }
    std::vector<int> values(81, 0);
    for(int i(0); i<9; i++){
        for(int j(0); j<9; j++){
            values[i*9+j] = i+1;
        }
    }

    int stuckCount(0);
    float pMax(0.0f);

    for(int k(0); k<n; k++){
        if(stuckCount > 100){
            pMax = std::max<float>(pMax, (81.0-(float)(indexs.size()))*(100.0/81.0));
            k = 0;
            grille = createEmptyGrid();
            indexs = std::vector<int>(81, 0);
            values = std::vector<int>(81, 0);
            for(int i(0); i<81; i++){
                indexs[i] = i;
            }
            for(int i(0); i<9; i++){
                for(int j(0); j<9; j++){
                    values[i*9+j] = i+1;
                }
            }
        }
        if(SDL_GetTicks()-timeout > 300){
            std::cout << "Generating grid " << (81.0-(float)(indexs.size()))*(100.0/81.0) << "%  Max percentage reach: " << pMax << "%" << std::endl;
            timeout = SDL_GetTicks();
        }
        std::uniform_int_distribution<> dis(0, 80-k);
        int ri(dis(gen)), rv(dis(gen));
        int i(indexs[ri]), v(values[rv]);
        int p(i%9), q(i/9);
        grille[p][q] = v;
        if(!(verifyGrid(grille))){
            k--;
            stuckCount++;
            grille[p][q] = 0;
            continue;
        }
        stuckCount = 0;
        indexs.erase(indexs.begin()+ri);
        values.erase(values.begin()+rv);
    }
    return grille;
}

SDL_Texture* Case::g_texture(0);
Font* Case::g_font(0);

void Case::loadTexture(){
    g_texture = _load_texture_from_file(Case::g_manager->getContext().renderer, "./sources/default.png");
    g_font = new Font("/usr/share/fonts/TTF/DejaVuSans.ttf");
    std::cout << g_manager << std::endl;
}

void Case::destroyTexture(){
    delete g_font;
    SDL_DestroyTexture(g_texture);
}

Case::Case(Pos pos, int i): Sprite(g_texture, pos, {CASE_LENGTH, CASE_LENGTH}), TextSprite("a", pos, {}, *g_font, {255, 255, 255, 255}, 30, {0, 0, 0, 0}){
    setValue(i);
}

void Case::draw(){
    Sprite::draw();
    if(TextSprite::isEnabled()){
        SDL_Rect tmp = {Sprite::m_pos.x+(Sprite::m_size.w-TextSprite::m_size.w)/2, Sprite::m_pos.y +(Sprite::m_size.h-TextSprite::m_size.h)/2, TextSprite::m_size.w, TextSprite::m_size.h};
        SDL_RenderCopy(g_manager->getContext().renderer, TextSprite::m_texture, 0, &tmp);
        SDL_SetRenderDrawColor(g_manager->getContext().renderer, 255, 255, 255, 255);
        // tmp.x --;
        // tmp.y --;
        // tmp.w +=2;
        // tmp.h +=2;
        // SDL_RenderDrawRect(g_manager->getContext().renderer, &tmp);
    }
}

int Case::getValue(){
    return m_value;
}

void Case::setValue(int value){
    if(value == 0){
        TextSprite::disable();
    }
    else{
        TextSprite::enable();
    }
    m_value = value;
    TextSprite::setText(std::to_string(m_value));
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

Case::~Case(){}

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
    std::array<int, 9> count;
    for(int i(0); i<9; i++){
        count[i] = 0;
    }

    for(int i(0); i<3; i++){
        for(int j(0); j<3; j++){
            if(grid[n%3+i][n/3+j] != 0){
                count[grid[n%3+i][n/3+j]-1] += 1;
                if(count[grid[n%3+i][n/3+j]-1]> 1){
                    return false;
                }
            }
        }
    }   
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