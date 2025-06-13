//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_ENEMY_BASE_CLASS_H
#define RAYLIBSTARTER_ENEMY_BASE_CLASS_H

#include "raylib.h"
#include <string>

namespace enemy
{
    class Enemy_Base_Class
    {
    protected:

        std::string enemy_Name;
        int enemy_Health;
        float enemy_Movement_Speed;
        int enemy_Damage;
        int enemy_Value; // Score etc.
        Rectangle enemy_Hitbox;
        Texture2D sprite;

    public:
        //Konstruktor
        Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
            const char* sprite_path, Vector2 start_position, int width, int height);

        //Destruktor
        virtual ~Enemy_Base_Class();

        // This default implementation can be inherited by all enemies.
        virtual void Pathfinding(float target_Position_X, float target_Position_Y, float delta_Time);

        // Diese Methoden sind erstmal ein Standard. Virtuell heißt sie können überschrieben werden aber ohne
        // = 0 müssen sie nicht unbedingt überschrieben werden und können als Standard genutzt werden.
        void Take_Damage(int damage_amount);
        virtual void Attack();
        virtual void Draw();
        virtual void Update();

        int get_Health() const { return enemy_Health; }
        int get_Damage() const { return enemy_Damage; }
        Rectangle get_Hitbox() const { return enemy_Hitbox; }
        float get_Movement_Speed() const;
    };
}


#endif //RAYLIBSTARTER_ENEMY_BASE_CLASS_H
