//
// Created by Kruse on 23/05/2025.
//

#include <iostream>
#include "PlayerBaseClass.h"
#include "PlayerProjectile.h"
#include "CollisionResponse.h"
#include "Object_Manager.h"
#include "raymath.h"
#include "../Config.h.in"

Player_Base_Class::Player_Base_Class(int max_Health, float movement_Speed, int damage, Vector2 start_Position)
    : player_Max_Health(max_Health), player_Health((float)max_Health), player_Movement_Speed(movement_Speed),
      player_Damage(damage),
      previous_Position(start_Position), melee_Cooldown(0.0f), range_Attack_Cooldown(0.0f),
      inventory_Is_Full(false), facing_Direction(Facing_Direction::DOWN), is_Moving(false)
{
    this->hitbox =
    {
        start_Position.x,
        start_Position.y,
        game::Config::player_Hittbox.x,
        game::Config::player_Hittbox.y
    };

    this->projectile_Speed = game::Config::player_Class_One_Projectile_Speed;
}

Player_Base_Class::~Player_Base_Class()
{

}


void Player_Base_Class::Player_Input()
{
    if (IsKeyPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0)
    {
        Melee_Attack();
    }

    if (IsKeyPressed(game::Config::key_Ranged_Attack) && range_Attack_Cooldown <= 0)
    {
        Ranged_Attack();
    }

    /*if (IsKeyPressed(game::Config::key_Use_Item) && inventory_Is_Full)
    {
        Use_Item();
    }*/
}

void Player_Base_Class::Tick(float delta_time)
{
    Update_Previous_Position();
    if (game::Config::enable_Health_Drain) {
        player_Health -= game::Config::player_Health_Drain_Rate * delta_time;
    }
    if (melee_Cooldown > 0) melee_Cooldown -= delta_time;
    if (range_Attack_Cooldown > 0) range_Attack_Cooldown -= delta_time;

    is_Moving = false;
    if (currentState != ATTACKING_RANGED || game::Config::allow_Move_While_Attacking) {
        Vector2 move_Direction = {0.0f, 0.0f};
        if (IsKeyDown(game::Config::key_Up))    move_Direction.y = -1.0f;
        if (IsKeyDown(game::Config::key_Down))  move_Direction.y = 1.0f;
        if (IsKeyDown(game::Config::key_Left))  move_Direction.x = -1.0f;
        if (IsKeyDown(game::Config::key_Right)) move_Direction.x = 1.0f;

        is_Moving = (move_Direction.x != 0.0f || move_Direction.y != 0.0f);
        if(is_Moving) {
            move_Direction = Vector2Normalize(move_Direction);
            hitbox.x += move_Direction.x * player_Movement_Speed * delta_time;
            hitbox.y += move_Direction.y * player_Movement_Speed * delta_time;
        }
    }
    player_Pos = {hitbox.x, hitbox.y};
    Update_Facing_Direction();
}

void Player_Base_Class::On_Collision(Collidable* other)
{
	Collision_Type otherType = other->Get_Collision_Type();

    if (otherType == Collision_Type::WALL ||
        otherType == Collision_Type::ENEMY_SPAWNER)
    {
        CollisionResponse::Resolve_Overlap(this, other);
	}
}

void Player_Base_Class::Draw()
{
    DrawTexture(this->maintex, this->hitbox.x,hitbox.y,WHITE);
}

void Player_Base_Class::Melee_Attack()
{
	melee_Cooldown = 0.0f;
}

void Player_Base_Class::Ranged_Attack()
{
    this->range_Attack_Cooldown = game::Config::player_Ranged_Attack_Cooldown;

    this->currentState = ATTACKING_RANGED;

    Vector2 fire_direction = {0.0f, 0.0f};
    switch (facing_Direction) {
        case UP:         fire_direction = {0.0f, -1.0f}; break;
        case DOWN:       fire_direction = {0.0f, 1.0f};  break;
        case LEFT:       fire_direction = {-1.0f, 0.0f}; break;
        case RIGHT:      fire_direction = {1.0f, 0.0f};  break;
        case UP_LEFT:    fire_direction = Vector2Normalize({-1.0f, -1.0f}); break;
        case UP_RIGHT:   fire_direction = Vector2Normalize({1.0f, -1.0f});  break;
        case DOWN_LEFT:  fire_direction = Vector2Normalize({-1.0f, 1.0f});  break;
        case DOWN_RIGHT: fire_direction = Vector2Normalize({1.0f, 1.0f});   break;
        case NONE:       return;
    }

    float offset_distance = (hitbox.width / 2.0f) + 1;
    Vector2 spawn_position = Vector2Add(Get_Player_Center(), Vector2Scale(fire_direction, offset_distance));

    int final_damage = static_cast<int>(this->player_Damage * game::Config::player_Ranged_Damage_Factor);

    auto* projectile = new game::Player_Projectile(
        spawn_position,
        fire_direction,
        projectile_Speed,
        final_damage
    );

    if (object_manager_ptr) {
        object_manager_ptr->AddObject(projectile);
    }
}

void Player_Base_Class::Update_Previous_Position()
{
    previous_Position.x = hitbox.x;
    previous_Position.y = hitbox.y;
}

void Player_Base_Class::Update_Facing_Direction()
{
    bool up = IsKeyDown(game::Config::key_Up);
    bool down = IsKeyDown(game::Config::key_Down);
    bool left = IsKeyDown(game::Config::key_Left);
    bool right = IsKeyDown(game::Config::key_Right);

    if ((up && down) || (left && right)) return;

    if (up && right) facing_Direction = Facing_Direction::UP_RIGHT;
    else if (up && left) facing_Direction = Facing_Direction::UP_LEFT;
    else if (down && right) facing_Direction = Facing_Direction::DOWN_RIGHT;
    else if (down && left) facing_Direction = Facing_Direction::DOWN_LEFT;
    else if (up) facing_Direction = Facing_Direction::UP;
    else if (down) facing_Direction = Facing_Direction::DOWN;
    else if (left) facing_Direction = Facing_Direction::LEFT;
    else if (right) facing_Direction = Facing_Direction::RIGHT;
}

Collision_Type Player_Base_Class::Get_Collision_Type() const
{
    return Collision_Type::PLAYER;
}

Vector2 Player_Base_Class::Get_Player_Pos() {
    return this->player_Pos;
}
void Player_Base_Class::Take_Damage(int damage_amount)
{
    player_Health -= damage_amount;
}
Vector2 Player_Base_Class::Get_Player_Center() {
    return (Vector2){this->hitbox.x + this->hitbox.width / 2, this->hitbox.y + this->hitbox.height / 2};
}
void Player_Base_Class::Set_Position(Vector2 position)
{
    this->hitbox.x = position.x;
    this->hitbox.y = position.y;
    this->player_Pos = position;
}
float Player_Base_Class::Get_Health() const
{
    return this->player_Health;
}
bool Player_Base_Class::Is_Dead() const
{
    return this->player_Health <= 0;
}