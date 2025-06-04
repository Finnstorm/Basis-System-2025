//
// Created by Kruse on 23/05/2025.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include "screen.h"
#include "../config.Puzzle1.h.in"

Screen::Screen() {

    //load tileset json
    std::ifstream tileset_Description_File("assets/maps/test_Tileset.json");
    if (tileset_Description_File.is_open()) {
        tileset_Description_File >> tileset_Description;
        tileset_Description_File.close();
    } else {
        std::cerr << "Failed to open tileset description file!" << std::endl;
    }

    if (tileset_Description.contains("columns") && tileset_Description["columns"].is_number()) {
        columns = tileset_Description["columns"];
        if (columns <= 0) { // Wichtig: Prüfe, ob columns 0 oder negativ ist
            std::cerr << "WARNING: 'columns' in tileset_Description was " << columns << ". Forcing to 1." << std::endl;
            columns = 1; // Setze auf 1, wenn der Wert aus JSON ungültig ist
        }
    } else {
        std::cerr << "ERROR: tileset_Description does not contain 'columns' or 'columns' "
                     "is not a number! Forcing to 1." << std::endl;
        columns = 1; // Fallback
    }

    //load level map
    std::ifstream levelmap_File("assets/maps/test_Levelmap.json");
    if (levelmap_File.is_open()) {
        levelmap_File >> levelmap;
        levelmap_File.close();
    } else {
        std::cerr << "Failed to open level map description File" << std::endl;
        return; // Wichtig: Bei Fehler hier nicht weitermachen
    }

    if (tileset_Description.contains("image")) { // Prüfen, ob "image" existiert
        std::string image_Path_String = tileset_Description["image"]; // Sicherer Zugriff
        tileatlas_Texture = LoadTexture(image_Path_String.c_str());
    } else {
        std::cerr << "Tileset description does not contain 'image' path!" << std::endl;
    }

    Load_Object_Layers();
}


