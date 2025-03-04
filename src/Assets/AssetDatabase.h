#ifndef ASSETDATABASE_H
#define ASSETDATABASE_H

#include <map>
#include <string>
#include <SDL.h>

class AssetDatabase {
public:
    AssetDatabase();

    ~AssetDatabase();

    void AddTexture(SDL_Renderer *renderer, const std::string &id, const std::string &filePath);

    SDL_Texture *GetTexture(const std::string &id) const;

    void Clear();

private:
    std::map<std::string, SDL_Texture *> _textures;
};

#endif //ASSETDATABASE_H
