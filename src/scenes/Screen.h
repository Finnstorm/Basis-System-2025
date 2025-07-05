#pragma once

#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "raylib.h"
#include <nlohmann/json.hpp>
#include <../../external/tileson/tileson.hpp>
#include "../Vectors.h"
#include "../game/Walls.h"
#include "../game/Consumables.h"
#include "Object_Manager.h"


class Screen
{
public:
    Screen(int* Level_Ptr);
    ~Screen();
    void Draw_Level(bool aboveObjects);
    void LoadGameObjects(bool includeEnemySpawnersAsCollidables = false); // VERÄNDERUNG: Parametername angepasst
    void Load_Levelmap();

private:
    Texture2D tileatlas_Texture;
    std::unique_ptr<tson::Map> map;
    int* Level_Nbr_Ptr = nullptr;
};


#endif //SCREEN_H