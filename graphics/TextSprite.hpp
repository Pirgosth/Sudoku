#ifndef TEXTSPRITE_H_INCLUDED
#define TEXTSPRITE_H_INCLUDED

#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "maths.hpp"

class TextSprite
{
private:
    std::shared_ptr<SDL_Renderer> m_renderer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
    std::shared_ptr<TTF_Font> m_font;
    std::string m_text;
    SDL_Color m_color;
    Vector2i m_pos;
    Vector2i m_size;

    bool m_enabled;

    void reloadTextTexture();
public:
    TextSprite(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<TTF_Font> font, Vector2i pos, std::string text, SDL_Color color);
    void draw();
    void setText(const std::string &text);
    void setPosition(Vector2i pos);
    const Vector2i &getSize() const;
    void enable();
    void disable();
    bool isEnabled() const;
};

#endif // TEXTSPRITE_H_INCLUDED