//
// Created by $Will on 01.06.2025.
//
// wall_collisions.cpp
#include "wall_collisions.h"

namespace game::core {

    // Implementierung für Wall::Update
    void Wall::Update(float delta_time, const std::vector<Wall*>& all_walls) {
        // Für eine statische Standard-Wand gibt es hier normalerweise nichts zu tun.
    }

    // Implementierung für Wall::Draw
    void Wall::Draw() const {
        // Optionale Debug-Anzeige der Hitbox für statische Wände
        // if (type_ == "Wall") { // Nur wenn der Typ "Wall" ist
        //     DrawRectangleLinesEx(rect_, 1, DARKGRAY);
        // }
    }

} // namespace game::core