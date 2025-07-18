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
    Enemy_Base_Class::Tick(delta_time);

    Vector2 self_center = { this->hitbox.x + this->hitbox.width / 2.0f, this->hitbox.y + this->hitbox.height / 2.0f };
    Vector2 target_center = { target_Position_X, target_Position_Y };
    float distance_to_target = Vector2Distance(self_center, target_center);

    // --- 2. DYNAMISCHE STOPP-DISTANZ BERECHNEN ---
    float delta_x = std::abs(target_center.x - self_center.x);
    float delta_y = std::abs(target_center.y - self_center.y);
    float stopping_distance;

    if (delta_x > delta_y) {
        stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);
    } else {
        stopping_distance = (this->hitbox.height / 2.0f) + (game::Config::player_Hittbox.y / 2.0f);
    }

    // --- 3. BLICKRICHTUNG ---
    if (target_center.x > self_center.x + 2.0f) {
        facing_Direction = RIGHT;
    } else if (target_center.x < self_center.x - 2.0f) {
        facing_Direction = LEFT;
    }

    // --- 4. ZUSTANDS-LOGIK ---
    if (currentState == E_ATTACKING) {
        // Wenn die Angriffs-Animation beendet ist, gehe in den Leerlauf.
        if (attack_animations.count(attack_Direction) && attack_animations.at(attack_Direction).IsFinished()) {
            currentState = E_IDLE;
        }
    } else {
        // Wenn wir nah genug dran sind und der Cooldown bereit ist, greife an.
        if (distance_to_target <= stopping_distance + 5.0f && attack_Cooldown_Timer <= 0) {
            Melee_Attack();
        } else {
            // Sonst: Laufen oder im Leerlauf warten.
            currentState = (distance_to_target > stopping_distance) ? E_WALKING : E_IDLE;
        }
    }

    // --- 5. AKTION & ANIMATION ---
    // Bewegung ausführen, wenn wir nicht angreifen und zu weit weg sind.
    if (currentState != E_ATTACKING && distance_to_target > stopping_distance) {
        Pathfinding(target_Position_X, target_Position_Y, delta_time, stopping_distance);
    }

    // Passende Animation abspielen.
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
        // 1. Logge die Richtung im Moment des Angriffs ein
        this->attack_Direction = this->facing_Direction;

        // 2. Setze den Zustand und den Cooldown
        currentState = E_ATTACKING;
        attack_Cooldown_Timer = game::Config::melee_enemy_1_attack_cooldown;

        // 3. Setze die Animation für die eingeloggte Richtung auf den Anfang
        if (attack_animations.count(this->attack_Direction)) {
            attack_animations.at(this->attack_Direction).First_Frame();
        }

        // 4. Setze den Schadens-Flag zurück, damit beim nächsten Treffer Schaden möglich ist
        this->damage_applied_this_attack = false;
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
        //DrawRectangleLinesEx(this->hitbox, 2.0f, RED);
    }
    void Melee_Enemy::On_Collision(Collidable* other)
    {
        // Prüfen, ob das andere Objekt der Spieler ist
        if (other->Get_Collision_Type() == Collision_Type::PLAYER)
        {
            // Prüfen, ob wir im Angriffszustand sind UND in diesem Angriff noch keinen Schaden verursacht haben
            if (currentState == E_ATTACKING && !damage_applied_this_attack)
            {
                // Füge dem Spieler Schaden zu
                CollisionResponse::Apply_Damage(other, this->enemy_Damage);

                // Setze den Flag, damit dieser Angriff keinen weiteren Schaden macht
                damage_applied_this_attack = true;
            }
        }

        // Rufe immer die Basis-Implementierung auf, damit das Wegschieben von Wänden etc. funktioniert.
        Enemy_Base_Class::On_Collision(other);
    }

}