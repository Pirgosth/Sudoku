#ifndef FONTSMANAGER_H_INCLUDED
#define FONTSMANAGER_H_INCLUDED

#include <memory>
#include <map>
#include <string>

#include <SDL2/SDL_ttf.h>

class FontsManager
{
private:
    std::map<std::string, std::shared_ptr<TTF_Font>> m_fonts;
public:
    bool addFont(std::string key, TTF_Font *font);
    std::shared_ptr<TTF_Font> getFont(std::string key) const;
};

#endif // FONTSMANAGER_H_INCLUDED