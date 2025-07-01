#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "raylib.h"
#include "tson/json.hpp"
#include "tson/tileson.h"

#include "../Vectors.h"
#include "../game/Walls.h"
#include "../game/Consumables.h"
#include "Object_Manager.h"




class Screen
{
private:
    std::string levelmap_Path = "assets/maps/level1.json";
    std::unique_ptr<tson::Map> map;
    Texture tileatlas_Texture = {0};

public:
    Screen();
    ~Screen();

    void LoadGameObjects(bool includeEnemySpawnersAsCollidables = false); // VERÄNDERUNG: Parametername angepasst
    void Draw_Level() const;
};