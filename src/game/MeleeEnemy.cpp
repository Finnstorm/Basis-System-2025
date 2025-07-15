//
// Created by Manza on 6/22/2025.
//

#include "MeleeEnemy.h"
#include <cmath>
#include <iostream>
#include <raylib.h>

#include "raymath.h"
#include "../Config.h.in"
namespace enemy {
    Melee_Enemy::Melee_Enemy(Vector2 start_position)
        : Enemy_Base_Class("Bauer",
                           game::Config::melee_enemy_1_health,
                           game::Config::melee_enemy_1_movement_speed,
                           game::Config::melee_enemy_1_damage,
                           10,
                           nullptr,
                           nullptr,
                           start_position,
                           game::Config::melee_enemy_1_hitbox.x,
                           game::Config::melee_enemy_1_hitbox.y,
                           game::Config::melee_enemy_1_attack_cooldown)
    {
        walk_animations.try_emplace(LEFT, game::Config::melee_enemy_1_walk_anim_size, game::Config::kMeleeEnemy1WalkLeftAnim,
                                  game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_frame_count,
                                  game::Config::melee_enemy_1_walk_anim_speed);
        walk_animations.try_emplace(RIGHT, game::Config::melee_enemy_1_walk_anim_size, game::Config::kMeleeEnemy1WalkRightAnim,
                                  game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_frame_count,
                                  game::Config::melee_enemy_1_walk_anim_speed);

        attack_animations.try_emplace(LEFT, game::Config::melee_enemy_1_attack_anim_size, game::Config::kMeleeEnemy1AttackLeftAnim,
                                    game::Config::melee_enemy_1_attack_frame_count, game::Config::melee_enemy_1_attack_frame_count,
                                    game::Config::melee_enemy_1_attack_anim_speed);
        attack_animations.try_emplace(RIGHT, game::Config::melee_enemy_1_attack_anim_size, game::Config::kMeleeEnemy1AttackRightAnim,
                                    game::Config::melee_enemy_1_attack_frame_count, game::Config::melee_enemy_1_attack_frame_count,
                                    game::Config::melee_enemy_1_attack_anim_speed);
    }

void Melee_Enemy::Tick(float delta_time, float target_Position_X, float target_Position_Y)
{
    // 1. Timer und grundlegende Werte aktualisieren
    Enemy_Base_Class::Tick(delta_time);
    Vector2 self_center = {hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2};
    float distance_to_target = Vector2Distance(self_center, {target_Position_X, target_Position_Y});
    facing_Direction = (target_Position_X < self_center.x) ? LEFT : RIGHT;

    // Definiere die Distanz, bei der der Gegner stehen bleibt und angreifen kann.
    float stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);

    // 2. Zustands-Übergänge
    // Priorität 1: Wenn ein Angriff läuft, prüfe, ob die Animation beendet ist.
    if (currentState == E_ATTACKING) {
        // Prüfen, ob die Animation existiert UND fertig ist.
        if (attack_animations.count(facing_Direction) && attack_animations.at(facing_Direction).IsFinished()) {
            currentState = E_IDLE; // Nach dem Angriff in den Leerlauf.
        }
    }
    // Priorität 2: Wenn wir NICHT angreifen, prüfe auf neuen Angriff.
    // Angriff startet, wenn Cooldown bereit und Distanz klein genug ist (direkter Kontakt).
    else if (distance_to_target <= stopping_distance && attack_Cooldown_Timer <= 0) {
        Melee_Attack(); // Startet einen neuen Angriff (setzt currentState auf E_ATTACKING)
    }

    // 3. Bewegungs- und Animationslogik basierend auf dem Zustand
    if (currentState == E_ATTACKING) {
        // Während des Angriffs weiterlaufen (wie gewünscht) und Animation updaten.
        Pathfinding(target_Position_X, target_Position_Y, delta_time);
        attack_animations.at(facing_Direction).Update_Frame(delta_time);

    } else if (distance_to_target > stopping_distance) {
        // Laufen, wenn zu weit weg
        currentState = E_WALKING;
        Pathfinding(target_Position_X, target_Position_Y, delta_time);
        walk_animations.at(facing_Direction).Update_Frame(delta_time);

    } else {
        // Stehen bleiben (Idle), wenn nah genug, aber nicht angreifend.
        currentState = E_IDLE;
        // Hier keine Animation updaten oder Idle-Animation, falls vorhanden.
    }
}

    void Melee_Enemy::Melee_Attack()
    {
        // Setzt alle relevanten Zustände und Timer
        currentState = E_ATTACKING;
        attack_Cooldown_Timer = game::Config::melee_enemy_1_attack_cooldown;

        if (attack_animations.count(facing_Direction)) {
            attack_animations.at(facing_Direction).First_Frame();
        }
    }

    void Melee_Enemy::Draw() {
        Vector2 draw_pos;
        Animations* attack_anim = nullptr;
        RepeatAnimation* walk_anim = nullptr;

        if (currentState == E_ATTACKING && attack_animations.count(facing_Direction)) {
            attack_anim = &attack_animations.at(facing_Direction);
        } else if (walk_animations.count(facing_Direction)) {
            walk_anim = &walk_animations.at(facing_Direction);
        }

        if (attack_anim != nullptr) {
            draw_pos.x = this->hitbox.x - (attack_anim->size.x - this->hitbox.width) / 2.0f;
            draw_pos.y = this->hitbox.y - (attack_anim->size.y - this->hitbox.height) / 2.0f;
            attack_anim->Draw_Current_Frame(draw_pos);
        } else if (walk_anim != nullptr) {
            draw_pos.x = this->hitbox.x - (walk_anim->size.x - this->hitbox.width) / 2.0f;
            draw_pos.y = this->hitbox.y - (walk_anim->size.y - this->hitbox.height) / 2.0f;
            walk_anim->Draw_Current_Frame(draw_pos);
        }
    }
}