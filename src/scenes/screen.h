#ifndef SCREEN_H
#define SCREEN_H

#include <raylib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory> // Für std::unique_ptr
#include "wall_collisions.h" // Enthält jetzt Wall und Movable_Wall

class Screen
{
public:
    nlohmann::json tileset_Description;
    nlohmann::json levelmap;
    Texture2D tileatlas_Texture{};
    int columns = 1; // Sicherer Standardwert

    // Kachelmaße (werden im Konstruktor aus JSONs geladen)
    int map_tile_width = 16;
    int map_tile_height = 16;
    int tileset_tile_width = 16;
    int tileset_tile_height = 16;

    // ÄNDERUNG: Speichert jetzt unique_ptr auf Wall-Objekte für Polymorphie
    std::vector<std::unique_ptr<game::core::Wall>> walls;

    Screen();
    ~Screen(); // Stellt sicher, dass unique_ptr die Objekte korrekt freigibt

    void draw_Level() const;
    void Load_Object_Layers();

    // Helfer-Methode, um einen Vektor von rohen Zeigern für Kollisionschecks zu bekommen,
    // wenn die aufrufende Funktion das braucht und nicht direkt mit unique_ptr arbeiten kann/soll.
    // Achtung: Die Lebenszeit dieser Zeiger ist an die unique_ptr im 'walls'-Vektor gebunden.
    std::vector<game::core::Wall*> Get_Wall_Pointers() const
    {
        std::vector<game::core::Wall*> wall_pointers;
        for (const auto& wall_ptr : walls)
        {
            wall_pointers.push_back(wall_ptr.get());
        }
        return wall_pointers;
    }
};

#endif //SCREEN_H