#include "TextSprite.hpp"

void TextSprite::reloadTextTexture()
{
    if (!m_font)
        return;
    
    SDL_Surface *tmpSurface = TTF_RenderText_Blended(m_font.get(), m_text.c_str(), m_color);
    m_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(SDL_CreateTextureFromSurface(m_renderer.get(), tmpSurface), SDL_DestroyTexture);

    SDL_FreeSurface(tmpSurface);
    SDL_QueryTexture(m_texture.get(), nullptr, nullptr, &m_size.x, &m_size.y);
}

TextSprite::TextSprite(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<TTF_Font> font, Vector2i pos, std::string text, SDL_Color color) : 
    m_renderer(renderer), m_texture(nullptr, SDL_DestroyTexture), m_font(font), m_pos(pos), m_text(text), m_color(color), m_enabled(true)
{
    reloadTextTexture();
}

void TextSprite::draw()
{
    if (!isEnabled())
        return;
    
    SDL_Rect dstRect = {m_pos.x, m_pos.y, m_size.x, m_size.y};
    SDL_RenderCopy(m_renderer.get(), m_texture.get(), nullptr, &dstRect);
}

void TextSprite::setFont(std::shared_ptr<TTF_Font> font)
{
    this->m_font = font;
    reloadTextTexture();
}

void TextSprite::setText(const std::string &text)
{
    m_text = text;
    reloadTextTexture();
}

void TextSprite::setPosition(Vector2i pos)
{
    m_pos = pos;
}

const Vector2i &TextSprite::getSize() const
{
    return m_size;
}

void TextSprite::enable()
{
    m_enabled = true;
}

void TextSprite::disable()
{
    m_enabled = false;
}

bool TextSprite::isEnabled() const
{
    return m_enabled;
}
