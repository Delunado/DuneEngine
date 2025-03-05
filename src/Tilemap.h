#ifndef TILEMAP_H
#define TILEMAP_H

#include <json.hpp>

class Tilemap {
public:
    struct TileData {
        int x;
        int y;
        int tilesetCoordX;
        int tilesetCoordY;
    };

    Tilemap() = default;

    ~Tilemap() = default;

    bool LoadFromLDtk(const std::string &filename);

    const std::vector<TileData> &GetTiles() const;

private:
    bool ParseLevel(const nlohmann::json &level);

    bool ParseLayer(const nlohmann::json &layer);

    std::vector<TileData> _tiles;
};

#endif //TILEMAP_H
