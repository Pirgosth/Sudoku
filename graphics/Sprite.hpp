#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <memory>

#include <SDL2/SDL.h>

#include "maths.hpp"

class Sprite
{
private:
    std::shared_ptr<SDL_Renderer> m_renderer;
    std::shared_ptr<SDL_Texture> m_texture;

    Vector2i m_pos;
    Vector2i m_size;

public:
    Sprite(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<SDL_Texture> texture, Vector2i pos = {0, 0}, Vector2i size = {0, 0});
    void setTexture(std::shared_ptr<SDL_Texture> texture);
    void draw();
    const Vector2i &getPosition() const;
    const Vector2i &getSize() const;
};

#endif // SPRITE_H_INCLUDED