#include "Cam.h"
#include <raylib.h>
#include "../config.h.in"
#include "raymath.h"

Cam::Cam(Player_Base_Class& mp) : pao(mp)
{
    this->cam = {0};
    this->cam.zoom = game::Config::cam_zoom; 
}

void Cam::Cam_Movement(double dtm, Vector2 mapDimensions)
{
    Vector2 mpp = pao.Get_Player_Center();
    this->cam.target = mpp;

    this->cam.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };

    this->cam.zoom = game::Config::cam_zoom;

    if (mapDimensions.x > 0 && mapDimensions.y > 0)
    {
        float halfScreenWidthInWorld = (float)GetScreenWidth() / 2.0f / this->cam.zoom;
        float halfScreenHeightInWorld = (float)GetScreenHeight() / 2.0f / this->cam.zoom;

        this->cam.target.x = Clamp(this->cam.target.x, halfScreenWidthInWorld, mapDimensions.x - halfScreenWidthInWorld);
        this->cam.target.y = Clamp(this->cam.target.y, halfScreenHeightInWorld, mapDimensions.y - halfScreenHeightInWorld);
    }
    this->cam.target.x = roundf(this->cam.target.x);
    this->cam.target.y = roundf(this->cam.target.y);
}