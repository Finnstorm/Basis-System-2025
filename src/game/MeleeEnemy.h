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
    protected:
        EnemyState currentState = E_WALKING;
        Facing_Direction facing_Direction = LEFT;
        Facing_Direction attack_Direction = LEFT;
        std::map<Facing_Direction, RepeatAnimation> walk_animations;
        std::map<Facing_Direction, Animations> attack_animations;
        bool damage_applied_this_attack = false;

        Texture2D* walk_texture_left;
        Texture2D* walk_texture_right;
        Texture2D* attack_texture_left;
        Texture2D* attack_texture_right;

        static std::map<std::string, Texture2D> s_melee_textures;

        Melee_Enemy(Vector2 start_position, const std::string& name, int health, float speed, int damage, int score,
            int souls, float cooldown, Vector2 hitbox_size, const char* walk_left_path, const char* walk_right_path,
            const char* attack_left_path, const char* attack_right_path, Vector2 walk_anim_size, int walk_frame_count,
            float walk_anim_speed, Vector2 attack_anim_size, int attack_frame_count, float attack_anim_speed);

    public:

        ~Melee_Enemy() override = default;

        static void Load_All_Melee_Assets();
        static void Unload_All_Melee_Assets();

        void Tick_Melee(float delta_time, Vector2 player_center);
        void Draw() override;
        void On_Collision(Collidable* other) override;
        void Melee_Attack() override;
    };
}