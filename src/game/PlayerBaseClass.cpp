//
// Created by Kruse on 23/05/2025.
//

#include <iostream>
#include "PlayerBaseClass.h"
#include "PlayerProjectile.h"
#include "CollisionResponse.h"
#include "Object_Manager.h"
#include "PlayerMeleeHitbox.h"
#include "raymath.h"
#include "../Config.h.in"

Player_Base_Class::Player_Base_Class(int max_Health, float movement_Speed, float damage_multiplier, Vector2 start_Position)
    : player_Max_Health(max_Health), player_Health((float)max_Health), player_Movement_Speed(movement_Speed),
      player_Damage_Multiplier(damage_multiplier),
      previous_Position(start_Position), melee_Cooldown(0.0f), range_Attack_Cooldown(0.0f),
      inventory_Is_Full(false), facing_Direction(Facing_Direction::DOWN), is_Moving(false)
{
    this->original_movement_speed = movement_Speed;
    this->original_damage_multiplier = damage_multiplier;
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
    Use_Item();
}

void Player_Base_Class::Tick(float delta_time)
{
    if (item_remove_ticker > 0)
    {
        item_remove_ticker++;
        if (item_remove_ticker > 5)
        {
            RemoveHeldItem();
            item_remove_ticker = 0;
        }
    }
    if (is_buffed)
    {
        buff_timer -= delta_time;
        if (buff_timer <= 0.0f)
        {
            is_buffed = false;
            player_Movement_Speed = original_movement_speed;
            player_Damage_Multiplier = original_damage_multiplier;
            RemoveHeldItem();
        }
    }

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
    else if (otherType == Collision_Type::CONSUMABLE)
    {
        if (auto* item = dynamic_cast<ItemBase*>(other))
        {
            if (item->GetType() == ItemType::FAIRY)
            {
                SetHasFairy(true);
                item->Mark_For_Destruction();
            }

            else if (!HasItem())
            {
                PickUpItem(item);
            }
        }
    }
}

void Player_Base_Class::Draw()
{
    DrawTexture(this->maintex, this->hitbox.x,hitbox.y,WHITE);
}

void Player_Base_Class::Ranged_Attack()
{
    if (is_buffed) return;
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

    int final_damage = static_cast<int>(game::Config::player_Ranged_Damage_Value * this->player_Damage_Multiplier);

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
    else {
        if (move_Y > 0.0f) facing_Direction = Facing_Direction::DOWN;
        else if (move_Y < 0.0f) facing_Direction = Facing_Direction::UP;
    }
}

Collision_Type Player_Base_Class::Get_Collision_Type() const
{
    return Collision_Type::PLAYER;
}

Vector2 Player_Base_Class::Get_Player_Pos()
{
    return this->player_Pos;
}

void Player_Base_Class::Take_Damage(int damage_amount)
{
    if (is_buffed && damage_amount > 0) return;

    player_Health -= damage_amount;
    player_Health = std::min(player_Health, (float)player_Max_Health);
}

