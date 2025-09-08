//
// Created by Manza on 6/22/2025.
//

#include "MeleeEnemy.h"
#include "CollisionResponse.h"
#include "raymath.h"
#include "../Config.h.in"

namespace enemy
{
    std::map<std::string, Texture2D> Melee_Enemy::s_melee_textures;

    void Melee_Enemy::Load_All_Melee_Assets()
    {
        s_melee_textures["Bauer_Walk_Left"] = LoadTexture(game::Config::kMeleeEnemy1WalkLeftAnim);
        s_melee_textures["Bauer_Walk_Right"] = LoadTexture(game::Config::kMeleeEnemy1WalkRightAnim);
        s_melee_textures["Bauer_Attack_Left"] = LoadTexture(game::Config::kMeleeEnemy1AttackLeftAnim);
        s_melee_textures["Bauer_Attack_Right"] = LoadTexture(game::Config::kMeleeEnemy1AttackRightAnim);

        s_melee_textures["Ritter_Walk_Left"] = LoadTexture(game::Config::kMeleeEnemy2WalkLeftAnim);
        s_melee_textures["Ritter_Walk_Right"] = LoadTexture(game::Config::kMeleeEnemy2WalkRightAnim);
        s_melee_textures["Ritter_Attack_Left"] = LoadTexture(game::Config::kMeleeEnemy2AttackLeftAnim);
        s_melee_textures["Ritter_Attack_Right"] = LoadTexture(game::Config::kMeleeEnemy2AttackRightAnim);

        s_melee_textures["Demonenritter_Walk_Left"] = LoadTexture(game::Config::kMeleeEnemy3WalkLeftAnim);
        s_melee_textures["Demonenritter_Walk_Right"] = LoadTexture(game::Config::kMeleeEnemy3WalkRightAnim);
        s_melee_textures["Demonenritter_Attack_Left"] = LoadTexture(game::Config::kMeleeEnemy3AttackLeftAnim);
        s_melee_textures["Demonenritter_Attack_Right"] = LoadTexture(game::Config::kMeleeEnemy3AttackRightAnim);
    }

    void Melee_Enemy::Unload_All_Melee_Assets()
    {
        for (auto const& [key, val] : s_melee_textures)
        {
            UnloadTexture(val);
        }
        s_melee_textures.clear();
    }

    Melee_Enemy::Melee_Enemy(Vector2 start_position, const std::string& name, int health, float speed, int damage,
        int score, int souls, float cooldown, Vector2 hitbox_size, const char* walk_left_path,
        const char* walk_right_path, const char* attack_left_path, const char* attack_right_path, Vector2 walk_anim_size,
        int walk_frame_count, float walk_anim_speed, Vector2 attack_anim_size, int attack_frame_count, float attack_anim_speed)

     : Enemy_Base_Class(name, health, speed, damage, score, souls, start_position, hitbox_size.x, hitbox_size.y, cooldown,
          game::Config::kAIBase_SeekWeight, game::Config::kAIBase_SeparationWeight, game::Config::kAIBase_PlayerSeparationWeight,
          game::Config::kAIBase_DesiredSeparation, game::Config::kAIBase_Drag)
    {
        this->walk_texture_left = &s_melee_textures.at(name + "_Walk_Left");
        this->walk_texture_right = &s_melee_textures.at(name + "_Walk_Right");
        this->attack_texture_left = &s_melee_textures.at(name + "_Attack_Left");
        this->attack_texture_right = &s_melee_textures.at(name + "_Attack_Right");

        walk_animations.try_emplace(LEFT, walk_anim_size, *this->walk_texture_left,
            walk_frame_count, walk_frame_count, walk_anim_speed);
        walk_animations.try_emplace(RIGHT, walk_anim_size, *this->walk_texture_right,
            walk_frame_count, walk_frame_count, walk_anim_speed);

        attack_animations.try_emplace(LEFT, attack_anim_size, *this->attack_texture_left,
            attack_frame_count, attack_frame_count, attack_anim_speed);
        attack_animations.try_emplace(RIGHT, attack_anim_size, *this->attack_texture_right,
            attack_frame_count, attack_frame_count, attack_anim_speed);
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