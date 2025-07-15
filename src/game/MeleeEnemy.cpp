//
// Created by Manza on 6/22/2025.
//

#include "MeleeEnemy.h"
#include <cmath>
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
                           game::Config::melee_enemy_1_attack_cooldown),
          attack_range(game::Config::melee_enemy_1_attack_range)
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
    // 1. Timer aktualisieren
    Enemy_Base_Class::Tick(delta_time);

    // 2. Distanz und Blickrichtung bestimmen
    Vector2 self_center = {hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2};
    float distance_to_target = Vector2Distance(self_center, {target_Position_X, target_Position_Y});
    facing_Direction = (target_Position_X < self_center.x) ? LEFT : RIGHT;

    // 3. --- FINALE STATE MACHINE ---

    // Priorität 1: Ist ein Angriff aktiv? Dann nur die Animation zu Ende führen.
    if (currentState == E_ATTACKING) {
        if (attack_duration_timer <= 0) {
            currentState = E_IDLE;
        }
    }
    // Priorität 2: Wenn nicht, entscheiden wir, was zu tun ist.
    else {
        // Definiere die Distanz, bei der der Gegner stehen bleibt.
        float stopping_distance = (this->hitbox.width / 2.0f) + (game::Config::player_Hittbox.x / 2.0f);

        // Fall A: Sind wir in Angriffsreichweite UND ist der Angriff bereit?
        if (distance_to_target <= attack_range && attack_Cooldown_Timer <= 0) {
            Melee_Attack(); // Ja -> ANGRIFF (setzt den Zustand auf E_ATTACKING)
        }
        // Fall B: Sind wir außerhalb des Stopp-Abstands?
        else if (distance_to_target > stopping_distance) {
            currentState = E_WALKING;
            Pathfinding(target_Position_X, target_Position_Y, delta_time);
        }
        // Fall C: Wir sind zu nah zum Laufen, aber der Angriff ist auf Cooldown.
        else {
            currentState = E_IDLE; // -> STEHEN BLEIBEN
        }
    }

    // 4. Animationen basierend auf dem finalen Zustand aktualisieren
    if (currentState == E_ATTACKING) {
        if (attack_animations.count(facing_Direction)) {
            attack_animations.at(facing_Direction).Update_Frame(delta_time);
        }
    } else if (currentState == E_WALKING) {
        if (walk_animations.count(facing_Direction)) {
            walk_animations.at(facing_Direction).Update_Frame(delta_time);
        }
    }
}

    void Melee_Enemy::Melee_Attack()
    {
        // Setzt alle relevanten Zustände und Timer
        currentState = E_ATTACKING;
        attack_Cooldown_Timer = game::Config::melee_enemy_1_attack_cooldown;
        attack_duration_timer = game::Config::melee_enemy_1_attack_duration;

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