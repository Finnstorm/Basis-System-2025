#include "Screen.h"
#include <iostream>

extern Object_Manager g_objectManager;
// in main erstellen: object_Manager g_objectManager;

Screen::Screen()
{
    tson::Tileson parser;
    map = parser.parse(levelmap_Path);

    if (map == nullptr)
    {
        std::cerr << "Failed to open or parse level map " << levelmap_Path << std::endl;
        return;
    }

    if (map->getStatus() != tson::ParseStatus::OK)
    {
            std::cerr << "Error parsing map: " << map->getStatusMessage() << std::endl;
        return;
    }

    for (auto& tileset : map->getTilesets())
    {
        std::string image_Path = "assets/maps/" + tileset.getImagePath().string();
        tileatlas_Texture = LoadTexture(image_Path.c_str());
        if (tileatlas_Texture.id == 0)
        {
            std::cerr << "Failed to load tile atlas texture: " << image_Path << std::endl;
        }
        break;
    }
}

Screen::~Screen()
{
    if (tileatlas_Texture.id != 0)
    {
        UnloadTexture(tileatlas_Texture);
    }
}


void Screen::LoadGameObjects(bool includeEnemySpawnersAsCollidables)
{
    if (map == nullptr)
    {
        std::cerr << "Kann Spielobjekte nicht laden: Karte ist nicht geladen." << std::endl;
        return;
    }

    for (auto& layer : map->getLayers())
    {
        if (layer.getType() == tson::LayerType::ObjectGroup)
        {
            const std::string& layer_Name = layer.getName();

            for (auto& object : layer.getObjects())
            {

                if (layer_Name == "walls")
                {
                    Vector2 temp_pos;
                    temp_pos.x = static_cast<float>(object.getPosition().x);
                    temp_pos.y = static_cast<float>(object.getPosition().y);
                    Vector2 temp_size;
                    temp_size.x = static_cast<float>(object.getSize().x);
                    temp_size.y = static_cast<float>(object.getSize().y);

                    Walls* new_wall = new Walls(temp_pos, temp_size /*, ggf. weitere Konstruktor-Parameter */);
                    g_objectManager.AddObject(new_wall);
                    std::cout << "Wand zu Object Manager hinzugefügt: " << new_wall << std::endl;
                }
                else if (layer_Name == "consumable")
                {
                    Vector2 temp_pos;
                    temp_pos.x = static_cast<float>(object.getPosition().x);
                    temp_pos.y = static_cast<float>(object.getPosition().y);

                    Consumables* new_consumable = new Consumables(temp_pos, object.getName() /*, ggf. weitere Konstruktor-Parameter */);
                    g_objectManager.AddObject(new_consumable);
                    std::cout << "Verbrauchsgegenstand zu Object Manager hinzugefügt: " << new_consumable << std::endl;
                }



                if (object.getGid() > 0)
                {
                    tson::Tile* tile = nullptr;
                    for (auto& tileset : map->getTilesets())
                    {
                        if (object.getGid() >= tileset.getFirstgid() &&
                            object.getGid() < (tileset.getFirstgid() + tileset.getTileCount()))
                        {
                            uint32_t localTileId = object.getGid() - tileset.getFirstgid();
                            tile = tileset.getTile(localTileId);
                            if (tile != nullptr) break;
                        }
                    }
                    if (tile != nullptr)
                    {
                        tson::Rect drawingRect = tile->getDrawingRect();
                        Rectangle srcRect = {
                                static_cast<float>(drawingRect.x), static_cast<float>(drawingRect.y),
                                static_cast<float>(drawingRect.width), static_cast<float>(drawingRect.height)
                        };
                        Vector2 destPos = {
                                static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y)
                        };
                        DrawTextureRec(tileatlas_Texture, srcRect, destPos, WHITE);
                    }
                }
            }
        }
    }
}


void Screen::Draw_Level() const
{
    if (map == nullptr)
    {
        std::cerr << "Kann Level nicht zeichnen: Karte nicht geladen." << std::endl;
        return;
    }

    for (auto& layer : map->getLayers())
    {
        if (layer.getType() == tson::LayerType::TileLayer)
        {
            auto& tile_Layer = layer.getTileData();
            for (const auto& pair : tile_Layer)
            {
                int x = std::get<0>(pair.first);
                int y = std::get<1>(pair.first);
                tson::Tile* tile = pair.second;

                if (tile != nullptr) {
                    tson::Rect drawingRect = tile->getDrawingRect();
                    tson::Vector2f worldPos = {
                            static_cast<float>(x * map->getTileSize().x),
                            static_cast<float>(y * map->getTileSize().y)
                    };

                    Rectangle srcRect = {
                            static_cast<float>(drawingRect.x), static_cast<float>(drawingRect.y),
                            static_cast<float>(drawingRect.width), static_cast<float>(drawingRect.height)
                    };

                    Vector2 destPos = {
                            static_cast<float>(worldPos.x), static_cast<float>(worldPos.y)
                    };

                    DrawTextureRec(tileatlas_Texture, srcRect, destPos, WHITE);
                }
            }
        }
    }
}