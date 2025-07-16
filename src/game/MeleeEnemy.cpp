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

    // Berechne die aktuelle Blickrichtung (nur für die visuelle Darstellung)
    float delta_x = target_Position_X - (this->hitbox.x + this->hitbox.width / 2.0f);
    if (delta_x > 2.0f) {
        facing_Direction = RIGHT;
    } else if (delta_x < -2.0f) {
        facing_Direction = LEFT;
    }
    // (Blickrichtung wird bei vertikaler Position nicht mehr geändert)

    // --- 2. ZUSTANDS-LOGIK MIT NEUER TRIGGER-PRÜFUNG ---
    if (currentState == E_ATTACKING) {
        // Bedingung zum Verlassen des Angriffszustands: Die Animation ist beendet.
        if (attack_animations.count(attack_Direction) && attack_animations.at(attack_Direction).IsFinished()) {
            currentState = E_IDLE;
        }
    } else { // Wenn wir gerade nicht angreifen...

        // --- NEUE, ROBUSTE ANGRIFFS-PRÜFUNG ---
        // Erstelle die Hitbox des Spielers für die Kollisionsprüfung.
        Rectangle player_hitbox = {
            target_Position_X - game::Config::player_Hittbox.x / 2.0f,
            target_Position_Y - game::Config::player_Hittbox.y / 2.0f,
            game::Config::player_Hittbox.x,
            game::Config::player_Hittbox.y
        };

        // Erstelle eine leicht vergrößerte "Trigger-Box" um den Gegner.
        float trigger_buffer = 2.0f;
        Rectangle attack_trigger_box = {
            this->hitbox.x - trigger_buffer,
            this->hitbox.y - trigger_buffer,
            this->hitbox.width + (trigger_buffer * 2),
            this->hitbox.height + (trigger_buffer * 2)
        };

        // Bedingung zum Starten eines Angriffs: Die Trigger-Box kollidiert mit dem Spieler UND Cooldown ist bereit.
        if (CheckCollisionRecs(attack_trigger_box, player_hitbox) && attack_Cooldown_Timer <= 0) {
            Melee_Attack();
        }
        else {
            // Wenn nicht angegriffen wird, bestimme, ob gelaufen oder gestanden wird.
            float distance_to_target = Vector2Distance({this->hitbox.x, this->hitbox.y}, {target_Position_X, target_Position_Y});
            float stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);
            currentState = (distance_to_target > stopping_distance) ? E_WALKING : E_IDLE;
        }
    }

    // --- 3. AKTIONS- & ANIMATIONS-LOGIK (unverändert) ---
    // (Bewegung und Animations-Updates bleiben wie im letzten Schritt)
    float distance_to_target = Vector2Distance({this->hitbox.x, this->hitbox.y}, {target_Position_X, target_Position_Y});
    float stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);
    if (distance_to_target > stopping_distance) {
        Pathfinding(target_Position_X, target_Position_Y, delta_time, stopping_distance);
    }

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
            // Angriffs-Logik (unverändert)
            if (attack_animations.count(attack_Direction)) {
                attack_anim = &attack_animations.at(attack_Direction);
            }
        } else { // Gilt für E_WALKING und E_IDLE
            // Wenn nicht angegriffen wird, ist die Laufanimation unser Standard.
            // Sie wird in Tick() nur aktualisiert, wenn der Zustand E_WALKING ist.
            if (walk_animations.count(facing_Direction)) {
                walk_anim = &walk_animations.at(facing_Direction);
            }
        }

        // Der Rest der Funktion zum Zeichnen bleibt gleich.
        if (attack_anim != nullptr) {
            draw_pos.x = this->hitbox.x - (attack_anim->size.x - this->hitbox.width) / 2.0f;
            draw_pos.y = this->hitbox.y - (attack_anim->size.y - this->hitbox.height) / 2.0f;
            attack_anim->Draw_Current_Frame(draw_pos);
        } else if (walk_anim != nullptr) {
            draw_pos.x = this->hitbox.x - (walk_anim->size.x - this->hitbox.width) / 2.0f;
            draw_pos.y = this->hitbox.y - (walk_anim->size.y - this->hitbox.height) / 2.0f;
            walk_anim->Draw_Current_Frame(draw_pos);
        }
        DrawRectangleLinesEx(this->hitbox, 2.0f, RED);
    }
}