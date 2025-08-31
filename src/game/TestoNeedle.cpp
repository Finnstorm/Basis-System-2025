//
// Created by $Will on 27.08.2025.
//

#include "TestoNeedle.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"

TestoNeedle::TestoNeedle(Vector2 position, bool has_despawn_timer)
    : ItemBase(position, ItemType::TESTO_NEEDLE, game::Config::kTestoNeedleAnim, has_despawn_timer,
               game::Config::testo_Needle_Anim_Size, game::Config::testo_Needle_Frame_Count, game::Config::testo_Needle_Anim_Speed)
{
}

void TestoNeedle::Activate(Player_Base_Class* player)
{
    player->ApplyTestoBuff();
}