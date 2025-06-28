//
// Created by Manza on 6/22/2025.
//

#pragma once
#include "raylib.h"

namespace game {
    /**
     * @brief Represents a single projectile fired by an enemy.
     */
    class Projectile {
    public:
        Vector2 position;
        Vector2 velocity;
        bool is_active;
        int damage;
        Texture2D sprite;

        /**
         * @brief Constructs a new Projectile.
         * @param start_position The initial position where the projectile spawns.
         * @param direction The normalized velocity vector.
         * @param projectile_speed The speed of the projectile.
         * @param damage The damage it deals on hit.
         * @param sprite_path Path to the projectile's texture.
         */
        Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int damage, const char* sprite_path);

        ~Projectile();

        void Tick(float delta_time);

        void Draw() const;
    };
}