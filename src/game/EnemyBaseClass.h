//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_ENEMY_BASE_CLASS_H
#define RAYLIBSTARTER_ENEMY_BASE_CLASS_H

#include "raylib.h"
#include <string>
#include "Collidable.h"

namespace enemy
{
    class Collision_Manager;
    class Enemy_Base_Class : public Collidable
    {
    protected:

        std::string enemy_Name;
        int enemy_Health;
        int enemy_Movement_Speed;
        int enemy_Damage;
        int enemy_Value;
        Texture2D sprite;
        Collision_Manager* manager_ptr;
        const char* projectile_sprite_path;
        Rectangle hitbox;

    public:
        //Konstruktor
        Enemy_Base_Class(std::string name, int health, int movement_speed, int damage, int value,
            const char* enemy_sprite_path, const char* proj_sprite_path,
            Vector2 start_position, int width, int height, Collision_Manager* manager);

        //Destruktor
        virtual ~Enemy_Base_Class();

        virtual void Pathfinding(float target_Position_X, float target_Position_Y, float delta_Time);
        void Take_Damage(int damage_amount);
        int get_Movement_Speed() const;

        virtual void Range_Attack();
        virtual void Melee_Attack();

        int Get_Health() const { return enemy_Health; }
        int Get_Damage() const { return enemy_Damage; }

        Rectangle Get_Hitbox() const { return hitbox; }
        Collision_Type Get_Collision_Type() const override { return Collision_Type::ENEMY; }

        void Tick(float delta_time);
        void On_Collision(Collidable* other) override;
        void Draw();
    };
}
#endif
