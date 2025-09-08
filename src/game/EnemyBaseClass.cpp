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
#include "EnemyBaseClass.h"
#include "CollisionResponse.h"
#include "../Config.h.in"
#include "raymath.h"
#include <cmath>

Enemy_Base_Class::Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int score, int souls,
    Vector2 start_position, float width, float height, float cooldown_duration, float seek_w, float sep_w,
    float player_sep_w, float desired_sep, float drag_factor)
    : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_speed), enemy_Damage(damage),
    score_value(score), souls_value(souls),  attack_Cooldown_Duration(cooldown_duration), attack_Cooldown_Timer(0.0f),
    seek_weight(seek_w), separation_weight(sep_w),  player_separation_weight(player_sep_w),
    desired_separation(desired_sep), drag(drag_factor)
    {
        hitbox = {start_position.x, start_position.y, width, height};
    }

Enemy_Base_Class::~Enemy_Base_Class() { }

void Enemy_Base_Class::Tick(float delta_time) { }

void Enemy_Base_Class::Tick_AI(float delta_time, Vector2 player_center, const std::vector<Enemy_Base_Class*>& all_enemies)
{
    if (attack_Cooldown_Timer > 0) {
    attack_Cooldown_Timer -= delta_time;
}

    Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
    float delta_x = std::abs(player_center.x - self_center.x);
    float delta_y = std::abs(player_center.y - self_center.y);
    float stopping_distance = (delta_x > delta_y) ? (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f)
    : (this->hitbox.height / 2.0f) + (game::Config::player_Hittbox.y / 2.0f);

    float distance_to_target;
    Vector2 seek_force = Calculate_Seek_Force(player_center, distance_to_target, stopping_distance);
    Vector2 separation_force = Calculate_Separation_Force(all_enemies);
    Vector2 player_separation_force = Calculate_Player_Separation_Force(player_center);
    Vector2 total_force = {0.0f, 0.0f};
    total_force = Vector2Add(total_force, Vector2Scale(seek_force, this->seek_weight));
    total_force = Vector2Add(total_force, Vector2Scale(separation_force, this->separation_weight));
    total_force = Vector2Add(total_force, Vector2Scale(player_separation_force, this->player_separation_weight));

    Vector2 acceleration = total_force;
    this->velocity = Vector2Add(this->velocity, Vector2Scale(acceleration, this->enemy_Movement_Speed * delta_time));
    float max_speed = this->enemy_Movement_Speed;
    if (Vector2Length(this->velocity) > max_speed)
    {
        this->velocity = Vector2Scale(Vector2Normalize(this->velocity), max_speed);
    }
    this->hitbox.x += this->velocity.x * delta_time;
    this->hitbox.y += this->velocity.y * delta_time;
    this->velocity = Vector2Scale(this->velocity, this->drag);
}

void Enemy_Base_Class::On_Collision(Collidable* other)
{
    Collision_Type other_Type = other->Get_Collision_Type();

    if (other_Type == Collision_Type::WALL)
    {
        CollisionResponse::Resolve_Overlap(this, other);
    }
}

void Enemy_Base_Class::Take_Damage(int damage_amount)
{
    this->enemy_Health -= damage_amount;
    if (this->enemy_Health <= 0)
    {
        this->Mark_For_Destruction();
    }
}

void enemy::Enemy_Base_Class::Melee_Attack()
{

}

void enemy::Enemy_Base_Class::Set_Position(Vector2 position)
{
    this->hitbox.x = position.x;
    this->hitbox.y = position.y;
}

Vector2 Enemy_Base_Class::Calculate_Seek_Force(Vector2 target_pos, float& distance_to_target, float stopping_distance) const
{
    Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
    Vector2 direction = Vector2Subtract(target_pos, self_center);
    distance_to_target = Vector2Length(direction);
    if (distance_to_target <= stopping_distance || distance_to_target == 0.0f) {
        return {0.0f, 0.0f};
    }

    return Vector2Normalize(direction);
}


Vector2 Enemy_Base_Class::Calculate_Separation_Force(const std::vector<Enemy_Base_Class*>& all_enemies) const
{
    Vector2 steer = {0.0f, 0.0f};
    int count = 0;
    Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
    for (const auto& other : all_enemies)
    {
        if (other == this) continue;
        Vector2 other_center = { other->Get_Hitbox().x + other->Get_Hitbox().width / 2.0f, other->Get_Hitbox().y + other->Get_Hitbox().height / 2.0f };
        float d = Vector2Distance(self_center, other_center);

        if ((d > 0) && (d < this->desired_separation))
        {
            Vector2 diff = Vector2Subtract(self_center, other_center);
            Vector2Normalize(diff);
            float strength = 1.0f - (d / this->desired_separation);
            strength *= strength;
            diff = Vector2Scale(diff, strength);
            steer = Vector2Add(steer, diff);
            count++;
        }
    }
    if (count > 0) { steer = Vector2Scale(steer, 1.0f / count); }
    if (Vector2Length(steer) > 0) { steer = Vector2Normalize(steer); }
    return steer;
}

Vector2 Enemy_Base_Class::Calculate_Player_Separation_Force(Vector2 player_center) const
{
    Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
    float repulsion_radius = game::Config::player_Hittbox.x / 2.0f;
    float d = Vector2Distance(self_center, player_center);
    if (d < repulsion_radius)
    {
        Vector2 diff = Vector2Subtract(self_center, player_center);
        Vector2Normalize(diff);
        return diff;
    }
     return {0.0f, 0.0f};
    }
}
