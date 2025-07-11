//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_SPAWNPOINTS_H
#define RAYLIBSTARTER_SPAWNPOINTS_H


#include "Collidable.h"

class Collision_Manager;
class Spawnpoints : public Collidable
{
private:
    int spawner_Hits;


public:
    Spawnpoints(Vector2 position, int hits);
    ~Spawnpoints();


    Collision_Type Get_Collision_Type() const override;
    bool Is_Destroyed() const;

    void Tick(float delta_time) override;
    void On_Collision(Collidable* other) override;
    void Draw() override;
};



#endif //RAYLIBSTARTER_SPAWNPOINTS_H
