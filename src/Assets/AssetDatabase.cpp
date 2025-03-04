#include "AssetDatabase.h"

#include <Logger.h>
#include <SDL_image.h>

AssetDatabase::AssetDatabase() {
    Logger::Log("AssetDatabase created");
}

AssetDatabase::~AssetDatabase() {
    Logger::Log("AssetDatabase destroyed");
}

void AssetDatabase::AddTexture(SDL_Renderer *renderer, const std::string &id, const std::string &filePath) {
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    _textures.emplace(id, texture);

    SDL_FreeSurface(surface);
}

SDL_Texture *AssetDatabase::GetTexture(const std::string &id) const {
    return _textures.at(id);
}

void AssetDatabase::Clear() {
    for (const auto &[id, texture]: _textures) {
        SDL_DestroyTexture(texture);
    }

    _textures.clear();
}