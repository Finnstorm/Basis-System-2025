#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include <raylib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "../core/wall_collisions.h"


class Screen {
public:
    nlohmann::json tileset_Description;
    nlohmann::json levelmap;
    Texture2D tileatlas_Texture{};

    int columns;

    std::vector<game::core::Wall> walls;

    Screen();
    ~Screen();

    void draw_Level() const;
    void Load_Object_Layers();
};


#endif //SCREEN_H