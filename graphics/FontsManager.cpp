#include "FontsManager.hpp"

bool FontsManager::addFont(std::string key, TTF_Font *font)
{
    // In this case, the key is already used.
    if (m_fonts.count(key) != 0)
        return false;

    m_fonts[key] = std::shared_ptr<TTF_Font>(font, &TTF_CloseFont);
    return true;
}

std::shared_ptr<TTF_Font> FontsManager::getFont(std::string key) const
{
    // If key does not exists, we return a null shared ptr.
    if (m_fonts.count(key) == 0)
        return std::shared_ptr<TTF_Font>(nullptr);

    return m_fonts.at(key);
}

void FontsManager::clear()
{
    m_fonts.clear();
}
