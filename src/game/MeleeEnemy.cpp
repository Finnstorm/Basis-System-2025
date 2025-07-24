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
        : Enemy_Base_Class("Bauer", game::Config::melee_enemy_1_health, game::Config::melee_enemy_1_movement_speed,
        game::Config::melee_enemy_1_damage, 10, nullptr, nullptr, start_position,
        game::Config::melee_enemy_1_hitbox.x, game::Config::melee_enemy_1_hitbox.y,
        game::Config::melee_enemy_1_attack_cooldown)
    {
        walk_animations.try_emplace(LEFT, game::Config::melee_enemy_1_walk_anim_size, walk_texture_left,
        game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_anim_speed);
        walk_animations.try_emplace(RIGHT, game::Config::melee_enemy_1_walk_anim_size, walk_texture_right,
        game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_anim_speed);

        attack_animations.try_emplace(LEFT, game::Config::melee_enemy_1_attack_anim_size, attack_texture_left,
        game::Config::melee_enemy_1_attack_frame_count, game::Config::melee_enemy_1_attack_frame_count, game::Config::melee_enemy_1_attack_anim_speed);
        attack_animations.try_emplace(RIGHT, game::Config::melee_enemy_1_attack_anim_size, attack_texture_right,
        game::Config::melee_enemy_1_attack_frame_count, game::Config::melee_enemy_1_attack_frame_count, game::Config::melee_enemy_1_attack_anim_speed);
    }

