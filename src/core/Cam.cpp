//
// Created by Kruse on 06/05/2025.
//

#include "Cam.h"
#include "raymath.h"
#include "../config.h.in"

void Cam::Cam_Movement(double dtm, Vector2 mapDimensions) {
    Vector2 mpp = pao.Get_Player_Center();
    this->cam.target = mpp;
    this->cam.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    this->cam.zoom = game::Config::cam_zoom;
    if (mapDimensions.x > 0 && mapDimensions.y > 0) {
        float halfScreenWidthInWorld = (float)GetScreenWidth() / 2.0f / this->cam.zoom;
        float halfScreenHeightInWorld = (float)GetScreenHeight() / 2.0f / this->cam.zoom;
        this->cam.target.x = Clamp(this->cam.target.x, halfScreenWidthInWorld, mapDimensions.x - halfScreenWidthInWorld);
        this->cam.target.y = Clamp(this->cam.target.y, halfScreenHeightInWorld, mapDimensions.y - halfScreenHeightInWorld);
    }
}
