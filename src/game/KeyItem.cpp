//
// Created by $Will on 27.08.2025.
//

#include "KeyItem.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"

KeyItem::KeyItem(Vector2 position)
    : ItemBase(position, ItemType::KEY, game::Config::kKeyAnim, false, // false -> hat keinen Despawn-Timer
               game::Config::key_Anim_Size, game::Config::key_Frame_Count, game::Config::key_Anim_Speed)
{
}

void KeyItem::Activate(Player_Base_Class* player)
{
    player->RemoveHeldItem();
}