#include "TexturesManager.hpp"

bool TexturesManager::addTexture(std::string key, SDL_Texture *texture)
{
    // In this case, the key is already used.
    if (m_textures.count(key) != 0)
        return false;

    m_textures[key] = std::shared_ptr<SDL_Texture>(texture, &SDL_DestroyTexture);
    return true;
}

std::shared_ptr<SDL_Texture> TexturesManager::getTexture(std::string key) const
{
    // If key does not exists, we return a null shared ptr.
    if (m_textures.count(key) == 0)
        return std::shared_ptr<SDL_Texture>(nullptr);

    return m_textures.at(key);
}

void TexturesManager::clear()
{
    m_textures.clear();
}
