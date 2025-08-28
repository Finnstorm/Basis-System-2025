//
// Created by $Will on 27.08.2025.
//

#include "BombItem.h"
#include "PlayerBaseClass.h"
#include "Object_Manager.h"
#include "BombExplosionHitbox.h"
#include "../Config.h.in"

BombItem::BombItem(Vector2 position)
    : ItemBase(position, ItemType::BOMB, game::Config::kBombItemAnim, true,
               game::Config::bomb_Item_Anim_Size, game::Config::bomb_Item_Frame_Count, game::Config::bomb_Item_Anim_Speed)
{
}

void BombItem::Activate(Player_Base_Class* player)
{
    if (!player || !player->object_manager_ptr) return;

    Vector2 player_center = player->Get_Player_Center();
    int damage = game::Config::bomb_Damage;
    const float TILE_SIZE = 16.0f;
    const float radius_in_pixels = game::Config::bomb_Radius_Tiles * TILE_SIZE;
    const float FATNESS_IN_TILES = (game::Config::bomb_Radius_Tiles * 2.0f) - 3.0f;
    const float arm_width = FATNESS_IN_TILES * TILE_SIZE;
    const float arm_half_width = arm_width / 2.0f;

    Rectangle center_box = {
        player_center.x - arm_half_width,
        player_center.y - arm_half_width,
        arm_width,
        arm_width
    };
    const float arm_length = radius_in_pixels - arm_half_width;
    Rectangle top_box = {
        player_center.x - arm_half_width,
        player_center.y - radius_in_pixels,
        arm_width,
        arm_length
    };

    Rectangle bottom_box = {
        player_center.x - arm_half_width,
        player_center.y + arm_half_width,
        arm_width,
        arm_length
    };

    Rectangle left_box = {
        player_center.x - radius_in_pixels,
        player_center.y - arm_half_width,
        arm_length,
        arm_width
    };

    Rectangle right_box = {
        player_center.x + arm_half_width,
        player_center.y - arm_half_width,
        arm_length,
        arm_width
    };

    // Erzeuge die Hitbox-Objekte
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(center_box, damage));
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(top_box, damage));
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(bottom_box, damage));
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(left_box, damage));
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(right_box, damage));

    player->RemoveHeldItem();
}