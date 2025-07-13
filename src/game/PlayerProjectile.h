//
// Created by Manza on 7/4/2025.
//

#pragma once
#include "raylib.h"
#include "Collidable.h"
#include "AttackDirection.h"
#include "PlayerBaseClass.h"
#include "../core/RepeatAnimation.h"

namespace game {
    /**
     * @brief Stellt ein einzelnes Projektil dar, das vom Spieler abgefeuert wird.
     * Erbt von Collidable, um mit der Spielwelt (Wände, Gegner) interagieren zu können.
     */
    class Player_Projectile : public Collidable {
    public:
        Vector2 velocity;
        bool is_active;
        int damage;
        RepeatAnimation animation;
        Player_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int final_damage,
        Facing_Direction facing_dir);
        ~Player_Projectile();

        void Tick(float delta_time) override;
        void Draw() override;

        Collision_Type Get_Collision_Type() const override;
        void On_Collision(Collidable* other) override;
    };
}
