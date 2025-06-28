//
// Created by Manza on 6/22/2025.
//

#include "Projectile.h"

namespace game {
    Projectile::Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int damage, const char* sprite_path)
        : position(start_position),
          is_active(true),
          damage(damage) {

        // The final velocity is the direction multiplied by speed
        this->velocity.x = direction.x * projectile_speed;
        this->velocity.y = direction.y * projectile_speed;
        this->sprite = LoadTexture(sprite_path);
    }

    Projectile::~Projectile() {
        // Unload texture when the projectile is destroyed
        if (sprite.id > 0) {
            UnloadTexture(sprite);
        }
    }

    void Projectile::Tick(float delta_time) {
        if (!is_active) return;
        position.x += velocity.x * delta_time;
        position.y += velocity.y * delta_time;

        // Add logic to deactivate the projectile if it goes off-screen or hits a wall or what u wanna do.
    }

    void Projectile::Draw() const {
        if (!is_active) return;
        DrawTextureV(sprite, position, WHITE);
    }
}