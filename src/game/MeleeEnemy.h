#pragma once
#include "EnemyBaseClass.h"
#include "../core/Animations.h"
#include "../core/RepeatAnimation.h"
#include <map>
#include "PlayerBaseClass.h"

enum EnemyState { E_IDLE, E_WALKING, E_ATTACKING };

namespace enemy
{
    class Melee_Enemy : public Enemy_Base_Class
    {
    private:
        EnemyState currentState = E_WALKING;
        Facing_Direction facing_Direction = LEFT;
        Facing_Direction attack_Direction = LEFT;
        std::map<Facing_Direction, RepeatAnimation> walk_animations;
        std::map<Facing_Direction, Animations> attack_animations;
        bool damage_applied_this_attack = false;
        static Texture2D walk_texture_left;
        static Texture2D walk_texture_right;
        static Texture2D attack_texture_left;
        static Texture2D attack_texture_right;

    public:
        Melee_Enemy(Vector2 start_position);
        ~Melee_Enemy() override = default;

        static void Load_Assets();
        void Tick_Melee(float delta_time, Vector2 player_center);

        void Draw() override;
        void On_Collision(Collidable* other) override;
        void Melee_Attack() override;
    };
}