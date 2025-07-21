//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"

#include <math.h>

#include "../config.h.in"

Player_Class_One::Player_Class_One(Vector2 start_Position)
    : Player_Base_Class(game::Config::player_Class_One_Max_Health, game::Config::player_Class_One_Movement_Speed,
        game::Config::player_Class_One_Damage, start_Position)
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
    int sprites_Per_Line_Ranged_Attack = player_Idle_Frame_Count;
    ranged_Attack_Animations.try_emplace(UP, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(UP_LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(UP_RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackUpRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN_LEFT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownLeftAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack, player_Ranged_Attack_Anim_Speed);
    ranged_Attack_Animations.try_emplace(DOWN_RIGHT, player_Ranged_Attack_Anim_Size, game::Config::kPlayerRangeAttackDownRightAnim, player_Ranged_Attack_Frame_Count, sprites_Per_Line_Ranged_Attack,player_Ranged_Attack_Anim_Speed);
}



Player_Class_One::~Player_Class_One() {}

void Player_Class_One::Tick(float delta_time)
{
    Player_Base_Class::Tick(delta_time);

    if (currentState == ATTACKING_RANGED) {
        // BENUTZE 'attack_Direction' für die Logik
        if (ranged_Attack_Animations.count(attack_Direction) && ranged_Attack_Animations.at(attack_Direction).IsFinished()) {
            currentState = IDLE;
        }
    } else {
        currentState = is_Moving ? WALKING : IDLE;
    }

    if (currentState == ATTACKING_RANGED) {
        // BENUTZE 'attack_Direction' zum Updaten
        if (ranged_Attack_Animations.count(attack_Direction)) {
            ranged_Attack_Animations.at(attack_Direction).Update_Frame(delta_time);
        }
    } else {
        // Logik für Laufen/Stehen bleibt wie gehabt
        Facing_Direction primaryDirection = facing_Direction;
        switch (facing_Direction) {
            case UP_LEFT: case DOWN_LEFT: primaryDirection = LEFT; break;
            case UP_RIGHT: case DOWN_RIGHT: primaryDirection = RIGHT; break;
            default: break;
        }

        if (currentState == WALKING) {
            if (walking_Animations.count(primaryDirection)) {
                walking_Animations.at(primaryDirection).Update_Frame(delta_time);
            }
        } else {
            if (idle_Animations.count(primaryDirection)) {
                idle_Animations.at(primaryDirection).Update_Frame(delta_time);
            }
        }
    }
    previousState = currentState;
}

void Player_Class_One::Draw()
{
    Animations* current_attack_anim = nullptr;
    RepeatAnimation* current_loop_anim = nullptr;
    Vector2 draw_pos;

    if (currentState == ATTACKING_RANGED) {
        // BENUTZE 'attack_Direction' zum Zeichnen
        if (ranged_Attack_Animations.count(attack_Direction)) {
            current_attack_anim = &ranged_Attack_Animations.at(attack_Direction);
        }
    } else {
        Facing_Direction primaryDirection = facing_Direction;
        switch (facing_Direction) {
            case UP_LEFT:    primaryDirection = LEFT;  break;
            case UP_RIGHT:   primaryDirection = RIGHT; break;
            case DOWN_LEFT:  primaryDirection = LEFT;  break;
            case DOWN_RIGHT: primaryDirection = RIGHT; break;
            default: break;
        }

        if (currentState == WALKING) {
            current_loop_anim = &walking_Animations.at(primaryDirection);
        } else {
            current_loop_anim = &idle_Animations.at(primaryDirection);
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
    //DrawRectangleLinesEx(this->hitbox, 2.0f, BLUE);
}

void Player_Class_One::Ranged_Attack()
{
    this->attack_Direction = this->facing_Direction;
    Player_Base_Class::Ranged_Attack();
    if (ranged_Attack_Animations.count(this->attack_Direction)) {
        ranged_Attack_Animations.at(this->attack_Direction).First_Frame();
    }
}