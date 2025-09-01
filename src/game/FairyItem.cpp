//
// Created by $Will on 27.08.2025.
//

#include "FairyItem.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"

FairyItem::FairyItem(Vector2 position, int current_level)
    : ItemBase(position, ItemType::FAIRY, "", false,
               game::Config::fairy_Anim_Size, game::Config::fairy_Frame_Count, game::Config::fairy_Anim_Speed)
{
    const char* anim_path;
    switch (current_level)
    {
        case 1:
            anim_path = game::Config::kFairyAnimLvl1;
        break;
        case 2:
            anim_path = game::Config::kFairyAnimLvl2;
        break;
        case 3:
            anim_path = game::Config::kFairyAnimLvl3;
        break;
        default:
            anim_path = game::Config::kFairyAnimLvl1;
        break;
    }

    this->animation = RepeatAnimation(game::Config::fairy_Anim_Size, anim_path,
    game::Config::fairy_Frame_Count, game::Config::fairy_Frame_Count, game::Config::fairy_Anim_Speed);
}