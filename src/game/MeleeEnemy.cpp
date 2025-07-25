//
// Created by Manza on 6/22/2025.
//

#include "MeleeEnemy.h"
#include <cmath>
#include <iostream>
#include <raylib.h>

#include "CollisionResponse.h"
#include "raymath.h"
#include "../Config.h.in"
namespace enemy
{
    Texture2D Melee_Enemy::walk_texture_left;
    Texture2D Melee_Enemy::walk_texture_right;
    Texture2D Melee_Enemy::attack_texture_left;
    Texture2D Melee_Enemy::attack_texture_right;

    void Melee_Enemy::Load_Assets()
    {
        walk_texture_left = LoadTexture(game::Config::kMeleeEnemy1WalkLeftAnim);
        walk_texture_right = LoadTexture(game::Config::kMeleeEnemy1WalkRightAnim);
        attack_texture_left = LoadTexture(game::Config::kMeleeEnemy1AttackLeftAnim);
        attack_texture_right = LoadTexture(game::Config::kMeleeEnemy1AttackRightAnim);
    }

    Melee_Enemy::Melee_Enemy(Vector2 start_position)
        : Enemy_Base_Class( "Bauer", game::Config::melee_enemy_1_health, game::Config::melee_enemy_1_movement_speed,
        game::Config::melee_enemy_1_damage, start_position, game::Config::melee_enemy_1_hitbox.x,
        game::Config::melee_enemy_1_hitbox.y, game::Config::melee_enemy_1_attack_cooldown,
        game::Config::kAIBase_SeekWeight, game::Config::kAIBase_SeparationWeight,
        game::Config::kAIBase_PlayerSeparationWeight, game::Config::kAIBase_DesiredSeparation, game::Config::kAIBase_Drag)
    {
        walk_animations.try_emplace(LEFT, game::Config::melee_enemy_1_walk_anim_size, walk_texture_left,
        game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_frame_count,
        game::Config::melee_enemy_1_walk_anim_speed); walk_animations.try_emplace(RIGHT,
        game::Config::melee_enemy_1_walk_anim_size, walk_texture_right, game::Config::melee_enemy_1_walk_frame_count,
        game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_anim_speed);

        attack_animations.try_emplace(LEFT, game::Config::melee_enemy_1_attack_anim_size, attack_texture_left,
        game::Config::melee_enemy_1_attack_frame_count, game::Config::melee_enemy_1_attack_frame_count,
        game::Config::melee_enemy_1_attack_anim_speed);  attack_animations.try_emplace(RIGHT,
        game::Config::melee_enemy_1_attack_anim_size, attack_texture_right,
        game::Config::melee_enemy_1_attack_frame_count, game::Config::melee_enemy_1_attack_frame_count,
        game::Config::melee_enemy_1_attack_anim_speed);
    }

    void Melee_Enemy::Tick_Melee(float delta_time, Vector2 player_center)
    {
        Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
        if (player_center.x > self_center.x + 2.0f) {
            facing_Direction = RIGHT;
        } else if (player_center.x < self_center.x - 2.0f) {
            facing_Direction = LEFT;
        }
        float distance_to_target = Vector2Distance(self_center, player_center);
        float stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);

        if (currentState == E_ATTACKING)
        {
            if (attack_animations.at(attack_Direction).IsFinished())
            {
                currentState = E_IDLE;
            }
        }
        else
        {
            if (distance_to_target <= stopping_distance + 5.0f && attack_Cooldown_Timer <= 0)
            {
                Melee_Attack();
            }
            else
            {
                currentState = (Vector2LengthSqr(this->velocity) > 0.1f) ? E_WALKING : E_IDLE;
            }
        }
        if (currentState == E_ATTACKING) {
            attack_animations.at(attack_Direction).Update_Frame(delta_time);
        } else if (currentState == E_WALKING) {
            walk_animations.at(facing_Direction).Update_Frame(delta_time);
        }
    }

void Melee_Enemy::Melee_Attack()
{
    this->attack_Direction = this->facing_Direction;
    currentState = E_ATTACKING;
    attack_Cooldown_Timer = game::Config::melee_enemy_1_attack_cooldown;
    if (attack_animations.count(this->attack_Direction))
    {
        attack_animations.at(this->attack_Direction).First_Frame();
    }
        this->damage_applied_this_attack = false;
    }

void Melee_Enemy::Draw()
{
    Vector2 draw_pos;
    Animations* attack_anim = nullptr;
    RepeatAnimation* walk_anim = nullptr;
    if (currentState == E_ATTACKING)
    {
        if (attack_animations.count(attack_Direction))
        {
            attack_anim = &attack_animations.at(attack_Direction);
        }
    }
    else
    {
        if (walk_animations.count(facing_Direction))
        {
            walk_anim = &walk_animations.at(facing_Direction);
        }
    }

    if (attack_anim != nullptr)
    {
        draw_pos.x = this->hitbox.x - (attack_anim->size.x - this->hitbox.width) / 2.0f;
        draw_pos.y = this->hitbox.y - (attack_anim->size.y - this->hitbox.height) / 2.0f;
        attack_anim->Draw_Current_Frame(draw_pos);
    }
    else if (walk_anim != nullptr)
    {
        draw_pos.x = this->hitbox.x - (walk_anim->size.x - this->hitbox.width) / 2.0f;
        draw_pos.y = this->hitbox.y - (walk_anim->size.y - this->hitbox.height) / 2.0f;
        walk_anim->Draw_Current_Frame(draw_pos);
    }
    //DrawRectangleLinesEx(this->hitbox, 2.0f, RED);
}

void Melee_Enemy::On_Collision(Collidable* other)
{
    if (other->Get_Collision_Type() == Collision_Type::PLAYER)
    {
        if (currentState == E_ATTACKING && !damage_applied_this_attack)
        {
            CollisionResponse::Apply_Damage(other, this->enemy_Damage);
            damage_applied_this_attack = true;
        }
    }
    Enemy_Base_Class::On_Collision(other);
}
}