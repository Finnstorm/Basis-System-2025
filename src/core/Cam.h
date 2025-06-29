//
// Created by Kruse on 06/05/2025.
//

#ifndef RAYLIBSTARTER_CAM_H
#define RAYLIBSTARTER_CAM_H
#include <raylib.h>
#include "config.h"
struct Cam
{
    Camera2D cam={0,0,0,0,0,2};

    void Cam_Movement();
};




#endif //RAYLIBSTARTER_CAM_H