Screen::~Screen()
{
    UnloadTexture(tileatlas_Texture);
}
void Screen::Load_Object_Layers() {
    if (!levelmap.contains("layers") || !levelmap["layers"].is_array()) {
        std::cerr << "ERROR in Load_Object_Layers: 'levelmap' does not contain 'layers' array or 'layers' is not an array!" << std::endl;
        return;
    }

    walls.clear(); // Wichtig: Vor dem Neuladen den Vektor leeren

    for (const auto& layer : levelmap["layers"]) {
        if (layer.contains("type") && layer["type"].is_string() && layer["type"] == "objectgroup" &&
            layer.contains("name") && layer["name"].is_string() && layer["name"] == "Wall" && // Weiterhin die Objektebene "Wall"
            layer.contains("objects") && layer["objects"].is_array()) {

            std::cout << "INFO: Found 'Wall' object layer. Processing objects..." << std::endl;

            for (const auto& object_data : layer["objects"]) {
                float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;

            if (object_data.contains("x") && object_data["x"].is_number()) x = object_data["x"];
            if (object_data.contains("y") && object_data["y"].is_number()) y = object_data["y"];
            if (object_data.contains("width") && object_data["width"].is_number()) width = object_data["width"];   // Für statische "Wall"
            if (object_data.contains("height") && object_data["height"].is_number()) height = object_data["height"]; // Für statische "Wall"

            // Ermittle object_class_type EINMAL
            std::string object_class_type = "Wall"; // Standard-Typ, falls nichts anderes gefunden wird
            if (object_data.contains("class") && object_data["class"].is_string()) {
                object_class_type = object_data["class"];
            } else if (object_data.contains("type") && object_data["type"].is_string()) {
                object_class_type = object_data["type"];
                std::cout << "WARNING: Object used 'type' field instead of 'class'. Value: " << object_class_type << std::endl;
            } else if (object_data.contains("properties") && object_data["properties"].is_array()) {
                for (const auto& prop : object_data["properties"]) {
                    if (prop.contains("name") && prop["name"].is_string() && prop["name"] == "type" &&
                        prop.contains("value") && prop["value"].is_string()) {
                        object_class_type = prop["value"];
                        std::cout << "WARNING: Object used 'type' property. Value: " << object_class_type << std::endl;
                        break;
                    }
                }
            }

            // Definiere initial_pos EINMAL
            Vector2 initial_pos = {x, y};

            std::cout << "  Loading object: Class/Type='" << object_class_type
                      << "', X=" << x << ", Y=" << y;
            if (object_class_type == "Wall") { // Nur für statische Wände die Breite/Höhe aus Tiled loggen
                std::cout << ", W=" << width << ", H=" << height;
            }
            std::cout << std::endl;


            // Jetzt die if/else if Kette basierend auf dem EINMAL ermittelten object_class_type
            if (object_class_type == "Wall") {
                walls.push_back(std::make_unique<game::core::Wall>(
                    Rectangle{x, y, width, height}, // Benutzt x, y, width, height von oben
                    object_class_type
                ));
            }
                    else if (object_class_type == "Movable_Wall_1") {
                        walls.push_back(std::make_unique<game::core::Movable_Wall>(
                            initial_pos, // Startposition aus Tiled
                            Vector2{puzzle1::Config_Puzzle1::movable_Wall_1_Target_X, puzzle1::Config_Puzzle1::movable_Wall_1_Target_Y},
                            puzzle1::Config_Puzzle1::movable_Wall_1_Sprite_Path_Inactive_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_1_Sprite_Path_Inactive_Bottom,
                            puzzle1::Config_Puzzle1::movable_Wall_1_Sprite_Path_Active_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_1_Sprite_Path_Active_Bottom
                        ));
                    }
                    else if (object_class_type == "Movable_Wall_2") {
                        walls.push_back(std::make_unique<game::core::Movable_Wall>(
                            initial_pos, // Startposition aus Tiled
                            Vector2{puzzle1::Config_Puzzle1::movable_Wall_2_Target_X, puzzle1::Config_Puzzle1::movable_Wall_2_Target_Y},
                            puzzle1::Config_Puzzle1::movable_Wall_2_Sprite_Path_Inactive_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_2_Sprite_Path_Inactive_Bottom,
                            puzzle1::Config_Puzzle1::movable_Wall_2_Sprite_Path_Active_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_2_Sprite_Path_Active_Bottom
                        ));
                    }
                    else if (object_class_type == "Movable_Wall_3") {
                        walls.push_back(std::make_unique<game::core::Movable_Wall>(
                            initial_pos, // Startposition aus Tiled
                            Vector2{puzzle1::Config_Puzzle1::movable_Wall_3_Target_X, puzzle1::Config_Puzzle1::movable_Wall_3_Target_Y},
                            puzzle1::Config_Puzzle1::movable_Wall_3_Sprite_Path_Inactive_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_3_Sprite_Path_Inactive_Bottom,
                            puzzle1::Config_Puzzle1::movable_Wall_3_Sprite_Path_Active_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_3_Sprite_Path_Active_Bottom
                        ));
                    }
                    else if (object_class_type == "Movable_Wall_4") {
                        walls.push_back(std::make_unique<game::core::Movable_Wall>(
                            initial_pos, // Startposition aus Tiled
                            Vector2{puzzle1::Config_Puzzle1::movable_Wall_4_Target_X, puzzle1::Config_Puzzle1::movable_Wall_4_Target_Y},
                            puzzle1::Config_Puzzle1::movable_Wall_4_Sprite_Path_Inactive_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_4_Sprite_Path_Inactive_Bottom,
                            puzzle1::Config_Puzzle1::movable_Wall_4_Sprite_Path_Active_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_4_Sprite_Path_Active_Bottom
                        ));
                    }
                    else if (object_class_type == "Movable_Wall_5") {
                        walls.push_back(std::make_unique<game::core::Movable_Wall>(
                            initial_pos, // Startposition aus Tiled
                            Vector2{puzzle1::Config_Puzzle1::movable_Wall_5_Target_X, puzzle1::Config_Puzzle1::movable_Wall_5_Target_Y},
                            puzzle1::Config_Puzzle1::movable_Wall_5_Sprite_Path_Inactive_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_5_Sprite_Path_Inactive_Bottom,
                            puzzle1::Config_Puzzle1::movable_Wall_5_Sprite_Path_Active_Top,
                            puzzle1::Config_Puzzle1::movable_Wall_5_Sprite_Path_Active_Bottom
                        ));
                    }
                else {
                    std::cout << "  WARNING: Unknown object class/type encountered or not yet handled: " << object_class_type << std::endl;
                }
            }
        }
    }
    std::cout << "INFO: Finished processing object layers. Total walls loaded: " << walls.size() << std::endl;
}

