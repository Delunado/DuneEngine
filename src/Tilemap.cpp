//
// Created by fjavi on 05/03/2025.
//

#include "Tilemap.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Tilemap::Tilemap(int tilePPU): _tilePPU(tilePPU) {
}

// METER TILEMAPS EN ASSET STORE
bool Tilemap::LoadFromLDtk(const std::string &filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Could not open file: " << filename << "\n";
        return false;
    }

    json project;

    try {
        file >> project;
    } catch (const std::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << "\n";
        return false;
    }

    _tiles.clear();
    _collisionGrid.clear();
    _collisionGridWidth = _collisionGridHeight = _collisionGridSize = 0;

    if (project.find("levels") != project.end()) {
        for (const auto &level: project["levels"]) {
            // First, process the collision (IntGrid) layer, if available.
            for (const auto &layer: level["layerInstances"]) {
                std::string type = layer.value("__type", "");
                if (type == "IntGrid") {
                    ParseIntGrid(layer);
                    break; // assume one collision layer per level
                }
            }
            // Then process all tile layers.
            for (const auto &layer: level["layerInstances"]) {
                std::string type = layer.value("__type", "");
                if (type == "Tiles" || type == "AutoLayer") {
                    ParseLayer(layer);
                }
            }
        }
    } else {
        std::cerr << "No levels found in JSON.\n";
        return false;
    }
    return true;
}

const std::vector<Tilemap::TileData> &Tilemap::GetTiles() const {
    return _tiles;
}

int Tilemap::GetWidth() const {
    return _width;
}

int Tilemap::GetHeight() const {
    return _height;
}

bool Tilemap::ParseLevel(const nlohmann::json &level) {
    if (level.contains("layerInstances") && !level["layerInstances"].is_null()) {
        for (const auto &layer: level["layerInstances"]) {
            if (std::string type = layer.value("__type", ""); type == "Tiles" || type == "AutoLayer") {
                ParseLayer(layer);
            } else if (type == "IntGrid") {
                ParseIntGrid(layer);
            }
        }
    }
    return true;
}

bool Tilemap::ParseLayer(const nlohmann::json &layer) {
    int offsetX = layer.value("pxOffsetX", 0);
    int offsetY = layer.value("pxOffsetY", 0);

    if (layer.contains("gridTiles") && layer["gridTiles"].is_array()) {
        for (const auto &tile: layer["gridTiles"]) {
            TileData data;
            data.x = (tile["px"][0].get<int>() + offsetX)  / _tilePPU;
            data.y = (tile["px"][1].get<int>() + offsetY) / _tilePPU;
            data.tilesetCoordX = tile["src"][0].get<int>();
            data.tilesetCoordY = tile["src"][1].get<int>();

            // If collision info is available, determine if this tile is collidable.
            if (_collisionGridSize > 0 && _collisionGridWidth > 0 && _collisionGridHeight > 0) {
                int cellX = data.x / _collisionGridSize;
                int cellY = data.y / _collisionGridSize;
                if (cellX >= 0 && cellX < _collisionGridWidth &&
                    cellY >= 0 && cellY < _collisionGridHeight) {
                    data.hasCollision = _collisionGrid[cellY * _collisionGridWidth + cellX];
                } else {
                    data.hasCollision = false;
                }
            } else {
                data.hasCollision = false;
            }

            _tiles.push_back(data);
            _width = std::max(_width, data.x);
            _height = std::max(_height, data.y);
        }
    }

    return true;
}

bool Tilemap::ParseIntGrid(const nlohmann::json &layer) {
    _collisionGridWidth = layer.value("__cWid", 0);
    _collisionGridHeight = layer.value("__cHei", 0);
    _collisionGridSize = layer.value("__gridSize", 0);

    if (_collisionGridWidth <= 0 || _collisionGridHeight <= 0) {
        std::cerr << "Invalid grid dimensions in IntGrid layer.\n";
        return false;
    }

    const auto &csv = layer["intGridCsv"];
    if (!csv.is_array() || csv.size() != static_cast<size_t>(_collisionGridWidth * _collisionGridHeight)) {
        std::cerr << "Invalid intGridCsv data.\n";
        return false;
    }

    _collisionGrid.resize(_collisionGridWidth * _collisionGridHeight, false);
    // Mark a cell as collidable if its value is non-zero.
    for (size_t i = 0; i < csv.size(); i++) {
        _collisionGrid[i] = (csv[i].get<int>() != 0);
    }

    return true;
}
