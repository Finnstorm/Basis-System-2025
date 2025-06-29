//
// Created by Kruse on 06/05/2025.
//

#include "Cam.h"
#include "config.h"
#include <raylib.h>

void Cam::Cam_Movement() {
    if (IsKeyDown(KEY_W)){
        this->cam.target.y= this->cam.target.y-10;
    }
    if (IsKeyDown(KEY_S)){
        this->cam.target.y= this->cam.target.y+10;
    }
    if (IsKeyDown(KEY_A)){
        this->cam.target.x= this->cam.target.x-10;
    }
    if (IsKeyDown(KEY_D)){
        this->cam.target.x= this->cam.target.x+10;
    }

}
