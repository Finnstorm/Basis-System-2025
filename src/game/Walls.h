//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_WALLS_H
#define RAYLIBSTARTER_WALLS_H


#include "Collidable.h"

class Collision_Manager;
class Walls : public Collidable
{
protected:


public:
    Walls(Vector2 position, Vector2 size);
    ~Walls();


    Collision_Type Get_Collision_Type() const override;


    void Tick(float delta_time) override;
    void On_Collision(Collidable* other) override;
    void Draw() override;
};

#endif //RAYLIBSTARTER_WALLS_H
