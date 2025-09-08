//
// Created by $Will on 14.08.2025.
//

#ifndef PLAYERMELEEHITBOX_H
#define PLAYERMELEEHITBOX_H

#include <vector>
#include "Collidable.h"
#include "PlayerBaseClass.h"
#include "../config.h.in"

class Player_Base_Class;
namespace game
{
    class Player_Melee_Hitbox : public Collidable
    {
    private:
        float lifetime;
        int damage;
        std::vector<Collidable*> hit_targets;
        Player_Base_Class* owner;
        std::vector<Rectangle> hitboxes;
        Facing_Direction attack_direction;
    public:
        Player_Melee_Hitbox(Player_Base_Class* owner, int damage, Facing_Direction direction);
        void Tick(float delta_time) override;
        void On_Collision(Collidable* other) override;
        void Draw() override;
        void Update_Hitbox_Position();

        Collision_Type Get_Collision_Type() const override { return Collision_Type::PLAYER_MELEE_ATTACK; }
    };
}


#endif //PLAYERMELEEHITBOX_H
