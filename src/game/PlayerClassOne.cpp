//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"
#include "../config.h.in"

Player_Class_One::Player_Class_One(Vector2 start_Position)
    : Player_Base_Class(game::Config::player_Class_One_Max_Health, game::Config::player_Class_One_Movement_Speed,
        game::Config::player_Class_One_Damage, start_Position)
{
    int anim_Speed = game::Config::player_Anim_Speed;
    Vector2 anim_Size = game::Config::player_Anim_Size;
    int frame_Count = game::Config::player_Frame_Count;
    int sprites_Per_Line = frame_Count;

    walking_Animations.try_emplace(UP, anim_Size, game::Config::kPlayerWalkUpAnim, frame_Count, sprites_Per_Line,anim_Speed);
    walking_Animations.try_emplace(DOWN, anim_Size, game::Config::kPlayerWalkDownAnim, frame_Count, sprites_Per_Line,anim_Speed);
    walking_Animations.try_emplace(LEFT, anim_Size, game::Config::kPlayerWalkLeftAnim, frame_Count, sprites_Per_Line,anim_Speed);
    walking_Animations.try_emplace(RIGHT, anim_Size, game::Config::kPlayerWalkRightAnim, frame_Count, sprites_Per_Line,anim_Speed);
    idle_Animations.try_emplace(UP, anim_Size, game::Config:: kPlayerIdleUpAnim, frame_Count, sprites_Per_Line,anim_Speed);
    idle_Animations.try_emplace(DOWN, anim_Size, game::Config::kPlayerIdleDownAnim, frame_Count, sprites_Per_Line,anim_Speed);
    idle_Animations.try_emplace(LEFT, anim_Size, game::Config::kPlayerIdleLeftAnim, frame_Count, sprites_Per_Line,anim_Speed);
    idle_Animations.try_emplace(RIGHT, anim_Size, game::Config::kPlayerIdleRightAnim, frame_Count, sprites_Per_Line,anim_Speed);
}



Player_Class_One::~Player_Class_One() {}

void Player_Class_One::Tick(float delta_time)
{
    Player_Base_Class::Tick(delta_time);
    Facing_Direction primaryDirection = facing_Direction;
    if (currentState == WALKING || currentState == IDLE) {
        switch (facing_Direction) {
            case UP_LEFT:    primaryDirection = LEFT;  break;
            case UP_RIGHT:   primaryDirection = RIGHT; break;
            case DOWN_LEFT:  primaryDirection = LEFT;  break;
            case DOWN_RIGHT: primaryDirection = RIGHT; break;
        }
    }
    switch (currentState) {
        case WALKING:
            if (walking_Animations.count(primaryDirection)) {
                walking_Animations.at(primaryDirection).Update_Frame(delta_time);
            }
        break;
        case IDLE:
            default:
                if (idle_Animations.count(primaryDirection)) {
                    idle_Animations.at(primaryDirection).Update_Frame(delta_time);
                }
        break;
    }
}

void Player_Class_One::Draw()
{
    // Schritt 1: Einen Zeiger für die aktuell aktive Animation vorbereiten.
    RepeatAnimation* current_anim = nullptr;

    // Schritt 2: Die primäre Richtung für Idle/Walk bestimmen (deine Logik, unverändert).
    Facing_Direction primaryDirection = facing_Direction;
    if (currentState == WALKING || currentState == IDLE) {
        switch (facing_Direction) {
            case UP_LEFT:    primaryDirection = LEFT;  break;
            case UP_RIGHT:   primaryDirection = RIGHT; break;
            case DOWN_LEFT:  primaryDirection = LEFT;  break;
            case DOWN_RIGHT: primaryDirection = RIGHT; break;
        }
    }

    switch (currentState) {
        case WALKING:
            if (walking_Animations.count(primaryDirection)) {
                current_anim = &walking_Animations.at(primaryDirection);
            }
            break;

        case IDLE:
        default:
            if (idle_Animations.count(primaryDirection)) {
                current_anim = &idle_Animations.at(primaryDirection);
            }
            break;
    }

    if (current_anim != nullptr) {
        this->current_Sprite_Size = current_anim->size;
        Vector2 draw_pos;
        draw_pos.x = this->hitbox.x - (current_anim->size.x - this->hitbox.width) / 2;
        draw_pos.y = this->hitbox.y - (current_anim->size.y - this->hitbox.height) / 2;
        current_anim->Draw_Current_Frame(draw_pos);
    }
    //DrawRectangleLinesEx(this->hitbox, 2, GREEN);
}