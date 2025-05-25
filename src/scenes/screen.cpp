#include <iostream>
#include <fstream>
#include <string>
#include "screen.h"

Screen::Screen() {

    //Create a Tileson parser Object
    tson::Tileson parser;

    //Attempt to parse Tiled map file
    //the parse function return a std::unique_ptr<tson::Map>
    map = parser.parse(levelmap_Path);

    //Check if the map was loaded successfully
    if (map == nullptr)
    {
        std::cerr << "Failed to open or parse level map" << levelmap_Path << std::endl;
        return;
    }

    //Check if the map version is supported by Tileson
    if (map->getStatus() != tson::ParseStatus::OK)
    {
        std::cerr << "Error parsing map: " << map->getStatusMessage() << std::endl;
        return;
    }

    //Iterate through all tilesets defined in the Tile map
    //A Tilemap can have multiple tilesets. We're loading the texture of the first one
    for (auto& tileset : map->getTilesets())
    {
        std::string image_Path = "assets/maps/" + tileset.getImagePath().string();

        //load texture
        tileatlas_Texture = LoadTexture(image_Path.c_str());
        if (tileatlas_Texture.id == 0)
        {
                std::cerr << "Failed to load tile atlas texture: " << image_Path << std::endl;
        }
        //breaking after one tileset_Texture
        break;
    }
}

Screen::~Screen() {
    if (tileatlas_Texture.id != 0)
    {
        UnloadTexture(tileatlas_Texture);
    }
}

void Screen::draw_Level() const {

    //make sure the map is loaded before drawing
    if (map == nullptr)
    {
        std::cerr << "Cannot draw level: Map not loaded." << std::endl;
        return;
    }

    //Iterate through the map layers
    for (auto& layer : map->getLayers()) {
        if (layer.getType() == tson::LayerType::TileLayer)
        {
            //get Tilelayer Data
            auto& tile_Layer = layer.getTileData();

            //Iterate through each Tile
            for (const auto& pair : tile_Layer)
                //'pair.first' is a std::tuple<int, int> representing the Tile's grid position
                //'pair.second' is a pointer to the tson::Tile object
            {
                // Extract x and y coordinates from the tuple
                int x = std::get<0>(pair.first);
                int y = std::get<1>(pair.first);
                tson::Tile* tile = pair.second;

                if (tile != nullptr) { //Check if there is a tile at the current posiotion
                    tson::Rect drawingRect = tile->getDrawingRect();
                    tson::Vector2f worldPos = {
                        static_cast<float>(x * map->getTileSize().x),
                        static_cast<float>(y * map->getTileSize().y)
                    };

                    Rectangle srcRect = {
                        static_cast<float>(drawingRect.x),
                        static_cast<float>(drawingRect.y),
                        static_cast<float>(drawingRect.width),
                        static_cast<float>(drawingRect.height)
                    };

                    Vector2 destPos = {
                        static_cast<float>(worldPos.x),
                        static_cast<float>(worldPos.y)
                    };

                    DrawTextureRec(tileatlas_Texture, srcRect, destPos, WHITE);
                }
            }
        }
    }
}