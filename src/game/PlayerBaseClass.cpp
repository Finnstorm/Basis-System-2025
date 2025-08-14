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

    Update_Input_Stacks();

    is_Moving = false;
    if (currentState != ATTACKING_RANGED || game::Config::allow_Move_While_Attacking) {
        Vector2 move_Direction = {0.0f, 0.0f};

        if (!horizontal_inputs.empty()) {
            Input_Direction current_h = horizontal_inputs.front();
            if (current_h == Input_Direction::LEFT) move_Direction.x = -1.0f;
            else if (current_h == Input_Direction::RIGHT) move_Direction.x = 1.0f;
        }

        if (!vertical_inputs.empty()) {
            Input_Direction current_v = vertical_inputs.front();
            if (current_v == Input_Direction::UP) move_Direction.y = -1.0f;
            else if (current_v == Input_Direction::DOWN) move_Direction.y = 1.0f;
        }

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
    float move_X = hitbox.x - previous_Position.x;
    float move_Y = hitbox.y - previous_Position.y;


    if (move_X == 0.0f && move_Y == 0.0f) {
        return;
    }

    if (move_X > 0.0f)
    {
        if (move_Y > 0.0f) facing_Direction = Facing_Direction::DOWN_RIGHT;
        else if (move_Y < 0.0f) facing_Direction = Facing_Direction::UP_RIGHT;
        else facing_Direction = Facing_Direction::RIGHT;
    }
    else if (move_X < 0.0f)
    {
        if (move_Y > 0.0f) facing_Direction = Facing_Direction::DOWN_LEFT;
        else if (move_Y < 0.0f) facing_Direction = Facing_Direction::UP_LEFT;
        else facing_Direction = Facing_Direction::LEFT;
    }
    else { // Nur vertikale Bewegung
        if (move_Y > 0.0f) facing_Direction = Facing_Direction::DOWN;
        else if (move_Y < 0.0f) facing_Direction = Facing_Direction::UP;
    }
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
void Player_Base_Class::Update_Input_Stacks()
{
    if (IsKeyPressed(game::Config::key_Left))  horizontal_inputs.push_front(Input_Direction::LEFT);
    if (IsKeyPressed(game::Config::key_Right)) horizontal_inputs.push_front(Input_Direction::RIGHT);

    if (IsKeyReleased(game::Config::key_Left))  horizontal_inputs.remove(Input_Direction::LEFT);
    if (IsKeyReleased(game::Config::key_Right)) horizontal_inputs.remove(Input_Direction::RIGHT);


    if (IsKeyPressed(game::Config::key_Up))    vertical_inputs.push_front(Input_Direction::UP);
    if (IsKeyPressed(game::Config::key_Down))  vertical_inputs.push_front(Input_Direction::DOWN);

    if (IsKeyReleased(game::Config::key_Up))    vertical_inputs.remove(Input_Direction::UP);
    if (IsKeyReleased(game::Config::key_Down))  vertical_inputs.remove(Input_Direction::DOWN);
}