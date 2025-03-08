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
        bool hasCollision;
    };

    Tilemap() = default;

    ~Tilemap() = default;

    bool LoadFromLDtk(const std::string &filename);

    const std::vector<TileData> &GetTiles() const;

private:
    bool ParseLevel(const nlohmann::json &level);

    bool ParseLayer(const nlohmann::json &layer);

    bool ParseIntGrid(const nlohmann::json &layer);

    std::vector<TileData> _tiles;
    std::vector<bool> _collisionGrid;
    int _collisionGridWidth; // number of cells horizontally
    int _collisionGridHeight; // number of cells vertically
    int _collisionGridSize; // cell size (in pixels)
};

#endif //TILEMAP_H
