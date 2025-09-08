//
// Created by $Will on 04.07.2025.
//

#include "CollisionResponse.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "EnemyBaseClass.h"
#include "PlayerBaseClass.h"

void CollisionResponse::Resolve_Overlap(Collidable* obj_A, const Collidable* obj_B)
{
    Rectangle hitbox_A = obj_A->Get_Hitbox();
    Rectangle hitbox_B = obj_B->Get_Hitbox();

    float overlap_X = std::min(hitbox_A.x + hitbox_A.width, hitbox_B.x + hitbox_B.width) - std::max(hitbox_A.x, hitbox_B.x);
    float overlap_Y = std::min(hitbox_A.y + hitbox_A.height, hitbox_B.y + hitbox_B.height) - std::max(hitbox_A.y, hitbox_B.y);

    const float epsilon = 0.01f;

    if (overlap_X < overlap_Y)
    {
        float center_A_x = hitbox_A.x + hitbox_A.width / 2.0f;
        float center_B_x = hitbox_B.x + hitbox_B.width / 2.0f;

        if (center_A_x < center_B_x)
        {
            obj_A->Set_Position({ hitbox_A.x - (overlap_X + epsilon), hitbox_A.y });
        }
        else
        {

            obj_A->Set_Position({ hitbox_A.x + (overlap_X + epsilon), hitbox_A.y });
        }
    }
    else
    {
        float center_A_y = hitbox_A.y + hitbox_A.height / 2.0f;
        float center_B_y = hitbox_B.y + hitbox_B.height / 2.0f;

        if (center_A_y < center_B_y)
        {

            obj_A->Set_Position({ hitbox_A.x, hitbox_A.y - (overlap_Y + epsilon) });
        }
        else
        {
            obj_A->Set_Position({ hitbox_A.x, hitbox_A.y + (overlap_Y + epsilon) });
        }
    }
}

void CollisionResponse::Apply_Damage(Collidable* target, int damage)
{
    if (!target) return;

    if (auto* player = dynamic_cast<Player_Base_Class*>(target))
    {
        player->Take_Damage(damage);
    }
    else if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(target))
    {
        enemy->Take_Damage(damage);
    }
}

void CollisionResponse::Mark_For_Destruction(Collidable* obj_To_Destroy)
{
    if (!obj_To_Destroy) return;
    obj_To_Destroy->Mark_For_Destruction();
}