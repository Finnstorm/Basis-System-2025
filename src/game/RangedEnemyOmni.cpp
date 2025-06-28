//
// Created by Manza on 6/22/2025.
//

#include "RangedEnemyOmni.h"
#include <cmath>
#include <raylib.h>

namespace enemy {
    Ranged_Enemy_Omni::Ranged_Enemy_Omni(std::string name, int health, float movement_speed, int damage, int value,
                                         const char* sprite_path, Vector2 start_position, int width, int height,
                                         float attack_range, float attack_cooldown, float projectile_speed, Collision_Manager* manager)
        : Enemy_Base_Class(name, health, movement_speed, damage, value, sprite_path, start_position, width, height, manager),
          attack_range(attack_range),
          attack_cooldown(attack_cooldown),
          projectile_speed(projectile_speed),
          time_since_last_attack(0.0f) {}

    void Ranged_Enemy_Omni::Tick(float delta_time, float target_Position_X, float target_Position_Y) {
        this->Pathfinding(target_Position_X, target_Position_Y, delta_time);
        time_since_last_attack += delta_time;

        // Check attack conditions
        if (time_since_last_attack >= attack_cooldown) {
            this->Attack(target_Position_X, target_Position_Y);
        }

        for (auto& p : sp_projectiles) {
            p->Tick(delta_time);
        }
        // Add logic to remove inactive projectiles from the vector to save memory.
    }

    void Ranged_Enemy_Omni::Draw() const {
        for (const auto& p : sp_projectiles) {
            p->Draw();
        }
    }

    void Ranged_Enemy_Omni::Attack(float target_Position_X, float target_Position_Y) {
        float delta_vector_x = target_Position_X - this->enemy_Hitbox.x;
        float delta_vector_y = target_Position_Y - this->enemy_Hitbox.y;
        float distance_to_target = std::sqrt(delta_vector_x * delta_vector_x + delta_vector_y * delta_vector_y);

        if (distance_to_target <= attack_range && distance_to_target > 0) {
            Vector2 fire_direction = {
                delta_vector_x / distance_to_target,
                delta_vector_y / distance_to_target
            };

            // Enter in *** the path of the projectile .png -> "***"
            sp_projectiles.push_back(std::make_unique<game::Projectile>(
                Vector2{this->enemy_Hitbox.x, this->enemy_Hitbox.y},
                fire_direction, this->projectile_speed, this->enemy_Damage,
                "***"
            ));
            time_since_last_attack = 0.0f;
        }
    }
}