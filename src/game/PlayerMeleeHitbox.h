//
// Created by $Will on 14.08.2025.
//

#ifndef PLAYERMELEEHITBOX_H
#define PLAYERMELEEHITBOX_H

#include <vector>
#include "Collidable.h"
#include "../config.h.in"

namespace game
{
    class Player_Melee_Hitbox : public Collidable
    {
    private:
        float lifetime;
        int damage;
        std::vector<Collidable*> hit_targets;

    public:
        Player_Melee_Hitbox(Rectangle hitbox, int damage);

        void Tick(float delta_time) override;

        void On_Collision(Collidable* other) override;

        void Draw();
        Collision_Type Get_Collision_Type() const override { return Collision_Type::PLAYER_MELEE_ATTACK; }
    };
}


#endif //PLAYERMELEEHITBOX_H
