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
    float radius = game::Config::bomb_Radius_Tiles * 16.0f;
    int damage = game::Config::bomb_Damage;
    float center_size = radius * 1.414f / 2.0f;
    Rectangle center_box = {
        player_center.x - center_size,
        player_center.y - center_size,
        center_size * 2,
        center_size * 2
    };

    float side_width = (radius - center_size) * 2;
    Rectangle top_box = {player_center.x - center_size, player_center.y - radius, center_size * 2, side_width};
    Rectangle bottom_box = {player_center.x - center_size, player_center.y + center_size, center_size * 2, side_width};
    Rectangle left_box = {player_center.x - radius, player_center.y - center_size, side_width, center_size * 2};
    Rectangle right_box = {player_center.x + center_size, player_center.y - center_size, side_width, center_size * 2};

    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(center_box, damage));
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(top_box, damage));
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(bottom_box, damage));
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(left_box, damage));
    player->object_manager_ptr->AddObjectDeferred(new BombExplosionHitbox(right_box, damage));

    player->RemoveHeldItem();
}