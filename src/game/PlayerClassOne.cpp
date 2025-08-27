//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"
#include <math.h>
#include "../config.h.in"

Player_Class_One::Player_Class_One(Vector2 start_Position)
    : Player_Base_Class(game::Config::player_Class_One_Max_Health, game::Config::player_Class_One_Movement_Speed,
        game::Config::player_Class_One_Damage_Multiplier, start_Position)
{
    int player_Walk_Anim_Speed = game::Config::player_Walk_Anim_Speed;
    Vector2 player_Walk_Anim_Size = game::Config::player_Walk_Anim_Size;
    int player_Walk_Frame_Count = game::Config::player_Walk_Frame_Count;
    int sprites_Per_Line_Walk = player_Walk_Frame_Count;
    walking_Animations.try_emplace(UP, player_Walk_Anim_Size, game::Config::kPlayerWalkUpAnim, player_Walk_Frame_Count, sprites_Per_Line_Walk,player_Walk_Anim_Speed);
    walking_Animations.try_emplace(DOWN, player_Walk_Anim_Size, game::Config::kPlayerWalkDownAnim, player_Walk_Frame_Count, sprites_Per_Line_Walk,player_Walk_Anim_Speed);
    walking_Animations.try_emplace(LEFT, player_Walk_Anim_Size, game::Config::kPlayerWalkLeftAnim, player_Walk_Frame_Count, sprites_Per_Line_Walk,player_Walk_Anim_Speed);
    walking_Animations.try_emplace(RIGHT, player_Walk_Anim_Size, game::Config::kPlayerWalkRightAnim, player_Walk_Frame_Count, sprites_Per_Line_Walk,player_Walk_Anim_Speed);

    int player_Idle_Anim_Speed = game::Config::player_Idle_Anim_Speed;
    Vector2 player_Idle_Anim_Size = game::Config::player_Idle_Anim_Size;
    int player_Idle_Frame_Count = game::Config::player_Idle_Frame_Count;
    int sprites_Per_Line_Idle = player_Idle_Frame_Count;
    idle_Animations.try_emplace(UP, player_Idle_Anim_Size, game::Config:: kPlayerIdleUpAnim, player_Idle_Frame_Count, sprites_Per_Line_Idle,player_Idle_Anim_Speed);
    idle_Animations.try_emplace(DOWN, player_Idle_Anim_Size, game::Config::kPlayerIdleDownAnim, player_Idle_Frame_Count, sprites_Per_Line_Idle,player_Idle_Anim_Speed);
    idle_Animations.try_emplace(LEFT, player_Idle_Anim_Size, game::Config::kPlayerIdleLeftAnim, player_Idle_Frame_Count, sprites_Per_Line_Idle,player_Idle_Anim_Speed);
    idle_Animations.try_emplace(RIGHT, player_Idle_Anim_Size, game::Config::kPlayerIdleRightAnim, player_Idle_Frame_Count, sprites_Per_Line_Idle,player_Idle_Anim_Speed);

    int player_Ranged_Attack_Anim_Speed = game::Config::player_Ranged_Attack_Anim_Speed;
    Vector2 player_Ranged_Attack_Anim_Size = game::Config::player_Ranged_Attack_Anim_Size;
    int player_Ranged_Attack_Frame_Count = game::Config::player_Ranged_Attack_Frame_Count;
    int sprites_Per_Line_Ranged_Attack =player_Ranged_Attack_Frame_Count;
    ranged_Attack_Animations.try_emplace(UP, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(UP_LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(UP_RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN_LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack, player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN_RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);

    int player_Melee_Attack_Anim_Speed = game::Config::player_Melee_Attack_Anim_Speed;
    Vector2 player_Melee_Attack_Anim_Size = game::Config::player_Melee_Attack_Anim_Size;
    int player_Melee_Attack_Frame_Count = game::Config::player_Melee_Attack_Frame_Count;
    int sprites_Per_Line_Melee_Attack = player_Melee_Attack_Frame_Count;
    melee_Attack_Animations.try_emplace(UP, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackUpAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(DOWN, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackDownAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(LEFT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackLeftAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(RIGHT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackRightAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(UP_LEFT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackUpLeftAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(UP_RIGHT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackUpRightAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(DOWN_LEFT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackDownLeftAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack, player_Melee_Attack_Anim_Speed);
    melee_Attack_Animations.try_emplace(DOWN_RIGHT, player_Melee_Attack_Anim_Size, game::Config::kPlayerMeleeAttackDownRightAnim, player_Melee_Attack_Frame_Count, sprites_Per_Line_Melee_Attack,player_Melee_Attack_Anim_Speed);

    int player_Buff_Walk_Anim_Speed = game::Config::player_Buff_Walk_Anim_Speed;
    Vector2 player_Buff_Walk_Anim_Size = game::Config::player_Buff_Walk_Anim_Size;
    int player_Buff_Walk_Frame_Count = game::Config::player_Buff_Walk_Frame_Count;
    int sprites_Per_Line_Buff_Walk = player_Buff_Walk_Frame_Count;
    buff_walking_Animations.try_emplace(UP, player_Buff_Walk_Anim_Size, game::Config::kPlayerBuffWalkUpAnim, player_Buff_Walk_Frame_Count, sprites_Per_Line_Buff_Walk, player_Buff_Walk_Anim_Speed);
    buff_walking_Animations.try_emplace(DOWN, player_Buff_Walk_Anim_Size, game::Config::kPlayerBuffWalkDownAnim, player_Buff_Walk_Frame_Count, sprites_Per_Line_Buff_Walk, player_Buff_Walk_Anim_Speed);
    buff_walking_Animations.try_emplace(LEFT, player_Buff_Walk_Anim_Size, game::Config::kPlayerBuffWalkLeftAnim, player_Buff_Walk_Frame_Count, sprites_Per_Line_Buff_Walk, player_Buff_Walk_Anim_Speed);
    buff_walking_Animations.try_emplace(RIGHT, player_Buff_Walk_Anim_Size, game::Config::kPlayerBuffWalkRightAnim, player_Buff_Walk_Frame_Count, sprites_Per_Line_Buff_Walk, player_Buff_Walk_Anim_Speed);

    int player_Buff_Idle_Anim_Speed = game::Config::player_Buff_Idle_Anim_Speed;
    Vector2 player_Buff_Idle_Anim_Size = game::Config::player_Buff_Idle_Anim_Size;
    int player_Buff_Idle_Frame_Count = game::Config::player_Buff_Idle_Frame_Count;
    int sprites_Per_Line_Buff_Idle = player_Buff_Idle_Frame_Count;
    buff_idle_Animations.try_emplace(UP, player_Buff_Idle_Anim_Size, game::Config::kPlayerBuffIdleUpAnim, player_Buff_Idle_Frame_Count, sprites_Per_Line_Buff_Idle, player_Buff_Idle_Anim_Speed);
    buff_idle_Animations.try_emplace(DOWN, player_Buff_Idle_Anim_Size, game::Config::kPlayerBuffIdleDownAnim, player_Buff_Idle_Frame_Count, sprites_Per_Line_Buff_Idle, player_Buff_Idle_Anim_Speed);
    buff_idle_Animations.try_emplace(LEFT, player_Buff_Idle_Anim_Size, game::Config::kPlayerBuffIdleLeftAnim, player_Buff_Idle_Frame_Count, sprites_Per_Line_Buff_Idle, player_Buff_Idle_Anim_Speed);
    buff_idle_Animations.try_emplace(RIGHT, player_Buff_Idle_Anim_Size, game::Config::kPlayerBuffIdleRightAnim, player_Buff_Idle_Frame_Count, sprites_Per_Line_Buff_Idle, player_Buff_Idle_Anim_Speed);

    int player_Buff_Melee_Attack_Anim_Speed = game::Config::player_Buff_Melee_Attack_Anim_Speed;
    Vector2 player_Buff_Melee_Attack_Anim_Size = game::Config::player_Buff_Melee_Attack_Anim_Size;
    int player_Buff_Melee_Attack_Frame_Count = game::Config::player_Buff_Melee_Attack_Frame_Count;
    int sprites_Per_Line_Buff_Melee_Attack = player_Buff_Melee_Attack_Frame_Count;
    buff_melee_Attack_Animations.try_emplace(UP, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeUpAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(DOWN, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeDownAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(LEFT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeLeftAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(RIGHT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeRightAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(UP_LEFT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeUpLeftAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(UP_RIGHT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeUpRightAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(DOWN_LEFT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeDownLeftAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
    buff_melee_Attack_Animations.try_emplace(DOWN_RIGHT, player_Buff_Melee_Attack_Anim_Size, game::Config::kPlayerBuffMeleeDownRightAnim, player_Buff_Melee_Attack_Frame_Count, sprites_Per_Line_Buff_Melee_Attack, player_Buff_Melee_Attack_Anim_Speed);
}



Player_Class_One::~Player_Class_One() {}

void Player_Class_One::Tick(float delta_time)
{
    Player_Base_Class::Tick(delta_time); // Dies setzt die 'is_Moving'-Variable

    // ====================================================================
    // 1. ZUSTAND VERWALTEN: Bestimme den korrekten Zustand des Spielers
    // ====================================================================

    // Wenn eine Angriffs-Animation läuft, prüfe, ob sie beendet ist.
    if (currentState == ATTACKING_MELEE)
    {
        auto* active_melee_map = IsBuffed() ? &buff_melee_Attack_Animations : &melee_Attack_Animations;
        if (!active_melee_map->count(attack_Direction) || active_melee_map->at(attack_Direction).IsFinished())
        {
            currentState = IDLE; // Angriff beenden
        }
    }
    else if (currentState == ATTACKING_RANGED)
    {
        // Annahme: Fernkampf hat keine Buff-Variante
        if (!ranged_Attack_Animations.count(attack_Direction) || ranged_Attack_Animations.at(attack_Direction).IsFinished())
        {
            currentState = IDLE; // Angriff beenden
        }
    }

    // Wenn wir nicht mitten in einem Angriff sind, bestimme, ob wir laufen oder stehen.
    if (currentState != ATTACKING_MELEE && currentState != ATTACKING_RANGED)
    {
        currentState = is_Moving ? WALKING : IDLE;
    }

    // ====================================================================
    // 2. ANIMATION AKTUALISIEREN: Update die Frames basierend auf dem finalen Zustand
    // ====================================================================

    // Wähle die korrekten Animations-Maps basierend auf dem Buff-Status aus
    auto* active_walking_map = IsBuffed() ? &buff_walking_Animations : &walking_Animations;
    auto* active_idle_map = IsBuffed() ? &buff_idle_Animations : &idle_Animations;
    auto* active_melee_map = IsBuffed() ? &buff_melee_Attack_Animations : &melee_Attack_Animations;

    // Vereinfache die Richtung für Lauf- und Idle-Animationen (nur 4 Richtungen)
    Facing_Direction primaryDirection = facing_Direction;
    if (facing_Direction == UP_LEFT || facing_Direction == DOWN_LEFT) primaryDirection = LEFT;
    if (facing_Direction == UP_RIGHT || facing_Direction == DOWN_RIGHT) primaryDirection = RIGHT;

    // Aktualisiere die passende Animation
    switch (currentState)
    {
        case WALKING:
            if (active_walking_map->count(primaryDirection)) {
                active_walking_map->at(primaryDirection).Update_Frame(delta_time);
            }
            break;
        case IDLE:
            if (active_idle_map->count(primaryDirection)) {
                active_idle_map->at(primaryDirection).Update_Frame(delta_time);
            }
            break;
        case ATTACKING_MELEE:
            if (active_melee_map->count(attack_Direction)) {
                active_melee_map->at(attack_Direction).Update_Frame(delta_time);
            }
            break;
        case ATTACKING_RANGED:
            if (ranged_Attack_Animations.count(attack_Direction)) {
                ranged_Attack_Animations.at(attack_Direction).Update_Frame(delta_time);
            }
            break;
    }
}

void Player_Class_One::Draw()
{
    Animations* current_attack_anim = nullptr;
    RepeatAnimation* current_loop_anim = nullptr;
    Vector2 draw_pos;
    auto* active_walking_map = &walking_Animations;
    auto* active_idle_map = &idle_Animations;
    auto* active_melee_map = &melee_Attack_Animations;

    if (IsBuffed())
    {
        active_walking_map = &buff_walking_Animations;
        active_idle_map = &buff_idle_Animations;
        active_melee_map = &buff_melee_Attack_Animations;
    }

    if (currentState == ATTACKING_MELEE)
    {
        if (active_melee_map->count(attack_Direction))
        {
            current_attack_anim = &active_melee_map->at(attack_Direction);
        }
    }
    else if (currentState == ATTACKING_RANGED)
    {
        if (ranged_Attack_Animations.count(attack_Direction))
        {
            current_attack_anim = &ranged_Attack_Animations.at(attack_Direction);
        }
    }
    else
    {
        Facing_Direction primaryDirection = facing_Direction;
        switch (facing_Direction) {
            case UP_LEFT:    primaryDirection = LEFT;  break;
            case UP_RIGHT:   primaryDirection = RIGHT; break;
            case DOWN_LEFT:  primaryDirection = LEFT;  break;
            case DOWN_RIGHT: primaryDirection = RIGHT; break;
            default: break;
        }

        if (currentState == WALKING)
        {
            current_loop_anim = &active_walking_map->at(primaryDirection);
        }
        else
        {
            current_loop_anim = &active_idle_map->at(primaryDirection);
        }
    }


    if (current_attack_anim != nullptr) {
        draw_pos.x = this->hitbox.x - (current_attack_anim->size.x - this->hitbox.width) / 2.0f;
        draw_pos.y = this->hitbox.y - (current_attack_anim->size.y - this->hitbox.height) / 2.0f;
        current_attack_anim->Draw_Current_Frame({roundf(draw_pos.x), roundf(draw_pos.y)});
    } else if (current_loop_anim != nullptr) {
        draw_pos.x = this->hitbox.x - (current_loop_anim->size.x - this->hitbox.width) / 2.0f;
        draw_pos.y = this->hitbox.y - (current_loop_anim->size.y - this->hitbox.height) / 2.0f;
        current_loop_anim->Draw_Current_Frame({roundf(draw_pos.x), roundf(draw_pos.y)});
    }
    if (game::Config::visualize_Player_Hitbox)
    {
        DrawRectangleLinesEx(this->hitbox, 1.0f, BLUE);
    }
}

void Player_Class_One::Ranged_Attack()
{
    this->attack_Direction = this->facing_Direction;
    Player_Base_Class::Ranged_Attack();
    if (ranged_Attack_Animations.count(this->attack_Direction)) {
        ranged_Attack_Animations.at(this->attack_Direction).First_Frame();
    }
}
void Player_Class_One::Melee_Attack()
{
    this->attack_Direction = this->facing_Direction;
    Player_Base_Class::Melee_Attack();
    auto* active_melee_map = IsBuffed() ? &buff_melee_Attack_Animations : &melee_Attack_Animations;
    if (active_melee_map->count(this->attack_Direction))
    {
        active_melee_map->at(this->attack_Direction).First_Frame();
    }
}
