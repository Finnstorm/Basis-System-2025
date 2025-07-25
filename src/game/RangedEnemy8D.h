//
// Created by Manza on 6/22/2025.
//

#pragma once

#include "EnemyBaseClass.h"
#include "AttackDirection.h"
#include "EnemyProjectile.h"
#include <vector>
#include <memory>
/*
class Collision_Manager;

namespace enemy {

    class Ranged_Enemy_8_Directional : public Enemy_Base_Class {
    private:
        float attack_range;
        float attack_cooldown;
        float projectile_speed;
        float time_since_last_attack;
        std::vector<std::unique_ptr<game::Enemy_Projectile>> sp_projectiles;

        void Attack(float target_Position_X, float target_Position_Y);

        Attack_Direction Determine_Attack_Direction(float direction_x, float direction_y);

    public:

        Ranged_Enemy_8_Directional(std::string name, int health, float movement_speed, int damage, int value,
                                   const char* enemy_sprite_path, const char* proj_sprite_path,
                                   Vector2 start_position, int width, int height,
                                   float attack_range, float attack_cooldown, float projectile_speed);

        ~Ranged_Enemy_8_Directional() override = default;

        void Tick(float delta_time, float target_Position_X, float target_Position_Y);
        void Draw() const;
    };
}*/