void Melee_Enemy::Tick(float delta_time, float target_Position_X, float target_Position_Y, const std::vector<Enemy_Base_Class*>& all_enemies)
{
    Enemy_Base_Class::Tick(delta_time); // Cooldown-Timer aktualisieren

    Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
    Vector2 target_center = { target_Position_X, target_Position_Y };

    // Stoppdistanz berechnen
    float delta_x = std::abs(target_center.x - self_center.x);
    float delta_y = std::abs(target_center.y - self_center.y);
    float stopping_distance;
    if (delta_x > delta_y) {
        stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);
    } else {
        stopping_distance = (this->hitbox.height / 2.0f) + (game::Config::player_Hittbox.y / 2.0f);
    }

    // Facing Direction aktualisieren
    if (target_center.x > self_center.x + 2.0f) {
        facing_Direction = RIGHT;
    } else if (target_center.x < self_center.x - 2.0f) {
        facing_Direction = LEFT;
    }

    // Logik für den Zustand (State Machine)
    float distance_to_target; // Wird von Calculate_Seek_Force befüllt
    Vector2 seek_force = Calculate_Seek_Force(target_center, distance_to_target, stopping_distance);

    if (currentState == E_ATTACKING) {
        if (attack_animations.count(attack_Direction) && attack_animations.at(attack_Direction).IsFinished()) {
            currentState = E_IDLE;
        }
    } else {
        if (distance_to_target <= stopping_distance + 5.0f && attack_Cooldown_Timer <= 0) {
            Melee_Attack();
        } else {
            currentState = (distance_to_target > stopping_distance) ? E_WALKING : E_IDLE;
        }
    }

        if (currentState != E_ATTACKING)
        {
            Vector2 seek_force = Calculate_Seek_Force(target_center, distance_to_target, stopping_distance);
            Vector2 separation_force = Calculate_Separation_Force(all_enemies);
            Vector2 player_separation_force = Calculate_Player_Separation_Force(target_center);
            float seek_weight = 2.0f;
            float separation_weight = 1.2f;
            float player_separation_weight = 3.0f;
            Vector2 total_force = {0.0f, 0.0f};
            total_force = Vector2Add(total_force, Vector2Scale(seek_force, seek_weight));
            total_force = Vector2Add(total_force, Vector2Scale(separation_force, separation_weight));
            total_force = Vector2Add(total_force, Vector2Scale(player_separation_force, player_separation_weight)); // NEU
            // ---- NEUE PHYSIK-BASIERTE BEWEGUNG ----

            // 1. Beschleunigung aus der Kraft ableiten (wir nehmen Masse = 1 an)
            Vector2 acceleration = total_force;

            // 2. Geschwindigkeit durch Beschleunigung aktualisieren
            this->velocity = Vector2Add(this->velocity, Vector2Scale(acceleration, Get_Movement_Speed() * delta_time));

            // 3. Geschwindigkeit auf ein Maximum begrenzen
            float max_speed = Get_Movement_Speed();
            if (Vector2Length(this->velocity) > max_speed)
            {
                this->velocity = Vector2Scale(Vector2Normalize(this->velocity), max_speed);
            }

            // 4. Position durch Geschwindigkeit aktualisieren
            this->hitbox.x += this->velocity.x * delta_time;
            this->hitbox.y += this->velocity.y * delta_time;

            // 5. Einen leichten "Drag" einbauen, damit die Gegner anhalten, wenn keine Kraft wirkt
            this->velocity = Vector2Scale(this->velocity, 0.97f);
        }
        else
        {
            // Wenn der Gegner angreift, sollte er stehen bleiben.
            this->velocity = {0.0f, 0.0f};
        }


    // Animationslogik (bleibt gleich)
    if (currentState == E_ATTACKING) {
        if (attack_animations.count(attack_Direction)) {
            attack_animations.at(attack_Direction).Update_Frame(delta_time);
        }
    } else if (currentState == E_WALKING) {
        if (walk_animations.count(facing_Direction)) {
            walk_animations.at(facing_Direction).Update_Frame(delta_time);
        }
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
Vector2 Melee_Enemy::Calculate_Seek_Force(Vector2 target_pos, float& distance_to_target, float stopping_distance) const
{
    Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
    Vector2 direction = Vector2Subtract(target_pos, self_center);
    distance_to_target = Vector2Length(direction);

    // Nur bewegen, wenn wir außerhalb der Stoppdistanz sind
    if (distance_to_target <= stopping_distance || distance_to_target == 0.0f) {
        return {0.0f, 0.0f}; // Keine Kraft
    }

    return Vector2Normalize(direction);
}


    Vector2 Melee_Enemy::Calculate_Separation_Force(const std::vector<Enemy_Base_Class*>& all_enemies)
    {
        float desired_separation = 28.0f; // Kann nach Bedarf angepasst werden
        Vector2 steer = {0.0f, 0.0f};
        int count = 0;
        Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };

        for (const auto& other : all_enemies)
        {
            if (other == this) continue;

            Vector2 other_center = { other->Get_Hitbox().x + other->Get_Hitbox().width / 2.0f, other->Get_Hitbox().y + other->Get_Hitbox().height / 2.0f };
            float d = Vector2Distance(self_center, other_center);

            // Nur wenn sie wirklich zu nah sind
            if ((d > 0) && (d < desired_separation))
            {
                Vector2 diff = Vector2Subtract(self_center, other_center);
                Vector2Normalize(diff);

                // NEU: Skaliere die Kraft basierend darauf, wie nah sie sind.
                // Die Kraft ist am Rand der Zone (d == desired_separation) 0 und wird
                // stärker, je näher sie kommen (d -> 0).
                float strength = 1.0f - (d / desired_separation);
                diff = Vector2Scale(diff, strength);

                steer = Vector2Add(steer, diff);
                count++;
            }
        }

        if (count > 0)
        {
            steer = Vector2Scale(steer, 1.0f / count);
        }

        if (Vector2Length(steer) > 0)
        {
            steer = Vector2Normalize(steer);
        }

        return steer;
    }
    Vector2 Melee_Enemy::Calculate_Player_Separation_Force(Vector2 player_center) const
    {
        Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };

        // Ein kleiner Radius um den Spieler, in den Gegner nicht eindringen sollen.
        // Sollte etwa der halben Breite der Spieler-Hitbox entsprechen.
        float repulsion_radius = game::Config::player_Hittbox.x / 2.0f;

        float d = Vector2Distance(self_center, player_center);

        if (d < repulsion_radius)
        {
            // Berechne eine sanfte Abstoßungskraft weg vom Spieler
            Vector2 diff = Vector2Subtract(self_center, player_center);
            Vector2Normalize(diff);
            return diff;
        }

        return {0.0f, 0.0f}; // Keine Kraft, wenn außerhalb des Radius
    }
}