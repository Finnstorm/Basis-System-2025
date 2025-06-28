//
// Created by Kruse on 23/05/2025.
//

#include "EnemyBaseClass.h"
#include "CollisionManager.h"
#include <cmath>

namespace enemy
{
    Enemy_Base_Class::Enemy_Base_Class(std::string name, int health, int movement_speed, int damage, int value,
                const char* sprite_path, Vector2 start_position, int width, int height, Collision_Manager* manager)
    {
    enemy_Hitbox = {start_position.x, start_position.y, (float)width, (float)height};
    sprite = LoadTexture(sprite_path);
    }

Enemy_Base_Class::~Enemy_Base_Class()
    {
    // Gibt die Textur frei, wenn der Gegner zerstört wird.
    UnloadTexture(sprite);
    }

void Enemy_Base_Class::Take_Damage(int damage_amount)
    {
    enemy_Health -= damage_amount;
    }

    int Enemy_Base_Class::get_Movement_Speed() const
{
    return this->enemy_Movement_Speed;
}

void Enemy_Base_Class::Pathfinding(float target_Position_X, float target_Position_Y, float delta_Time)
{
    // Berechnet den Richtungs-Vektor vom Gegner zum Ziel.
    float delta_Vector_X = target_Position_X - this->enemy_Hitbox.x;
    float delta_Vector_Y = target_Position_Y - this->enemy_Hitbox.y;

    // Berechnet die exakte Distanz zum Ziel.
    float distance_To_Target = std::sqrt(delta_Vector_X * delta_Vector_X + delta_Vector_Y * delta_Vector_Y);

    // Sicherheitscheck, um eine Division durch Null (Fehler den ich bei meinem Test oft hatte) zu verhindern.
    // Die Bewegung wird nur ausgeführt, wenn der Gegner sein Ziel noch nicht erreicht hat.
    if (distance_To_Target > 0.01f)
    {
        // Normalisiert den Vektor: Macht den Pfeil zur reinen Richtung, indem seine Länge auf 1 gekürzt wird.
        // Dies ist der entscheidende Schritt für eine konstante Geschwindigkeit.
        float normalized_Direction_X = delta_Vector_X / distance_To_Target;
        float normalized_Direction_Y = delta_Vector_Y / distance_To_Target;

        // Holt die individuelle Geschwindigkeit dieses Gegners.
        float current_Movement_Speed = this->get_Movement_Speed();

        // Berechnet die "Schrittgröße" für diesen einzelnen Frame.
        float movement_Step_Size = current_Movement_Speed * delta_Time;

        // Bewegt die Hitbox des Gegners um den kleinen Schritt in die korrekte Richtung.
        this->enemy_Hitbox.x += normalized_Direction_X * movement_Step_Size;
        this->enemy_Hitbox.y += normalized_Direction_Y * movement_Step_Size;
    }
}

//Core Methoden
void Enemy_Base_Class::Tick(float delta_time)
    {
    }
void Enemy_Base_Class::On_Collision(Collidable* other)
{

}
void Enemy_Base_Class::Draw()
{
}
}