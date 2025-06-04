//
// Created by Kruse on 23/05/2025.
//

#include "EnemyBaseClass.h"

#include <cmath>


namespace enemy
{
    //Konstruktor
    Enemy_Base_Class::Enemy_Base_Class(std::string& name, int health, int movement_speed, int damage, int value)
            : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_speed),
              enemy_Damage(damage), enemy_Value(value) {}

    //Destruktor
    Enemy_Base_Class::~Enemy_Base_Class() = default;

    //Getter
    std::string Enemy_Base_Class::get_Name() const
    {
        return enemy_Name;
    }
    int Enemy_Base_Class::get_Health() const
    {
        return enemy_Health;
    }
    int Enemy_Base_Class::get_Movement_Speed() const
    {
        return enemy_Movement_Speed;
    }
    int Enemy_Base_Class::get_Damage() const
    {
        return enemy_Damage;
    }
    int Enemy_Base_Class::get_Value() const
    {
        return enemy_Value;
    }

    //Setter
    void Enemy_Base_Class::set_Health(int new_Health)
    {
        enemy_Health = new_Health;
    }

    /* bei dem Player bitte "Rectangle player_Hitbox" in protected und "Rectangle get_Player_Hitbox() const;" in public
    /  dies bitte in Warrior auch noch einfügen.
    */

    void Enemy_Base_Class::Enemy_Pathfinding(float target_Position_X, float target_Position_Y, float delta_Time)
    {

        // Berechne den Vektor vom Gegner zum Player.
        float delta_Vector_X = target_Position_X - this->enemy_Hitbox.x;
        float delta_Vector_Y = target_Position_Y - this->enemy_Hitbox.y;

        // Berechne die Distanz zum Player.
        float distance_To_Target = std::sqrt(delta_Vector_X * delta_Vector_X + delta_Vector_Y * delta_Vector_Y);

        // Dieser normalisierte Vektor gibt nur noch die Richtung an.
        float normalized_Direction_X = delta_Vector_X / distance_To_Target;
        float normalized_Direction_Y = delta_Vector_Y / distance_To_Target;

        // Hole die Bewegungsgeschwindigkeit des Gegners.
        float current_Movement_Speed = this->get_Movement_Speed();

        // Berechnet wie weit sich der Gegner in diesem Frame bewegen soll.
        float movement_Step_Size = current_Movement_Speed * delta_Time;

        // Aktualisiere die Position der Hitbox des Gegners.
        this->enemy_Hitbox.x += normalized_Direction_X * movement_Step_Size;
        this->enemy_Hitbox.y += normalized_Direction_Y * movement_Step_Size;

    }

//Virtuelle Methoden
void Enemy_Base_Class::Enemy_Attack() {}
    void Enemy_Base_Class::Enemy_Take_Damage() {}
    void Enemy_Base_Class::Enemy_Draw() {}
}
