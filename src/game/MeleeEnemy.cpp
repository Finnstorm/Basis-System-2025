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
    // --- 1. SETUP ---
    Enemy_Base_Class::Tick(delta_time); // Aktualisiert den Cooldown-Timer

    // Berechne die aktuelle Distanz und Blickrichtung zum Ziel
    Vector2 self_center = { hitbox.x + hitbox.width / 2.0f, hitbox.y + hitbox.height / 2.0f };
    facing_Direction = (target_Position_X < self_center.x) ? LEFT : RIGHT;
    float distance_to_target = Vector2Distance(self_center, { target_Position_X, target_Position_Y });
    float stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);

    // --- 2. ZUSTANDS-LOGIK ---
    if (currentState == E_ATTACKING) {
        // Bedingung zum Verlassen des Angriffszustands: Die Animation ist beendet.
        if (attack_animations.count(attack_Direction) && attack_animations.at(attack_Direction).IsFinished()) {
            currentState = E_IDLE;
        }
    } else { // Wenn wir gerade nicht angreifen...
        // Bedingung zum Starten eines Angriffs: In Reichweite und Cooldown bereit.
        if (distance_to_target <= (stopping_distance + 2.0f) && attack_Cooldown_Timer <= 0) {
            Melee_Attack(); // Diese Funktion kümmert sich um alles Weitere.
        }
        // Ansonsten: Entscheiden, ob wir laufen oder stehen.
        else {
            currentState = (distance_to_target > stopping_distance) ? E_WALKING : E_IDLE;
        }
    }

    // --- 3. AKTIONS- & ANIMATIONS-LOGIK (basiert auf dem finalen Zustand) ---
    // Der Gegner bewegt sich immer, wenn er zu weit weg ist, auch während des Angriffs.
    if (distance_to_target > stopping_distance) {
        Pathfinding(target_Position_X, target_Position_Y, delta_time);
    }

    // Aktualisiere die korrekte Animation für den aktuellen Zustand.
    if (currentState == E_ATTACKING) {
        // BENUTZE 'attack_Direction' für die Angriffsanimation.
        if (attack_animations.count(attack_Direction)) {
            attack_animations.at(attack_Direction).Update_Frame(delta_time);
        }
    } else if (currentState == E_WALKING) {
        // BENUTZE die Live-'facing_Direction' für die Laufanimation.
        if (walk_animations.count(facing_Direction)) {
            walk_animations.at(facing_Direction).Update_Frame(delta_time);
        }
    }
}

    void Melee_Enemy::Melee_Attack()
    {
        std::cout << "ANGRIFF GESTARTET! Richtung: " << facing_Direction << std::endl; // <-- DEBUG-ZEILE
        // 1. Logge die Richtung im Moment des Angriffs ein
        this->attack_Direction = this->facing_Direction;

        // 2. Setze den Zustand und den Cooldown
        currentState = E_ATTACKING;
        attack_Cooldown_Timer = game::Config::melee_enemy_1_attack_cooldown;

        // 3. Setze die Animation für die eingeloggte Richtung auf den Anfang
        if (attack_animations.count(this->attack_Direction)) {
            attack_animations.at(this->attack_Direction).First_Frame();
        }
    }

    void Melee_Enemy::Draw() {
        Vector2 draw_pos;
        Animations* attack_anim = nullptr;
        RepeatAnimation* walk_anim = nullptr;

        if (currentState == E_ATTACKING) {
            std::cout << "ZEICHNE ANGRIFF! Zustand: " << currentState << std::endl; // <-- DEBUG-ZEILE
            if (attack_animations.count(attack_Direction)) {
                attack_anim = &attack_animations.at(attack_Direction);
            }
        } else if (currentState == E_WALKING) {
            // BENUTZE die Live-'facing_Direction' für die Laufanimation
            if (walk_animations.count(facing_Direction)) {
                walk_anim = &walk_animations.at(facing_Direction);
            }
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