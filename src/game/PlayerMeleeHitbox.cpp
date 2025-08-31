//
// Created by $Will on 14.08.2025.
//

#include "PlayerMeleeHitbox.h"
#include "CollisionResponse.h"
#include "PlayerBaseClass.h"
#include <limits>

namespace game
{
    Player_Melee_Hitbox::Player_Melee_Hitbox(Player_Base_Class* owner, int damage, Facing_Direction direction)
    : lifetime(game::Config::player_Melee_Hitbox_Lifetime), damage(damage), owner(owner), attack_direction(direction) {}

    void Player_Melee_Hitbox::Tick(float delta_time)
    {
        lifetime -= delta_time;
        if (lifetime <= 0 || owner == nullptr || owner->Is_Dead()) {
            Mark_For_Destruction();
            return;
        }
        owner->Calculate_Melee_Hitboxes(this->hitboxes, this->attack_direction);
        if (!hitboxes.empty())
        {
            float minX = std::numeric_limits<float>::max();
            float minY = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::min();
            float maxY = std::numeric_limits<float>::min();

            for (const auto& box : hitboxes) {
                minX = std::min(minX, box.x);
                minY = std::min(minY, box.y);
                maxX = std::max(maxX, box.x + box.width);
                maxY = std::max(maxY, box.y + box.height);
            }
            this->hitbox = {minX, minY, maxX - minX, maxY - minY};
        }
    }

    void Player_Melee_Hitbox::On_Collision(Collidable* other)
    {
        if (other->Get_Collision_Type() != Collision_Type::ENEMY) return;
        bool real_hit = false;
        Rectangle other_hitbox = other->Get_Hitbox();
        for (const auto& box : hitboxes) {
            if (CheckCollisionRecs(box, other_hitbox)) {
                real_hit = true;
                break;
            }
        }

        if (!real_hit) return;

        for (Collidable* target : hit_targets) {
            if (target == other) return;
        }

        CollisionResponse::Apply_Damage(other, this->damage);
        hit_targets.push_back(other);
    }

    void Player_Melee_Hitbox::Draw()
    {
        if (game::Config::visualize_Attack_Hitboxes)
        {
            for (const auto& box : hitboxes) {
                DrawRectangleRec(box, Fade(RED, 0.5f));
            }
            DrawRectangleLinesEx(this->hitbox, 1.0f, Fade(YELLOW, 0.7f));
        }
    }
}