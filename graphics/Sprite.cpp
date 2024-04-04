#include "Sprite.hpp"

Sprite::Sprite(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<SDL_Texture> texture, Vector2i pos, Vector2i size): 
    m_renderer(renderer), m_texture(texture), m_pos(pos), m_size(size)
{
}

void Sprite::setTexture(std::shared_ptr<SDL_Texture> texture)
{
    m_texture = std::shared_ptr<SDL_Texture>(texture);
}

void Sprite::draw()
{
    SDL_Rect dstRect = {m_pos.x, m_pos.y, m_size.x, m_size.y};
    SDL_RenderCopy(m_renderer.get(), m_texture.get(), nullptr, &dstRect);
}

const Vector2i &Sprite::getPosition() const
{
    return m_pos;
}

const Vector2i &Sprite::getSize() const
{
    return m_size;
}
