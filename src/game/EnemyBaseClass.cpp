//
// Created by Kruse on 23/05/2025.
//

#include <valarray>
#include "EnemyBaseClass.h"
#include "CollisionManager.h"
#include "CollisionResponse.h"
#include "PlayerBaseClass.h"

namespace enemy
{
Enemy_Base_Class::Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
    const char* sprite_path, const char* projectile_sprite_path,Vector2 start_position, int width, int height, float cooldown_Duration)
    : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_speed), enemy_Damage(damage),
      enemy_Value(value),attack_Cooldown_Duration(cooldown_Duration), attack_Cooldown_Timer(0.0f), is_Moving(false)
    {
    hitbox = {start_position.x, start_position.y, (float)width, (float)height};
    sprite = LoadTexture(sprite_path);
    }

Enemy_Base_Class::~Enemy_Base_Class()
{
    UnloadTexture(sprite);
}

    void Enemy_Base_Class::Take_Damage(int damage_amount)
{
    this->enemy_Health -= damage_amount;


    if (this->enemy_Health <= 0)
    {
        this->Mark_For_Destruction();
    }
}

    void Enemy_Base_Class::Pathfinding(float target_Position_X, float target_Position_Y, float delta_Time, float stopping_distance)
{
    float self_Center_X = this->hitbox.x + this->hitbox.width / 2.0f;
    float self_Center_Y = this->hitbox.y + this->hitbox.height / 2.0f;
    float delta_Vector_X = target_Position_X - self_Center_X;
    float delta_Vector_Y = target_Position_Y - self_Center_Y;
    float distance_To_Target = std::sqrt(delta_Vector_X * delta_Vector_X + delta_Vector_Y * delta_Vector_Y);
    float travel_distance = distance_To_Target - stopping_distance;

    if (travel_distance > 0)
    {
        float normalized_Direction_X = delta_Vector_X / distance_To_Target;
        float normalized_Direction_Y = delta_Vector_Y / distance_To_Target;
        float movement_Step_Size = this->Get_Movement_Speed() * delta_Time;
        if (movement_Step_Size > travel_distance)
        {
            movement_Step_Size = travel_distance;
        }
        this->hitbox.x += normalized_Direction_X * movement_Step_Size;
        this->hitbox.y += normalized_Direction_Y * movement_Step_Size;
    }
    is_Moving = true;
}

void Enemy_Base_Class::Tick(float delta_time)
{
    if (attack_Cooldown_Timer > 0)
    {
        attack_Cooldown_Timer -= delta_time;
    }
}

void Enemy_Base_Class::On_Collision(Collidable* other)
{
    Collision_Type other_Type = other->Get_Collision_Type();

    if (other_Type == Collision_Type::WALL)
    {
        CollisionResponse::Resolve_Overlap(this, other);
    }
}

void Enemy_Base_Class::Draw()
{

}

void enemy::Enemy_Base_Class::Range_Attack()
{

}

void enemy::Enemy_Base_Class::Melee_Attack()
{

}

void enemy::Enemy_Base_Class::Set_Position(Vector2 position)
{
    this->hitbox.x = position.x;
    this->hitbox.y = position.y;
}

}
