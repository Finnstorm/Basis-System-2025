//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_ENEMY_BASE_CLASS_H
#define RAYLIBSTARTER_ENEMY_BASE_CLASS_H

#include "raylib.h"
#include <string>
#include <vector>
#include "Collidable.h"

namespace enemy
{
    class Collision_Manager;
    class Enemy_Base_Class : public Collidable
    {
    protected:
        // KI-Parameter
        float seek_weight;
        float separation_weight;
        float player_separation_weight;
        float desired_separation;
        float drag;

        // Zustand
        Vector2 velocity = {0.0f, 0.0f};
        std::string enemy_Name;
        int enemy_Health;
        float enemy_Movement_Speed;
        int enemy_Damage;
        const float attack_Cooldown_Duration;
        float attack_Cooldown_Timer;

        int score_value;
        int souls_value;

        Vector2 Calculate_Seek_Force(Vector2 target_pos, float& distance_to_target, float stopping_distance) const;
        Vector2 Calculate_Separation_Force(const std::vector<Enemy_Base_Class*>& all_enemies) const;
        Vector2 Calculate_Player_Separation_Force(Vector2 player_center) const;

    public:
        Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int score, int souls,
                         Vector2 start_position, float width, float height, float cooldown_duration,
                         float seek_w, float sep_w, float player_sep_w, float desired_sep, float drag_factor);

        virtual ~Enemy_Base_Class();

        // Öffentliche Methoden
        void Set_Position(Vector2 position) override;
        void Take_Damage(int damage_amount);

        void Tick_AI(float delta_time, Vector2 player_center, const std::vector<Enemy_Base_Class*>& all_enemies);
        void Tick(float delta_time) override;

        void On_Collision(Collidable* other) override;
        virtual void Draw() = 0;
        virtual void Melee_Attack();

        int Get_Score_Value() const { return score_value; }
        int Get_Souls_Value() const { return souls_value; }
        float Get_Movement_Speed() const { return enemy_Movement_Speed; };
        Vector2 Get_Velocity() const { return velocity; }
        Collision_Type Get_Collision_Type() const override { return Collision_Type::ENEMY; }
    };
}
#endif