void Screen::draw_Level() const
{
    // Sicherstellen, dass levelmap "layers" enthält und es ein Array ist
    if (!levelmap.contains("layers") || !levelmap["layers"].is_array())
    {
        std::cerr << "draw_Level: 'levelmap' does not contain 'layers' or it's not an array." << std::endl;
        return;
    }

    // Kachelmaße aus der Map-Definition (sollten im Screen-Konstruktor initialisiert worden sein)
    // Annahme: tile_width_from_map, tile_height_from_map,
    //          tile_width_from_tileset, tile_height_from_tileset sind Member von Screen
    //          und im Screen-Konstruktor aus levelmap bzw. tileset_Description geladen.
    //          Fürs Erste können wir die hardcodierten 16px annehmen, wenn diese Member noch nicht da sind.
    //          Besser ist es aber, sie aus den JSONs zu laden.
    //          Deine test_Tileset.json und test_Levelmap.json haben beide tilewidth/height: 16.
    int map_tile_width = 16;
    int map_tile_height = 16;
    int tileset_tile_width = 16;
    int tileset_tile_height = 16;

    // Lese sie dynamisch, falls du die Member hinzugefügt hast:
    // if (levelmap.contains("tilewidth") && levelmap["tilewidth"].is_number()) map_tile_width = levelmap["tilewidth"];
    // if (levelmap.contains("tileheight") && levelmap["tileheight"].is_number()) map_tile_height = levelmap["tileheight"];
    // if (tileset_Description.contains("tilewidth") && tileset_Description["tilewidth"].is_number()) tileset_tile_width = tileset_Description["tilewidth"];
    // if (tileset_Description.contains("tileheight") && tileset_Description["tileheight"].is_number()) tileset_tile_height = tileset_Description["tileheight"];


    for (const auto& current_layer : levelmap["layers"])
    {
        // Verarbeite nur Kachelebenen (tilelayer)
        if (current_layer.contains("type") && current_layer["type"].is_string() && current_layer["type"] == "tilelayer" &&
            current_layer.contains("data") && current_layer["data"].is_array() &&
            current_layer.contains("width") && current_layer["width"].is_number() &&
            current_layer.contains("height") && current_layer["height"].is_number())
        {
            int layer_width_in_tiles = current_layer["width"];
            int layer_height_in_tiles = current_layer["height"];
            const auto& tile_data = current_layer["data"];

            for (int y = 0; y < layer_height_in_tiles; ++y)
            {
                for (int x = 0; x < layer_width_in_tiles; ++x)
                {
                    int index = y * layer_width_in_tiles + x;
                    if (index < tile_data.size() && tile_data[index].is_number())
                    {
                        int tiled_g_id = tile_data[index];
                        if (tiled_g_id == 0) continue;

                        int tile_index_in_tileset = tiled_g_id - 1; // Annahme: firstgid ist 1

                        // 'columns' wird im Konstruktor sicher auf >= 1 gesetzt.
                        Rectangle src_rect = {
                            static_cast<float>((tile_index_in_tileset % columns) * tileset_tile_width),
                            static_cast<float>((tile_index_in_tileset / columns) * tileset_tile_height),
                            static_cast<float>(tileset_tile_width),
                            static_cast<float>(tileset_tile_height)
                        };

                        Vector2 pos = { static_cast<float>(x * map_tile_width), static_cast<float>(y * map_tile_height) };

                        // Stelle sicher, dass die Textur gültig ist, bevor sie verwendet wird
                        if (tileatlas_Texture.id != 0) {
                            DrawTextureRec(tileatlas_Texture, src_rect, pos, WHITE);
                        } else {
                            // std::cerr << "draw_Level: tileatlas_Texture is not valid!" << std::endl;
                            // Diese Meldung würde jeden Frame kommen, wenn die Textur fehlt.
                        }
                    }
                }
            }
        }
    }
}
