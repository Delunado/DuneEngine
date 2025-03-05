//
// Created by fjavi on 05/03/2025.
//

#include "Tilemap.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

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

    if (project.find("levels") != project.end()) {
        for (const auto &level: project["levels"]) {
            ParseLevel(level);
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

bool Tilemap::ParseLevel(const nlohmann::json &level) {
    if (level.contains("layerInstances") && !level["layerInstances"].is_null()) {
        for (const auto &layer: level["layerInstances"]) {
            if (std::string type = layer.value("__type", ""); type == "Tiles" || type == "AutoLayer") {
                ParseLayer(layer);
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
            data.x = tile["px"][0].get<int>() + offsetX;
            data.y = tile["px"][1].get<int>() + offsetY;
            data.tilesetCoordX = tile["src"][0].get<int>();
            data.tilesetCoordY = tile["src"][1].get<int>();

            _tiles.push_back(data);
        }
    }

    return true;
}