Vector2 Player_Base_Class::Get_Player_Center() const
{
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

void Player_Base_Class::Melee_Attack()
{
    this->melee_Cooldown = game::Config::player_Melee_Attack_Cooldown;
    this->currentState = ATTACKING_MELEE;
    int final_damage = static_cast<int>(game::Config::player_Melee_Damage_Value * this->player_Damage_Multiplier);
    auto* melee_box = new game::Player_Melee_Hitbox(this, final_damage, this->facing_Direction);

    if (object_manager_ptr)
    {
        object_manager_ptr->AddObject(melee_box);
    }
}

bool Player_Base_Class::HasItem() const
{
    return held_item != nullptr;
}

void Player_Base_Class::PickUpItem(ItemBase* item_to_pick_up)
{
    if (!HasItem() && object_manager_ptr != nullptr)
    {
        held_item = item_to_pick_up;
        object_manager_ptr->RemoveObject(item_to_pick_up);
    }
}

void Player_Base_Class::Use_Item()
{
    if (IsKeyPressed(game::Config::key_Use_Item) && HasItem()&& !is_buffed)
    {
        held_item->Activate(this);
    }
}

void Player_Base_Class::RemoveHeldItem()
{
    if (held_item != nullptr)
    {
        delete held_item;
        held_item = nullptr;
    }
}

void Player_Base_Class::ApplyTestoBuff()
{
    if (!is_buffed)
    {
        is_buffed = true;
        player_Movement_Speed *= game::Config::testo_Needle_Speed_Boost;
        player_Damage_Multiplier *= game::Config::testo_Needle_Damage_Boost;
    }
    buff_timer = game::Config::testo_Needle_Buff_Duration;
}

bool Player_Base_Class::IsBuffed() const
{
    return is_buffed;
}

void Player_Base_Class::Calculate_Melee_Hitboxes(std::vector<Rectangle>& out_hitboxes, Facing_Direction direction) const
{
    out_hitboxes.clear();

    const bool buffed = IsBuffed();
    const float side_reach = (buffed ? game::Config::player_Melee_Buff_Side_Reach_Tiles : game::Config::player_Melee_Side_Reach_Tiles) * 16.0f;
    const float side_width = (buffed ? game::Config::player_Melee_Buff_Side_Width_Tiles : game::Config::player_Melee_Side_Width_Tiles) * 16.0f;
    const float diag_reach = (buffed ? game::Config::player_Melee_Buff_Diag_Reach_Tiles : game::Config::player_Melee_Diag_Reach_Tiles) * 16.0f;
    const float diag_width = (buffed ? game::Config::player_Melee_Buff_Diag_Width_Tiles : game::Config::player_Melee_Diag_Width_Tiles) * 16.0f;

    const float side_offset = game::Config::player_Melee_Side_Offset;
    const float vert_offset = game::Config::player_Melee_Vertical_Offset;

    const Vector2 player_center = Get_Player_Center();
    const Rectangle player_box = Get_Hitbox();

    switch (direction)
    {
        case UP:
            out_hitboxes.push_back({player_center.x - side_width / 2, player_box.y - side_reach + vert_offset, side_width, side_reach});
            break;
        case DOWN:
            out_hitboxes.push_back({player_center.x - side_width / 2, player_box.y + player_box.height - vert_offset, side_width, side_reach});
            break;
        case LEFT:
            out_hitboxes.push_back({player_box.x - side_reach + side_offset, player_center.y - side_width / 2, side_reach, side_width});
            break;
        case RIGHT:
            out_hitboxes.push_back({player_box.x + player_box.width - side_offset, player_center.y - side_width / 2, side_reach, side_width});
            break;

        case UP_LEFT:
        {
            float anchor_x = player_box.x - diag_reach;
            float anchor_y = player_box.y - diag_reach;
            out_hitboxes.push_back({anchor_x, anchor_y, diag_reach, diag_width});
            out_hitboxes.push_back({anchor_x, anchor_y, diag_width, diag_reach});
            break;
        }
        case UP_RIGHT:
        {
            float anchor_x = player_box.x + player_box.width + diag_reach;
            float anchor_y = player_box.y - diag_reach;
            out_hitboxes.push_back({anchor_x - diag_reach, anchor_y, diag_reach, diag_width});
            out_hitboxes.push_back({anchor_x - diag_width, anchor_y, diag_width, diag_reach});
            break;
        }
        case DOWN_LEFT:
        {
            float anchor_x = player_box.x - diag_reach;
            float anchor_y = player_box.y + player_box.height + diag_reach;
            out_hitboxes.push_back({anchor_x, anchor_y - diag_width, diag_reach, diag_width});
            out_hitboxes.push_back({anchor_x, anchor_y - diag_reach, diag_width, diag_reach});
            break;
        }
        case DOWN_RIGHT:
        {
            float anchor_x = player_box.x + player_box.width + diag_reach;
            float anchor_y = player_box.y + player_box.height + diag_reach;
            out_hitboxes.push_back({anchor_x - diag_reach, anchor_y - diag_width, diag_reach, diag_width});
            out_hitboxes.push_back({anchor_x - diag_width, anchor_y - diag_reach, diag_width, diag_reach});
            break;
        }

        case NONE:
            return;
    }
}
