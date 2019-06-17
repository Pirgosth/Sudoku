#include <iostream>

#include <Stone/Stone.h>

#include "game.h"

Case* getCurrentCase(const std::array<std::array<Case*, 9>, 9> &grid, const Pos &pos);

bool isNumberKey(int key);
void refreshCurrent(Case* current, int key);

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    Font::Init();
    SDL_Window *window(0);
    SDL_Renderer *renderer(0);
    window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 9*CASE_LENGTH+10*CASE_SPACE, 9*CASE_LENGTH+10*CASE_SPACE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    SpriteManager manager(window);
    AbstractSprite::setSpriteManager(manager);

    Case::loadTexture();

    Grid grille(generateValidGrid(81));
    printGrid(grille);

    std::array<std::array<Case*, 9>, 9> test;
    for(int i(0); i<9; i++){
        for(int j(0); j<9; j++){
            test[i][j] = new Case({j*CASE_LENGTH+(j+1)*CASE_SPACE, i*CASE_LENGTH+(i+1)*CASE_SPACE}, grille[i][j]);
            if(grille[i][j] != 0){
                test[i][j]->lock();
            }
        }
    }

    std::cout << "Cases créées" << std::endl;

    std::cout << verifyLine(grille, 0) << std::endl;
    std::cout << verifyColumn(grille, 0) << std::endl;
    std::cout << verifySquare(grille, 0) << std::endl;
    std::cout << verifyGrid(grille) << std::endl;

    Case* current(0);

    // for(int i(0); i<9; i++){
    //     for(int j(0); j<9; j++){
    //         std::cout << (j)/3+((int)((i)/3))*3 << std::endl;
    //     }
    // }

    bool quit(false);
    SDL_Event event;
    
    while(!quit){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    switch(event.button.button){
                        case SDL_BUTTON_LEFT:
                            {Pos tmp;
                            SDL_GetMouseState(&tmp.x, &tmp.y);
                            current = getCurrentCase(test, tmp);}
                            std::cout << current << std::endl;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    if(isNumberKey(event.key.keysym.sym)){
                        std::cout << "Number Key pressed" << std::endl;
                        refreshCurrent(current, event.key.keysym.sym);
                    }
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        manager.draw();

        SDL_RenderPresent(renderer);
    }

    for(int i(0); i<9; i++){
        for(int j(0); j<9; j++){
            delete test[i][j];
        }
    }

    Case::destroyTexture();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Font::Quit();
    SDL_Quit();

    return 0;
}

Case* getCurrentCase(const std::array<std::array<Case*, 9>, 9> &grid, const Pos &pos){
    // float i1((float)(pos.x)/(float)CASE_SPACE), i2((float)(pos.x)/(float)(CASE_SPACE+CASE_LENGTH));
    int x(-1), y(-1);
    for(int k(1); k<10; k++){
        if(pos.x > (k*CASE_SPACE + (k-1)*CASE_LENGTH) && pos.x < k*(CASE_SPACE+CASE_LENGTH)){
            x=k-1;
        }
    }
    for(int k(1); k<10; k++){
        if(pos.y > (k*CASE_SPACE + (k-1)*CASE_LENGTH) && pos.y < k*(CASE_SPACE+CASE_LENGTH)){
            y=k-1;
        }
    }
    if(x == -1 || y == -1 || grid[y][x]->isLocked()){
        return nullptr;
    }
    return grid[y][x];
    // std::cout << "Mouse is in case " << x << ":" << y << std::endl;
}

bool isNumberKey(int key){
    std::array<int, 10> numberKeys = {{SDLK_KP_0, SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_7, SDLK_KP_8, SDLK_KP_9}};
    for(int i(0); i<10; i++){
        if(key == numberKeys[i]){
            return true;
        }
    }
    return false;
}

void refreshCurrent(Case* current, int key){
    std::array<int, 10> numberKeys = {{SDLK_KP_0, SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_7, SDLK_KP_8, SDLK_KP_9}};
    int index(-1);
    for(int i(0); i<10; i++){
        if(key == numberKeys[i]){
            index = i;
        }
    }
    if(current != 0){
        current->setValue(index);
    }
}