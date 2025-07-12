//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"
#include "../config.h.in"

Player_Class_One::Player_Class_One(Vector2 start_Position)
    : Player_Base_Class(game::Config::player_Class_One_Max_Health, game::Config::player_Class_One_Movement_Speed,
        game::Config::player_Class_One_Damage, start_Position)
{
    Vector2 anim_Size = game::Config::player_Anim_Size;
    int frame_Count = game::Config::player_Frame_Count;
    int sprites_Per_Line = frame_Count;

    walking_Animations.try_emplace(UP, anim_Size, game::Config::kPlayerWalkUpAnim, frame_Count, sprites_Per_Line);
    walking_Animations.try_emplace(DOWN, anim_Size, game::Config::kPlayerWalkDownAnim, frame_Count, sprites_Per_Line);
    walking_Animations.try_emplace(LEFT, anim_Size, game::Config::kPlayerWalkLeftAnim, frame_Count, sprites_Per_Line);
    walking_Animations.try_emplace(RIGHT, anim_Size, game::Config::kPlayerWalkRightAnim, frame_Count, sprites_Per_Line);
    idle_Animations.try_emplace(UP, anim_Size, game::Config:: kPlayerIdleUpAnim, frame_Count, sprites_Per_Line);
    idle_Animations.try_emplace(DOWN, anim_Size, game::Config::kPlayerIdleDownAnim, frame_Count, sprites_Per_Line);
    idle_Animations.try_emplace(LEFT, anim_Size, game::Config::kPlayerIdleLeftAnim, frame_Count, sprites_Per_Line);
    idle_Animations.try_emplace(RIGHT, anim_Size, game::Config::kPlayerIdleRightAnim, frame_Count, sprites_Per_Line);
}



Player_Class_One::~Player_Class_One() {}

void Player_Class_One::Draw()
{
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
                    auto& anim = walking_Animations.at(primaryDirection);
                    anim.Next_Frame();
                    anim.Draw_Current_Frame(player_Pos);
                }
        break;

        case IDLE:
            default:
                if (idle_Animations.count(primaryDirection)) {
                    auto& anim = idle_Animations.at(primaryDirection);
                    anim.Next_Frame();
                    anim.Draw_Current_Frame(player_Pos);
                }
        break;

        // case ATTACKING_RANGED:
        // Hier kommt später die Angriffslogik. Man würde die 8-direktionale
        // facing_Direction ohne Reduzierung verwenden.
        // break;
    }
}