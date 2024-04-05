#ifndef TEXTURESMANAGER_H_INCLUDED
#define TEXTURESMANAGER_H_INCLUDED

#include <memory>
#include <map>
#include <string>

#include <SDL2/SDL.h>

class TexturesManager
{
private:
    std::map<std::string, std::shared_ptr<SDL_Texture>> m_textures;
public:
    bool addTexture(std::string key, SDL_Texture *texture);
    std::shared_ptr<SDL_Texture> getTexture(std::string key) const;
};

#endif // TEXTURESMANAGER_H_INCLUDED